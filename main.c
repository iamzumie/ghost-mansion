#include <stdio.h>
#include <time.h>
#include <string.h>
#ifdef __unix__
#include <strings.h>	/* Defines: strcasecmp */
#endif
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>


#define UNUSED(var) (void)(var)	/* Pretends to use an unused parameter. */

// FUNCTIONS
static char * readLine();
static void execute(char * buffer);
static void exitGame(const char *noun);
static void executeOpen(const char *noun);
static void executeRead(const char *noun);
static void executeGo(const char *noun);
static void executeTake(const char *noun);
static void startUp();
static bool words_match();

// INIT
static bool whilePlaying = true;
static const char* current_loc = "porch";
static int inside = 0;

// STRUCTS
typedef struct VERBS {
    const char * word;
    void (*function) (const char * noun); // helper function
} VERBS;

VERBS verbs[] = {
    { "quit", exitGame},
    { "go",   executeGo},
    { "open", executeOpen},
    { "read", executeRead},
    { "take", executeTake}
};

#define NUM_VERBS ((int)(sizeof(verbs) / sizeof(VERBS)))

typedef struct LOCATION {
    const char * word;
    const char * enter_msg;
} LOCATION;

LOCATION locations[] = {
    { "hall",     "You have access to the kitchen, toilet, living room"
	   	" & upstairs.\n" },
    { "kitchen",  "There are several drawers ajar, there's also a weird"
	          " smell coming\nfrom the fridge.\n" },
    { "living",	  "The furniture is covered with white cloth, but the"
	    	  " colour has become\nyellowed out of age. Above the"
	          " fireplace you see a double-barreled shotgun.\n" },
    { "toilet",	  "You sure have a small bladder, couldn't you go"
	          " before we started playing?\n" },
    { "upstairs", "There are 2 doors, which one do you want to take?"
	          " Left or right?\n" },
    { "first",    "You entered a bedroom, there's a bed and a closet"
	          " in there.\n" },
};

#define NUM_LOCATIONS ((int)(sizeof(locations) / sizeof(LOCATION)))

enum flag {
    CAN_OPEN = 0x01,
    CAN_TAKE = 0x02,
    CAN_READ = 0x04,
    CANT_TAKE = 0x08,
    CANT_OPEN = 0x16
};

typedef struct OBJECTS {
    unsigned flag;
    const char *location;
    const char *item;
    const char *msg;
    const char *already;
} OBJECTS;

OBJECTS objs[] = {
    {CAN_OPEN, "kitchen", "drawer", "In one of the drawers you see some ammo.\n", NULL},
    {CAN_OPEN, "kitchen", "fridge", "Oh wish you didnt opened that. Whatever's in it, it's definitely out-of-date.\n", NULL },
    {CAN_OPEN, "upstairs", "open", "OPEN CLOSET.\n", NULL },
    {CAN_TAKE, "kitchen", "ammo", "These might come in handy later!\n", "You already took some ammo.\n"},
    {CAN_TAKE, "living", "gun", "You got yourself a gun, you filled it up with the salt bullets you found in the kitchen.\nWhen you put the bullets in the gun, you hear a door being slammed shut upstairs.\n", "You already took the gun.\n"},
    {CAN_READ, "porch", "sign", "\"Begone, leave the dead in peace!\"\n", NULL}
};

#define NUM_OBJECTS ((int)(sizeof(objs) / sizeof(OBJECTS)))

// MAIN GAME
int main()
{    
    startUp(); // INTRO

    while (whilePlaying) 
    {
        char * buffer = readLine();
        execute(buffer);
    }

    return false;
}


// FUNCTIONS
// COMMAND & READLINE
static char * readLine ()
{
    static char buffer[1024];
	static char * save_ptr = NULL;

    if (!save_ptr)
    {
        fputs(">  ", stdout);
    
        save_ptr = buffer;

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            *buffer = 0;
            whilePlaying = false;
            return buffer;
        }
    }
    
    char * result = save_ptr;

    save_ptr = strchr(save_ptr, ';');
    if (save_ptr)
    {
        *save_ptr++ = 0;
    }

    return result;
}

// EXECUTE
static void execute(buffer)
    char * buffer;
{
    char *verb = strtok(buffer, " \n");
    char *noun = strtok(NULL, " \n");

    for (int i = 0; i < NUM_VERBS; i++)
    {
        if (words_match(verb, verbs[i].word))
        {
            verbs[i].function(noun);
            return;
        }
    }
    puts("I don't understand what you mean.\n");
}

static void exitGame(noun)
    const char *noun;
{
	UNUSED(noun);

	whilePlaying = false;
}

static void executeGo(noun)
    const char *noun;
{
    for (int i = 0; i < NUM_LOCATIONS; i++)
    {
        if (inside && words_match(noun, current_loc))
        {
            printf("You are already standing in the %s\n\n", current_loc);
            return;
        }
        else if (inside && words_match(noun, locations[i].word)) {
            puts(locations[i].enter_msg);
            current_loc = locations[i].word;
            return;
        }
    }
    puts("I don't understand where you want to go.\n");
    return;
}

static void executeRead(noun)
    const char *noun;
{
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        if(CAN_READ && objs[i].location == current_loc && words_match(noun, objs[i].item))
        {
            puts(objs[i].msg);
            return;
        }
    }
    puts("I don't know what you want to read.\n");
}

static void executeTake(noun)
    const char *noun;
{
    for (int k = 0; k < NUM_OBJECTS; k++)
    {
        if (objs[k].location == current_loc && words_match(noun, objs[k].item) && objs[k].flag == CAN_TAKE)
        {
            puts(objs[k].msg);
            objs[k].flag = CANT_TAKE;
            return;
        }
        else if (objs[k].location == current_loc && words_match(noun, objs[k].item) && objs[k].flag == CANT_TAKE)
        {
            puts(objs[k].already);
            return;
        }
    }
    puts("I don't understand what you want to take.\n");
    return;
}

static void executeOpen(noun)
    const char *noun;
{
    if(!inside && words_match(noun, "door")) 
    {
        puts("You enter the mansion's hall, seems like nobody's been here in years..\nYou now have access to the kitchen, toilet, living room & upstairs.\n");
        current_loc = "hall";
        inside = 1;
        return;
    }
    for (int l = 0; l < NUM_OBJECTS; l++)
    {
        if (inside && words_match(noun, objs[l].item) &&  objs[l].location == current_loc)
        {
            puts(objs[l].msg);
            return;
        }
    }
    puts("I don't understand what you want to open.\n");
    return;
}

static void startUp()
{
    puts("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            "\t\t\t G H O S T   M A N O R\n\n"
            "\t\tA  T E X T - A D V E N T U R E  G A M E"
            "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
            "In the early 90s a girl paid a visit to a mansion, never to be seen again.\n"
            "Rumors say she is still roaming around the mansion, waiting..\n"
            "Find out if the stories are true are if it's just a hoax.\n"
            "You can direct me with the use of some basic words.\n\n\n"
            "You stand in front of the mansion, there is a sign on the door.\n"
    );
}

static bool words_match (word1, word2)
    const char *word1;
    const char *word2;
{
    return strcasecmp(word1, word2) == 0;
}