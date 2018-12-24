#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myLinkedList.h"


/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||

  Modify the functions below for new kind of Link

  ||||||||||||||||||||||||||||||||||||||||||||||||||||||*/


/*******************************************************
 * linkSetup
 * Description: function that defines how a link is
 * initialized.
 ******************************************************/
void linkSetup (Link * curLink, TYPE linkValue)
{
    curLink->dataId = (int)linkValue;
    curLink->dataValues = NULL;
    curLink->dataValuesNumCols = 0;
    curLink->dataValuesNumRows = 0;
    curLink->curRow = 0;
    curLink->nextLink = NULL;
}


/*******************************************************
 * linkUninitialize
 * Description: Prepares a link for deletion.
 ******************************************************/
void linkUninitialize (Link * curLink)
{
    int col;

    // free data values 2D array
    if (curLink->dataValues != NULL)
    {
        for (col = 0; col < curLink->dataValuesNumCols; col++)
            free(curLink->dataValues[col]);
        free(curLink->dataValues);
    }

    // set variables to zero or NULL
    curLink->dataId = 0;
    curLink->dataValues = NULL;
    curLink->dataValuesNumCols = 0;
    curLink->dataValuesNumRows = 0;
    curLink->curRow = 0;
    curLink->nextLink = NULL;
}

/*******************************************************
 * compare
 * Description: Returns 1 if the first Link is related
 * to the second Link by the provided parameter. If they
 * are not related returns 0.
 * If the parameter is not either >, <, ==, >=, or <=
 * or if any of the links are NULL, returns -1.
 ******************************************************/
int compare(Link * firstLink, char * comparator, Link * secondLink)
{
    // check for null pointers
    if (firstLink == NULL || comparator == NULL || secondLink == NULL)
    {
        fprintf(stderr, "compare(): null parameter provided\n");
        return -1;
    }

    // greater than check
    if (strcmp(comparator, ">") == 0)
    {
        if (firstLink->dataId > secondLink->dataId)
            return 1;
        else
            return 0;
    }

    // less than check
    else if (strcmp(comparator, "<") == 0)
    {
        if (firstLink->dataId < secondLink->dataId)
            return 1;
        else
            return 0;
    }

    // equal to check
    else if (strcmp(comparator, "==") == 0 || strcmp(comparator, "=") == 0)
    {
        if (firstLink->dataId == secondLink->dataId)
            return 1;
        else
            return 0;
    }

    // less than or equal to check
    else if (strcmp(comparator, "<=") == 0)
    {
        if (firstLink->dataId <= secondLink->dataId)
            return 1;
        else
            return 0;
    }

    // greater than or equal to check
    else if (strcmp(comparator, ">=") == 0)
    {
        if (firstLink->dataId >= secondLink->dataId)
            return 1;
        else
            return 0;
    }

    // handler if invalid comparator provided
    else
    {
        fprintf(stderr, "compare(): invalid comparator provided\n");
        return -1;
    }
}

/*******************************************************
 * linkPrint
 * Description: function that defines how a link is
 * printed.
 ******************************************************/
void linkPrint (Link * curLink)
{
    int i,
        j;
    char printVal;

    printf("Guard ID: %i, numCols: %i, numRows: %i\n",
        (int)(curLink->dataId), curLink->dataValuesNumCols,
        curLink->dataValuesNumRows);

    if (curLink->dataValues != NULL)
    {
        for (i = 0; i < curLink->dataValuesNumRows; i++)
        {
            for (j = 0; j < curLink->dataValuesNumCols; j++)
            {
                switch((curLink->dataValues)[j][i])
                {
                    case -1:
                        printVal = 'x';
                        break;
                    case 0:
                        printVal = 'a';
                        break;
                    case 1:
                        printVal = 's';
                        break;
                    default:
                        break;
                }
                printf("%c", printVal);
            }
            printf("\n");
        }
    }
}


/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||

  The functions below are static and are not to be
  Changed with new kind of link.

  ||||||||||||||||||||||||||||||||||||||||||||||||||||||*/


/*******************************************************
 * _createLinkedList
 * Description: creates a new linked list and returns
 * a pointer to it. If there is an error, returns a null
 * pointer. This version is a wrapper function to be
 * used by the function createLinkedList.
 ******************************************************/
LinkedList * _createLinkedList(void (* linkSetupFxn)(Link *, TYPE))
{
    LinkedList * newList;

    // create the new link
    newList = malloc(sizeof(LinkedList));
    if (newList == NULL)
        return newList;

    // create the front sentinel
    newList->frontSentinel = malloc(sizeof(Link));
    if (newList->frontSentinel == NULL)
    {
        free(newList);
        newList = NULL;
        return newList;
    }

    // create the back sentinel
    newList->backSentinel = malloc(sizeof(Link));
    if (newList->backSentinel == NULL)
    {
        free(newList->frontSentinel);
        newList->frontSentinel = NULL;
        free(newList);
        newList = NULL;
        return newList;
    }

    // initialize the sentinels
    linkSetupFxn(newList->frontSentinel, 0);
    linkSetupFxn(newList->backSentinel, 0);

    // make the sentinels point at each other
    newList->frontSentinel->nextLink = newList->backSentinel;
    newList->backSentinel->nextLink = newList->frontSentinel;

    // set general linked list default data
    newList->numLinks = 0;

    return newList;
}

/*******************************************************
 * createLinkedList
 * Description: creates a new linked list and returns
 * a pointer to it. If there is an error, returns a null
 * pointer.
 ******************************************************/
LinkedList * createLinkedList()
{
    return _createLinkedList(linkSetup);
}



/*******************************************************
 * _addLink
 * Description: adds a link to the end of the linked
 * list. This is a wrapper function for addLink.
 ******************************************************/
Link * _addLink(LinkedList * myList, TYPE linkInfo, void (* linkSetupFxn)(Link * , TYPE))
{
    Link * newLink;
    Link * previousTail;

    // allocate memory for new link
    newLink = malloc(sizeof(Link));
    if (newLink == NULL)
        return newLink;

    // set up new link and point to back sentinel
    linkSetupFxn(newLink, linkInfo);
    newLink->nextLink = myList->backSentinel;

    // attach the new link to the linked list
    previousTail = myList->backSentinel->nextLink;
    previousTail->nextLink = newLink;
    myList->backSentinel->nextLink = newLink;

    // increase the list size
    (myList->numLinks)++;

    return newLink;
}

/*******************************************************
 * addLink
 * Description: adds a link to the end of the linked
 * list.
 ******************************************************/
Link * addLink(LinkedList * myList, TYPE linkInfo)
{
   return _addLink(myList, linkInfo, linkSetup);
}


/*******************************************************
 * removeLink
 * Description: removes the link whose address is
 * provided. Returns 1 if the link is not in the list
 * or if the list is empty. Returns 0 if the link was
 * sucessfully deleted.
 ******************************************************/
int removeLink(LinkedList * myList, Link * delLink)
{
    int returnValue = 1;
    Link * tempLink;
    Link * prevLink;

    // initialize the search pointers
    prevLink = myList->frontSentinel;
    // search for the link
    while (prevLink->nextLink != myList->backSentinel)
    {
        if (prevLink->nextLink == delLink)
        {
            // if on last link ensure the back sentinel points
            // to the front sentinel
            if (myList->numLinks == 1)
                myList->backSentinel->nextLink = myList->frontSentinel;
            // disconnect the link and delete it
            tempLink = prevLink->nextLink;
            prevLink->nextLink = (prevLink->nextLink)->nextLink;
            linkUninitialize(tempLink);
            free(tempLink);
            (myList->numLinks)--;
            returnValue = 0;
            break;
        }
        // if link not yet found, move to the next link
        else
            prevLink = prevLink->nextLink;
    }
    return returnValue;
}



/*******************************************************
 * removeFrontLink
 * Description: removes the link pointed at by the front
 * sentinel from the linked list. Returns 0 unless list
 * empty
 ******************************************************/
int removeFrontLink(LinkedList * myList)
{
    return removeLink(myList, myList->frontSentinel->nextLink);
}


/*******************************************************
 * resetLinkedList
 * Description: removes every element in the linked list.
 ******************************************************/
void resetLinkedList(LinkedList * myList)
{
    while (removeFrontLink(myList) == 0);
}

/*******************************************************
 * searchList
 * Description: Searches the linked list for the first
 * instance of a link containing the provided value
 * and returns a pointer to that link. Returns a null
 * pointer of not found.
 ******************************************************/
Link * searchList(LinkedList * myList, TYPE linkValue)
{
    Link testLink,
         * curLink;

    // initialize values for the testlink (contains
    // value to be searched for) and the current link
    linkSetup(&testLink, linkValue);
    curLink = myList->frontSentinel->nextLink;

    // search for character in links
    while (curLink != myList->backSentinel)
    {
        // case when character is found
        if (compare (&testLink, "=", curLink) == 1)
            break;
        // character not yet found, move to next link
        else
            curLink = curLink->nextLink;
    }

    // set curLink to null if we did not find the value
    if (curLink == myList->backSentinel)
        curLink = NULL;

    return curLink;
}


/*******************************************************
 * getSize
 * Description: returns the number of links in the list
 ******************************************************/
int getSize(LinkedList * myList)
{
    return myList->numLinks;
}


/*******************************************************
 * listPrint
 * Description: prints all links
 ******************************************************/
void listPrint(LinkedList * myList)
{
    Link * curLink;

    curLink = myList->frontSentinel->nextLink;
    while (curLink != myList->backSentinel)
    {
        linkPrint(curLink);
        curLink = curLink->nextLink;
    }
    printf("\n");
}

/*******************************************************
 * destroyList
 * Description: destroys the linked list
 ******************************************************/
void destroyList(LinkedList * myList)
{
    resetLinkedList(myList);
    free(myList->frontSentinel);
    free(myList->backSentinel);
    myList->frontSentinel = NULL;
    myList->backSentinel = NULL;
}

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||

  The functions below are custom functions that can be
  modified or removed by the user.

  ||||||||||||||||||||||||||||||||||||||||||||||||||||||*/



/*******************************************************
 * customGuardAdd
 * Description: adds a new guard to the list and keeps
 * a count of how many entries
 ******************************************************/
int customGuardAdd(LinkedList * myList, int guardId)
{
    Link * guardLink;
    // see if guard is already in list
    guardLink = searchList(myList, guardId);
    if (guardLink == NULL)
    {
        // add guard if not already in list
        guardLink = addLink(myList, guardId);
        if (guardLink == NULL)
            return -1;
        (guardLink->dataValuesNumRows)++;
        guardLink->dataValuesNumCols = 60;
    }
    // otherwise increase the number of rows
    else
        (guardLink->dataValuesNumRows)++;

    return 0;
}

/*******************************************************
 * customLinkInit
 * Description: initialize the dataValues array in a
 * single link
 ******************************************************/
int customLinkInit(Link * curLink)
{
    int i,
        j;

    // dynamically allocate memory for the dataValues 2D array columns
    curLink->dataValues = malloc(sizeof(int *) * curLink->dataValuesNumCols);
    if (curLink->dataValues == NULL)
    {
        fprintf(stderr, "customLinkInit: dataValues malloc error\n");
        return -1;
    }


    // allocate memory for the dataValues 2D array rows
    for (i = 0; i < curLink->dataValuesNumCols; i++)
    {
        (curLink->dataValues)[i] = malloc(sizeof(int) * curLink->dataValuesNumRows);

        // error handler, free everything already created
        if ((curLink->dataValues)[i] == NULL)
        {
            for (j = 0; j < i; j++)
                free((curLink->dataValues)[j]);
            free(curLink->dataValues);
            fprintf(stderr, "customLinkInit: dataValues malloc error\n");
            return -1;
        }
        // initialize everything to -1
        else
        {
            for (j = 0; j < curLink->dataValuesNumRows; j++)
                (curLink->dataValues)[i][j] = -1;
        }
    }

    return 0;
}


/*******************************************************
 * customAllLinkInit
 * Description: initialize the dataValues array in each
 * link
 ******************************************************/
int customAllLinkInit(LinkedList * myList)
{
    Link * curLink;

    // point to first link
    curLink = myList->frontSentinel->nextLink;
    // initialize every link
    while (curLink != myList->backSentinel)
    {
        if (customLinkInit(curLink) == -1)
        {
            printf("customAllLinkInit: error initializing linked list arrays\n");
            return -1;
        }
        curLink = curLink->nextLink;
    }
    return 0;
}

/*******************************************************
 * customDataInsert
 * Description: Inserts data from the schedule based on
 * the value of the parameter scheduleAction. 'b' and 'w'
 * insert a value of 0 and 's' insert a value of 1.
 * Warning: replaces everything after start time so should
 * be called in order of increasing startMinute.
 * The last event switch is zero if an event is not the
 * last to be held in a day. If it is 1, this means the
 * event is the last of the day.
 ******************************************************/
void customDataInsert(Link * guardLink, int startMinute, char scheduleAction, int lastEventSwitch)
{
    int i,
        insertNum;

    // ensure no overflow
    if (guardLink->curRow >= guardLink->dataValuesNumRows)
    {
        printf("customDataInsert: guard %i has reached it's final schedule\n", (int)guardLink->dataId);
        return;
    }
    // determine the number to insert
    if (scheduleAction == 'b' || scheduleAction == 'w')
        insertNum = 0;
    else if (scheduleAction == 's')
        insertNum = 1;
    else
    {
        printf("customDataInsert: %c ( %i ) is an incorrect scheduleAction\n", scheduleAction, scheduleAction);
        return;
    }
    // insert the number in all columns of the current row starting
    // at start minute
    for (i = startMinute; i < guardLink->dataValuesNumCols; i++)
        (guardLink->dataValues)[i][guardLink->curRow] = insertNum;
    // if last event of the day, go on to the next row
    if (lastEventSwitch == 1)
        (guardLink->curRow)++;
}

/*******************************************************
 * customGetTotalTimeSlept
 * Description: returns the total time slept by a guard
 ******************************************************/
int customGetTotalTimeSlept(Link * guardLink)
{
    int i,
        j,
        totalTime = 0;

        // sum of the total time slept from the values in
        // the dataValues 2D array
        for (i = 0; i < guardLink->dataValuesNumCols; i++)
        {
            for (j = 0; j < guardLink->dataValuesNumRows; j++)
            {
                if ((guardLink->dataValues)[i][j] == 1)
                    totalTime++;
            }
        }

        return totalTime;
}

/*******************************************************
 * customGetSleepiestGuard
 * Description: returns the guard Link of the guard who
 * slept the most. Will also update the guard Id and timeSlept
 * variables with this guard's id and total time slept
 * respectively.
 ******************************************************/
Link * customGetSleepiestGuard(LinkedList * myList, TYPE * guardId, int * timeSlept)
{
    int curGuardSleepTime;
    Link * curSleepiestGuard;
    Link * curLink;

    // point to first link
    curLink = myList->frontSentinel->nextLink;
    // get search link and record the current link with the most
    // sleep time.
    *timeSlept = -1;
    while (curLink != myList->backSentinel)
    {
        curGuardSleepTime = customGetTotalTimeSlept(curLink);
        if (curGuardSleepTime > *timeSlept)
        {
            curSleepiestGuard = curLink;
            *timeSlept = curGuardSleepTime;
        }
        curLink = curLink->nextLink;
    }

    *guardId = curSleepiestGuard->dataId;

    return curSleepiestGuard;
}

/*******************************************************
 * customGetSleepiestMinute
 * Description: Obtains the minute that the guard slept
 * the most amount of time. Also returns the number of
 * of times slept during this minute.
 ******************************************************/
void customGetSleepiestMinute(Link * curLink, int * sleepiestMinute, int * sleepiestMinuteTimeSlept)
{
    int i,
        j,
        timeSleptInCurHour;

    // get the sleepiest hour. Check each minute for total time
    // slept on sleepiest guard.
    *sleepiestMinute = -1;
    *sleepiestMinuteTimeSlept = -1;
    for (i = 0; i < curLink->dataValuesNumCols; i++)
    {
        // calculate the total amount slept at a certain minute
        timeSleptInCurHour = 0;
        for (j = 0; j < curLink->dataValuesNumRows; j++)
        {
            if ((curLink->dataValues)[i][j] == 1)
                timeSleptInCurHour++;
        }
        // see of the time slept in that minute is the most yet discovered
        if (timeSleptInCurHour > *sleepiestMinuteTimeSlept)
        {
            *sleepiestMinute = i;
            *sleepiestMinuteTimeSlept = timeSleptInCurHour;
        }
    }
}

/*******************************************************
 * customGetGuardWithSleepiestMinute
 * Description: returns the guard Link of the guard who
 * slept the most at a specific minute.
 ******************************************************/
Link * customGetGuardWithSleepiestMinute(LinkedList * myList, TYPE * guardId, int * sleepiestMinute, int * sleepiestMinuteTimeSlept)
{
    int curGuardSleepiestMinute,
        curGuardSleepiestMinuteTimeSlept;
    Link * curSleepiestMinuteGuard;
    Link * curLink;

    // point to first link
    curLink = myList->frontSentinel->nextLink;
    // get search link and record the current link with the most
    // sleep time.
    *sleepiestMinute = -1;
    while (curLink != myList->backSentinel)
    {
        customGetSleepiestMinute(curLink, &curGuardSleepiestMinute, &curGuardSleepiestMinuteTimeSlept);
        if (curGuardSleepiestMinuteTimeSlept > *sleepiestMinuteTimeSlept)
        {
            curSleepiestMinuteGuard = curLink;
            *sleepiestMinute = curGuardSleepiestMinute;
            *sleepiestMinuteTimeSlept = curGuardSleepiestMinuteTimeSlept;
        }
        // go to next link
        curLink = curLink->nextLink;
    }
    // get the ID of the sleepiest minute guard
    *guardId = curSleepiestMinuteGuard->dataId;

    return curSleepiestMinuteGuard;
}



