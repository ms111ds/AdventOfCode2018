#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "myLinkedList.h"

#define INPUT "p5Input_2.txt"

void error(char * errorMgs, LinkedList * myList)
{
    fprintf(stderr, errorMgs);
    destroyList(myList);
    exit(1);
}

int charCheck(char curChar)
{
    if (curChar >= 0x41 && curChar <= 0x5A)
        return 1;
    else if (curChar >= 0x61 && curChar <= 0x7A)
        return 1;
    else
        return 0;
}

int updateList(LinkedList * myList)
{
    Link * prevLink;
    Link * curLink;

    if (myList->numLinks <= 1)
        return 0;

    prevLink = myList->frontSentinel->nextLink;
    curLink = prevLink->nextLink;

    while (curLink != myList->backSentinel)
    {
        if (customCompare(curLink, prevLink) == 32)
        {
            //linkPrint(curLink);
            //linkPrint(prevLink);
            removeLink(myList, curLink);
            removeLink(myList, prevLink);
            return 1;
        }

        prevLink = curLink;
        curLink = curLink->nextLink;
    }
    return 0;
}

int main()
{
    // define variables
    int fd,
        i = 0,
        errorCheck,
        bestSize;
    char curChar[1],
         bestChar,
         * alphabet = "abcdefghijklmnopqrstuvwxyz";
    LinkedList * myList = NULL,
               * testList = NULL;

    // open file
    fd = open(INPUT, O_RDONLY);
    if (fd == -1)
        error("error opening file", myList);

    // create linked list and insert all characters into it
    myList = createLinkedList();
    errorCheck = read(fd, curChar, 1);
    while (errorCheck != 0 && errorCheck != -1)
    {
        if (charCheck(curChar[0]) == 1)
            addLink(myList, curChar[0]);
        errorCheck = read(fd, curChar, 1);
    }
    if (errorCheck == -1)
        error("error reading a character fom file", myList);

    // search through the list and remove all instances of a capital
    // letter being side by side with an instance of its small letter
    while(updateList(myList) != 0);
    // print the final size
    printf("final size %d\n", getSize(myList));

    // listPrint(myList);

    bestSize = getSize(myList);

    i = 0;
    curChar[0] = *(alphabet + i);
    while (curChar[0] != '\0')
    {
        testList = copyList(myList);
        if (testList == NULL)
        {
            error("testList could not be initialized\n", myList);
            return -1;
        }

        while(removeLink(testList, searchList(testList, curChar[0])) == 0);
        while(removeLink(testList, searchList(testList, curChar[0] - 0x20)) == 0);
        while(updateList(testList) != 0);
        if (getSize(testList) < bestSize)
        {
            bestChar = curChar[0];
            bestSize = getSize(testList);
        }

        destroyList(testList);
        i++;
        curChar[0] = *(alphabet + i);
    }

    printf("best Char to Remove: %c, size: %i\n", bestChar, bestSize);

    destroyList(myList);
    close(fd);
    return 0;
}
