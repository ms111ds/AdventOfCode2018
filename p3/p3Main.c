#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "twoDdynarr.h"

// global variables
char * fileName = "p3Input";

/***************************************************************
 * error
 * Description: shortcut to error message
 ***************************************************************/
int error(char * message)
{
    perror(message);
    return -1;
}

/***************************************************************
 * intPow
 * Description: integer exponent function.
 ***************************************************************/
int intPow(int base, int exponent)
{
    int i,
        expResult = 1;

    if (exponent < 0)
        expResult = 0;
    else
    {
        for (i = 0; i < exponent; i++)
            expResult *= base;
    }
    return expResult;
}

/***************************************************************
 * subStringNum
 * Description: get the positive integer between two substrings.
 * returns the number of successful or -1 if there is
 * an error
 ***************************************************************/
// get num between sub strings
int subStringNum(char * string, char * firstSub, char * secondSub)
{
    int     i,
            powerOfTen,
            returnNum;
    char    * curChar,
            * countChar,
            * startChar,
            * endChar;

    // get starting and ending point of where the numbers are located
    startChar = strstr(string, firstSub);
    endChar = strstr(string, secondSub);
    if (startChar == NULL || endChar == NULL)
        return -1;

    // ensure we are looking at a number
    curChar = startChar;
    while (curChar < endChar && ((int)(*curChar) < 0x30 || (int)(*curChar) > 0x39))
    {

        curChar += sizeof(char);
    }

    // get the max power of 10
    i = 0;
    powerOfTen = -1;
    countChar = curChar;
    while ((int)(*countChar) >= 0x30 && (int)(*countChar) <= 0x39)
    {
        countChar += sizeof(char);
        i++;
    }
    powerOfTen += i;
    if (powerOfTen < 0)
        return -1;

    // get the return value
    returnNum = 0;
    for (i = powerOfTen; i >= 0; i--)
    {
        returnNum += ((int)(*curChar) - 0x30) * intPow(10, i);
        curChar += sizeof(char);
    }

    return returnNum;
}

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

int main()
{
    // declare variables
    int myFile,
        id,
        leftOffset,
        topOffset,
        width,
        height,
        lineSize,
        usage,
        shareCheck;
    char * buffer = NULL;
    TwoDDynArr dynArr;

    // open file
    myFile = open(fileName, O_RDONLY);
    if (myFile == -1)
        return error("error opening file");

    // initialize 2D dynamic array
    initTwoDDynArr(&dynArr);
    // get line of text
    lineSize = myGetline(&buffer, myFile, '\n');
    while (lineSize > 0)
    {
        // pull out id, offsets, width, and height from line of text
        id = subStringNum(buffer, "#", "@");
        leftOffset = subStringNum(buffer, "@", ",");
        topOffset = subStringNum(buffer, ",", ":");
        width = subStringNum(buffer, ":", "x");
        height = subStringNum(buffer, "x", "\n");
        // printf("string: %s, id: %i, leftO: %i, topO: %i, width: %i, height: %i\n", buffer, id, leftOffset, topOffset, width, height);

        // add values to 2D array
        if (addValues(&dynArr, leftOffset, topOffset, width, height) == -1)
            return error("2D array add error");
        lineSize = myGetline(&buffer, myFile, '\n');
    }
    if (lineSize < 0)
        return -1;

    // get the number of coordinates that are used more than once, the 2D array width
    // and the 2D array height
    usage = getUsage(&dynArr, 1);
    width = getWidth(&dynArr);
    height = getHeight(&dynArr);
    printf("width: %i, height: %i, area: %i, usage: %i\n", width, height, width * height, usage);

    // reset the file pointer to the beginning of the file
    lseek(myFile, 0, SEEK_SET);
    // get line of text
    lineSize = myGetline(&buffer, myFile, '\n');
    while (lineSize > 0)
    {
        // pull out id, offsets, width, and height from line of text
        id = subStringNum(buffer, "#", "@");
        leftOffset = subStringNum(buffer, "@", ",");
        topOffset = subStringNum(buffer, ",", ":");
        width = subStringNum(buffer, ":", "x");
        height = subStringNum(buffer, "x", "\n");
        // check to see if rectangle not shared by another
        shareCheck = checkValues(&dynArr, leftOffset, topOffset, width, height, 0);
        if (shareCheck == -1)
            return error("2D array check value error");
        else if (shareCheck > 0)
            lineSize = myGetline(&buffer, myFile, '\n');
        else
            break;
    }
    if (lineSize < 0)
        return -1;

    printf("unshared rectangle id: %i", id);


    // clean up and return
    if (buffer != NULL)
        free(buffer);
    deleteTwoDArr(&dynArr);
    close(myFile);
    return 0;
}
