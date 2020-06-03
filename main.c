#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define __STDC_WANT_LIB_EXT1__	1
#include <string.h>

#ifdef __unix__
#  include <strings.h>	/* Defines: strcasecmp */
#endif

#include <time.h>

#include "log.h"	/* Taken from github:rxi/log.c */

/* Restricted to unix so only aghast sees this output. If you want to see 
 * it, remove that part of the condition.
 */
#if defined(__unix__) && defined(DEBUG)
#define LOG_LEVEL LOG_TRACE
#else
#define LOG_LEVEL LOG_WARN
#endif

#define UNUSED(var) (void)(var)	/* Pretends to use an unused parameter. */

#define STARTING_LOCATION "Porch"

/* Forward declarations of types */
typedef struct LOCATION LOCATION;
typedef struct OBJECTS OBJECTS;

typedef enum {
	OSF_LOCATION = 1 << 0,
	OSF_ROOM = 1 << 1,
	OSF_PLAYER = 1 << 2,
	OSF_NUM_FLAGS,
} OBJECT_SOURCE_FLAG;

// FUNCTIONS
static LOCATION * loc_find(const char * name);
static char * readLine();
static void execute(char * buffer);
static void exitGame(const char *, OBJECT_SOURCE_FLAG, void *);
static void executeGo(const char *, OBJECT_SOURCE_FLAG, void *);
static void executeInventory(const char *, OBJECT_SOURCE_FLAG, void *);
static void executeLook(const char *, OBJECT_SOURCE_FLAG, void *);
static void executeOpen(const char *, OBJECT_SOURCE_FLAG, void *);
static void executeRead(const char *, OBJECT_SOURCE_FLAG, void *);
static void executeTake(const char *, OBJECT_SOURCE_FLAG, void *);
static void itm_guns_and_ammo(void);
static void itm_open_door(void);
static void loc_enter(LOCATION * target);
static OBJECTS * obj_find_in_room(const char * noun);
static OBJECTS * obj_find_in_player(const char * noun);
static void pl_add_item(OBJECTS * item);
static bool pl_has_item(const char * name);
static char * pl_list_items(const char * delim);
static void pl_message(const char * fmt, ...);
static void pl_snark(const char * fmt, ...);
static void startUp();
static bool words_match(const char * w1, const char * w2);

// INIT
static bool whilePlaying = true;
static int inside = 0;


// STRUCTS
const OBJECT_SOURCE_FLAG  os_flags[OSF_NUM_FLAGS] = {
	OSF_LOCATION,
	OSF_ROOM,
	OSF_PLAYER,
};

typedef enum {
	VT_INTRANS,
	VT_MONOTRANS,
	VT_DITRANS,
	VT_TRITRANS,
	VT_NUM_TRANSES,
} VERB_TRANS;

typedef enum {
	IV_CAN_OPEN = 0x01,
	IV_CAN_TAKE = 0x02,
	IV_CAN_READ = 0x04,
	IV_CANT_TAKE = 0x08
} ITEM_VERB_FLAGS;

typedef struct VERB {
	const char 	*word;
	unsigned 	vb_trans		:4;
	unsigned	vb_object_source	:4;
	unsigned	vb_object_flags		:8;
	void 		(*vb_handler)(const char * noun, OBJECT_SOURCE_FLAG source, void * obj);
	const char     *vb_bad_object_msg;
} VERB;

VERB verbs[] = {
	{ "quit",   
		.vb_trans = VT_INTRANS, 
		.vb_object_source = 0, 
		.vb_object_flags = 0,	/* No items - intransitive */
		exitGame,
		NULL,
	},
	{ "go",     
		.vb_trans = VT_MONOTRANS, 
		.vb_object_source = OSF_LOCATION,
		.vb_object_flags = 0,	/* No items, only locations */
		executeGo,
		"I don't understand where you want to go!",
    	},
	{ "inventory",
		.vb_trans = VT_INTRANS,
		.vb_object_source = 0,
		.vb_object_flags = 0,
		executeInventory,
		NULL,
	},
	{ "look",
		.vb_trans = VT_INTRANS,
		.vb_object_source = 0,
		.vb_object_flags = 0,
		executeLook,
		NULL,
	},
	{ "open", 
		.vb_trans = VT_MONOTRANS, 
		.vb_object_source = OSF_ROOM | OSF_PLAYER, 
		.vb_object_flags = IV_CAN_OPEN,
		executeOpen,
		"I don't understand what you want to open.",
	},
	{ "read", 
		.vb_trans = VT_MONOTRANS, 
		.vb_object_source = OSF_ROOM | OSF_PLAYER,
		.vb_object_flags = IV_CAN_READ,
		executeRead,
		"I don't know what you want to read.",
	},
	{ "take", 
		.vb_trans = VT_MONOTRANS, 
		.vb_object_source = OSF_ROOM | OSF_PLAYER,
		.vb_object_flags = IV_CAN_TAKE,
		executeTake,
		"I don't understand what you want to take.",
	},
};

#define NUM_VERBS ((int)(sizeof(verbs) / sizeof(VERB)))

typedef enum {
	LOC_INSIDE,
	LOC_DOWNSTAIRS,
	LOC_UPSTAIRS,
} LOC_FLAGS;

typedef struct LOCATION {
    const char * loc_name;
    bool	 loc_visited;
    unsigned	 loc_flags	:8;
    const char * loc_enter_msg;
} LOCATION;

LOCATION locations[] = {
	{ "Porch", 
		.loc_visited=false,
		.loc_flags=0,
		"You stand in front of the mansion, there is a sign on the"
		" door." 
	},
	{ "hall", 
		.loc_visited=false,
		"You have access to the kitchen, toilet, living room"
		  " & upstairs." },
	{ "kitchen", .loc_visited=false,
		"There are several drawers ajar, there's also a weird"
		  " smell coming\nfrom the fridge." },
	{ "living", .loc_visited=false,
		"The furniture is covered with white cloth, but the "
		"colour has become\nyellowed out of age. Above the "
		"fireplace you see a double-barreled shotgun." },
	{ "toilet", .loc_visited=false,
		"You sure have a small bladder, couldn't you go before we"
		" started playing?" },
	{ "upstairs", .loc_visited=false,
		"There are 2 doors, which one do you want to take? Left"
		" or right?" },
	{ "first", .loc_visited=false,
		"You entered a bedroom, there's a bed and a closet in"
		" there." },
};

#define NUM_LOCATIONS ((int)(sizeof(locations) / sizeof(LOCATION)))

typedef struct OBJECTS {
    unsigned flag;
    const char *location;
    const char *item_name;
    const char *msg;
    const char *already;
    void       (*obj_extra_fn)(void);
} OBJECTS;

#define OBJ_MAX_NAME 32		// Max length of name string

OBJECTS objs[] = {
	{IV_CAN_OPEN, "Porch", "door", 
		"You enter the mansion's hall, seems like nobody's been"
		" here in years..",
		.already=NULL,
		.obj_extra_fn=itm_open_door,
	},
	{IV_CAN_OPEN, "kitchen", "drawer", 
		"In one of the drawers you see some ammo.\n", 
		.already=NULL,
		.obj_extra_fn=NULL,
	},
	{IV_CAN_OPEN, "kitchen", "fridge", 
		"Oh wish you didnt opened that. Whatever's in it, it's"
		" definitely out-of-date.\n", 
		.already=NULL,
		.obj_extra_fn=NULL,
	},
	{IV_CAN_OPEN, "upstairs", "open", 
		"OPEN CLOSET.\n", 
		.already=NULL,
		.obj_extra_fn=NULL,
       	},
	{IV_CAN_TAKE, "kitchen", "ammo", 
		"These might come in handy later!\n", 
		.already="You already took ammo.\n",
		.obj_extra_fn=itm_guns_and_ammo,
	},
	{IV_CAN_TAKE, "living", "gun", 
		"You got yourself a gun! (Were you born under a bad sign?)", 
		.already="You already took the gun.",
		.obj_extra_fn=itm_guns_and_ammo,
	},
	{IV_CAN_READ, "Porch", "sign", 
		"\"Begone, leave the dead in peace!\"", 
		.already=NULL,
		.obj_extra_fn=NULL,
       	}, 
};

#define NUM_OBJECTS ((int)(sizeof(objs) / sizeof(OBJECTS)))

#define PL_MAX_ITEMS 20

typedef struct PLAYER {
	LOCATION * pl_location;
	FILE *     pl_ostream;
	OBJECTS *  pl_inventory[PL_MAX_ITEMS]; 
} PLAYER;

static PLAYER Player;

// MAIN GAME
int main()
{    
	log_set_level(LOG_LEVEL);
    startUp();

    while (whilePlaying) 
    {
	char * buffer = readLine();
        execute(buffer);
    }

    return false;
}


// FUNCTIONS

/* Parse player input, find a matching verb and call its handler */

static void 
execute(buffer)
	char * buffer;
{
	char * verb = strtok(buffer, " \n");

	log_debug("verb = '%s'", verb);

	if (!verb)
		return;

	for (VERB * v = verbs; v < verbs + NUM_VERBS; ++v)
	{
		if (!words_match(verb, v->word))
			continue;

		/* Found a match for the verb! */

		if (v->vb_trans == VT_INTRANS) 
		{
			log_debug("matched '%s' (intrans)", v->word);
			v->vb_handler(NULL, 0, NULL);
			return;
		}

		char *noun = strtok(NULL, " \n");
		unsigned voflags = v->vb_object_flags;

		log_debug("matched '%s', noun = '%s'", v->word, noun);
		log_debug("obj source = %d", v->vb_object_source);

		if (v->vb_object_source & OSF_LOCATION)
		{
			log_debug("loc_find");
			void * target = loc_find(noun);
			log_debug("target = %s", ((LOCATION*)target)->loc_name);

			if (target != NULL)
			{
				v->vb_handler(noun, OSF_LOCATION, target);
				return;
			}
		}

		if (v->vb_object_source & OSF_ROOM)
		{
			log_debug("obj_find_in_room");
			OBJECTS * target = obj_find_in_room(noun);

			if (target != NULL
				&& (target->flag & voflags) == voflags)
			{
				log_debug("target = %s", target->item_name);
				v->vb_handler(noun, OSF_ROOM, target);
				if (target->obj_extra_fn)
					target->obj_extra_fn();
				return;
			}
		}

		if (v->vb_object_source & OSF_PLAYER)
		{
			log_debug("obj_find_in_player");
			OBJECTS * target = obj_find_in_player(noun);

			if (target
				&& (target->flag & voflags) == voflags)
			{
				log_debug("target = %s", target->item_name);
				v->vb_handler(noun, OSF_PLAYER, target);
				if (target->obj_extra_fn)
					target->obj_extra_fn();
				return;
			}
		}

		/* No object was found for the transitive verb. */
		log_debug("bad object");
		pl_snark(v->vb_bad_object_msg);
		return;
	}

	pl_snark("I don't understand what you mean, try again.");
}

static void exitGame(noun, osf, object)
	const char * noun;
	OBJECT_SOURCE_FLAG osf;
	void * object;
{
	UNUSED(noun);
	UNUSED(osf);
	UNUSED(object);

	whilePlaying = false;
}

static void executeGo(noun, osf, object)
	const char *	   noun;
	OBJECT_SOURCE_FLAG osf;
	void *		   object;
{
	UNUSED(noun);
	UNUSED(osf);

	LOCATION * target = (LOCATION *)object;

	assert(osf == OSF_LOCATION);
	assert(target != NULL);

	loc_enter(target);
}

static void 
executeRead(noun, osf, object)
	const char	  *noun;
	OBJECT_SOURCE_FLAG osf;
	void		  *object;
{
	UNUSED(noun);
	UNUSED(osf);

	OBJECTS * item = object;

	assert(osf == OSF_ROOM || osf == OSF_PLAYER);
	assert(item != NULL);

	pl_message(item->msg);
}

static void 
executeTake(noun, osf, object)
	const char	  *noun;
	OBJECT_SOURCE_FLAG osf;
	void		  *object;
{
	UNUSED(noun);

	OBJECTS * item = object;

	assert(osf == OSF_ROOM || osf == OSF_PLAYER);
	assert(item != NULL);

	if (osf == OSF_ROOM)
	{
		pl_message(item->msg);
		pl_add_item(item);
	}
	else if (osf == OSF_PLAYER)
		pl_message(item->already);
}

/* Print a list of items in the player's inventory. */

static void
executeInventory(noun, osf, object)
	const char        *noun;
	OBJECT_SOURCE_FLAG osf;
	void		  *object;
{
	UNUSED(noun);
	UNUSED(osf);
	UNUSED(object);

	log_trace("enter");

	const char * inv_str = pl_list_items(", ");
	log_debug("inventory: %s\n", inv_str);
	pl_message("You are holding: %s.", inv_str);
	log_trace("return");
}

/* Look around (re-print room description). */

static void
executeLook(noun, osf, object)
	const char        *noun;
	OBJECT_SOURCE_FLAG osf;
	void		  *object;
{
	UNUSED(noun);
	UNUSED(osf);
	UNUSED(object);

	log_trace("enter");

	pl_message("%s.", Player.pl_location->loc_name);
	pl_message(Player.pl_location->loc_enter_msg);

	log_trace("leave");
}

static void 
executeOpen(noun, osf, object)
	const char	  *noun;
	OBJECT_SOURCE_FLAG osf;
	void		  *object;
{
	UNUSED(noun);
	UNUSED(osf);

	OBJECTS * item = object;

	assert(osf == OSF_ROOM || osf == OSF_PLAYER);
	assert(item != NULL);
	assert(item->flag & IV_CAN_OPEN);
	
	pl_message(item->msg);
}

/* Extra-actions function for taking gun or ammo. */

static void
itm_guns_and_ammo()
{
	if (!pl_has_item("gun") || !pl_has_item("ammo"))
		return;

	pl_message("You got yourself a gun, you filled it up with the salt"
		   " bullets you found in the kitchen.\n"
		   "When you put the bullets in the gun, you hear a door"
		   " being slammed shut upstairs.");
}

/* Extra-actions function for opening front door. */

static void
itm_open_door()
{
	loc_enter(loc_find("hall"));
	inside = true;
}

/* Enter a new location. */

static void
loc_enter(target)
	LOCATION * target;
{
	const char * fmt = "%s\n";
	const char * msg = target->loc_enter_msg;

	assert(target != NULL);

	log_debug("destination = %s", target->loc_name);

	if (target == Player.pl_location)
	{
		fmt = "You are already standing in the %s\n\n";
		msg = target->loc_name;
	}
	else if (target->loc_visited)
	{
		msg = target->loc_name;
	}

	Player.pl_location = target;
	pl_message(fmt, msg);
	target->loc_visited = true;
}

static LOCATION *
loc_find(target)
	const char * target;
{
	assert(target != NULL);

	log_debug("target=%s", target);

	for (LOCATION * loc = locations; loc < locations + NUM_LOCATIONS; 
		++loc) 
	{
		log_debug("loc = %s", loc->loc_name);

		if (words_match(target, loc->loc_name)) 
		{
			log_debug("match!");
			return loc;
		}
	}

	log_debug("not found");
	return NULL;
}

static OBJECTS *
obj_find_in_player(name)
	const char * name;
{
	log_debug("name = %s", name);

	for (int i = 0; i < PL_MAX_ITEMS; ++i)
	{
		OBJECTS * o = Player.pl_inventory[i];
		if (o && words_match(o->item_name, name))
		{
			log_debug("found it: %s", o->item_name);
			return o;
		}
	}

	log_debug("not found");
	return NULL;
}

static OBJECTS *
obj_find_in_room(name)
	const char * name;
{
	const char * room_name = Player.pl_location->loc_name;

	log_debug("name = %s, room = %s", name, room_name);

	for (OBJECTS * o = objs; o < objs + NUM_OBJECTS; ++o)
	{
		if (!words_match(o->location, room_name))
			continue;

		if (!words_match(o->item_name, name))
			continue;

		/* Found it! */
		log_debug("found it: %s", o->item_name);
		return o;
	}

	log_debug("found nothing");
	return NULL;
}

/* Add item to player inventory. */

static void
pl_add_item(OBJECTS * item)
{
	for (int i = 0; i < PL_MAX_ITEMS; ++i)
	{
		if (Player.pl_inventory[i] == NULL)
		{
			Player.pl_inventory[i] = item;
			return;
		}
	}

	log_debug("FAIL: inventory must be full");
	pl_snark("Your inventory is full!");
}

/* Query if the player has an item in inventory. */

static bool
pl_has_item(name)
	const char * name;
{
	return obj_find_in_player(name) != NULL;
}

/* Compute a string of item names in inventory, delimited by given text. */

static char *
pl_list_items(delim)
	const char * delim;
{
#define PL_LIST_ITEMS_BUFFER_SZ (1 + PL_MAX_ITEMS * (OBJ_MAX_NAME + 10))
	static char buffer[PL_LIST_ITEMS_BUFFER_SZ];

	log_trace("enter");

	bool first = true;
	size_t offset = 0;

	for (int i=0; i < PL_MAX_ITEMS; ++i)
	{
		OBJECTS * o = Player.pl_inventory[i];

		if (o)
		{
			offset += snprintf(buffer + offset, 
				sizeof(buffer) - offset, 
				"%s%s", 
				first ? "" : delim, 
				o->item_name);
			first = false;

			if (offset >= sizeof(buffer))
				break;
		}
	}

	log_trace("return: %s", buffer);
	return buffer;
}

/* Print a plain message to the player. */

static void
pl_message(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(Player.pl_ostream, fmt, args);
	va_end(args);

	fputs("\n", Player.pl_ostream);
}

/* Print a snarky message when the player makes a mistake. */

static void 
pl_snark(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(Player.pl_ostream, fmt, args);
	va_end(args);

	fputs("\n", Player.pl_ostream);
}

/* Print a prompt, then read commands (delimited by ';') from player. */

static char * 
readLine()
{
	static char buffer[1024];
	static char * save_ptr = NULL;

	if (!save_ptr)
	{
		fputs(">  ", stdout);
    
		save_ptr = buffer;

		if (fgets(buffer, sizeof(buffer), stdin) == NULL)
		{
			/* End of input. So the game must be over. */
			*buffer = 0;
			whilePlaying = false;
			return buffer;
		}
	}

	char * result = save_ptr;

	save_ptr = strchr(save_ptr, ';');
	if (save_ptr)
		*save_ptr++ = 0;

	return result;
}


/* Initialize the game data, send a welcome message to the player. */

static void startUp()
{
	log_debug("Startup - initialize Player");

	Player.pl_ostream = stdout;


	puts("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	     "\t\t\t G H O S T   M A N O R\n\n"
             "\t\tA  T E X T - A D V E N T U R E  G A M E"
             "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
             "In the early 90s a girl paid a visit to a mansion, never to be seen again.\n"
             "Rumors say she is still roaming around the mansion, waiting..\n"
             "Find out if the stories are true are if it's just a hoax.\n"
             "You can direct me with the use of some basic words.\n\n\n"
	);

	loc_enter(loc_find(STARTING_LOCATION));
}

/* Helper function to make comparisons read more smoothly. */

static bool
words_match(word1, word2)
	const char * word1;
	const char * word2;
{
	return strcasecmp(word1, word2) == 0;
}

