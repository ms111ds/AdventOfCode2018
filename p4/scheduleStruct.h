#ifndef SCHEDULESTRUCT_H
#define SCHEDULESTRUCT_H

#define MAX_EVENTS 20

typedef struct Schedule
{
    int guardId;
    // events[x][0] = event hex code
    // shift begins 'b' = 0x62
    // sleep        's' = 0x73
    // wake up      'w' = 0x77
    // events[x][1] = time of event
    int events[MAX_EVENTS][2];
    int curEvents;
} Schedule;

typedef struct Day
{
    int day;
    Schedule curSchedule;
} Day;

typedef struct Month
{
    int month;
    int numDays;
    Day ** days;
} Month;

typedef struct Year
{
    int year;
    int numMonths;
    Month ** months;
} Year;


int createDay(Day **, int, int);
int createMonth(Month **, int);
int createYear(Year **, int);
void freeDay(Day **);
void freeMonth(Month **);
void freeYear(Year **);
int addSchedule(Year *, int, int, int, int, int, int, char);
int getYear(Year *);
int getGuardId(Year *, int, int);
int getNumEvents(Year *, int, int);
// int (*getEvents(Year *, int, int))[MAX_EVENTS][2];
int * getEvents(Year *, int, int);
void printYear(Year *);
int nextDay (int *, int *, int *);

#endif // SCHEDULESTRUCT_H
