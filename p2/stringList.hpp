#ifndef STRING_LIST_H
#define STRING_LIST_H
#include <string>

using std::string;

struct Link
{
    string linkString;
    Link * nextLink;
};

class LinkedList
{
    private:
        Link * frontSentinel;
        Link * backSentinel;
        int numLinks;
        Link * iter;
    public:
        LinkedList();
        ~LinkedList();
        void addLink(string newString);
        int removeFrontLink();
        int compareStrings(string string1, string string2);
        int getBestMatch(string * testStringPtr, string * bestMatchPtr);
        int getSize();
        void print();
        void clearList();
        string iterGetString();
        string * iterGetStringPtr();
        int iterNext();
};

#endif // STRING_LIST_H
