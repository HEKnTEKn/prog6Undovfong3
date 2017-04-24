/*
	Author: Victor Fong
	Assignment: #5, Word Across
	TA: Nianzu Ma, Tues 4PM
	Apr 7, 2017
*/


//include libraries, define global variables,set namespace to reduce cout/in redundancy
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <ctime>
using namespace std;	//set namespace for ease of cin/cout

#define RANDOM 0


/*---------------------------------------------------------------------------STRUCT CREATION---------------------------------------------------------------------------------------------*/


typedef struct Node
{
	int moveNumber = 0;
    char table[16] = {0};
    struct Node *pNext = NULL;
} Node;


/*----------------------------------------------------------------------------PROTOTYPES-------------------------------------------------------------------------------------------------*/


char correctUpper(char input);        //corrects a lowercase character into a capital character for ease of use
char getInput(void);     //function acquires input of a character as prompted by other functions.
//function that takes character variable and checks for for 'x' or 'X'. if true than the function will return an integer of 1 to main(), which will then break away from the loop.
int randomize(int min, int max);	//function uses rand() to supply a random number
void displayInformationAndInstructions(void);		//function displays user info and instructions
int promptAndScanForDictionarySize(char selection);		//function asks for dictionary size and returns userInput
int analyzeDictionary(FILE* dictionaryChosen);	//function gets word count of file
void setDictionary(FILE* dictionaryChosen, char**& dictionaryPTR, int fourWordCount); // function allocates memory for dictionary
void scanFileTo(FILE* file, char**& dictionaryPTR);		//function scans a string from file and copies to dictionary
void shiftRowCol(int numberOfShifts,
                 char *first, char *second, char *third, char *fourth);		//function shifts a row or column by a given distance
void swapRow(char* characters, char row, int distance);		//function sets up a row to be swapped by shiftRowCol()
void swapColumn(char* characters, char column, int distance);		//function sets up a column to be swapped by shiftRowCol()
void mixWords(char *characters);		//function is the master for all row and column shifts found above
void displayTable(char c1,  char c2,  char c3,  char c4,    //displays table
                  char c5,  char c6,  char c7,  char c8,
                  char c9,  char c10, char c11, char c12,
                  char c13, char c14, char c15, char c16);
void promptScanAndReact(char *input1, int *input2, char *characters, char *originalCharacters);		//prompts for move and reacts accordingly
int checkForWinner(char**& dictionaryPTR,int wordOnePos, int wordTwoPos, int wordThreePos, int wordFourPos, char c1,  char c2,  char c3,  char c4,  //function checks for winner
                   char c5,  char c6,  char c7,  char c8,
                   char c9,  char c10, char c11, char c12,
                   char c13, char c14, char c15, char c16);


/*------------------------------------------------------------------------------------CODE-----------------------------------------------------------------------------------------------*/


int main()		//main
{
//initialize variables
    FILE* smallDictionary  = NULL;
    FILE* mediumDictionary = NULL;
    FILE* largeDictionary  = NULL;

    Node *pNode = new Node;
    Node *pTemp;


    char **dictionaryPTR;


    char inputChar = 0;
    char selection = 0;

    char characters[16] = { 0 };
    char originalCharacters[16] = { 0 };

    int exitCondition = -1;
    int inputInt = 0;
    int fourWordCount = 0;
    int wordOnePos, wordTwoPos, wordThreePos, wordFourPos;

    displayInformationAndInstructions();

    switch (promptAndScanForDictionarySize(selection))
    {
/*-------------------------------------------------------------------------------*/
        case 'X':
        {
            cout << "You chose X to exit" << endl;
            return -1;
        }
/*-------------------------------------------------------------------------------*/
        case '1':	//small dictionary
        {
            srand((unsigned int) time(NULL));
	        //set files to corresponding txt file
            smallDictionary = fopen("smallDictionary.txt", "r");
            //check if file opened correctly
            if (smallDictionary == NULL)
            {
                cout << "Could not open dictionary.\n";
                return -1;	 //-1 indicates error
            }
            //get fourWordCount then close file
            fourWordCount = analyzeDictionary(smallDictionary);
            rewind(smallDictionary);
           //create dynamic array
            setDictionary(smallDictionary, dictionaryPTR, fourWordCount);
	        //rescan file into the created array.
            scanFileTo(smallDictionary, dictionaryPTR);
            //choose 4 indexes to be used as the words for our table.
            wordOnePos   = randomize(0, fourWordCount); wordTwoPos  = randomize(0, fourWordCount);
            wordThreePos = randomize(0, fourWordCount); wordFourPos = randomize(0, fourWordCount);
            //set table characters equal to random mixed index words while also saving the original table
            originalCharacters[ 0] = characters[ 0] = dictionaryPTR[  wordOnePos][0]; originalCharacters[ 1] = characters[ 1] = dictionaryPTR[  wordOnePos][1]; originalCharacters[ 2] = characters[ 2] = dictionaryPTR[  wordOnePos][2]; originalCharacters[ 3] = characters[ 3] = dictionaryPTR[  wordOnePos][3];
            originalCharacters[ 4] = characters[ 4] = dictionaryPTR[  wordTwoPos][0]; originalCharacters[ 5] = characters[ 5] = dictionaryPTR[  wordTwoPos][1]; originalCharacters[ 6] = characters[ 6] = dictionaryPTR[  wordTwoPos][2]; originalCharacters[ 7] = characters[ 7] = dictionaryPTR[  wordTwoPos][3];
            originalCharacters[ 8] = characters[ 8] = dictionaryPTR[wordThreePos][0]; originalCharacters[ 9] = characters[ 9] = dictionaryPTR[wordThreePos][1]; originalCharacters[10] = characters[10] = dictionaryPTR[wordThreePos][2]; originalCharacters[11] = characters[11] = dictionaryPTR[wordThreePos][3];
            originalCharacters[12] = characters[12] = dictionaryPTR[ wordFourPos][0]; originalCharacters[13] = characters[13] = dictionaryPTR[ wordFourPos][1]; originalCharacters[14] = characters[14] = dictionaryPTR[ wordFourPos][2]; originalCharacters[15] = characters[15] = dictionaryPTR[ wordFourPos][3];

            mixWords(characters);

            do
            {
                displayTable(characters[0], characters[1], characters[ 2], characters[ 3], characters[ 4], characters[ 5], characters[ 6], characters[ 7],
                             characters[8], characters[9], characters[10], characters[11], characters[12], characters[13], characters[14], characters[15]);

                promptScanAndReact(&inputChar, &inputInt, characters, originalCharacters);

                exitCondition = checkForWinner(dictionaryPTR, wordOnePos, wordTwoPos, wordThreePos, wordFourPos, characters[ 0], characters[ 1], characters[ 2], characters[ 3],
                                               characters[ 4], characters[ 5], characters[ 6], characters[ 7],
                                               characters[ 8], characters[ 9], characters[10], characters[11],
                                               characters[12], characters[13], characters[14], characters[15]);

            } while (exitCondition != 1);

            fclose(smallDictionary);
            break;
        }
/*--------------------------------------------------------------------------------*/
        case '2':	//medium dictionary
        {
            //SEED
	        srand((unsigned int) time(NULL));
            //set files to corresponding txt file
            mediumDictionary = fopen("mediumDictionary.txt", "r");
            //check if file opened correctly
            if (mediumDictionary == NULL)
            {
                cout << "Could not open dictionary.\n";
                return -1;	// -1 indicates error
            }
            //get fourWordCount then close file
            fourWordCount = analyzeDictionary(mediumDictionary);
            rewind(mediumDictionary);
            //create dynamic array
            setDictionary(mediumDictionary, dictionaryPTR, fourWordCount);
            //rescan file into the created array.
            scanFileTo(mediumDictionary, dictionaryPTR);
            //choose 4 indexes to be used as the words for our table.
            wordOnePos   = randomize(0, fourWordCount); wordTwoPos  = randomize(0, fourWordCount);
            wordThreePos = randomize(0, fourWordCount); wordFourPos = randomize(0, fourWordCount);
            //set table characters equal to random mixed index words while also saving the original table
            originalCharacters[ 0] = characters[ 0] = dictionaryPTR[wordOnePos][0]  ; originalCharacters[ 1] = characters[ 1] = dictionaryPTR[wordOnePos][1]  ; originalCharacters[ 2] = characters[ 2] = dictionaryPTR[wordOnePos][2]  ; originalCharacters[ 3] = characters[ 3] = dictionaryPTR[wordOnePos][3]  ;
            originalCharacters[ 4] = characters[ 4] = dictionaryPTR[wordTwoPos][0]  ; originalCharacters[ 5] = characters[ 5] = dictionaryPTR[wordTwoPos][1]  ; originalCharacters[ 6] = characters[ 6] = dictionaryPTR[wordTwoPos][2]  ; originalCharacters[ 7] = characters[ 7] = dictionaryPTR[wordTwoPos][3]  ;
            originalCharacters[ 8] = characters[ 8] = dictionaryPTR[wordThreePos][0]; originalCharacters[ 9] = characters[ 9] = dictionaryPTR[wordThreePos][1]; originalCharacters[10] = characters[10] = dictionaryPTR[wordThreePos][2]; originalCharacters[11] = characters[11] = dictionaryPTR[wordThreePos][3];
            originalCharacters[12] = characters[12] = dictionaryPTR[wordFourPos][0] ; originalCharacters[13] = characters[13] = dictionaryPTR[wordFourPos][1] ; originalCharacters[14] = characters[14] = dictionaryPTR[wordFourPos][2] ; originalCharacters[15] = characters[15] = dictionaryPTR[wordFourPos][3] ;

            mixWords(characters);

            do
            {
                displayTable(characters[0], characters[1], characters[ 2], characters[ 3], characters[ 4], characters[ 5], characters[ 6], characters[ 7],
                             characters[8], characters[9], characters[10], characters[11], characters[12], characters[13], characters[14], characters[15]);

                promptScanAndReact(&inputChar, &inputInt, characters, originalCharacters);

                exitCondition = checkForWinner(dictionaryPTR, wordOnePos, wordTwoPos, wordThreePos, wordFourPos, characters[ 0], characters[ 1], characters[ 2], characters[ 3],
                                               characters[ 4], characters[ 5], characters[ 6], characters[ 7],
                                               characters[ 8], characters[ 9], characters[10], characters[11],
                                               characters[12], characters[13], characters[14], characters[15]);

                exitCondition = 1;
            } while (exitCondition != 1);

            fclose(mediumDictionary);
            break;
        }
/*--------------------------------------------------------------------------------*/
        case '3':	//large dictionary
        {
            srand((unsigned int) time(NULL));
            //set files to corresponding txt file
            largeDictionary = fopen("largeDictionary.txt", "r");
            //check if file opened correctly
            if (largeDictionary == NULL)
            {
                cout << "Could not open dictionary.\n";
                return -1;	// -1 indicates error
            }
            //get fourWordCount then close file
            fourWordCount = analyzeDictionary(largeDictionary);
            rewind(largeDictionary);
            //create dynamic array
            setDictionary(largeDictionary, dictionaryPTR, fourWordCount);
            //rescan file into the created array.
            scanFileTo(largeDictionary, dictionaryPTR);
            //choose 4 indexes to be used as the words for our table.
            wordOnePos   = randomize(0, fourWordCount); wordTwoPos  = randomize(0, fourWordCount);
            wordThreePos = randomize(0, fourWordCount); wordFourPos = randomize(0, fourWordCount);
            //set table characters equal to random mixed index words while also saving the original table
            originalCharacters[ 0] = characters[ 0] = dictionaryPTR[wordOnePos][0]  ; originalCharacters[ 1] = characters[ 1] = dictionaryPTR[wordOnePos][1]  ; originalCharacters[ 2] = characters[ 2] = dictionaryPTR[wordOnePos][2]  ; originalCharacters[ 3] = characters[ 3] = dictionaryPTR[wordOnePos][3]  ;
            originalCharacters[ 4] = characters[ 4] = dictionaryPTR[wordTwoPos][0]  ; originalCharacters[ 5] = characters[ 5] = dictionaryPTR[wordTwoPos][1]  ; originalCharacters[ 6] = characters[ 6] = dictionaryPTR[wordTwoPos][2]  ; originalCharacters[ 7] = characters[ 7] = dictionaryPTR[wordTwoPos][3]  ;
            originalCharacters[ 8] = characters[ 8] = dictionaryPTR[wordThreePos][0]; originalCharacters[ 9] = characters[ 9] = dictionaryPTR[wordThreePos][1]; originalCharacters[10] = characters[10] = dictionaryPTR[wordThreePos][2]; originalCharacters[11] = characters[11] = dictionaryPTR[wordThreePos][3];
            originalCharacters[12] = characters[12] = dictionaryPTR[wordFourPos][0] ; originalCharacters[13] = characters[13] = dictionaryPTR[wordFourPos][1] ; originalCharacters[14] = characters[14] = dictionaryPTR[wordFourPos][2] ; originalCharacters[15] = characters[15] = dictionaryPTR[wordFourPos][3] ;

            mixWords(characters);

            do
            {
                displayTable(characters[0], characters[1], characters[ 2], characters[ 3], characters[ 4], characters[ 5], characters[ 6], characters[ 7],
                             characters[8], characters[9], characters[10], characters[11], characters[12], characters[13], characters[14], characters[15]);

                promptScanAndReact(&inputChar, &inputInt, characters, originalCharacters);

                exitCondition = checkForWinner(dictionaryPTR, wordOnePos, wordTwoPos, wordThreePos, wordFourPos, characters[ 0], characters[ 1], characters[ 2], characters[ 3],
                                               characters[ 4], characters[ 5], characters[ 6], characters[ 7],
                                               characters[ 8], characters[ 9], characters[10], characters[11],
                                               characters[12], characters[13], characters[14], characters[15]);

                exitCondition = 1;
            } while (exitCondition != 1);

            fclose(largeDictionary);
            break;
        }
/*------------------------------------------------------------------------------------*/
        default:
        {
            cout << "ERROR : INCORRECT RESPONSE CHOSEN. PLEASE TRY AGAIN" << endl;
            break;
        }
    }

    cout << "End of program. Thank you for your time!" << endl; return 0;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char correctUpper(char input)        //corrects a lowercase character into a capital character for ease of use
{
    if (input >= 97 && input <= 122)
    {
        input -= 32;
    }
    return input;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char getInput(void)     //function acquires input of a character as prompted by other functions.
{
    char input = ' ';
    scanf(" %c", &input);

    return input;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int randomize(int min, int max)	//function uses rand() to supply a random number
{ return rand() % max + min; }


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void displayInformationAndInstructions(void)		//function displays user info and instructions
{
    cout << "Author: Victor Fong \n"
         << "Assignment: #5, Word Across \n"
         << "TA: Nianzu Ma, Tues 4PM \n"
         << "Apr 7, 2017\n"
         << endl;
    cout << "Welcome to AcrossWord puzzle, where you rotate rows or columns \n"
         << "to restore the board back to containing four words. Each move is \n"
         << "a row or column letter followed by the rotation distance 1,2 or 3. \n"
         << endl;
    cout << "When prompted to provide input you may enter:\n"
         << "	Enter 'r' to reset the board to user-defined values.\n"
         << "	Enter 'u' to unveil the underlying words.\n"
         << "	Enter 's' to auto solve the board (Extra Credit)\n"
         << "	Enter 'x' to exit the program.\n"
         << endl;

    return;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int promptAndScanForDictionarySize(char selection)		//function asks for dictionary size and returns userInput
{
    cout << "Which dictionary size do you want? (1=small, 2=medium, 3=large): "; cin >> selection; cout << endl;
    selection = correctUpper(selection);
    return selection;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int analyzeDictionary(FILE* dictionaryChosen)	//function gets word count of file
{
    char tempWord[81];

    int fourWordCount = 0;

    while (fscanf(dictionaryChosen, "%s", &tempWord) != EOF)
    {
        if (strlen(tempWord) == 4)
        {
            fourWordCount++;
        }
    }
    cout << "There are " << fourWordCount << " 4-letter words." << "\n" << endl;

    return fourWordCount;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void setDictionary(FILE* dictionaryChosen, char**& dictionaryPTR, int fourWordCount) // function allocates memory for dictionary
{
    dictionaryPTR = new char*[fourWordCount];

    for (int i = 0; i < fourWordCount; ++i)
    {
        dictionaryPTR[i] = new char[5];
    }

    return;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scanFileTo(FILE* file, char**& dictionaryPTR)		//function scans a string from file and copies to dictionary
{
    int i = 0;
    char tempWord[81] = { 0 };

    while (fscanf(file, "%s", &tempWord) != EOF)
    {
        if (strlen(tempWord) == 4)
        {
            strcpy(dictionaryPTR[i], tempWord);
            i++;
        }
    }

    return;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void shiftRowCol(int numberOfShifts,
                 char *first, char *second, char *third, char *fourth)		//function shifts a row or column by a given distance
{
    char tempChar = 0;

    switch (numberOfShifts)
    {
        case 1:
        {
            tempChar  = *fourth;
            *fourth   = *third;
            *third    = *second;
            *second   = *first;
            *first    = tempChar;

            break;
        }
        case 2:
        {
            tempChar  = *first;
            *first    = *third;
            *third    = tempChar;

            tempChar  = *second;
            *second   = *fourth;
            *fourth   = tempChar;

            break;
        }
        case 3:
        {
            tempChar  = *first;
            *first    = *second;
            *second   = *third;
            *third    = *fourth;
            *fourth   = tempChar;

        }
        default: { break; }
    }

    return;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void swapRow(char* characters, char row, int distance)		//function sets up a row to be swapped by shiftRowCol()
{
    static int timesRan = 0;
    static char rowBuffer = 0;


    ++timesRan;
    if (row == RANDOM)
    {
        if (timesRan == 1)
        {
            rowBuffer = row = randomize(65, 4);
        }
        else
        {
            row = randomize(65, 4);

            if (row == rowBuffer)	//if statement checks if second selected row was already selected, if so it reruns function until it isn't
            {
                --timesRan;
                swapRow(characters, RANDOM, RANDOM);
            }
        }
        distance = randomize(1, 3);
    }

    switch (row)
    {	//							  random distance, [                      positions of chars                        ]
        case 'A': {shiftRowCol(distance , &characters[ 0], &characters[ 1], &characters[ 2], &characters[ 3]) ; break;}
        case 'B': {shiftRowCol(distance , &characters[ 4], &characters[ 5], &characters[ 6], &characters[ 7]) ; break;}
        case 'C': {shiftRowCol(distance , &characters[ 8], &characters[ 9], &characters[10], &characters[11]) ; break;}
        case 'D': {shiftRowCol(distance , &characters[12], &characters[13], &characters[14], &characters[15]) ; break;}

        default: { break; }
    }


    return;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void swapColumn(char* characters, char column, int distance)		//function sets up a column to be swapped by shiftRowCol()
{
    if (column == RANDOM)
    {
        column = randomize(69, 4);
        distance = randomize(1, 3);
    }

//move column once
    switch (column)
    {	//							random distance, [positions of chars]
        case 'E': {shiftRowCol(distance, &characters[0], &characters[4], &characters[ 8], &characters[12])   ; break;}
        case 'F': {shiftRowCol(distance, &characters[1], &characters[5], &characters[ 9], &characters[13])   ; break;}
        case 'G': {shiftRowCol(distance, &characters[2], &characters[6], &characters[10], &characters[14])   ; break;}
        case 'H': {shiftRowCol(distance, &characters[3], &characters[7], &characters[11], &characters[15])   ; break;}

        default: { break; }
    }

    return;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mixWords(char *characters)		//function is the master for all row and column shifts found above
{

    int randomOrder = randomize(1,3);

    switch (randomOrder)
    {
        case 1:
        {
            swapColumn(characters, RANDOM, RANDOM);
            swapRow(characters, RANDOM, RANDOM);
            swapRow(characters, RANDOM, RANDOM);
            break;
        }
        case 2:
        {
            swapRow(characters, RANDOM, RANDOM);
            swapColumn(characters, RANDOM, RANDOM);
            swapRow(characters, RANDOM, RANDOM);
            break;
        }
        case 3:
        {
            swapRow(characters, RANDOM, RANDOM);
            swapRow(characters,RANDOM, RANDOM);
            swapColumn(characters, RANDOM, RANDOM);
        }

        default: { break; }
    }


    return;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void displayTable(char c1,  char c2,  char c3,  char c4,
                  char c5,  char c6,  char c7,  char c8,
                  char c9,  char c10, char c11, char c12,
                  char c13, char c14, char c15, char c16)		//displays table
{
//printf over cout for ease of formatting!
    printf("\n");
    printf("   E F G H\n");
    printf("   -------\n");
    printf("A| %c %c %c %c\n", c1,  c2,  c3,  c4);
    printf("B| %c %c %c %c\n", c5,  c6,  c7,  c8);
    printf("C| %c %c %c %c\n", c9,  c10, c11, c12);
    printf("D| %c %c %c %c\n", c13, c14, c15, c16);
    printf("\n");

    return;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void promptScanAndReact(char *input1, int *input2, char *characters, char *originalCharacters)		//prompts for move and reacts accordingly
{
    static int moveCount = 0;

    cout << "\n" << ++moveCount << ". Enter the row or column to be rotated, and a number 1..3: ";

    *input1 = correctUpper(getInput());

    switch (*input1)
    {
        /*-----------------------------------------------------------------------------------*/
        case 'X':
        {
            cout << "\n.  . .Pressed X to exit. Thanks for playing!. .  ." << endl;
            exit(1);
        }
            /*-----------------------------------------------------------------------------------*/
        case 'U':
        {
            cout << "\nOriginal Table:\n" << endl;
            displayTable(originalCharacters[0], originalCharacters[1], originalCharacters[ 2], originalCharacters[ 3], originalCharacters[ 4], originalCharacters[ 5], originalCharacters[ 6], originalCharacters[ 7],
                         originalCharacters[8], originalCharacters[9], originalCharacters[10], originalCharacters[11], originalCharacters[12], originalCharacters[13], originalCharacters[14], originalCharacters[15]);
            cout << "\nMixed Table:\n"    << endl;

            return;
        }
            /*-----------------------------------------------------------------------------------*/
        case 'R':
        {
            int doneStat = -1;

            char buffer[16] = { 0 };
            cout << "\nPressed R to Reset . . . ";
            do
            {
                cout << "Please provide exactly 16 characters to set table to: "; cin >> buffer; cout << "\n" << endl;
                if (strlen(buffer) != 16)
                {
                    cout << "ERROR: INPUT NOT EQUAL TO 16, PLEASE TRY AGAIN!!!\n" << endl;

                    for (int i = 0; i < 16; ++i)
                    { buffer[i] = { 0 }; }

                    continue;
                }
                else { doneStat = 1; }
                strcpy(characters, buffer);

                displayTable(characters[0], characters[1], characters[ 2], characters[ 3], characters[ 4], characters[ 5], characters[ 6], characters[ 7],
                             characters[8], characters[9], characters[10], characters[11], characters[12], characters[13], characters[14], characters[15]);

                doneStat = 1;
            } while (doneStat == -1);

            return;
        }

            /*-----------------------------------------------------------------------------------*/
        case 'S':
        {
            cout << "Option not currently available. Check back again later!" << endl;
            break;
        }
            /*-----------------------------------------------------------------------------------*/
        default: { break; }
    }

    *input2 = getInput() - '0';	// - '0' turns char to proper int

    if (*input1 >= 'A' &&  *input1 <= 'D')
    {
        swapRow(characters, *input1, *input2);
    }
    else if (*input1 >= 'E' &&  *input1 <= 'H')
    {
        swapColumn(characters, *input1, *input2);
    }

    return;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int checkForWinner(char**& dictionaryPTR,int wordOnePos, int wordTwoPos, int wordThreePos, int wordFourPos, char c1,  char c2,  char c3,  char c4,
                   char c5,  char c6,  char c7,  char c8,
                   char c9,  char c10, char c11, char c12,
                   char c13, char c14, char c15, char c16)		//function checks for winner
{
    char wordOne  [5] = {  c1,  c2,  c3,  c4, '\0' };
    char wordTwo  [5] = {  c5,  c6,  c7,  c8, '\0' };
    char wordThree[5] = {  c9, c10, c11, c12, '\0' };
    char wordFour [5] = { c13, c14, c15, c16, '\0' };

    if ((strcmp(dictionaryPTR[  wordOnePos], wordOne  ) == 0) && (strcmp(dictionaryPTR[ wordTwoPos], wordTwo ) == 0) &&
        (strcmp(dictionaryPTR[wordThreePos], wordThree) == 0) && (strcmp(dictionaryPTR[wordFourPos], wordFour) == 0))
    {
        cout << "WOW!!! YOU WON! GREAT JOB!!!!" << endl;
        return 1;
    }
    else { return -1; }
}