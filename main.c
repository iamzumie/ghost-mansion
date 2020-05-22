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
void showlocations();
void endGame();
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
    {"hallway", "HALL"},
    {"kitchen", "KITCHEN"},
    {"living room", "LIVING"},
    {"toilet", "TOILET"},
    {"upstairs", "UPSTAIRS"},
};
void loc_kitchen();
void loc_living();
void loc_hall();
void loc_toilet();
void loc_upstairs();
void loc_firstFloor();

// INIT
int answer, location, current_loc;
int bullets, key, gun = 0;
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
    printf("You enter the mansion, it's too dark inside to see anything.\n");
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
        case 1: loc_hall(); break;
        case 2: loc_kitchen();break;
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
        {
            printf("You are already standing in the %s.\n\n", locs[answer].description);
            command();
            read = fWord();
        }
        else
        {
        switch (answer) // GO TO THE LOCATIONS
            {
                case 1: 
                    printf("You entered the hall.\n");
                    loc_hall(); break;
                case 2: 
                    printf("You entered the kitchen.\n");
                    loc_kitchen();break;
                case 3: 
                    printf("You entered the living room.\n");
                    loc_living(); break;
                case 4: 
                    loc_toilet(); break;
                case 5: 
                    loc_upstairs(); break;
                default:
                    printf("Come again?\n");
            }
        }
    } while (gun != 2 || answer == 5 || current_loc == answer);
}

void showlocations() 
{
    for (int i = 1; i < 6; i++)
    {
        printf("%i. %s\n", i,  locs[i].desc_upper);
    }   
}

void loc_hall()
{
    current_loc = 1; // ADD LOCATION
    printf("You have access to the kitchen, toilet, living room & upstairs.\n\n");
    while (1)
    {
        command();
        read = fWord();

        if (strcasecmp(read, "go") == 0) 
        {
            map();
            break;
        } 
        else
        {
            printf("I don't know the word %s.\n\n", read);
        }
    }
}

void loc_kitchen()
{
    current_loc = 2; // ADD LOCATION
    printf("There are several cupboards and drawers ajar, there's also a weird\n");
    printf("smell coming from the fridge.\n\n");

    while (1)
    {
        command();
        read = fWord();

        if (strcasecmp(read, "search") == 0) 
        {
            if (gun == 1) {
                printf("You filled your shotgun with bullets.\n\n");
                gun = 2;
                printf("When you put the bullets in the gun, you hear a door being slammed shut upstairs.\n");
                loc_upstairs();
            }
            else if (gun == 2){
                printf("You already have ammo.\n\n");
            }
            else 
            {
                printf("In one of the drawers you found some salt bullets. These might come in handy!\n\n");
                bullets = 1;
            }
            
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
        else
        {
            printf("I don't know the word %s.\n\n", read);
        }
    }
}

void loc_living()
{
    current_loc = 3; // ADD LOCATION
    printf("The furniture is covered with white cloth, but the colour has become\n");
    printf("yellow out of age. The carpet has blood and dirt stains on it.\n");
    printf("Above the fireplace you see a double-barreled shotgun.\n\n");
    
    while (1)
    {
        command();
        read = fWord();

        if (strcasecmp(read, "take") == 0) 
        {
            function("take", "gun");
            if (bullets) {
                printf("You got yourself a gun, you filled it up with the salt bullets you found in the kitchen.\n");
                gun = 2;
                printf("When you put the bullets in the gun, you hear a door being slammed shut upstairs.\n");
                printf("You run upstairs. ");
                loc_upstairs();
            }
            else {
                printf("Hmm empty, we need some find some bullets.\n\n");
                gun = 1;
            }
        }
        else if (strcasecmp(read, "go") == 0) 
        {
            map();
            break;
        } 
        else
        {
            printf("I don't know the word %s.\n\n", read);
        }
    }
}

void loc_toilet()
{
    current_loc = 4; 
    printf("You sure have a small bladder, couldn't you go before we started playing?\n\n");
    while (1)
    {
        command();
        read = fWord();

        if (strcasecmp(read, "go") == 0) 
        {
            map();
            break;
        } 
        else
        {
            printf("I don't know the word %s.\n\n", read);
        }
    }
}

void loc_upstairs()
{
    current_loc = 5; 
    if (gun != 2) {
        printf("Maybe we need to find something to defend ourself first.\n\n");
    }
    else
    {
        printf("Which door do you want to take? Left or right?\n\n");
        loc_firstFloor();
    }
}

void loc_firstFloor()
{   
    while (1)
    {
        command();
        read = fWord();

        if (strcasecmp(read, "left") == 0) 
        {
            printf("You entered a bedroom, there's a closet in there.\n\n");

            command();
            read = fWord();
            if (strcasecmp(read, "leave") == 0) 
            {
                loc_upstairs();
                break;
            } 
            else if (strcasecmp(read, "open") == 0) 
            {
                function("open", "closet");
                endGame();
            }
            else
            {
                printf("I don't know the word %s.\n\n", read);
            }
        }
        else if (strcasecmp(read, "right") == 0) 
        {
            printf("It's locked.\n\n");
        } 
        else
        {
            printf("I don't know the word %s.\n\n", read);
        }
    }
}
void endGame()
{   
    printf("You open the closet door, which makes a huge creaking sound.\n");
    printf("Sombody heard it and is coming toward your room! What do you do?\n\n");
    while (1)
    {
        command();
        read = fWord();

        if (strcasecmp(read, "hide") == 0) 
        {
            printf("You hide in the closet.\n\n");
            printf("The person goes back downstairs.");
            break;
        }
        else
        {
            printf("I don't know the word %s.\n\n", read);
        }
    }
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
    printf("Find out if the stories are true are if it's just a hoax.\n"); 
    printf("You can direct me with the use of some basic words.\n\n\n");
    printf("You stand in front of the mansion, there is a sign on the door.\n\n");
}