#include <iostream>
#include <assert.h>
#include "quadList.hpp"

using std::cout;
using std::cin;
using std::endl;

/*******************************************************
 * LinkedList
 * Description: Linked List constructor
 ******************************************************/
LinkedList::LinkedList()
{
    frontSentinel = new Link;
    backSentinel = new Link;
    frontSentinel->nextLink = backSentinel;
    backSentinel->nextLink = frontSentinel;
    numLinks = 0;
}

/*******************************************************
 * ~LinkedList
 * Description: Linked List destructor
 ******************************************************/
LinkedList::~LinkedList()
{
    clearList();
}

/*******************************************************
 * addLink
 * Description: adds a link to the end of the linked
 * list.
 ******************************************************/
void LinkedList::addLink(char newChar)
{
     Link * newLink;
     Link * previousTail;

     newLink = new Link;
     newLink->linkChar = newChar;
     newLink->nextLink = backSentinel;

     previousTail = backSentinel->nextLink;
     previousTail->nextLink = newLink;
     backSentinel->nextLink = newLink;
     numLinks++;
}

/*******************************************************
 * removeFrontLink
 * Description: removes the link pointed at by the front
 * sentinel from the linked list. Returns 0 unless list
 * empty
 ******************************************************/
int LinkedList::removeFrontLink()
{
    Link * tempLink;

    if (numLinks == 0)
        return 1;
    else
    {
        if (numLinks == 1)
            backSentinel->nextLink = frontSentinel;
        // cout << "num links: " << numLinks << endl;
        assert(frontSentinel->nextLink != backSentinel);
        tempLink = frontSentinel->nextLink;
        frontSentinel->nextLink = tempLink->nextLink;
        delete tempLink;
        numLinks--;
        return 0;
    }
}

/*******************************************************
 * searchAndRemove
 * Description: Searches the linked list for the provided
 * character and removes it if found. Returns 0 if the
 * character was found and removed and 1 if not.
 ******************************************************/
int LinkedList::searchAndRemove(char someChar)
{
    Link * curLink;
    Link * prevLink;

    if (numLinks == 0)
        return 1;

    curLink = frontSentinel->nextLink;
    prevLink = frontSentinel;

    // search for character in links
    while (curLink != backSentinel)
    {
        // case when character is found
        if (curLink->linkChar == someChar)
        {
            // if link at the end of the list
            if (backSentinel->nextLink == curLink)
                backSentinel->nextLink = prevLink;
            // delete link
            prevLink->nextLink = curLink->nextLink;
            delete curLink;
            numLinks--;
            return 0;
        }
        // character not yet found
        else
        {
            prevLink = curLink;
            curLink = curLink->nextLink;
        }
    }

    return 1;
}


/*******************************************************
 * getSize
 * Description: returns the number of links in the list
 ******************************************************/
int LinkedList::getSize()
{
    return numLinks;
}


/*******************************************************
 * print
 * Description: prints the characters on the list
 ******************************************************/
void LinkedList::print()
{
    Link * curLink = frontSentinel->nextLink;

    while (curLink != backSentinel)
    {
        cout << curLink->linkChar << endl;
        curLink = curLink->nextLink;
    }
    cout << "number of links: " << numLinks << endl;
    cout << "front sentinel: " << frontSentinel << ", points to: " << frontSentinel->nextLink << endl;
    cout << "back sentinel: " << backSentinel << ", points to: " << backSentinel->nextLink << endl;
}

/*******************************************************
 * clearList
 * Description: clears out the linked list of all links
 ******************************************************/
void LinkedList::clearList()
{
    while (removeFrontLink() == 0);
    //cout << "front sentinel: " << frontSentinel << ", points to: " << frontSentinel->nextLink << endl;
    //cout << "back sentinel: " << backSentinel << ", points to: " << backSentinel->nextLink << endl;
}


/*******************************************************
 * addChar
 * Description: adds a character to the quad list.
 ******************************************************/
void QuadList::addChar(char newChar)
{
    if (overThree.searchAndRemove(newChar) == 0 || three.searchAndRemove(newChar) == 0)
        overThree.addLink(newChar);
    else if (two.searchAndRemove(newChar) == 0)
        three.addLink(newChar);
    else if (one.searchAndRemove(newChar) == 0)
        two.addLink(newChar);
    else
        one.addLink(newChar);
}


/*******************************************************
 * getOverThreeSize
 * Description: gets the size of the "overThree" linked
 * list.
 ******************************************************/
int QuadList::getOverThreeSize()
{
    return overThree.getSize();
}


/*******************************************************
 * getThreeSize
 * Description: gets the size of the "three" linked
 * list.
 ******************************************************/
int QuadList::getThreeSize()
{
    return three.getSize();
}


/*******************************************************
 * getTwoSize
 * Description: gets the size of the "two" linked
 * list.
 ******************************************************/
int QuadList::getTwoSize()
{
    return two.getSize();
}


/*******************************************************
 * getOneSize
 * Description: gets the size of the "one" linked
 * list.
 ******************************************************/
int QuadList::getOneSize()
{
    return one.getSize();
}

/*******************************************************
 * print
 * Description: prints the Triple List.
 ******************************************************/
void QuadList::print()
{
    cout << "----------------------- Found Over Three Times ----------------------" << endl;
    overThree.print();
    cout << "------------------------- Found Three Times -------------------------" << endl;
    three.print();
    cout << "---------------------------- Found Twice ----------------------------" << endl;
    two.print();
    cout << "---------------------------- Found Once _----------------------------" << endl;
    one.print();
}

/*******************************************************
 * clearList
 * Description: clears the Triple List.
 ******************************************************/
void QuadList::clearList()
{
    overThree.clearList();
    three.clearList();
    two.clearList();
    one.clearList();
}

