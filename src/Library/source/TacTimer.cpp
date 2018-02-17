/*
  @file     TacTimer.cpp
  @author   Nathan Park
*/

#include "TacTimer.h"
#include "TacErrorIf.h"
namespace Tac
{
  #define ClockNeverStarted -1
  Timer::Timer() 
    //: mLastClock(ClockNeverStarted)
  {
    Reset();
  }

  void Timer::Start()
  {
    //mElapsedClocks = 0;
    //mLastClock = clock();
    mLastTimePoint = std::chrono::high_resolution_clock::now();
  }

  void Timer::Stop()
  {
    //ErrorIf(mLastClock == ClockNeverStarted, 
      //"Stopping a timer that has never been started");

    //clock_t currentClock = clock();
    //mElapsedClocks += currentClock - mLastClock;
    //mLastClock = currentClock;
    std::chrono::high_resolution_clock::time_point now 
      = std::chrono::high_resolution_clock::now();
    

    std::chrono::duration<float> timeDiff 
      = std::chrono::duration_cast<std::chrono::duration<float> >(
      now - mLastTimePoint);

    mElapsedSeconds += timeDiff.count();
  }

  float Timer::GetElapsedTime()
  {
    //return (float)mElapsedClocks / CLOCKS_PER_SEC;
    return mElapsedSeconds;
  }

  void Timer::Reset()
  {
    //mElapsedClocks = 0;
    mElapsedSeconds = 0;
  }
} // Tac
