#ifndef QUAD_LIST_H
#define QUAD_LIST_H

struct Link
{
    char linkChar;
    Link * nextLink;
};

class LinkedList
{
    private:
        Link * frontSentinel;
        Link * backSentinel;
        int numLinks;
    public:
        LinkedList();
        ~LinkedList();
        void addLink(char newChar);
        int removeFrontLink();
        int searchAndRemove(char someChar);
        int getSize();
        void print();
        void clearList();
};

class QuadList
{
    private:
        LinkedList overThree;
        LinkedList three;
        LinkedList two;
        LinkedList one;
    public:
        void addChar(char newChar);
        int getOverThreeSize();
        int getThreeSize();
        int getTwoSize();
        int getOneSize();
        void print();
        void clearList();
};

#endif // QUAD_LIST_H
