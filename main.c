#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

// FUNCTIONS
static bool readLine();
static int execute();
static void startUp();
static void executeOpen();
static void executeRead();
static void executeGo();
static void executeTake();

// INIT
static char buffer[100];
static const char* current_loc = "hall";
static int inside = 0;

// STRUCTS
typedef struct VERBS {
    const char * word;
    void (*function) (char * noun); // helper function
} VERBS;
VERBS verbs[] = {
    {"go", executeGo},
    {"open", executeOpen},
    {"read", executeRead},
    {"take", executeTake}
};

typedef struct LOCATION {
    const char * word;
    const char *enter_msg;
} LOCATION;
LOCATION location[] = {
    {"hall", "You have access to the kitchen, toilet, living room & upstairs.\n"},
    {"kitchen", "There are several drawers ajar, there's also a weird smell coming\nfrom the fridge.\n"},
    {"living", "The furniture is covered with white cloth, but the colour has become\nyellow out of age. Above the fireplace you see a double-barreled shotgun.\n"},
    {"toilet", "You sure have a small bladder, couldn't you go before we started playing?\n"},
    {"upstairs", "There are 2 doors, which one do you want to take? Left or right?\n"},
    {"first", "You entered a bedroom, there's a bed and a closet in there.\n"}
};

enum flag {
    CAN_OPEN = 0x01,
    CAN_TAKE = 0x02,
    CAN_READ = 0x04,
    CANT_TAKE = 0x08
};

typedef struct OBJECTS {
    unsigned flag;
    const char *location;
    const char *item;
    const char *msg;
    const char *already;
} OBJECTS;
OBJECTS objs[] = {
    {CAN_OPEN, "kitchen", "drawer", "In one of the drawers you see some ammo.\n"},
    {CAN_OPEN, "kitchen", "fridge", "Oh wish you didnt opened that. Whatever's in it, it's definitely out-of-date.\n"},
    {CAN_OPEN, "upstairs", "open", "OPEN CLOSET.\n", ""},
    {CAN_TAKE, "kitchen", "ammo", "These might come in handy later!\n", "You already took ammo.\n"},
    {CAN_TAKE, "living", "gun", "You got yourself a gun, you filled it up with the salt bullets you found in the kitchen.\nWhen you put the bullets in the gun, you hear a door being slammed shut upstairs.\n", "You already took the gun.\n"}
};


// MAIN GAME
int main()
{    
    startUp(); // INTRO

    while (readLine() && execute()); // GAME LOOP

    return false;
}


// FUNCTIONS
// COMMAND & READLINE
static bool readLine ()
{
    fputs(">  ", stdout);
    return fgets(buffer, sizeof(buffer), stdin) != NULL;
}

// EXECUTE
static int execute()
{
    char *verb = strtok(buffer, " \n");
    char *noun = strtok(NULL, " \n");

    if (strcasecmp(verb, "quit") == 0)
    {
        return false;
    }
    for (int i = 0; i < sizeof(VERBS); i++)
    {
        if (strcasecmp(verb, verbs[i].word) == 0)
        {
            verbs[i].function(noun);
            return true;
        }
    }

    puts("I don't understand what you mean, try again.\n");
    return true;
}

static void executeGo(const char *noun)
{
    for (int i = 0; i < sizeof(location); i++)
    {
        if (inside && strcasecmp(noun, current_loc) == 0)
        {
            printf("You are already standing in the %s\n\n", current_loc);
            return;
        }
        else if (inside && strcasecmp(noun, location[i].word) == 0) {
            puts(location[i].enter_msg);
            current_loc = location[i].word;
            return;
        }
    }
    puts("I don't understand where you want to go.\n");
    return;
}

static void executeRead(const char *noun)
{
    if (!inside && (strcasecmp(noun, "sign") == 0))
    {
        puts("\"Begone, leave the dead in peace!\"\n");
    }
    else
    {
        puts("I don't know what you want to read.\n");
    }
}

static void executeTake(const char *noun)
{
    for (int k = 0; k < sizeof(objs); k++)
    {
        if (objs[k].location == current_loc && strcasecmp(noun, objs[k].item) == 0 && objs[k].flag == CAN_TAKE)
        {
            puts(objs[k].msg);
            objs[k].flag = CANT_TAKE;
            return;
        }
        else if (objs[k].location == current_loc && strcasecmp(noun, objs[k].item) == 0 && objs[k].flag == CANT_TAKE)
        {
            puts(objs[k].already);
            return;
        }
    }
    puts("I don't understand what you want to take.\n");
    return;
}

static void executeOpen(const char *noun)
{
    if(!inside && strcasecmp(noun, "door") == 0) 
    {
        puts("You enter the mansion's hall, seems like nobody's been here in years..\nYou now have access to the kitchen, toilet, living room & upstairs.\n");
        inside = 1;
        return;
    }
    for (int l = 0; l < sizeof(objs); l++)
    {
        if (inside && strcasecmp(noun, objs[l].item) == 0 && objs[l].location == current_loc)
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