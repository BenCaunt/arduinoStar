// array containing our pins
int pins[] = {A2,A3,A4,A5};
// elapsed time since board startup
unsigned long time;
// elapsed hours since startuo
unsigned int hours;
// amount of milliseconds in 5 minutes
unsigned long fiveMinMilliseconds = 300000;
const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600*SECOND;
// time that the board starts at, redefined each day
unsigned long boardStartTime = millis();
// boolean that decides if the board is currently in an off state or not
// used to go to sleep
bool boardSleep = false;

void setup() {
	// configure pins A2 - A5 to be OUTPUT to power LED's
	for (int i = 0; i < 4; i++)
	{
		pinMode(pins[i], OUTPUT);
	}
}
// function that on call, preforms the default LED pattern
void originalPattern() {
	for (int i = 0; i < 4; i++)
	{
		// test if the current LED is the first LED as it is special because it means that the last LED must be off, not the one before it
		if (i == 0)
		{
			// turn on first LED and turn off the last
			digitalWrite(pins[i], HIGH);
			// turn off last LED
			digitalWrite(pins[3], LOW);
		}
		else
		{
			// it is not the first LED so the program can follow the simple algorithm stating the following:
			// given any ith led,  turn led i to HIGH and set led i-1 to Low
			digitalWrite(pins[i], HIGH);
			digitalWrite(pins[i-1], LOW);
		}
		// delay for 100 milliseconds
		delay(100);
	}
}

// function that on call, preforms the REVERSED LED pattern
void reversedPattern() {
	for (int i = 3; i >= 0; i--)
	{
		// test if the current LED is the Last  LED as it is special because it means that the First LED must be off, not the one after it
		if (i == 3)
		{
			// turn on Last LED and turn off the last
			digitalWrite(pins[i], HIGH);
			// turn off First LED
			digitalWrite(pins[0], LOW);
		}
		else
		{
			// it is not the Last LED so the program can follow the simple algorithm stating the following:
			// given any ith led,  turn led i to HIGH and set led i+1 to Low
			digitalWrite(pins[i], HIGH);
			digitalWrite(pins[i+1], LOW);
		}
		// delay for 100 milliseconds
		delay(100);
	}
}


// on the call of this function, the last LED flashes the number of times of the value of flashes
void ledFlash(int flashes, int pin) {
	for (int i = 0; i < flashes; i++)
	{
		digitalWrite(pins[pin],HIGH);
		delay(100);
		digitalWrite(pins[pin],LOW);
		delay(50);
	}
}

// runs the full pattern
void fullPattern() {
	// repeats pattern of one flashing after the other 3 times
	for (int pattern1 = 0; pattern1 < 3; pattern1++)
	{
		originalPattern();
	}
	// flash last led 3 times
	ledFlash(3, 3);
	// do reverse pattern 3 times
	for (int pattern2 = 0; pattern2 < 3; pattern2++)
	{
		reversedPattern();
	}
	// flash first led 3 times
	ledFlash(3,0);

}

void loop() {
  // turn the board off if it has been on for 6 hours or more
  if (time > boardStartTime + (HOUR * 6))
  {
    boardSleep = true;
  }
  Serial.print("Time: ");
  time = millis();
  // start of loop
  long startTime = time;
  // the board is NOT asleep
  if (boardSleep == false)
  {
    // preformLED blink sequence 40 times
    for (int i = 0; i < 40; i++)
    {
      Serial.println("preforming blink sequence ");
			fullPattern();
    }
    if (time < startTime + ((SECOND * 60) * 5))
    {
      Serial.println("waiting 5 minutes...");
      delay((SECOND * 60) * 5);
    }
  }
  // THE BOARD IS ASLEEP AND SHOULD DO SO FOR 16 HOURS (from 3pm to 6am next day)
  else
  {
    Serial.println("board is asleep for 16 hours...");
    delay(16*HOUR);
    Serial.println("board is awake");
    // turn the board back on after delay
    boardSleep = false;
    // time that the board starts at
    unsigned long boardStartTime = millis();
    Serial.print("board is awake at: ");
    Serial.println(millis());
    delay(3000);
  }

  Serial.println(time); //prints time since program started
  delay(1000);          // wait a second so as not to send massive amounts of data

}
