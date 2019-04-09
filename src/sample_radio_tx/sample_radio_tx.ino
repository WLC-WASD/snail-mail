#include "Sll.cpp"

/* DATA TX PINS */
#define DATA_1 2
#define DATA_2 3
#define DATA_3 4
#define DATA_4 5
#define MAIL_SWITCH 8
#define BATTERY_PIN 9 

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

enum EVENT {
  DOOR_OPEN_EVENT,
  DOOR_CLOSED_EVENT,
  //TODO battery events
};


void loop(){
  
  /*Timing variables*/
  const static unsigned long TRANSMISSION_TIME = 1000;
  static unsigned long transmission_start = 0;
  /*event queue*/
  static Sll <EVENT> event_queue;

//  /*TEST EVENTS*/
//  EVENT a = DOOR_OPEN_EVENT;
//  EVENT b = DOOR_CLOSED_EVENT;
//  static boolean addedEvents = false;
//  if(!addedEvents){
//    for(int i = 0; i < 50; i++){
//      event_queue.insert(a);
//      event_queue.insert(b);
//    }
//    addedEvents = true;
//  }

  static boolean opened = false;
  /*Check for new events*/
  if(digitalRead(MAIL_SWITCH) == HIGH){
    if(!opened){
      Serial.println("open event added");
      event_queue.insert(DOOR_OPEN_EVENT);
      opened = true;
    }
  }else{
    if(opened){
      Serial.println("close event added");
      event_queue.insert(DOOR_CLOSED_EVENT);
      opened = false;
    }
  }

  delay(25);

  /*transmit new events from queue*/
  EVENT e;
  if(millis() - transmission_start >= TRANSMISSION_TIME){
    if(event_queue.remove(e)){ //pop event from queue
      unsigned short data = 0xC; //TODO: change from C
      switch(e){
        case DOOR_OPEN_EVENT:
          data |= 1 << 1; //set second bit 
          Serial.println("door opened");
        break;
        case DOOR_CLOSED_EVENT:
          data &= ~(1 << 1); //clear second bit
          Serial.println("door closed");
        break;
      }
      /*count parity*/
      int bitTotal = 0;
      for (int i = 3; i > 0; i--) {
          if(data & (1 << i)) bitTotal++;
      }
      
      if (bitTotal % 2 == 0) {
        data |= 1 << 0;
      }
      else {
        data &= ~(1 << 0);
      }
      
      /*send data*/
      h_write(data);
    }
    transmission_start = millis(); //get current time
  }

}
