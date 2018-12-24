#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "myStringFxns.h"
#include "scheduleStruct.h"
#include "myLinkedList.h"

#define NUMYEARS 3

void cleanup(int fd, Year ** yrArr, LinkedList * guardList)
{
    int k;

    close(fd);
    for (k = 0; k < NUMYEARS; k++)
    {
        if (yrArr[k] != NULL)
            freeYear(&(yrArr[k]));
    }
    if (guardList != NULL)
        destroyList(guardList);
}

int main()
{
    int i,
        k,
        fd,
        lineSize,
        curYear,
        curMonth,
        curDay,
        curHour,
        curMinute,
        curId,
        nextYearIndex = 0,
        yearIndex,
        * curEvents = NULL,
        curNumEvents,
        lastEventSwitch,
        curStartMinute,
        curScheduleAction,
        sleepiestGuardId,
        timeSlept,
        sleepiestMinute,
        sleepiestMinuteTimeSlept;
    char * buffer = NULL,
         bufferCpy[64],
         * token = NULL,
         curEvent;
    Year * years[NUMYEARS];
    LinkedList * guardList = NULL;
    Link * curGuard = NULL,
         * sleepiestGuard = NULL;


    // initialize the years array
    for (i = 0; i < NUMYEARS; i++)
        years[i] = NULL;
    // open file
    fd = open("p4Input", O_RDONLY);

    // get lines of text from files
    lineSize = myGetline(&buffer, fd, '\n');
    if (lineSize == -1)
        return -1;
    // printf("buffer: %s", buffer);
    while (lineSize > 0)
    {
        memset(bufferCpy, '\0', sizeof(bufferCpy));
        strcpy(bufferCpy, buffer);


        // break the line into tokens
        token = strtok(buffer, "[]- :#");
        i = 0;
        while (token != NULL)
        {
            // obtain the desired variables from the tokens
            switch (i)
            {
                case 0:
                    curYear = strToPosInt(token);
                    break;
                case 1:
                    curMonth = strToPosInt(token);
                    break;
                case 2:
                    curDay = strToPosInt(token);
                    break;
                case 3:
                    curHour = strToPosInt(token);
                    break;
                case 4:
                    curMinute = strToPosInt(token);
                    break;
                case 5:
                    if (strcmp(token, "Guard") == 0)
                        curEvent = 'b';
                    else if (strcmp(token, "falls") == 0)
                        curEvent = 's';
                    else if (strcmp(token, "wakes") == 0)
                        curEvent = 'w';
                    else
                    {
                        printf("curYear %i, curMonth %i, curDay %i, curHour %i, curMinute %i\n", curYear, curMonth, curDay, curHour, curMinute);
                        fprintf(stderr, "string: %s.", token);
                        perror("Incorrect event token read");
                        cleanup(fd, years, guardList);
                        return -1;
                    }
                    break;
                case 6:
                    if (curEvent == 'b')
                        curId = strToPosInt(token);
                    else
                        curId = -1;
                    break;
                default:
                    break;
            }
            // get next token
            i++;
            token = strtok(NULL, "[]- :#");
        }

//        printf("buffer: %s", bufferCpy);

        // update the day to the next day if needed
        if (curHour == 23)
        {
            if (nextDay(&curYear, &curMonth, &curDay) == -1)
            {
                cleanup(fd, years, guardList);
                return -1;
            }

            curHour = 0;
            curMinute = 0;
        }
//        printf("curId %i, curYear %i, curMonth %i, ", curId, curYear, curMonth);
//        printf("curDay %i, curHour %i, curMinute %i\n", curDay, curHour, curMinute);
//        printf("---------------------------------------------------------------\n");

        // check to see if the year is already in the year array
        for (i = 0; i < nextYearIndex; i++)
        {
            if (getYear(years[i]) == curYear)
                break;
        }
        yearIndex = i;
        // if the year is in the year array, add the schedule
        if (yearIndex < nextYearIndex)
        {
            if (addSchedule(years[yearIndex], curYear, curMonth, curDay, curHour, curMinute, curId, curEvent) == -1)
            {
                perror("Add Schedule error");
                cleanup(fd, years, guardList);
                return -1;
            }
        }
        // if the year is not in the year index, add the year to the year array
        else if (yearIndex == nextYearIndex && nextYearIndex < NUMYEARS)
        {
            if (createYear(&(years[nextYearIndex]), curYear) == -1)
            {
                perror("create Year error");
                cleanup(fd, years, guardList);
                return -1;
            }
            if (addSchedule(years[nextYearIndex], curYear, curMonth, curDay, curHour, curMinute, curId, curEvent) == -1)
            {
                perror("Add Schedule error");
                cleanup(fd, years, guardList);
                return -1;
            }
            nextYearIndex++;
        }
        // if there is a need to add a new year and there is no more room
        // in the year array, produce an error
        else
        {
            perror("Need to expand NUMYEARS");
            cleanup(fd, years, guardList);
            return -1;
        }

        // get next line of text
        lineSize = myGetline(&buffer, fd, '\n');
        if (lineSize == -1)
        {
            cleanup(fd, years, guardList);
            return -1;
        }
    }

//    // print year structs
//    printf("\n");
//    for (i = 0; i < NUMYEARS; i++)
//         printYear(years[i]);

    // initialize a linked list of the guards
    guardList = createLinkedList();
    printf("------------getting guard ids from Year struct and inserting them into guard list-------------\n");
    // search through the Year structs for all guards and
    // insert them into the guardlist.
    for (i = 0; i < NUMYEARS && years[i] != NULL; i++)
    {
        curYear = getYear(years[i]);
        curMonth = 1;
        curDay = 1;
        while (curYear == getYear(years[i]))
        {
            curId = getGuardId(years[i], curMonth, curDay);
            if (curId == -1)
            {
                printf("could not obtain a guard ID from Year struct\n");
                cleanup(fd, years, guardList);
                return -1;
            }
            if (curId != 0)
                customGuardAdd(guardList, curId);
            if (nextDay(&curYear, &curMonth, &curDay) == -1)
            {
                cleanup(fd, years, guardList);
                return -1;
            }
        }
    }
    printf("----------------finished inserting guard IDs into guard list, now printing list-------------\n");
    listPrint(guardList);
    printf("----------------finished printing list, now initializing guard list data-------------\n");
    // initialize linked list dataValues 2D arrays
    customAllLinkInit(guardList);
    printf("------------------finished initializing, now inserting the schedules-----------------------\n");

    // insert the schedules
    for (i = 0; i < NUMYEARS && years[i] != NULL; i++)
    {
        curYear = getYear(years[i]);
        curMonth = 1;
        curDay = 1;

        // for all years
        while (curYear == getYear(years[i]))
        {
            // printf("curYear: %i, curMonth: %i, curDay: %i\n",curYear,curMonth,curDay);
            // get the guard ID
            curId = getGuardId(years[i], curMonth, curDay);
            if (curId == -1)
            {
                printf("could not get info for guardID for schedule insertion into linked list\n");
                cleanup(fd, years, guardList);
                return -1;
            }
            // get pointer to guard link, event schedule, and number of events if a guard worked
            // that day
            else if (curId != 0)
            {

                // get the guard link from the linked list
                curGuard = searchList(guardList, curId);
                if (curGuard == NULL)
                {
                    printf("could not get guard link for schedule insertion into linked list\n");
                    cleanup(fd, years, guardList);
                    return -1;
                }
                // get the daily event schedule
                curEvents = getEvents(years[i], curMonth, curDay);
                if (curEvents == NULL)
                {
                    printf("could not get events for schedule insertion into linked list\n");
                    cleanup(fd, years, guardList);
                    return -1;
                }
                // get the number of events in a day
                curNumEvents = getNumEvents(years[i], curMonth, curDay);
                if (curNumEvents == -1)
                {
                    printf("could not get the number of events for guard schedule insertion into linked list\n");
                    cleanup(fd, years, guardList);
                    return -1;
                }
                // insert schedule into linked list data values 2D array
                if (curId != 0)
                {
                    for (k = 0; k < curNumEvents; k++)
                    {
                        // check to see if event is last event of the day
                        if (k < curNumEvents - 1)
                            lastEventSwitch = 0;
                        else
                            lastEventSwitch = 1;
                        // insert data into 2D data array's current row
                        curScheduleAction = *(curEvents + k * 2);
                        curStartMinute = *(curEvents + k * 2 + 1);
                        customDataInsert(curGuard, curStartMinute, curScheduleAction, lastEventSwitch);
                    }
                }
            }
            // go to next day
            if (nextDay(&curYear, &curMonth, &curDay) == -1)
            {
                cleanup(fd, years, guardList);
                return -1;
            }
        }
    }
    printf("finished inserting schedules, now obtaining sleepiest guard\n");
    //get sleepiest guard data
    sleepiestGuard = customGetSleepiestGuard(guardList, &sleepiestGuardId, &timeSlept);
    customGetSleepiestMinute(sleepiestGuard, &sleepiestMinute, &sleepiestMinuteTimeSlept);
    printf("Sleepiest Guard ID: %i\n", sleepiestGuardId);
    printf("Total Time Slept: %i\n", timeSlept);
    printf("Sleepiest Minute: %i\n", sleepiestMinute);
    printf("Sleepiest Minute Time Slept: %i\n", sleepiestMinuteTimeSlept);
    printf("Guard ID * sleepiest Minute: %i\n", sleepiestGuardId * sleepiestMinute);
    // get guard with sleepiest minute
    sleepiestGuard = customGetGuardWithSleepiestMinute(guardList, &sleepiestGuardId, &sleepiestMinute, &sleepiestMinuteTimeSlept);
    printf("Sleepiest Minute Guard ID: %i\n", sleepiestGuardId);
    printf("Sleepiest Minute: %i\n", sleepiestMinute);
    printf("Sleepiest Minute Time Slept: %i\n", sleepiestMinuteTimeSlept);
    printf("Guard ID * sleepiest Minute: %i\n", sleepiestGuardId * sleepiestMinute);

    //cleanup and return
    cleanup(fd, years, guardList);
    return 0;
}
