#ifndef MY_LINKED_LIST_H
#define MY_LINKED_LIST_H

#ifndef TYPE
#define TYPE int
#endif // TYPE definition

typedef struct Link
{
    // The Link members below can be modified.
    int dataId;
    int ** dataValues;
    int dataValuesNumCols;
    int dataValuesNumRows;
    int curRow;

    // the link member below should not be modified.
    struct Link * nextLink;
} Link;

typedef struct LinkedList
{
    Link * frontSentinel;
    Link * backSentinel;
    int numLinks;
} LinkedList;

// functions to modify based on Link member modifiable variables
void linkSetup (Link * curLink, TYPE linkValue);
void linkUninititialize (Link * curLink);
int compare (Link * firstLink, char * comparator, Link * secondLink);
void linkPrint (Link * curLink);

// functions to not modify
LinkedList * _createLinkedList(void (* linkSetupFxn)(Link *, TYPE));
LinkedList * createLinkedList();
Link * _addLink(LinkedList * myList, TYPE linkInfo, void (* linkSetupFxn)(Link *, TYPE));
Link * addLink(LinkedList * myList, TYPE linkInfo);
int removeLink (LinkedList * myList, Link * delLink);
int removeFrontLink(LinkedList * myList);
void resetLinkedList (LinkedList * myList);
Link * searchList(LinkedList * myList, TYPE linkValue);
int getSize(LinkedList * myList);
void listPrint(LinkedList * myList);
void destroyList(LinkedList * myList);


// custom functions
int customGuardAdd(LinkedList * myList, int guardId);
int customLinkInit(Link * curLink);
int customAllLinkInit(LinkedList * myList);
void customDataInsert(Link * guardLink, int startMinute, char scheduleAction, int lastEventSwitch);
int customGetTotalTimeSlept(Link * guardLink);
Link * customGetSleepiestGuard(LinkedList * myList, TYPE * guardId, int * timeSlept);
void customGetSleepiestMinute(Link * curLink, int * sleepiestMinute, int * sleepiestMinuteTimeSlept);
Link * customGetGuardWithSleepiestMinute(LinkedList * myList, TYPE * guardId, int * sleepiestMinute, int * sleepiestMinuteTimeSlept);



#endif // MY_LINKED_LIST_H
