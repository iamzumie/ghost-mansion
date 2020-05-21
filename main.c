#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// FUNCTIONS
void command();
void startUp();
void action();
void function();
void showmap();
void showlocations();
int choice();
int map();
int location2();
char* fWord();

// LOCATIONS
struct location {
    const char *description;
    const char *desc_upper;
}
locs[] = {
    {""},
    {"kitchen", "KITCHEN"},
    {"hallway", "HALL"},
    {"living room", "LIVING"},
    {"toilet", "TOILET"},
    {"upstairs", "UPSTAIRS"},
};
void loc_kitchen();
void loc_living();
void loc_hall();
void loc_toilet();
void loc_upstairs();

// INIT
int answer, location;
int current_loc = 4;
int bullets = 0;
char* read;
char name[40];
char buffer[1024];


// MAIN GAME
int main()
{
    // RANDOM NUMBER GENERATOR //
    srand(time(NULL));
    int random = rand() % 1 + 1; // + 1 because it's 0 otherwise, % 5 so we won't 'spawn' upstairs.
    current_loc = random;
    //

    startUp(); // INTRO


    // 1ST QUESTION
    while (1) 
    {
        command();
        read = fWord();

        if (strcasecmp(read, "open") == 0) 
        {
            function("open", "door");
            break;
        } 
        else if (strcasecmp(read, "knock") == 0) 
            printf("Nobody's home, it seems abandonded too.\n\n");
        else if (strcasecmp(read, "read") == 0) 
        {
            function("read", "sign");
            printf("\"Begone, leave the dead in peace!\"\n\n");
        }
        else 
            printf("I don't know the word %s, try again.\n\n", read);
    } 

    // 2ND QUESTION
    printf("You entered the mansion, it's too dark inside to see anything.\n");
    printf("You try to put on some light but the power seems to be out.\n");    
    printf("\n[i] Use your 'LIGHTER' to lit some candles.\n\n");

    action("USE");
    function("USE", "lighter");
    printf("The %s lightened up, seems like nobody has been here in a while..\n", locs[current_loc].description);
    location2();

    return 0;
}


// FUNCTIONS

// -->
void command() {
    printf(">  ");
}

// INT INPUT
int choice(int *answer) 
{
    if (fgets(buffer, sizeof(buffer), stdin)) // RETURNS IF NOT A NUMBER
    {
        *answer = atoi(buffer);
    }
    return *answer;
}

// ACTION
void action(char* input)
{
    while (1) {
        command();
        read = fWord();
        if (read)
        {
            if (strcasecmp(read, input) == 0)
                break;
            else
                printf("I don't know the word %s.\n\n", read);
        }
        else
            printf("Come again?\n\n");
    }
}

// FUNCTION
void function(char* cmd, char* input)
{
    while (1)
    {
        printf("%s: ", cmd);
        read = fWord();
        if (read)
        {
            if (strcasecmp(read, input) == 0)
                break;
            else
                printf("I don't know the word %s.\n\n", read);
        }
        else
            printf("Come again?\n\n");
    }
}

// ONLY READ THE FIRST WORD
char* fWord()
{
    char* input = buffer;

    fgets(input, sizeof(buffer), stdin);

    input[strcspn(input, "\r\n")] = 0; // REMOVES NEWLINE FROM GETLINE
    input = strtok(input, " "); // ONLY READS UNTIL THE FIRST WHITESPACE

    return input;
}


int location2() 
{
    // location
    switch (current_loc) // GO TO THE LOCATIONS
    {
        case 1: loc_kitchen();break;
        case 2: loc_hall(); break;
        case 3: loc_living(); break;
        case 4: loc_toilet(); break;
        case 5: loc_upstairs(); break;
        default: break;
        }
} 


int map() 
{
    showlocations();
    // CHOOSE A LOCATION
    do
    {
        printf("\nGo to: ");
        choice(&answer);
        
        // CHECKS IF WE AREN'T IN THIS LOCATION ALREADY FIRST
        if (current_loc == answer && answer != 0)
            printf("You are already standing in the %s.\n", locs[answer].description);
        else
        {
            switch (answer) // GO TO THE LOCATIONS
            {
                case 1: 
                    printf("You entered the kitchen.\n");
                    loc_kitchen();break;
                case 2: 
                    printf("You entered the hall.\n");
                    loc_hall(); break;
                case 3: 
                    printf("You entered the living room.\n");
                    loc_living(); break;
                case 4: 
                    loc_toilet(); break;
                case 5: loc_upstairs(); break;
                default:
                    printf("Come again?\n\n"); break;
            }
        }
    } while (answer == 5);
        printf("UPSTAIRS PART!\n");
}

void showmap()
{
    puts(
        "\n"
        "   ______________________________________\n"
        "  /            |        | UP |           \\ \n"
        " |             | TOILET |----|            |\n"
        " |   KITCHEN   |___ ____|----|            |\n"
        " |                      |----|            |\n"
        " |______  _____|        |                 |\n"
        " |             |        |                 |\n"
        " |    DINING   |              LIVING      |\n"
        " |     ROOM             |      ROOM       |\n"
        " |             |  HALL  |                 |\n"
        " |_____________|________|_________________|\n"
    );
}

void showlocations() 
{
    for (int i = 1; i < 6; i++)
    {
        printf("%i. %s\n", i,  locs[i].desc_upper);
    }   
}

void loc_kitchen()
{
    printf("There are several cupboards and drawers ajar, there's also a weird\n");
    printf("smell coming from the fridge.\n\n");

    while (1)
    {
        command();
        read = fWord();

        if (strcasecmp(read, "search") == 0) 
        {
            printf("In one of the drawers you found some salt bullets. These might come in handy!\n\n");
            bullets = 1;
        } 
        else if (strcasecmp(read, "open") == 0) 
        {
            function("open", "fridge");
            printf("Oh wish you didnt opened that. Whatever's in it, it's definitely out-of-date.\n\n");
        }
        else if (strcasecmp(read, "go") == 0) 
        {
            map();
            break;
        } 
    }
    current_loc = 1; // ADD LOCATION
}

void loc_hall()
{
    printf("HALLWAY\n\n");
    printf("There are several cupboards and drawers ajar, there's also a weird\n\n");

    while (1)
    {
        command();
        read = fWord();

        if (strcasecmp(read, "search") == 0) 
        {
            printf("In one of the drawers you found some salt bullets. These might come in handy!\n\n");
            bullets = 1;
        } 
        else if (strcasecmp(read, "open") == 0) 
        {
            function("open", "fridge");
            printf("Oh wish you didnt opened that. Whatever's in it, it's definitely out-of-date.\n\n");
        }
        else if (strcasecmp(read, "go") == 0) 
        {
            map();
            break;
        } 
    }
    current_loc = 2; // ADD LOCATION
}

void loc_living()
{
    printf("LIVING ROOM\n\n");
    current_loc = 3; // ADD LOCATION
}

void loc_toilet()
{
    printf("You sure have a small bladder, couldn't you go before we started playing?\n");
    command();
    current_loc = 4; 
}

void loc_upstairs()
{
    printf("UPSTAIRS\n\n");
    printf("You aren't allowed up here yet!\n\n");
}

// TITLE SCREEN & INTRODUCTION
void startUp()
{
    // TITLE SCREEN
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\t\t\t G H O S T   M A N O R\n");
    printf("\n");
    printf("\t\tA  T E X T - A D V E N T U R E  G A M E");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");    

    // INTRODUCTION
    printf("In the early 90s a girl paid a visit to a mansion, never to be seen again.\n");
    printf("Rumors say she is still roaming around the mansion, waiting..\n");
    printf("You can address me with some simple words: \"use, open, search & go\".\n");
    printf("Explore this godforsaken place and see if the stories are true.\n\n\n"); 
    printf("You stand in front of the mansion, there is a sign on the door.\n\n");
}
