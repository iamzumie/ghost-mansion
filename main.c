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
char* fWord();

// LOCATIONS
struct location {
    const char *description;
    const char *desc_upper;
}
locs[] = {
    {""},
    {"kitchen", "KITCHEN"},
    {"dining room", "DINING"},
    {"hall", "HALL"},
    {"living room", "LIVING"},
    {"toilet", "TOILET"},
    {"upstairs", "UPSTAIRS"},
};
void loc_kitchen();
void loc_living();
void loc_hall();
void loc_dining();
void loc_toilet();
void loc_upstairs();

// INIT
int answer, location;
int current_loc = 4;
char* read;
char name[40];
char buffer[1024];



// MAIN GAME
int main()
{
    startUp(); // INTRO

    // FIRST QUESTION
    printf("[1] look around\t [2] go north\t [3] go south\n\n");
    do {
        command();
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            return 1;
        }
        answer = atoi(buffer);
        switch (answer)
        {
            case 1: printf("It's pouring, maybe it's better if we go somewhere to hide.\n\n"); break;
            case 2: printf("You go towards the mansion.\nThere's a sign on the door that says: \"Begone humans! Leave the dead in peace!\"\n\n"); break;
            case 3: printf("It's too dark going south, it's freezing outside.\n\n");
                    break;
            default: printf("Come again? Please choose a number and hit enter.\n\n"); break;
        }
    } while (answer != 2);


    // SECOND QUESTION
    printf("[1] Go inside\t [2] Knock on the door\t [3] Kick a flower pot\n\n");

    do {
        command();
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            return 1;
        }
        answer = atoi(buffer);
        switch (answer)
        {
        case 1: printf("It's locked. You need to find a key.\n\n"); break;
        case 2: printf("Nobody's home, it seems abandonded too.\n\n"); break;
        case 3: printf("You found the key under the flower pot!");
            printf("\nYou open the door, it's very dark inside..\n\n");
            break;
        default: printf("Come again?\n\n"); break;
        }
    } while (answer != 3);


    // THIRD QUESTION
    printf("We could use some light..\n");
    printf("\n[!] To use a lighter, type 'USE' and hit enter. Then type 'LIGHTER'.\n\n");

    action("USE"); // LOOKS AT USE FIRST
    function("USE", "lighter");


    // FOURTH QUESTION
    printf("The room lightened up a little, all windows are boarded.\n");
    printf("What do you want to do next?\n\n");
    printf("[1] use toilet     [2] turn on some lights     [3] search for candles\n\n");

    do {
        command();
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            return 1;
        }
        answer = atoi(buffer);
        switch (answer)
        {
        case 1: printf("Aaah that felt good!\n\n"); break;
        case 2: printf("Nothing happened.. I guess the power isn't working.\n\n"); break;
        case 3: printf("You found a candle, just in time. Your lighter was almost empty.\n"); break;
        default: printf("Come again?\n\n"); break;
        }
    } while (answer != 3);
    printf("While looking for candles you also found a map which seems to be like the floorplan.\n");

    // MAP
    printf("\n[!] \"Use\" map where you want to go to:\n\n");
    action("USE"); // LOOKS AT USE FIRST
    function("USE", "map");
    showmap();

    printf("\nChoose where you want to go to:\n");
    // LOCATIONS
    showmap();
    printf("[1] %s     [2] %s     [3] %s     [4] %s     [5] %s     [6] %s\n\n", locs[1].desc_upper, locs[2].desc_upper, locs[3].desc_upper, locs[4].desc_upper, locs[5].desc_upper, locs[6].desc_upper);
    do
    {
        command();
        if (!fgets(buffer, sizeof(buffer), stdin)) // RETURNS IF NOT A NUMBER
        {
            return 1;
        }
        answer = atoi(buffer);
        
        // CHECKS IF WE AREN'T IN THIS LOCATION ALREADY FIRST
        if (current_loc == answer && answer != 0)
            printf("You are already standing in the %s.\n\n", locs[answer].description);
        else
        {
            switch (answer) // GO TO THE LOCATIONS
            {
                case 1: loc_kitchen();break;
                case 2: loc_dining(); break;
                case 3: loc_hall(); break;
                case 4: loc_living(); break;
                case 5: loc_toilet(); break;
                case 6: loc_upstairs(); break;
                default:
                    printf("Come again?\n\n"); break;
            }
        }
    } while (answer != 6);
        printf("UPSTAIRS PART!\n");
}


// FUNCTIONS

// -->
void command() {
    printf(">  ");
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
        printf("%s:  ", cmd);
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

void showmap()
{
    puts(
        "\n\n"
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

void loc_kitchen()
{
    
    printf("KITCHEN\n\n");
    current_loc = 1; // ADD LOCATION
}

void loc_dining()
{
    
    printf("DINING ROOM\n\n");
    current_loc = 2; // ADD LOCATION
}

void loc_hall()
{
    
    printf("HALL\n\n");
    current_loc = 3; // ADD LOCATION
}

void loc_living()
{
    
    printf("LIVING\n\n");
    current_loc = 4; // ADD LOCATION
}

void loc_toilet()
{
    printf("You sure have a small bladder!\n");
    printf("Where do you want to go now?\n\n");
    current_loc = 5; 
}

void loc_upstairs()
{
    printf("UPSTAIRS\n\n");
    printf("You aren't allowed up here!\n\n");
}

// TITLE SCREEN & INTRODUCTION
void startUp()
{
    // TITLE SCREEN
    printf("\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\t\t\t G H O S T   M A N O R\n");
    printf("\n");
    printf("\t\tA  T E X T - A D V E N T U R E  G A M E    \n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");

    // INTRODUCTION
    printf("\nIt's a cold day, mid december and you find yourself wandering outside. In the distance you see a mansion.\n\n");
}
