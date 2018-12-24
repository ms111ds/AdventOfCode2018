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
    curLink->curChar = linkValue;
    curLink->nextLink = NULL;
}


/*******************************************************
 * linkUninitialize
 * Description: Prepares a link for deletion.
 ******************************************************/
void linkUninitialize (Link * curLink)
{
    // set variables to zero or NULL
    curLink->curChar = 0;
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
        if (firstLink->curChar > secondLink->curChar)
            return 1;
        else
            return 0;
    }

    // less than check
    else if (strcmp(comparator, "<") == 0)
    {
        if (firstLink->curChar < secondLink->curChar)
            return 1;
        else
            return 0;
    }

    // equal to check
    else if (strcmp(comparator, "==") == 0 || strcmp(comparator, "=") == 0)
    {
        if (firstLink->curChar == secondLink->curChar)
            return 1;
        else
            return 0;
    }

    // less than or equal to check
    else if (strcmp(comparator, "<=") == 0)
    {
        if (firstLink->curChar <= secondLink->curChar)
            return 1;
        else
            return 0;
    }

    // greater than or equal to check
    else if (strcmp(comparator, ">=") == 0)
    {
        if (firstLink->curChar >= secondLink->curChar)
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
    printf("%c",curLink->curChar);
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
 * sucessfully deleted. Returns -1 in case of NULL
 * parameters
 ******************************************************/
int removeLink(LinkedList * myList, Link * delLink)
{
    int returnValue = 1;
    Link * tempLink;
    Link * prevLink;

    // in case of null list or Link
    if (myList == NULL)
    {
        fprintf(stderr, "removeLink: NULL linked list provided\n");
        return -1;
    }
    if (delLink == NULL)
    {
        fprintf(stderr, "removeLink: NULL link provided\n");
        return -1;
    }

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
    if (myList == NULL)
        return;
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
 * customCompare
 * Description: Custom compare function to determine
 * the difference in ascii values between the characters
 * contained in 2 links.
 ******************************************************/
int customCompare(Link * firstLink, Link * secondLink)
{
    // check for null pointers
    if (firstLink == NULL || secondLink == NULL)
    {
        fprintf(stderr, "compare(): null parameter provided\n");
        return -1;
    }

    return abs((int)(firstLink->curChar) - (int)(secondLink->curChar));
}

/*******************************************************
 * copyList
 * Description: creates a copy of the linked list pointed.
 * at by the parameter.
 ******************************************************/
LinkedList * copyList(LinkedList * myList)
{
    // variables
    Link * curLink;
    LinkedList * newList;

    // create new linked list
    newList = createLinkedList();
    if (newList == NULL)
        return NULL;

    // iterate through links of linked list in parameter
    // and create new links in new linked list with the same
    // value as parameter list.
    curLink = myList->frontSentinel->nextLink;
    while (curLink != myList->backSentinel)
    {
        if (addLink(newList, curLink->curChar) == NULL)
        {
            destroyList(newList);
            return NULL;
        }
        curLink = curLink->nextLink;
    }

    return newList;
}


