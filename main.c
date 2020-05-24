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
static void readLocation();
static void executeOpen();
static void executeOpenDoor();
static void executeOpenFridge();
static void executeReadSign();
static void executeGo();
static void locationRoom();
static void loc_kitchen();
static void loc_living();
static void loc_hall();
static void loc_toilet();
static void loc_upstairs();
static void loc_firstFloor();

// LOCATIONS
static struct location {
    const char *description;
    const char *name;
}
locs[] = {
    {""},
    {"hallway", "hall"},
    {"kitchen", "kitchen"},
    {"living room", "living room"},
    {"toilet", "toilet room"},
    {"upstairs", "first floor"},
};

// INIT
static int answer, location;
static int bullets, key, gun = 0;
static char* current_loc = "hall";
static char input[100];

// MAIN GAME
int main()
{    
    startUp(); // INTRO

    while (readLine() && execute()); // GAME LOOP

    return false;
}


// FUNCTIONS
// COMMAND & READLINE
bool readLine ()
{
    fputs(">  ", stdout);
    return fgets(input, sizeof(input), stdin) != NULL;
}

int execute()
{
    char *verb = strtok(input, " \n");
    char *noun = strtok(NULL, " \n");

    if (verb != NULL)
    {        
        if (strcasecmp(verb, "open") == 0) 
        {
            executeOpenDoor(noun);
        }
        else if (strcasecmp(verb, "read") == 0) 
        {
            executeReadSign(noun);
        }
        else 
            printf("I don't know the word %s, try again.\n\n", verb);
    }
    return true;
}

void executeOpenDoor(const char *noun)
{
    if (noun == NULL)
    {
        puts("What do you want to open?\n");
    }
    else if (strcasecmp(noun, "door") == 0)
    {        
        puts("You enter the mansion, seems like nobody's been here in years..");
        puts("You now have access to the kitchen, toilet, living room & upstairs.\n");
        readLocation();
    }
    else
    {
        puts("I don't understand what you want to open.\n");
    }
}

void readLocation() 
{
    while (true)
    {
        readLine();
        
        char *verb = strtok(input, " \n");
        char *noun = strtok(NULL, " \n");

        if (strcasecmp(verb, "go") == 0)
        {
            executeGo(noun);
        }
        else
        {
            puts("I don't understand where you want to go.\n");
        }
    }
}

void executeOpenFridge(const char *noun)
{
    if (noun == NULL)
    {
        puts("What do you want to open?\n");
    }
    else if (strcasecmp(noun, "fridge") == 0)
    {        
        puts("Oh wish you didnt opened that. Whatever's in it, it's definitely out-of-date.\n");
    }
    else
    {
        puts("I don't know what you want to open.\n");
    }
}

void executeReadSign(const char *noun)
{
    if (noun == NULL)
    {
        puts("What do you want to read?\n");
    }
    else if (strcasecmp(noun, "sign") == 0)
    {
        puts("\"Begone, leave the dead in peace!\"\n");
    }
    else
    {
        puts("I don't know what you want to read.\n");
    }
}

void executeGo(const char *noun)
{    
    if (strcasecmp(noun, current_loc) == 0)
    {
        printf("You are already standing in the %s.\n\n", current_loc);
    }
    else if (noun == NULL)
    {
        puts("Where do you want to go?\n");
    }
    else if (strcasecmp(noun, "kitchen") == 0)
    {
        loc_kitchen();
    }
    else if (strcasecmp(noun, "toilet") == 0)
    {
        loc_toilet();
    }
    else if (strcasecmp(noun, "hall") == 0)
    {
        loc_hall();
    }
    else if (strcasecmp(noun, "living") == 0)
    {
        loc_living();
    }
    else if (strcasecmp(noun, "upstairs") == 0)
    {
        loc_upstairs();
    }
    else
    {
        puts("I don't know where you want to go.\n");
    }
}

void loc_hall()
{
    current_loc = "hall"; // ADD LOCATION
    puts("You have access to the kitchen, toilet, living room & upstairs.\n");
    
    while (true)
    {
        readLine();

        char *verb = strtok(input, " \n");
        char *noun = strtok(NULL, " \n");

        if (strcasecmp(verb, "go") == 0) 
        {
            executeGo(noun);
        } 
        else
        {
            printf("I don't know the word %s.\n\n", verb);
        }
    }
}

void loc_kitchen()
{
    current_loc = "kitchen"; // ADD LOCATION
    puts("There are several cupboards and drawers ajar, there's also a weird");
    puts("smell coming from the fridge.\n");

    while (true)
    {
        readLine();

        char *verb = strtok(input, " \n");
        char *noun = strtok(NULL, " \n");

        if (strcasecmp(verb, "search") == 0) 
        {
            if (gun == 1) {
                gun++;
                puts("You filled your shotgun with bullets.");
                puts("When you put the bullets in the gun, you hear a door being slammed shut upstairs.\n");
            }
            else if (gun == 2 || bullets == 1){
                puts("You already found ammo in the drawers.\n");
            }
            else 
            {
                puts("In one of the drawers you found some salt bullets. These might come in handy!\n");
                bullets++;
            }
        } 
        else if (strcasecmp(verb, "open") == 0) 
        {
            executeOpenFridge(noun);
        }
        else if (strcasecmp(verb, "go") == 0) 
        {
            executeGo(noun);
        } 
        else
        {
            printf("I don't know the word %s.\n\n", verb);
        }
    }
}

void loc_living()
{
    current_loc = "living"; // ADD LOCATION
    puts("The furniture is covered with white cloth, but the colour has become");
    puts("yellow out of age. The carpet has blood and dirt stains on it.");
    if (!gun)
    {
        puts("Above the fireplace you see a double-barreled shotgun.");
    }
    puts("");

    while (true)
    {
        readLine();

        char* verb = strtok(input, " \n");
        char* noun = strtok(NULL, " \n");

        if (strcasecmp(verb, "take") == 0) 
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
        else if (strcasecmp(verb, "go") == 0) 
        {
            executeGo(noun);
        } 
        else
        {
            printf("I don't know the word %s.\n\n", verb);
        }
    }
}

void loc_toilet()
{
    current_loc = "toilet"; 
    puts("You sure have a small bladder, couldn't you go before we started playing?\n");
    readLocation();
}

void loc_upstairs()
{
    current_loc = "upstairs"; 
    if (gun != 2) {
        puts("Maybe we need to find something to defend ourself first.\n");
    }
    else
    {
        puts("There are 2 doors, which one do you want to take? Left or right?\n");
        loc_firstFloor();
    }
}

void loc_firstFloor()
{   
    while (true)
    {
        readLine();

        char* verb = strtok(input, " \n");
        char* noun = strtok(NULL, " \n");

        if (strcasecmp(verb, "left") == 0) 
        {
            puts("You entered a bedroom, there's a bed and a closet in there.\n");
            locationRoom();
        }
        else if (strcasecmp(verb, "right") == 0) 
        {
            puts("It's locked.\n");
        } 
        else
        {
            printf("I don't know the word %s.\n\n", verb);
        }
    }
}

void locationRoom()
{
    while (true)
    {
        readLine();
        
        char* verb = strtok(input, " \n");
        char* noun = strtok(NULL, " \n");

        if (strcasecmp(verb, "leave") == 0) 
        {
            loc_upstairs();
            break;
        } 
        else if (strcasecmp(verb, "open") == 0) 
        {
            puts("OPEN CLOSET\n");
        }
        else if (strcasecmp(verb, "check") == 0) 
        {
            puts("CHECK UNDER BED\n");
        }
        else
        {
            printf("I don't know the word %s.\n\n", verb);
        }
    }
}

void startUp()
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