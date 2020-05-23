#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// FUNCTIONS
int readLine();
int execute();
void startUp();
void readLocation();
void executeOpen();
void executeOpenDoor();
void executeOpenFridge();
void executeReadSign();
void executeGo();
void locationRoom();

// LOCATIONS
struct location {
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
void loc_kitchen();
void loc_living();
void loc_hall();
void loc_toilet();
void loc_upstairs();
void loc_firstFloor();

// INIT
int answer, location;
int bullets, key, gun = 0;
char* current_loc = "hall";
static char input[100];

// MAIN GAME
int main()
{    
    startUp(); // INTRO

    while (readLine() && execute()); // GAME LOOP

    return 0;
}


// FUNCTIONS
void startUp()
{
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\t\t\t G H O S T   M A N O R\n\n");
    printf("\t\tA  T E X T - A D V E N T U R E  G A M E");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");    

    // INTRODUCTION
    printf("In the early 90s a girl paid a visit to a mansion, never to be seen again.\n");
    printf("Rumors say she is still roaming around the mansion, waiting..\n");
    printf("Find out if the stories are true are if it's just a hoax.\n"); 
    printf("You can direct me with the use of some basic words.\n\n\n");
    printf("You stand in front of the mansion, there is a sign on the door.\n\n");
}

// COMMAND & READLINE
int readLine ()
{
    printf(">  ");
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
    return 1;
}

void executeOpenDoor(const char *noun)
{
    if (noun == NULL)
    {
        printf("What do you want to open?\n\n");
    }
    else if (strcasecmp(noun, "door") == 0)
    {        
        printf("You enter the mansion, seems like nobody's been here in years..\n");
        printf("You now have access to the kitchen, toilet, living room & upstairs.\n\n");
        readLocation();
    }
    else
    {
        printf("I don't understand what you want to open.\n\n");
    }
}

void readLocation() 
{
    while (1)
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
            printf("I don't understand where you want to go.\n\n");
        }
    }
}

void executeOpenFridge(const char *noun)
{
    if (noun == NULL)
    {
        printf("What do you want to open?\n\n");
    }
    else if (strcasecmp(noun, "fridge") == 0)
    {        
        printf("Oh wish you didnt opened that. Whatever's in it, it's definitely out-of-date.\n\n");
    }
    else
    {
        printf("I don't know what you want to open.\n\n");
    }
}

void executeReadSign(const char *noun)
{
    if (noun == NULL)
    {
        printf("What do you want to read?\n\n");
    }
    else if (strcasecmp(noun, "sign") == 0)
    {
        printf("\"Begone, leave the dead in peace!\"\n\n");
    }
    else
    {
        printf("I don't know what you want to read.\n\n");
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
        printf("Where do you want to go?\n\n");
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
        printf("I don't know where you want to go.\n\n");
    }
}

void loc_hall()
{
    current_loc = "hall"; // ADD LOCATION
    printf("You have access to the kitchen, toilet, living room & upstairs.\n\n");
    
    while (1)
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
    printf("There are several cupboards and drawers ajar, there's also a weird\n");
    printf("smell coming from the fridge.\n\n");

    while (1)
    {
        readLine();

        char *verb = strtok(input, " \n");
        char *noun = strtok(NULL, " \n");

        if (strcasecmp(verb, "search") == 0) 
        {
            if (gun == 1) {
                gun++;
                printf("You filled your shotgun with bullets.\n");
                printf("When you put the bullets in the gun, you hear a door being slammed shut upstairs.\n\n");
            }
            else if (gun == 2 || bullets == 1){
                printf("You already found ammo in the drawers.\n\n");
            }
            else 
            {
                printf("In one of the drawers you found some salt bullets. These might come in handy!\n\n");
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
    printf("The furniture is covered with white cloth, but the colour has become\n");
    printf("yellow out of age. The carpet has blood and dirt stains on it.\n");
    if (!gun)
    {
        printf("Above the fireplace you see a double-barreled shotgun.\n");
    }
    printf("\n");

    while (1)
    {
        readLine();

        char* verb = strtok(input, " \n");
        char* noun = strtok(NULL, " \n");

        if (strcasecmp(verb, "take") == 0) 
        {
            if (bullets) {
                gun = 2;
                printf("You got yourself a gun, you filled it up with the salt bullets you found in the kitchen.\n");
                printf("When you put the bullets in the gun, you hear a door being slammed shut upstairs.\n\n");
            }
            else if (gun > 0)
            {
                printf("You already have the gun.\n\n");
            }
            else 
            {
                gun++;
                printf("You took the gun, empty.. We need some find some bullets.\n\n");
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
    printf("You sure have a small bladder, couldn't you go before we started playing?\n\n");
    readLocation();
}

void loc_upstairs()
{
    current_loc = "upstairs"; 
    if (gun != 2) {
        printf("Maybe we need to find something to defend ourself first.\n\n");
    }
    else
    {
        printf("There are 2 doors, which one do you want to take? Left or right?\n\n");
        loc_firstFloor();
    }
}

void loc_firstFloor()
{   
    while (1)
    {
        readLine();

        char* verb = strtok(input, " \n");
        char* noun = strtok(NULL, " \n");

        if (strcasecmp(verb, "left") == 0) 
        {
            printf("You entered a bedroom, there's a bed and a closet in there.\n\n");
            locationRoom();
        }
        else if (strcasecmp(verb, "right") == 0) 
        {
            printf("It's locked.\n\n");
        } 
        else
        {
            printf("I don't know the word %s.\n\n", verb);
        }
    }
}

void locationRoom()
{
    while (1)
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
            printf("OPEN CLOSET\n\n");
        }
        else if (strcasecmp(verb, "check") == 0) 
        {
            printf("CHECK UNDER BED\n\n");
        }
        else
        {
            printf("I don't know the word %s.\n\n", verb);
        }
    }
}