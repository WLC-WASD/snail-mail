
/* DATA TX PINS */
#define DATA_1 2
#define DATA_2 3
#define DATA_3 4
#define DATA_4 5

/* POT Pin */
#define RATE 1

// Number of Pins
const int PIN_C = 4;
// Global pins array (assigned in setup)
const int PINS[PIN_C] =
{
  DATA_1,
  DATA_2,
  DATA_3,
  DATA_4,
};
// Max Analog Read
const int ANALOG_MAX = 1023;

void log(String message)
{
  Serial.println("[LOG] " + message);
}

int getRate()
{
  int curVal = analogRead(RATE);

  // If curVal below minimum threshold, return false (infinite delay)
  if(curVal < 35) return false;

  // Set the lowest number you wish this 
  // function to return.
  int maxRate = 25; 
  
  return (ANALOG_MAX + maxRate) - curVal;
}

void setup() {

  /* Enable Serial Communication */
  Serial.begin(9600);
  log("Setup Begin");

  /* Set up TX Pins */
  pinMode(DATA_1, OUTPUT);
  pinMode(DATA_2, OUTPUT);
  pinMode(DATA_3, OUTPUT);
  pinMode(DATA_4, OUTPUT);

  // Clear All Pins
  groupWrite(HIGH);
}

//void loop() {
//  log("Loop Begin");
//  while(!getRate()); // Don't continue if rate is at lowest position!
//
//  // State Tracker
//  static int state = 0;
//
//  // Turn On Pins 1 at a time
//  int curPin = state % PIN_C;
//  
//  b_write(PINS[curPin], LOW);
//
//  // Nice Delay :)
//  delay(getRate());
//  while(!getRate()); // Don't continue if rate is at lowest position!
//
//  // Turn Off Pin
//  b_write(PINS[curPin], HIGH);
//
//  // Update State
//  state++;
//}

void loop(){
  //b_write(PINS[0],HIGH);
  static int count = 0;
  log("Loop Begin");
  
  h_write(count++);
  delay(getRate());
  if (count == 16) count = 0;
}
