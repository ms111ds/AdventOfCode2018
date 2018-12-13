#ifndef TWODDYNARR_H
#define TWODDYNARR_H

// The 2D integer array will represent each square
// inch of the cloth. The array will contain integer
// values greater than or equal to -1.
//
// if MxN = -1, the piece of cloth is unused
// if MxN = 0, the piece of cloth is used once
// if MxN > 0, the piece of cloth is contested by multiple users

typedef struct TwoDDynArr
{
    int ** arr;
    int width;
    int height;
} TwoDDynArr;


// functions
int initTwoDDynArr(TwoDDynArr *);
int getWidth(TwoDDynArr *);
int getHeight(TwoDDynArr *);
int getUsage(TwoDDynArr *, int);
int doubleWidth(TwoDDynArr *);
int doubleHeight(TwoDDynArr *);
int addValues(TwoDDynArr *, int, int, int, int);
int checkValues(TwoDDynArr *, int, int, int, int, int);
void deleteTwoDArr(TwoDDynArr *);
void printTwoDArr(TwoDDynArr *);

#endif // TWODDYNARR_H
