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
static void loc_kitchen();
static void loc_living();
static void loc_hall();
static void loc_toilet();
static void loc_upstairs();
static void locationRoom();

// INIT
static int bullets, key, gun = 0;
static char buffer[100];
static const char* current_loc = "hall";
static int inside = 0;

// STRUCTS
typedef struct VERBS {
    const char* word;
    void (*function) (char* noun); // helper function?
} VERBS;
VERBS verbs[] = {
    {"go", executeGo},
    {"open", executeOpen},
    {"read", executeRead},
    {"take", executeTake}
};

typedef struct NOUNS {
    const char * word;
    void (*function) (void); // helper function?
    const char *enter_msg;
} NOUNS;
NOUNS nouns[] = {
    {"hall",  loc_hall, "You have access to the kitchen, toilet, living room & upstairs.\n"},
    {"kitchen", loc_kitchen, "There are several drawers ajar, there's also a weird smell coming\nfrom the fridge.\n"},
    {"living", loc_living, "The furniture is covered with white cloth, but the colour has become\nyellow out of age. The carpet has blood and dirt stains on it.\n"},
    {"toilet", loc_toilet, "You sure have a small bladder, couldn't you go before we started playing?\n"},
    {"upstairs", loc_upstairs, "There are 2 doors, which one do you want to take? Left or right?\n"},
    {"first", locationRoom, "You entered a bedroom, there's a bed and a closet in there.\n"}
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
    for (int i = 0; i < 5; i++)
    {
        if (inside && strcasecmp(noun, current_loc) == 0)
        {
            printf("You are already standing in the %s\n\n", current_loc);
            return;
        }
        else if (inside && strcasecmp(noun, nouns[i].word) == 0) {
            puts(nouns[i].enter_msg);
            current_loc = nouns[i].word;
            nouns[i].function;
            return;
        }
    }

    puts("I don't understand where you want to go.\n");
    return;
}

static void executeOpen(const char *noun)
{
    if (noun == NULL)
    {
        puts("What do you want to open?\n");
    }
    else if (!inside && (strcasecmp(noun, "door") == 0))
    {        
        puts("You enter the mansion's hall, seems like nobody's been here in years..");
        puts("You now have access to the kitchen, toilet, living room & upstairs.\n");
        inside = 1;
    }
    else if (current_loc == "kitchen" && strcasecmp(noun, "fridge") == 0)
    {        
        puts("Oh wish you didnt opened that. Whatever's in it, it's definitely out-of-date.\n");
    }
    else if (current_loc == "locationRoom" && strcasecmp(noun, "closet") == 0) 
    {
        puts("OPEN CLOSET\n");
    }
    else if (current_loc == "kitchen" && (strcasecmp(noun, "drawer") == 0 || strcasecmp(noun, "drawers") == 0))
    {
        if (gun == 1) {
            gun = 2;
            bullets = 1;
            puts("You filled your shotgun with bullets.");
            puts("When you put the bullets in the gun, you hear a door being slammed shut upstairs.\n");
        }
        else if (gun == 2 && bullets == 1){
            puts("You already found ammo in the drawers.\n");
        }
        else 
        {
            puts("In one of the drawers you found some salt bullets. These might come in handy!\n");
            bullets = 1;
        }
    }
    else
    {
        puts("I don't understand what you want to open.\n");
    }
}

static void executeRead(const char *noun)
{
    if (noun == NULL)
    {
        puts("What do you want to read?\n");
    }
    else if (!inside && (strcasecmp(noun, "sign") == 0))
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
    if (current_loc == "living")
    {
        if (bullets) {
            gun = 2;
            puts("You got yourself a gun, you filled it up with the salt bullets you found in the kitchen.");
            puts("When you put the bullets in the gun, you hear a door being slammed shut upstairs.\n");
        }
        else if (gun > 0)
        {
            puts("You already have the gun.\n");
        }
        else 
        {
            gun++;
            puts("You took the gun, empty.. We need some find some bullets.\n");
        }
    }
    else
    {
        puts("There is nothing to take.\n");
    }
}

static void loc_hall()
{
    return;
}

static void loc_kitchen()
{
    return;
}

static void loc_living()
{
    if (!gun)
    {
        puts("Above the fireplace you see a double-barreled shotgun.");
    }
    puts("");
    return;
}

static void loc_toilet()
{
    return;
}

static void loc_upstairs()
{   
    if (gun != 2) {
        puts("Maybe we need to find something to defend ourself first.\n");
        return;
    }
    return;
}

static void locationRoom()
{
    //current_loc = "locationRoom";
    return;
}

static void startUp()
{
    puts("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            "\t\t\t G H O S T   M A N O R\n\n"
            "\t\tA  T E X T - A D V E N T U R E  G A M E"
            "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
    );   

    // INTRODUCTION
    puts("In the early 90s a girl paid a visit to a mansion, never to be seen again.\n"
            "Rumors say she is still roaming around the mansion, waiting..\n"
            "Find out if the stories are true are if it's just a hoax.\n"
            "You can direct me with the use of some basic words.\n\n\n"
            "You stand in front of the mansion, there is a sign on the door.\n"
    );
}