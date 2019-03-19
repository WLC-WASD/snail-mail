#include "Sll.cpp"

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
  Serial.begin(115200);
  log("Setup Begin");

  /* Set up TX Pins */
  pinMode(DATA_1, OUTPUT);
  pinMode(DATA_2, OUTPUT);
  pinMode(DATA_3, OUTPUT);
  pinMode(DATA_4, OUTPUT);

  // Clear All Pins
  groupWrite(HIGH);
}

enum EVENT {
  DOOR_OPEN_EVENT,
  DOOR_CLOSED_EVENT,
  //TODO battery events
};


void loop(){

  static Sll <EVENT> event_queue;
  EVENT a = DOOR_OPEN_EVENT;
  EVENT b = DOOR_CLOSED_EVENT;
  event_queue.insert(a);
  event_queue.insert(b);

  /*get current event*/
  unsigned short data = 0xC; //TODO: change from 8
  EVENT e;
  while(event_queue.remove(e)){
    switch(e){
      case DOOR_OPEN_EVENT:
        data |= 1 << 1; //set second bit 
        log("door opened");
      break;
      case DOOR_CLOSED_EVENT:
        data &= ~(1 << 1); //clear second bit
        log("door closed");
      break;
    }
    /*count parity*/
    int bitTotal = 0;
    for (int i = 3; i > 0; i--) {
        if(data & (1 << i)) bitTotal++;
    }
    
    log("total: " + (String)bitTotal);
    
    if (bitTotal % 2 == 0) {
      data |= 1 << 0;
    }
    else {
      data &= ~(1 << 0);
    }
    
    /*send data*/
    h_write(data);
    delay(1000);
  }

}
