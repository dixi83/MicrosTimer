#include "MicrosTimer.h"

// Constructor
MicrosTimer::MicrosTimer(uint32_t interval, microsTimerEventHandler handler)
{
  m_interval = interval;
  m_state = STOPPED;                   // Stopped
  m_repeat = m_repeatCount = 0;        // Repeat indefinitely
  m_targetTime = micros() + interval;
  m_remainingTime = 0;
  cb_onExpired = handler;
}


bool MicrosTimer::isRunning() const
{
  return (m_state == RUNNING);
}


void MicrosTimer::run()
{
  expired();
}


bool MicrosTimer::expired()
{
  // Only if we're running
  if (m_state == RUNNING)
  {
    // If we have passed the target time...
    if (micros() >= m_targetTime)
    {
      // Calculate repeat. If repeat = 0, then we
      // repeat forever until stopped.
      // Otherwise, when we've hit the last repeat (1),
      // then we stop.
      if (m_repeatCount != 1)
      {
        if (m_repeatCount > 0)
        {
          m_repeatCount--;
        }
        // Set the new target (based on our last target time
        // for accuracy)
        m_targetTime += m_interval;
      }
      else
      {
        m_state = EXPIRED;
      }

      // Fire the call back.
      if (cb_onExpired)
      {
        cb_onExpired(*this);
      }

      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}


void MicrosTimer::stop()
{
  m_state = STOPPED;
  
  // If we have stopped before the target time,
  // let's save the remaining time so we can resume later.
  if (micros() < m_targetTime)
  {
    m_remainingTime = m_targetTime - micros();
  }
}


// Start the timer.
void MicrosTimer::start()
{
  startFrom(micros());
}


// Start from a specific time provided.
void MicrosTimer::startFrom(uint32_t startTime)
{
  m_state = RUNNING;
  // If we have some remaining time, then let's use that.
  if (m_remainingTime > 0)
  {
    m_targetTime = startTime + m_remainingTime;
    m_remainingTime = 0;
  }
  else
  {
    // otherwise, we start normally
    m_targetTime = startTime + m_interval;
  }
}


// Arbitrarily set the target time.
void MicrosTimer::setTargetTime(uint32_t targetTime)
{
  m_targetTime = targetTime;
}


// Reset the timer. Stop, and reset repeat count.
void MicrosTimer::reset()
{
  m_state = STOPPED;
  m_remainingTime = 0;
  m_repeatCount = m_repeat;
}


void MicrosTimer::setInterval(uint32_t interval)
{
  m_interval = interval;
}


void MicrosTimer::setRepeats(uint32_t repeatCount)
{
  m_repeat = m_repeatCount = repeatCount;
}


void MicrosTimer::expiredHandler(microsTimerEventHandler handler)
{
  cb_onExpired = handler;
}


uint32_t MicrosTimer::getTargetTime() const
{
  return m_targetTime;
}


uint32_t MicrosTimer::getRemainingTime() const
{
  if (m_state == RUNNING)
  {
    return m_targetTime - micros();
  }
  else
  {
    return m_remainingTime;
  }
}


uint32_t MicrosTimer::getRemainingRepeats() const
{
  if (m_state == EXPIRED && m_repeatCount == 1)
    return 0;
  else
    return m_repeatCount;
}