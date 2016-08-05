//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  CODE FILENAME: Batzel-assn2-prog.cpp
//  DESCRIPTION:   this program reads and organizes numbers into two doubly linked lists, odd and even numbers, and will search through those lists from inputs
//                 given by a file displaying data about the location of the found numbers
//  CLASS/TERM:    CS372 / 14S8W2
//  DESIGNER:      Andrew C. Batzel
//  FUNCTIONS:     main- declares the major variables used in the program and makes calls to other functions
//                 fileValidation- ensures that the input files are valid
//                 fileExists- test if a file exists
//                 initializeList- initializes a count of zero and a listTop pointer to NULL in a structure node
//                 listController- tests an integer from file stream to be even or odd and adds nodes that are doubly linked to each list
//                 isUnique- traverses a linked list searching for an integer equal one passed into it
//                 emptyList- tests if a list contains any nodes
//                 fullList- tests if more nodes can be added into a list
//                 numInList- traverses a linked list looking for a target value to return a pointer to it
//                 orderedListInsertion- traverses a linked list to insert a node in ascending order
//                 listLength- traverses a linked list counting the nodes
//                 positionInList- traverses a linked list from a pointer to the top node counting as it passes each node returning its result
//                 displayList- outputs to the display the contents of the odd and even lists and the results of finding integers from second file stream
//                 destroyList- deletes linked lists
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstddef>
#include<stdlib.h>

using namespace std;

// constants
const int MAX_DISP_COLUMN = 10;       // used to display numbers in a list in a set amount of columns
enum commandLineIdx {FILENAME = 1, INPUTFILE1, INPUTFILE2};   // used for validating files in command line
enum fileIdx {FILE1 = 1, FILE2};     // used to index character array pointer in command line
struct numList {
       int number;
       numList *prevPtr;
       numList *nextPtr;
       };
struct numData {
       int length;
       numList *listTop;
       };
// function prototypes
void fileValidation(int argc, char *argv[], ifstream& dataIn1, ifstream& dataIn2, string& inputFile1, string& inputFile2);
bool fileExists(string fileName, ifstream& dataIn);
void initializeList(numData *&initialNum);
void listController(numData *&evenNums, numData *&oddNums, ifstream& dataIn1);
bool isUnique(numData *uniqueNum, int number);
bool emptyList(numData *numPtr);
bool fullList();
numList *numInList(numData *numPtr, int target);
bool orderedListInsertion(numData *&numPtr, int number);
int listLength(numData *numPtr);
int positionInList(numData *numPtr, int target);
void displayList(numData *evenNums, numData *oddNums, ifstream& dataIn2, string fileName1, string fileName2);
void destroyList(numData *&numKill);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     main
//  DESCRIPTION:  the main function recieves files via the command line and makes calls to other functions
//  INPUT:
//     Parameter:   argc
//                  argv
//  OUTPUT:
//       Return Val:   Returns the value 0 upon sucessful completion of the program.
//  CALLS TO:     fileValidation
//                fullList
//                initializeList
//                listController
//                displayList
//                emptyList
//                destroyList
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    ifstream dataIn1,           // file stream variable to read first file in command line
             dataIn2;           // file stream variable to read second file in command line
    string inputFile1,          // first file name used with dataIn1
           inputFile2;          // second file name used with dataIn2
    numData *evenNums,          // structure pointer for even numbers
            *oddNums;           // structure pointer for odd numbers
    
    // tests both input files and makes corrections if the files don't exist
    fileValidation(argc, argv, dataIn1, dataIn2, inputFile1, inputFile2);
    
    // create even and odd lists if memory exists to create them
    if (!fullList())
       initializeList(evenNums);
    if (!fullList())
       initializeList(oddNums);
    
    system("cls");
    cout << "Doubly Linked List Program" << endl << endl;
    
    // load data from first input file into lists
    listController(evenNums, oddNums, dataIn1);
    
    // display list data if memory heap failure didn't occur
    if (!fullList())  
       displayList(evenNums, oddNums, dataIn2, inputFile1, inputFile2);
    else
        cout << "ERROR - Memory heap error has occured" << endl;
    
    // delete lists if they aren't empty
    if (!emptyList(evenNums))
       destroyList(evenNums);
       
    if (!emptyList(oddNums))
       destroyList(oddNums);
    
    system("pause");
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     fileValidation
//  DESCRIPTION:  tests if files provided exist and loops until valid files are provided
//  INPUT:
//     Parameter:   argc
//                  argv
//                  dataIn1
//                  dataIn2
//                  inputFile1
//                  inputFile2
//  OUTPUT:
//       Parameter:   dataIn1
//                    dataIn2
//                    inputFile1
//                    inputFile2
//  CALLS TO:     fileExists
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void fileValidation(int argc, char *argv[], ifstream& dataIn1, ifstream& dataIn2, string& inputFile1, string& inputFile2)
{
      bool firstInput = false,     // used 
           secondInput = false;
    
    if (argc == FILENAME)
        cout << "ERROR - must enter 2 input files as a command line arguments." << endl;
    else if (argc == INPUTFILE1)
    {
         cout << "ERROR - missing second file from command line." << endl;
         inputFile1 = argv[FILE1];
         firstInput = fileExists(inputFile1, dataIn1);
    } // only 1 file in command line
    else if (argc == INPUTFILE2)
    {
       inputFile1 = argv[FILE1];
       firstInput = fileExists(inputFile1, dataIn1);
       
       inputFile2 = argv[FILE2];
       secondInput = fileExists(inputFile2, dataIn2);
    } // 2 files in command line
    // more than 2 files in command line
    else
        cout << "ERROR - Too many files in command line." << endl;
    
    if (!firstInput)
    {
       do
       {
          cout << "Enter a file name to import a list of numbers: ";
          cin >> inputFile1;
          firstInput = fileExists(inputFile1, dataIn1);
          if (!firstInput)
             cout << "ERROR - " << inputFile1 << " doesn't exist" << endl;
       } while (!firstInput);
    } // fist input file not in command line or doesn't exist
    
    if (!secondInput)
    {
       do
       {
          cout << "Enter a file name to find a list of numbers from " << inputFile1 << ": ";
          cin >> inputFile2;
          secondInput = fileExists(inputFile2, dataIn2);
          if (!secondInput)
             cout << "ERROR - " << inputFile2 << " doesn't exist" << endl;
       } while (!secondInput);
    } // second input file not in command line or doesn't exist
    
 
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     fileExists
//  DESCRIPTION:  tests if file provided can be opened in file stream
//  INPUT:
//     Parameter:   fileName
//                  dataIn
//  OUTPUT:
//       Return Val:   fileExists
//       Parameter:    dataIn
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool fileExists(string fileName, ifstream& dataIn)
{
     bool fileExists;
     
     dataIn.clear();
     dataIn.open (fileName.c_str());
     if (dataIn)
        fileExists = true;
     else
         fileExists = false;

     return fileExists;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     initializeList
//  DESCRIPTION:  creates a new list and initializes it to a length of zero and the top pointer to NULL
//  INPUT:
//     Parameter:   initialNum
//  OUTPUT:
//       Parameter: initialNum     
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void initializeList(numData *&initialNum)
{     
     initialNum = new (nothrow) numData;
     
     initialNum->length = 0;
     initialNum->listTop = NULL;
    
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     listController
//  DESCRIPTION:  reads an integer from the file stream and makes calles based on whether that integer is odd or even
//  INPUT:
//     Parameter:   evenNums
//                  oddNums
//                  dataIn1
//  OUTPUT:
//       Parameter: evenNums
//                  oddNums
//                  dataIn1
//  CALLS TO:     fullList
//                orderesListInsertion
//                isUnique
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void listController(numData *&evenNums, numData *&oddNums, ifstream& dataIn1)
{
     int number;
     
     dataIn1 >> number;
     
     cout << "Reading input" << endl;

     while (dataIn1 && !fullList())
     {
           if ((number % 2 == 0))
           {
              if (isUnique(evenNums, number))
              {
                 orderedListInsertion(evenNums, number);
              } // end if number is unique
              else
                  cout << setw(6) << right << number << left << " is a duplicate and will be ignored." << endl;
           } // end if number is even
           else
           {
              if (isUnique(oddNums, number))
              {
                 orderedListInsertion(oddNums, number);
              } // end if number is unique
              else
                  cout << setw(6) << right << number << left << " is a duplicate and will be ignored." << endl;
           } // end else number is odd
           
           dataIn1 >> number;
     } // end while there is data in the first input file and memory can allocate new nodes
     dataIn1.close();
     
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     isUnique
//  DESCRIPTION:  tests if an integer from file stream already exists in linked list
//  INPUT:
//     Parameter:    uniqueNum
//                   number
//  OUTPUT:
//       Return Val: unique    
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool isUnique(numData *uniqueNum, int number)
{
     bool unique = true;
     
     numList *nextNum;
     
     nextNum = uniqueNum->listTop;
     
     while ((nextNum != NULL) && unique)
     {
           if (nextNum->number == number)
              unique = false;
           else
           {
               nextNum = nextNum->nextPtr;
           }
     } // end while the top of odds list isn't empty and the number is unique
     
     return unique;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     emptyList
//  DESCRIPTION:  tests if a linked list contains any nodes
//  INPUT:
//     Parameter:   numPtr
//  OUTPUT:
//       Return Val:   isEmpty
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool emptyList(numData *numPtr)
{
     bool isEmpty;
     
     numPtr->listTop == NULL ? isEmpty = true: isEmpty = false;
     
     return isEmpty;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     fullList
//  DESCRIPTION:  tests if memory can allocate a new node into a linked list
//  INPUT:
//     Parameter:   numPtr
//  OUTPUT:
//       Return Val:   heapError
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool fullList()
{
     bool heapError;
     numList *newNode;
     
     newNode = new(nothrow) numList;
     
     if (!newNode)
        heapError = true;
     else
         heapError = false;

     delete newNode;
     
     return heapError;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     numInList
//  DESCRIPTION:  finds a target value in a linked list and returns a pointer to the node that contains that integer or NULL if target is not found
//  INPUT:
//     Parameter:   numPtr
//                  target
//  OUTPUT:
//       Return Val:   findNum   
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

numList *numInList(numData *numPtr, int target)
{
     numList *findNum;
     
     findNum = numPtr->listTop;

     while ((findNum->number != target) && (findNum->nextPtr != NULL))  
           findNum = findNum->nextPtr;
     if (findNum->number != target)
        findNum = NULL;

     return findNum;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     orderedListInsertion
//  DESCRIPTION:  inserts a new node containing an integer and two pointers, forward and backward, into ascending order
//  INPUT:
//     Parameter:   numPtr
//                  number
//  OUTPUT:
//       Return Val:   newNodePass
//       Parameter: numPtr
//  CALLS TO:     fullList
//                emptyList
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool orderedListInsertion(numData *&numPtr, int number)
{
     bool newNodePass,
          endList = false;
     
     numList *tempPtr,
              *newNode;

    if (!fullList())
    {
        tempPtr = numPtr->listTop;
        newNode = new (nothrow) numList;
        newNode->number = number;
        if (emptyList(numPtr))
        {
           newNode->prevPtr = NULL;
           newNode->nextPtr = NULL;
           numPtr->listTop = newNode;
           numPtr->length++;
        } // end if the list is empty
        else if (tempPtr->number > number)
        {
             newNode->prevPtr = NULL;
             tempPtr->prevPtr = newNode;
             newNode->nextPtr = tempPtr;
             numPtr->listTop = newNode;
             numPtr->length++;
        } // end if new number is less than the first element
        else
        {           
            while ((tempPtr->nextPtr != NULL) && (tempPtr->number < number))
                  tempPtr = tempPtr->nextPtr;
            
            if (tempPtr->number < number)
            {
               tempPtr->nextPtr = newNode;
               newNode->nextPtr = NULL;
               newNode->prevPtr = tempPtr;
            } // end if number in current node is less than number from file stream
            else
            {
                tempPtr->prevPtr->nextPtr = newNode;
                newNode->prevPtr = tempPtr->prevPtr;
                tempPtr->prevPtr = newNode;
                newNode->nextPtr = tempPtr;
            } // end if number in current not is greater than number from file stream
            numPtr->length++;
        } // end else the new number is not to be placed in the first element
        newNodePass = true;
    } // end if the list isn't full
    else
        newNodePass = false;
   
     return newNodePass;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     listLength
//  DESCRIPTION:  traverses a linked list counting how many nodes it contains
//  INPUT:
//     Parameter:   numPtr
//  OUTPUT:
//       Return Val:   returnLength 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

int listLength(numData *numPtr)
{
     int returnLength = 0;
     numList *tempPtr;
     
     tempPtr = numPtr->listTop;
     
     while (tempPtr != NULL)
     {
           tempPtr = tempPtr->nextPtr;
           returnLength++;
     }
     
     
     return returnLength;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     positionInList
//  DESCRIPTION:  traverses a linked list from a pointer to the top node counting as it passes each node returning its result
//  INPUT:
//     Parameter:   numPtr
//                  target
//  OUTPUT:
//       Return Val:   returnVal
//  CALLS TO:     
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

int positionInList(numData *numPtr, int target)
{
     numList *tempPtr;
     int returnVal = 0;
     
    tempPtr = numInList(numPtr, target);
    while (tempPtr != NULL)
    {
          tempPtr = tempPtr->prevPtr;
          returnVal++;
    }

     return returnVal;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     displayList
//  DESCRIPTION:  outputs to the display the contents of the odd and even lists and the results of finding integers from second file stream
//  INPUT:
//     Parameter:   evenNums
//                  oddNums
//                  dataIn2
//                  fileName1
//                  fileName2
//     File:        inputFile2
//  OUTPUT:
//       Parameter: dataIn2
//  CALLS TO:     emptyList
//                listLength
//                positionInList
//                
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void displayList(numData *evenNums, numData *oddNums, ifstream& dataIn2, string fileName1, string fileName2)
{
     numList *oddPtr,
             *evenPtr;
     int tempTarget = 0;
     
     int columns = 0;
     oddPtr = oddNums->listTop;
     evenPtr = evenNums->listTop;
     
     cout << "\nLists created using input file " << fileName1 << ":" << endl << endl
          << setw(5) << right << listLength(oddNums) << left << " integers inserted into Odd list:" << endl
          << setw(5) << "";
     
     if (emptyList(oddNums))
        cout << "Odd list is empty" << endl;
     else
     {
         while (oddPtr != NULL)
         {
               if ((columns % 10 == 0) && (columns != 0))
               {
                  columns = 0;
                  cout << endl << setw(5) << "";
               } // end if 10 numbers displayed in one line
               cout << setw(5) << right << oddPtr->number;
               columns++;
               oddPtr = oddPtr->nextPtr;
         } // end while odd pointer is not NULL
     } // end if odd list is not empty
     
     columns = 0;
     cout << endl << endl
          << setw(5) << right << listLength(evenNums) << left << " integers inserted into Even list:" << endl
          << setw(5) << "";
     
     if (emptyList (evenNums))
        cout << "Even list is empty" << endl;
     else
     {
         while (evenPtr != NULL)
         {
               if ((columns % 10 == 0) && (columns != 0))
               {
                  columns = 0;
                  cout << endl << setw(5) << "";
               } // end if 10 numbers displayed in one line
               cout << setw(5) << right << evenPtr->number;
               columns++;
               evenPtr = evenPtr->nextPtr;
         } // end while even pointer is not NULL
     } // end if even list is not empty
     cout << endl << endl
          << "Search results from input file " << fileName2 << ":" << endl << endl;
     
     dataIn2 >> tempTarget;

     while (dataIn2)
     {
           cout << setw(4) << "" << tempTarget;
           if ((tempTarget % 2 == 0) && emptyList(evenNums))
              cout << " not found because even list is empty" << endl;
           else if ((tempTarget % 2 == 0) && (positionInList(evenNums, tempTarget) == 0))
                  cout << " not found in even list" << endl;           
           else if ((tempTarget % 2 == 0) && (positionInList(evenNums, tempTarget) != 0))
                cout << " found " << positionInList(evenNums, tempTarget) << " numbers down in the even list" << endl;
           else if ((tempTarget % 2 != 0) && emptyList(oddNums))
                cout << " not found because odd list is empty" << endl;
           else if ((tempTarget % 2 != 0) && (positionInList(oddNums, tempTarget) == 0))   
                cout << " not found in odd list" << endl;
           else
               cout << " found " << positionInList(oddNums, tempTarget) << " numbers down in the odd list" << endl;
           dataIn2 >> tempTarget;
     } // end while there is data in second input file
     
     dataIn2.close();
     
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     destroyList
//  DESCRIPTION:  traverses a list removing the pointers until it has reaches a top node of NULL and deletes the list
//  INPUT:
//     Parameter:   numKill
//  OUTPUT:
//       Parameter: numKill   
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void destroyList(numData *&numKill)
{
     while (numKill->listTop != NULL)
           numKill->listTop = numKill->listTop->nextPtr;

           
     delete numKill;
     
     return;
}
