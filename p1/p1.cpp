#include <iostream>
#include <fstream>
#include "dynarr.hpp"

using std::string;
using std::getline;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;

// integer exponent function
int intPow(int base, int exponent)
{
    int i = 0;
    int retVal = 1;

    while (i < exponent)
    {
        retVal *= base;
        i++;
    }
    return retVal;
}

int main()
{
    // declare variables
    string curLine;
    unsigned int i,
                 k = 0,
                 maxFileOpens = 1000,
                 repeatedSwitch = 0;
    int signMult,
        numSize,
        curNum,
        sum = 0;
    ifstream myFile;
    DynArr myDynArr;


    // add zero to the frequency
    myDynArr.add(0);

    while (repeatedSwitch == 0 && k < maxFileOpens)
    {
        // open file
        myFile.open("p1Input");
        // get lines from file
        while (getline(myFile, curLine))
        {
            // determine if line contains a positive or negative number
            if (curLine.compare(0,1,"+") == 0)
                signMult = 1;
            else
                signMult = -1;
            // determine length of number
            numSize = curLine.length() - 1;
            // cout << "|||||||||||||||||||||||||||" << curLine << "||||||||||||||||||||||||||||" << endl;
            // get the current number as an integer
            curNum = 0;
            for (i = 1; i < curLine.length(); i++)
            {
                curNum += signMult * ((int)curLine[i] - 0x30) * intPow(10,(numSize - i));
            }
            // sum current number to the running total
            sum += curNum;
            // cout << "curNum: " << curNum << "\n" << "sum: " << sum << endl;
            //myDynArr.print();

            // add the current sum to the dynamic array and stop loop if duplicate found
            if (myDynArr.add(sum))
            {
                repeatedSwitch = 1;
                break;
            }
        }
        myFile.close();
        k++;
    }
    cout << "repeated sum: " << sum << endl;
    cout << "file opened " << k << " out of " << maxFileOpens << " times" << endl;

    return 0;
}
