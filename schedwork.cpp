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
bool backtrackSchedule(
  const AvailabilityMatrix& avail, 
  const size_t dailyNeed, 
  const size_t maxShifts, 
  DailySchedule& sched, 
  std::vector<size_t>& shiftsUsed, 
  size_t day, 
  size_t slot
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }

    size_t nDays = avail.size();
    size_t nWorkers = avail[0].size();

    if(dailyNeed > nWorkers) {
      return false;
    }

    if(nDays * dailyNeed > nWorkers * maxShifts) {
      return false;
    }

    sched.assign(nDays, std::vector<Worker_T>());

    std::vector<size_t> shiftsUsed(nWorkers, 0);

    return backtrackSchedule(avail, dailyNeed, maxShifts, sched, shiftsUsed, 0, 0);



    // sched.clear();
    // Add your code below
}


bool backtrackSchedule(
  const AvailabilityMatrix& avail, 
  const size_t dailyNeed, 
  const size_t maxShifts, 
  DailySchedule& sched, 
  std::vector<size_t>& shiftsUsed, 
  size_t day, 
  size_t slot
){
  size_t nDays = avail.size();
  size_t nWorkers = avail[0].size();

  if(day == nDays) {
    return true;
  }

  if(slot == dailyNeed) {
    return backtrackSchedule(avail, dailyNeed, maxShifts, sched, shiftsUsed, day + 1, 0);
  }

  for(Worker_T w = 0; w < nWorkers; ++ w) {
    if(!avail[day][w]){  // If not avaiable
      continue;
    }

    if(shiftsUsed[w] >= maxShifts) {
      continue;
    }

    // Already on the list
    if(std::find(sched[day].begin(), sched[day].end(), w) != sched[day].end()){
      continue;
    }

    sched[day].push_back(w);
    shiftsUsed[w] ++;

    if(backtrackSchedule(avail, dailyNeed, maxShifts, sched, shiftsUsed, day, slot + 1)){
      return true;
    }

    shiftsUsed[w] --;
    sched[day].pop_back();


  }   
  return false;
}

