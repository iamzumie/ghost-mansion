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

// LOCATIONS
typedef struct LOCATION {
    const char *name;
    const char *enter_msg;
} LOCATION;
LOCATION locs[] = {
    {""},
    {"hall",  "You have access to the kitchen, toilet, living room & upstairs.\n"},
    {"kitchen", "There are several drawers ajar, there's also a weird smell coming\nfrom the fridge.\n"},
    {"living room", "The furniture is covered with white cloth, but the colour has become\nyellow out of age. The carpet has blood and dirt stains on it."},
    {"toilet", "You sure have a small bladder, couldn't you go before we started playing?\n"},
    {"upstairs", "There are 2 doors, which one do you want to take? Left or right?\n"},
    {"first floor", "You entered a bedroom, there's a bed and a closet in there.\n"}
};

static void loc_kitchen();
static void loc_living();
static void loc_hall();
static void loc_toilet();
static void loc_upstairs();
static void locationRoom();

// INIT
static int answer, location;
static int bullets, key, gun = 0;
static char input[100];
static char* current_loc = "hall";
static int inside = 0;

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
    return fgets(input, sizeof(input), stdin) != NULL;
}

static int execute()
{
    char *verb = strtok(input, " \n");
    char *noun = strtok(NULL, " \n");

    if (verb != NULL)
    {        
        if (strcasecmp(verb, "open") == 0) 
        {
            executeOpen(noun);
        }
        else if (strcasecmp(verb, "read") == 0) 
        {
            executeRead(noun);
        }
        else if (strcasecmp(verb, "go") == 0)
        {
            executeGo(noun);
        }
        else if (strcasecmp(verb, "take") == 0)
        {
            executeTake(noun);
        }
        else 
            printf("I don't know the word %s, try again.\n\n", verb);
    }
    return true;
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

static void executeGo(const char *noun)
{
    if (strcasecmp(noun, current_loc) == 0)
    {
        printf("You are already standing in the %s.\n\n", current_loc);
    }
    else if (noun == NULL)
    {
        puts("Where do you want to go?\n");
    }
    else if (inside && (strcasecmp(noun, "kitchen") == 0))
    {
        loc_kitchen();
    }
    else if (inside && (strcasecmp(noun, "toilet") == 0))
    {
        loc_toilet();
    }
    else if (inside && (strcasecmp(noun, "hall") == 0))
    {
        loc_hall();
    }
    else if (inside && (strcasecmp(noun, "living") == 0))
    {
        loc_living();
    }
    else if (inside && (strcasecmp(noun, "upstairs") == 0))
    {
        loc_upstairs();
    }
    else if (current_loc == "upstairs" && strcasecmp(noun, "right") == 0) 
    {
        puts("It's locked.\n");
    } 
    else if (current_loc == "upstairs" && strcasecmp(noun, "left") == 0) 
    {
        locationRoom();
    }
    else
    {
        puts("I don't know where you want to go.\n");
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
    current_loc = "hall"; // ADD LOCATION
    puts(locs[1].enter_msg);
    return;
}

static void loc_kitchen()
{
    current_loc = "kitchen"; // ADD LOCATION
    puts(locs[2].enter_msg);
    return;
}

static void loc_living()
{
    current_loc = "living"; // ADD LOCATION
    puts(locs[3].enter_msg);
    if (!gun)
    {
        puts("Above the fireplace you see a double-barreled shotgun.");
    }
    puts("");
    return;
}

static void loc_toilet()
{
    current_loc = "toilet"; 
    puts(locs[4].enter_msg);
    return;
}

static void loc_upstairs()
{   
    current_loc = "upstairs";
    if (gun != 2) {
        puts("Maybe we need to find something to defend ourself first.\n");
        return;
    }
    puts(locs[5].enter_msg);
    return;
}

static void locationRoom()
{
    current_loc = "locationRoom";
    puts(locs[6].enter_msg);
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