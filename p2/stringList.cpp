#include <iostream>
#include <assert.h>
#include "stringList.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;

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
    frontSentinel->linkString = "";
    backSentinel->linkString = "";
    iter = frontSentinel;
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
void LinkedList::addLink(string newString)
{
     Link * newLink;
     Link * previousTail;

     newLink = new Link;
     newLink->linkString = newString;
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
        assert(frontSentinel->nextLink != backSentinel);
        tempLink = frontSentinel->nextLink;
        frontSentinel->nextLink = tempLink->nextLink;
        // reset the iterator to the front sentinel if it is to
        // be deleted
        if (iter == tempLink)
            iter = frontSentinel;
        delete tempLink;
        numLinks--;
        return 0;
    }
}


/*******************************************************
 * compareStrings
 * Description: takes two strings and compares each
 * character at every index. Returns the number of
 * characters that are different
 ******************************************************/
int LinkedList::compareStrings(string str1, string str2)
{
    int i,
        difference = 0,
        string1Len,
        string2Len,
        minLen;

    // get parameter string lengths
    string1Len = str1.length();
    string2Len = str2.length();

    // find the minimum string length and add the difference
    // in length to "difference"
    if (string1Len > string2Len)
    {
        minLen = string2Len;
        difference += string1Len - string2Len;
    }
    else
    {
        minLen = string1Len;
        difference += string2Len - string1Len;
    }
    // calculate the difference between strings
    for (i = 0; i < minLen; i++)
    {
        if (str1[i] != str2[i])
            difference++;
    }

    return difference;
}


/*******************************************************
 * getBestMatch
 * Description: Searches the linked list for the smallest
 * difference between the given string and any other
 * string in the list. Makes bestMatch point to the
 * string with the smallest difference and returns the
 * number of characters that are different. If list is
 * empty or only contains the current string, returns -1.
 * WARNING: ensure testStringPtr is a pointer to a string
 * contained in one of the linked list's links to avoid
 * comparison with itself.
 * WARNING: the string in bestMatchPtr will be overriden!
 ******************************************************/
int LinkedList::getBestMatch(string * testStringPtr, string * bestMatchPtr)
{
    Link * curLink;
    int bestDifference = -1,
        testDifference;

    curLink = frontSentinel->nextLink;
    // search for character in links
    while (curLink != backSentinel)
    {
        // ensure we are not comparing one string against itself
        if (testStringPtr != &(curLink->linkString))
        {
            // the string we first test equal to the best string
            if (bestDifference == -1)
            {
                bestDifference = compareStrings(*testStringPtr, curLink->linkString);
                *bestMatchPtr = curLink->linkString;
            }
            // all other tests
            else
            {
                testDifference = compareStrings(*testStringPtr, curLink->linkString);
                if (testDifference < bestDifference)
                {
                    bestDifference = testDifference;
                    *bestMatchPtr = curLink->linkString;
                }
            }
        }
        curLink = curLink->nextLink;
    }

    return bestDifference;
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
 * Description: prints the strings on the list
 ******************************************************/
void LinkedList::print()
{
    Link * curLink = frontSentinel->nextLink;

    while (curLink != backSentinel)
    {
        cout << curLink->linkString << endl;
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
}


/*******************************************************
 * iterGetString
 * Description: Gets the string of the link the iterator
 * is on. Returns an empty string if on a sentinel.
 ******************************************************/
string LinkedList::iterGetString()
{
    return iter->linkString;
}

/*******************************************************
 * iterGetStringPtr
 * Description: Gets the string address of the link the
 * iterator is on. Returns 0 if on a sentinel.
 ******************************************************/
string * LinkedList::iterGetStringPtr()
{
    if (iter == frontSentinel || iter == backSentinel)
        return 0;
    else
        return &(iter->linkString);
}

/*******************************************************
 * iterNext
 * Description: Moves iter to the next link. Returns -1
 * if it lands on the front sentinel, -2 if it lands on
 * the back sentinel, and 0 if it lands on any other link.
 ******************************************************/
int LinkedList::iterNext()
{
    int returnVal = 0;

    if (iter == backSentinel)
    {
        iter = frontSentinel;
        returnVal = -1;
    }
    else
    {
        iter = iter->nextLink;
        if (iter == backSentinel)
            returnVal = -2;
        else
            returnVal = 0;
    }
    return returnVal;
}


