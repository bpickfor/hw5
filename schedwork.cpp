#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

// Add prototypes for any helper functions here
bool scheduleHelper(
    size_t day,
    const AvailabilityMatrix &avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule &sched,
    vector<size_t> &workerShifts);

bool fillDaySchedule(
    size_t day,
    size_t workeridx,
    const AvailabilityMatrix &avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule &sched,
    vector<size_t> &workerShifts,
    vector<Worker_T> &tempDay);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix &avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule &sched)
{
    if (avail.size() == 0U)
    {
        return false;
    }
    sched.clear();
    // Add your code below

    // track # of shifts each worker worked
    vector<size_t> workerShifts(avail[0].size(), 0);
    // resizes sched  to the num of days (n) that scheduling needs to happen for
    // bc avail has 1 row for each day for its size give num of days
    // initialize new elements added to sched with empty worker vector
    sched.resize(avail.size(), vector<Worker_T>());

    // call helper & return output
    return scheduleHelper(0, avail, dailyNeed, maxShifts, sched, workerShifts);
}

bool scheduleHelper(
    size_t day,
    const AvailabilityMatrix &avail,
    size_t dailyNeed, size_t maxShifts,
    DailySchedule &sched,
    vector<size_t> &workerShifts)
{
    // base case, if all days scheduled
    if (day == sched.size())
    {
        return true;
    }

    vector<Worker_T> tempDay; // temp store 1 days schedule
    return fillDaySchedule(day, 0, avail, dailyNeed, maxShifts, sched, workerShifts, tempDay);
}

bool fillDaySchedule(
    size_t day,
    size_t workeridx,
    const AvailabilityMatrix &avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule &sched,
    vector<size_t> &workerShifts,
    vector<Worker_T> &tempDay)
{
    if (tempDay.size() == dailyNeed)
    {                         // if curr day fully scheduled
        sched[day] = tempDay; // put into main schedule
        // move to next day
        return scheduleHelper(day + 1, avail, dailyNeed, maxShifts, sched, workerShifts);
    }

    // iterate thru workers
    for (size_t worker = workeridx; worker < avail[day].size(); ++worker)
    {
        // if worker availible on this day
        // and worker can still work more shifts
        if (avail[day][worker] && workerShifts[worker] < maxShifts)
        {
            workerShifts[worker]++;    // increase num of shifts worked
            tempDay.push_back(worker); // schedule worker on this day

            // try to fill in next worker!
            if (fillDaySchedule(day, worker + 1, avail, dailyNeed, maxShifts, sched, workerShifts, tempDay))
            {
                return true;
            }

            // if didnt get all the way deep to leaf in if statement (return true), backtrack

            // backtrack
            tempDay.pop_back();     // unschedule worker
            workerShifts[worker]--; // reduce num of shifts worked by that worker
        }
    }
    // if finished loop without returning true, no solution
    return false;
}