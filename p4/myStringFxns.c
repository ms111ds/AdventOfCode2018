#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "myStringFxns.h"


/***************************************************************
 * myGetline
 * Description: reads a file for all characters before it finds
 * a delimiter or the end of file. Then updates strPtr with the
 * string of characters found. Returns the number of characters
 * in a line if sucessful or -1 if there is an error.
 ***************************************************************/
int myGetline(char ** strPtr, int fileDescriptor, char delimiter)
{
    // declare variables
    int     i,
            lineSize = 0,
            readReturn,
            decisionSwitch = 0;
    off_t   startOffset;
    char    charBuffer[1];

    // get the offset we are already at
    startOffset = lseek(fileDescriptor, 0, SEEK_CUR);
    if (startOffset == -1)
    {
        perror("myGetline: startOffset lseek error");
        return -1;
    }
    // counts the number of characters from the starting
    // offset to the delimiter or end of file
    while (decisionSwitch == 0)
    {
        readReturn = read(fileDescriptor, charBuffer, 1);
        // read error handler
        if (readReturn == -1)
        {
             lseek(fileDescriptor, startOffset, SEEK_SET);
             perror("myGetline: read error");
             return -1;
        }
        // end of file handler
        else if (readReturn == 0)
            decisionSwitch = 1;
        // letter or delimiter handler
        else
        {
            lineSize++;
            if (charBuffer[0] == delimiter)
                decisionSwitch = 1;
        }
    }

    // case when end of file is reached
    if (lineSize == 0)
        return lineSize;

    // reset file pointer to position when myGetline was called
    if (lseek(fileDescriptor, startOffset, SEEK_SET) == -1)
    {
        perror("myGetline: lseek back to start offset error");
        return -1;
    }

    // free the previous string buffer and dynamically assign a
    // new buffer for the new line
    if (*strPtr != NULL)
        free(*strPtr);
    *strPtr = malloc(sizeof(char) * (lineSize + 1));
    if (strPtr == NULL)
    {
        perror("myGetline: malloc error");
        return -1;
    }
    // initialize buffer with null terminators
    memset(*strPtr, '\0', lineSize + 1);

    // copy line into buffer
    for (i = 0; i < lineSize; i++)
    {
        if (read(fileDescriptor, charBuffer, 1) == -1)
        {
            lseek(fileDescriptor, startOffset, SEEK_SET);
            perror("myGetline: read error");
        }
        (*strPtr)[i] = charBuffer[0];
    }

    return lineSize;
}

/***************************************************************
 * strToPosInt
 * Description: Reads a string containing only numbers and
 * returns the number as a positive integer.
 ***************************************************************/
int strToPosInt(char * intStr)
{
    int i,
        k,
        numDigits = 0,
        exponent,
        muliplier,
        number = 0;
    char * curChar;

    // check all characters are valid numbers and determine the
    // number of digits
    curChar = intStr;
    while (*curChar != '\0')
    {
        if ((int)(*curChar) >= 0x30 && (int)(*curChar) <= 0x39)
        {
            curChar += sizeof(char);
            numDigits++;
        }
        else
            return -1;
    }
    // for each digit, obtain the correct power of 10 and
    // multiply this power of 10 by the digit
    exponent = numDigits - 1;
    curChar = intStr;
    for (i = 0; i < numDigits; i++)
    {
        muliplier = 1;
        for (k = 0; k < exponent; k++)
            muliplier *= 10;
        number += ((int)(*curChar) - 0x30) * muliplier;
        exponent--;
        curChar += sizeof(char);
    }
    return number;
}
