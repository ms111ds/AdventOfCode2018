#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "scheduleStruct.h"

int maxDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/***************************************************************
 * createDay
 * Description: Allocates memory for a new day. Returns 0 if
 * successful, otherwise returns -1.
 * Warning: should only be called by createMonth
 ***************************************************************/
int createDay(Day ** curDay, int dayNum, int maxDay)
{
    int i;

    // check day number in the correct range
    if (dayNum < 1 || dayNum > maxDay)
        return -1;
    // allocate memory for new Day
    *curDay = malloc(sizeof(Day));
    if ((*curDay) == NULL)
        return -1;
    // initialize variables
    (*curDay)->day = dayNum;
    ((*curDay)->curSchedule).guardId = 0;
    ((*curDay)->curSchedule).curEvents = 0;
    for (i = 0; i < MAX_EVENTS; i++)
    {
        (((*curDay)->curSchedule).events)[i][0] = -1;
        (((*curDay)->curSchedule).events)[i][0] = -1;
    }

//    ((*curDay)->curSchedule).guardId = -1;
//    ((*curDay)->curSchedule).beginsShift = -1;
//    ((*curDay)->curSchedule).fallsAsleep = -1;
//    ((*curDay)->curSchedule).wakesUp = -1;

    return 0;
}

/***************************************************************
 * createMonth
 * Description: Allocates memory for a new 31 day month. Returns 0 if
 * successful, otherwise returns -1.
 * Warning: should only be called by createYear.
 ***************************************************************/
int createMonth(Month ** curMonth, int monthNum)
{
    int numDays = maxDays[monthNum - 1],
        d;

    // validate value of monthNum
    if (monthNum < 1 || monthNum > 12)
        return -1;
    // allocate memory for new Month
    *curMonth = malloc(sizeof(Month));
    if ((*curMonth) == NULL)
        return -1;
    // initialize values for month including allocating memory for all days
    (*curMonth)->month = monthNum;
    (*curMonth)->numDays = numDays;
    (*curMonth)->days = malloc(sizeof(Day *) * numDays);
    if ((*curMonth)->days == NULL)
        return -1;
    for (d = 1; d <= numDays; d++)
    {
        if (createDay(&(((*curMonth)->days)[d - 1]), d, numDays) == -1)
            return -1;
    }

    return 0;
}

/***************************************************************
 * createYear
 * Description: Allocates memory for a new 12 month year. Returns 0 if
 * successful, otherwise returns -1.
 ***************************************************************/
int createYear(Year ** curYear, int yearNum)
{
    int numMonths = 12,
        m;

    // allocate memory for new Year
    *curYear = malloc(sizeof(Year));
    if ((*curYear) == NULL)
        return -1;
    // initialize values for Year including allocating memory for all months
    (*curYear)->year = yearNum;
    (*curYear)->numMonths = numMonths;
    (*curYear)->months = malloc(sizeof(Month *) * numMonths);
    if ((*curYear)->months == NULL)
        return -1;
    for (m = 1; m <= numMonths; m++)
    {
        if (createMonth(&(((*curYear)->months)[m - 1]), m) == -1)
            return -1;
    }

    return 0;
}

/***************************************************************
 * freeDay
 * Description: deallocates memory for a Day.
 * Warning: should only be called by freeMonth.
 ***************************************************************/
void freeDay(Day ** curDay)
{
    int i;

    // set all values to zero
    (*curDay)->day = 0;
    for (i = 0; i < MAX_EVENTS; i++)
    {
        (((*curDay)->curSchedule).events)[i][0] = 0;
        (((*curDay)->curSchedule).events)[i][1] = 0;
    }
//    ((*curDay)->curSchedule).guardId = 0;
//    ((*curDay)->curSchedule).beginsShift = 0;
//    ((*curDay)->curSchedule).fallsAsleep = 0;
//    ((*curDay)->curSchedule).wakesUp = 0;
    // deallocate memory for new Day
    free(*curDay);
    *curDay = NULL;
}

/***************************************************************
 * freeMonth
 * Description: deallocates memory for a Month.
 * Warning: should only be called by freeYear.
 ***************************************************************/
void freeMonth(Month ** curMonth)
{
    int d;

    // free allocated memory
    for (d = 0; d < (*curMonth)->numDays; d++)
        freeDay(&(((*curMonth)->days)[d]));
    // set values to zero
    (*curMonth)->days = NULL;
    (*curMonth)->numDays = 0;
    (*curMonth)->month = 0;
}

/***************************************************************
 * freeYear
 * Description: deallocates memory for a Year.
 ***************************************************************/
void freeYear(Year ** curYear)
{
    int m;

    // free allocated memory
    for (m = 0; m < (*curYear)->numMonths; m++)
        freeMonth(&(((*curYear)->months)[m]));
    // set values to zero
    (*curYear)->months = NULL;
    (*curYear)->numMonths = 0;
    (*curYear)->year = 0;
}

/***************************************************************
 * addSchedule
 * Description: adds schedule to specified date.
 ***************************************************************/
int addSchedule(Year * curYear, int year, int month, int day, int hour, int minute, int id, char event)
{
    int i,
        m,
        d,
        eventIndex;
    Month * curMonth;
    Day * curDay;

    // check given year matches the year struct we are inserting to
    if (curYear->year != year)
        return -1;

    // get the current month
    for (m = 0; m < curYear->numMonths; m++)
    {
        if (((curYear->months)[m])->month == month)
        {
            curMonth = (curYear->months)[m];
            break;
        }
    }
    // get the current day
    for (d = 0; d < curMonth->numDays; d++)
    {
        if (((curMonth->days)[d])->day == day)
        {
            curDay = (curMonth->days)[d];
            break;
        }
    }
    // in case the provided month or day is too high
    if (m > curYear->numMonths || d > curMonth->numDays)
    {
        fprintf(stderr, "addSchedule: number of months or days is too high\n");
        return -1;
    }
    // get the id for the schedule if available
    if (id != -1)
        (curDay->curSchedule).guardId = id;
    // check that the schedule can hold more events
    if ((curDay->curSchedule).curEvents >= MAX_EVENTS)
    {
        fprintf(stderr, "addSchedule: events in year %i, month %i, ", curYear->year, curMonth->month);
        fprintf(stderr, "day %i exceed the max of %i", curDay->day, MAX_EVENTS);
        return -1;
    }
    // add the event in sequential order to the schedule struct
    else
    {
        // get the index the event is supposed to be in
        i = 0;
        while (i < (curDay->curSchedule).curEvents
        && ((curDay->curSchedule).events)[i][1] < minute)
            i++;
        eventIndex = i;
        // move over all events that happen at a later time
        for (i = (curDay->curSchedule).curEvents - 1; i >= eventIndex; i--)
        {
            ((curDay->curSchedule).events)[i + 1][0] = ((curDay->curSchedule).events)[i][0];
            ((curDay->curSchedule).events)[i + 1][1] = ((curDay->curSchedule).events)[i][1];
        }
        // add the event
        ((curDay->curSchedule).events)[eventIndex][0] = (int)event;
        ((curDay->curSchedule).events)[eventIndex][1] = minute;
        (curDay->curSchedule).curEvents++;
    }

    return 0;
}

/***************************************************************
 * getYear
 * Description: get the year value Year.
 ***************************************************************/
int getYear(Year * curYear)
{
    return curYear->year;
}

/***************************************************************
 * getGuardId
 * Description: Get the ID of the guard who worked on a certain
 * day. Returns -1 if incorrect month or day provided.
 ***************************************************************/
int getGuardId(Year * curYear, int month, int day)
{
    Month * curMonth;
    Day * curDay;

    // check for invalid month input
    if (month > curYear->numMonths || month <= 0)
    {
        fprintf(stderr, "getGuardId: month must be between 1 and 12 (inclusive)\n");
        return -1;
    }

    // get month and check for invalid day input
    curMonth = (curYear->months)[month - 1];
    if (day > curMonth->numDays || day <= 0)
    {
        fprintf(stderr, "getGuardId: invalid number of days provided\n");
        return -1;
    }
    // get day
    curDay = (curMonth->days)[day - 1];

    return (curDay->curSchedule).guardId;
}

/***************************************************************
 * getNumEvents
 * Description: Get the number of events for a specified day.
 * Returns -1 if incorrect month or day provided.
 ***************************************************************/
int getNumEvents(Year * curYear, int month, int day)
{
    Month * curMonth;
    Day * curDay;

    // check for invalid month input
    if (month > curYear->numMonths || month <= 0)
    {
        fprintf(stderr, "getGuardId: month must be between 1 and 12 (inclusive)\n");
        return -1;
    }

    // get month and check for invalid day input
    curMonth = (curYear->months)[month - 1];
    if (day > curMonth->numDays || day <= 0)
    {
        fprintf(stderr, "getGuardId: invalid number of days provided\n");
        return -1;
    }
    // get day
    curDay = (curMonth->days)[day - 1];

    return (curDay->curSchedule).curEvents;
}

/***************************************************************
 * getEvents
 * Description: Get the events schedule of a certain day.
 * Returns NULL if incorrect month or day provided.
 ***************************************************************/
int * getEvents(Year * curYear, int month, int day)
{
    Month * curMonth;
    Day * curDay;

    // check for invalid month input
    if (month > curYear->numMonths || month <= 0)
    {
        fprintf(stderr, "getGuardId: month must be between 1 and 12 (inclusive)\n");
        return NULL;
    }

    // get month and check for invalid day input
    curMonth = (curYear->months)[month - 1];
    if (day > curMonth->numDays || day <= 0)
    {
        fprintf(stderr, "getGuardId: invalid number of days provided\n");
        return NULL;
    }
    // get day
    curDay = (curMonth->days)[day - 1];
    return (curDay->curSchedule).events;
}

/***************************************************************
 * printYear
 * Description: print the year details.
 ***************************************************************/
void printYear(Year * curYear)
{
    int i,
        m,
        d;
    Month * curMonth;
    Day * curDay;
    char * start = "start";
    char * sleep = "sleep";
    char * awake = "awake";
    char * curEvent;

    if (curYear == NULL)
    {
         printf("The selected year is null\n");
         return;
    }

    printf("year\t");
    printf("month\t");
    printf("day\t");
    printf("id\t");
    printf("event\t");
    printf("time\n");
    for (m = 0; m < curYear->numMonths; m++)
    {
        curMonth = curYear->months[m];
        for (d = 0; d < curMonth->numDays; d++)
        {
                curDay = curMonth->days[d];
                printf("%i\t", curYear->year);
                printf("%i\t", curMonth->month);
                printf("%i\t", curDay->day);
                printf("%i\t", ((curDay->curSchedule).guardId));
                for (i = 0; i < (curDay->curSchedule).curEvents; i++)
                {
                    if ((char)((curDay->curSchedule).events)[i][0] == 'b')
                        curEvent = start;
                    else if ((char)((curDay->curSchedule).events)[i][0] == 's')
                        curEvent = sleep;
                    else
                        curEvent = awake;
                    if (i > 0)
                        printf("\t\t\t\t");
                    printf("%s\t", curEvent);
                    printf("%i\n", ((curDay->curSchedule).events)[i][1]);
                }
                if (i == 0)
                    printf("\n");

//                printf("%i\t", (curDay->curSchedule).guardId);
//                printf("%i\t", (curDay->curSchedule).beginsShift);
//                printf("%i\t", (curDay->curSchedule).fallsAsleep);
//                printf("%i\n", (curDay->curSchedule).wakesUp);
        }
    }
}

/***************************************************************
 * nextDay
 * Description: update the year, month, and day variables to
 * represent the next day. Returns -1 on error and 0 if successful.
 ***************************************************************/
int nextDay(int * year, int * month, int * day)
{
    // check the month range
    if (*month < 1 || *month > 12)
    {
         fprintf(stderr, "nextDay(): provided month, %i, greater than 12\n", *month);
         return -1;
    }

    // error check the current day
    if (*day < 1 || *day > maxDays[*month - 1])
    {
        fprintf(stderr, "nextDay(): provided day, %i, is greater than", *day);
        fprintf(stderr, "the %i available for month %i\n", maxDays[*month - 1], *month);
        return -1;
    }
    // if next day belongs to next year
    if (*month == 12 && *day == maxDays[11])
    {
        (*year)++;
        *month = 1;
        *day = 1;
    }
    // if next day belongs to the first day of the next month
    else if (*day == maxDays[*month - 1])
    {
        (*month)++;
        *day = 1;
    }
    else
        (*day)++;

    return 0;
}
