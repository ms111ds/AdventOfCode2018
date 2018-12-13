#include <stdio.h>
#include <stdlib.h>
#include "twoDdynarr.h"


/***************************************************
 * initTwoDDynArr
 * Description: initialize two dimensional dynamic
 * array as a 2x2 integer array.
 **************************************************/
int initTwoDDynArr (TwoDDynArr * dynArr)
{
    int w,
        h;

    // set initial width and height and allocate space
    // for 2d array
    dynArr->width = 2;
    dynArr->height = 2;
    dynArr->arr = malloc(sizeof(int *) * dynArr->width);
    if (dynArr->arr == NULL)
        return -1;
    // allocate space of the second dimension and initialize
    // array with values of -1
    for(w = 0; w < dynArr->width; w++)
    {
        (dynArr->arr)[w] = malloc(sizeof(int) * dynArr->height);
        if ((dynArr->arr)[w] == NULL)
            return -1;
        for (h = 0; h < dynArr->height; h++)
            (dynArr->arr)[w][h] = -1;
    }
    return 0;
}

/***************************************************
 * getWidth
 * Description: gets the current width of the 2d
 * array
 **************************************************/
int getWidth(TwoDDynArr * dynArr)
{
    return dynArr->width;
}

/***************************************************
 * getHeight
 * Description: gets the current height of the 2d
 * array
 **************************************************/
int getHeight(TwoDDynArr * dynArr)
{
    return dynArr->height;
}

/***************************************************
 * getUsage
 * Description: Returns the number of values above
 * or equal to the given amount in the 2D array.
 **************************************************/
int getUsage(TwoDDynArr * dynArr, int checkVal)
{
    int w,
        h,
        sum = 0;

    for (w = 0; w < dynArr->width; w++)
    {
        for (h = 0; h < dynArr->height; h++)
        {
            if (dynArr->arr[w][h] >= checkVal)
                sum++;
        }
    }
    return sum;
}

/***************************************************
 * doubleWidth
 * Description: doubles the width of the 2D dynamic
 * array
 **************************************************/
int doubleWidth(TwoDDynArr * dynArr)
{
    int w,
        h;
    int ** newArr = NULL;

    // allocate memory for array with double the columns
    newArr = malloc(sizeof(int *) * dynArr->width * 2);
    if (newArr == NULL)
        return -1;
    // copy to pointers to the old columns
    for (w = 0; w < dynArr->width; w++)
        newArr[w] = (dynArr->arr)[w];
    // add new columns filled with -1
    for (; w < (dynArr->width) * 2; w++)
    {
        newArr[w] = malloc(sizeof(int) * dynArr->height);
        if (newArr[w] == NULL)
            return -1;
        for (h = 0; h < dynArr->height; h++)
            newArr[w][h] = -1;
    }

    // free old 2d array, point to the new one, and double
    // the width
    free(dynArr->arr);
    dynArr->arr = newArr;
    dynArr->width *= 2;

    return 0;
}

/***************************************************
 * doubleHeight
 * Description: doubles the height of the 2D dynamic
 * array
 **************************************************/
int doubleHeight(TwoDDynArr * dynArr)
{
    int w,
        h;
    int * newArr = NULL;

    // replace every column...
    for (w = 0; w < dynArr->width; w++)
    {
        // make a new column that is double the height
        newArr = malloc(sizeof(int) * dynArr->height * 2);
        if (newArr == NULL)
            return -1;
        // copy all the old values into the new column
        for (h = 0; h < dynArr->height; h++)
            newArr[h] = (dynArr->arr)[w][h];
        // all values outside the range of the old column
        // are set to -1
        for (; h < dynArr->height * 2; h++)
            newArr[h] = -1;
        // free the old column and attach the new one
        free((dynArr->arr)[w]);
        (dynArr->arr)[w] = newArr;
    }
    // double the height
    dynArr->height *= 2;

    return 0;
}

/***************************************************
 * addValues
 * Description: adds new values into the 2D array.
 * The values describe rectangles given by their
 * 1) offset from the zero width and height positions
 * 2) their actual width and height
 * Will increase the size of the 2D array if needed
 **************************************************/
int addValues(TwoDDynArr * dynArr, int widthOffset, int heightOffset, int width, int height)
{
    int w,
        h;

    // double width if 2D array too thin
    while (dynArr->width < widthOffset + width)
    {
        if (doubleWidth(dynArr) == -1)
            return -1;
    }
    // double height if 2D array too short
    while (dynArr->height < heightOffset + height)
    {
        if (doubleHeight(dynArr) == -1)
            return -1;
    }
    // add one to each value in the 2D array specified by the
    // given dimensions
    for (w = widthOffset; w < widthOffset + width; w++)
    {
        for (h = heightOffset; h < heightOffset + height; h++)
            (dynArr->arr)[w][h] += 1;
    }

    return 0;
}

/***************************************************
 * checkValues
 * Description: checks the values the 2D array that
 * correspond to the provided rectangle. Counts the
 * values in the rectangle that are not equal to the
 * checkVal parameter
 * The values describe rectangles given by their
 * 1) offset from the zero width and height positions
 * 2) their actual width and height
 * Will increase the size of the 2D array if needed
 **************************************************/
int checkValues(TwoDDynArr * dynArr, int widthOffset, int heightOffset, int width, int height, int checkVal)
{
    int w,
        h,
        sum = 0;

    // double width if 2D array too thin
    while (dynArr->width < widthOffset + width)
    {
        if (doubleWidth(dynArr) == -1)
            return -1;
    }
    // double height if 2D array too short
    while (dynArr->height < heightOffset + height)
    {
        if (doubleHeight(dynArr) == -1)
            return -1;
    }
    // add one to each value in the 2D array specified by the
    // given dimensions
    for (w = widthOffset; w < widthOffset + width; w++)
    {
        for (h = heightOffset; h < heightOffset + height; h++)
        {
            if ((dynArr->arr)[w][h] != checkVal)
                sum++;
        }
    }

    return sum;
}


/***************************************************
 * deleteTwoDArr
 * Description: frees the memory allocated for the
 * 2D dynamic array
 **************************************************/
void deleteTwoDArr(TwoDDynArr * dynArr)
{
    int w;

    // free each column
    for (w = 0; w < dynArr->width; w++)
    {
        free((dynArr->arr)[w]);
        (dynArr->arr)[w] = NULL;
    }
    // free the 2D array
    free(dynArr->arr);
    dynArr->arr = NULL;
    // set height and width to 0
    dynArr->width = 0;
    dynArr->height = 0;
}


/***************************************************
 * print
 * Description: prints the items in the array.
 **************************************************/
void printTwoDArr(TwoDDynArr * dynArr)
{
    int w,
        h;

    for (h =0; h < dynArr->height; h++)
    {
        for (w = 0; w < dynArr->width; w++)
            printf("%i",dynArr->arr[w][h]);
        printf("\n");
    }
}


