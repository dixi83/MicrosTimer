#include "MicrosTimer.h"

// Create a timer that fires every 1000 milliseconds.
MicrosTimer timer1 = MicrosTimer(1000);

// This is the function that is called when the timer expires.
void myTimerFunction(MicrosTimer &mt)
{
  Serial.print(Constant("Repeat: "));
  Serial.println(mt.getRemainingRepeats());
}


void setup()
{
  Serial.begin(9600);

  timer1.setInterval(1000);
  timer1.expiredHandler(myTimerFunction);
  timer1.setRepeats(5);
  timer1.start();
}


void loop()
{
  timer1.run();

  if (!timer1.isRunning())
  {
    Serial.println(Constant("Timer finished!"));
    for (;;);
  }
  
  delay(10);
}