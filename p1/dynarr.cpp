#include <iostream>
#include "dynarr.hpp"

using std::string;
using std::getline;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;

/***************************************************
 * DynArr
 * Description: DynArr constructor
 **************************************************/
DynArr::DynArr()
{
    capacity = 2;
    arrSize = 0;
    intArr = new int[2];
}

/***************************************************
 * ~DynArr
 * Description: DynArr destructor
 **************************************************/
DynArr::~DynArr()
{
    delete[] intArr;
    intArr = NULL;
    arrSize = 0;
    capacity = 0;
}

/***************************************************
 * getCapacity
 * Description: gets the current capacity of the
 * DynArr object
 **************************************************/
int DynArr::getCapacity()
{
    return capacity;
}

/***************************************************
 * getSize
 * Description: gets the number of integers currently
 * in the DynArr object
 **************************************************/
int DynArr::getSize()
{
    return arrSize;
}

/***************************************************
 * doubleSize
 * Description: doubles the capacity of the DynArr
 * object
 **************************************************/
void DynArr::doubleSize()
{
    int i;
    int newCap;
    int * newArr;

    newCap = 2 * capacity;
    newArr = new int[newCap];

    for (i = 0; i < arrSize; i++)
        newArr[i] = intArr[i];

    delete [] intArr;
    intArr = newArr;
    capacity = newCap;
}

/***************************************************
 * add
 * Description: adds a new integer to the DynArr
 * object. If the integer already in the DynArr object,
 * then the number 1 is returned, otherwise 0 is
 * returned.
 **************************************************/
int DynArr::add(int element)
{
    int i;
    int middleIndex;
    int upperIndex;
    int lowerIndex;
    int insertIndex;
    int found = 0;

    // make sure there is capacity for new integer in array
    while (arrSize >= capacity)
          doubleSize();

    // special case when array is empty
    if (arrSize == 0)
    {
        intArr[0] = element;
        arrSize++;
        return 0;
    }
    // binary search to find if element is in list
    upperIndex = arrSize - 1;
    lowerIndex = 0;
    while (upperIndex != lowerIndex)
    {

        middleIndex = (upperIndex + lowerIndex)/2;
        if (intArr[middleIndex] == element)
        {
            found = 1;
            break;
        }
        else if (intArr[middleIndex] < element)
            lowerIndex = middleIndex + 1;
        else
            upperIndex = middleIndex;
        i++;
    }

    // determine where to insert the element based on the binary search results
    if (found == 1)
        insertIndex = middleIndex;
    else if (intArr[upperIndex] < element)
        insertIndex = upperIndex + 1;
    else
        insertIndex = upperIndex;

    // move larger elements forward
    for (i = arrSize - 1; i >= insertIndex; i--)
        intArr[i+1] = intArr[i];

    // insert element and increase array size
    intArr[insertIndex] = element;
    arrSize++;

    return found;
}

/***************************************************
 * print
 * Description: prints the items in the array.
 **************************************************/
void DynArr::print()
{
    int columns = 5,
        curCol,
        i = 0;

    while (i < arrSize)
    {
        curCol = 0;
        while (curCol < columns && i < arrSize)
        {
            cout << intArr[i] << "\t";
            curCol++;
            i++;
        }
        cout << endl;
    }
}


