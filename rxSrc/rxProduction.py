#!/usr/bin/python

import RPi.GPIO as GPIO
import os

###################### PIN SETUP #################
# Pins used for recieving data
DATA_PINS = [26, 19, 13, 6]#6, 13, 19, 26]

GPIO.setmode(GPIO.BCM)

# Set Up DATA_PINS to be INPUT pins
for pin in DATA_PINS:
    GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

###################### /PIN SETUP ###############


###################### EVENT FUNCTIONS ##########
def DoorOpen():
    print("DoorOpen")

def DoorClose():
    print("DoorClose")    

def BatteryLow():
    print("BatteryLow")

def BatteryNotLow():
    print("BatteryNotLow")

codeActions = {
        0x02: DoorOpen,
        0x00: DoorClose,
        0x08: BatteryLow,
        0x0c: BatteryNotLow
}

###################### /EVENT FUNCTIONS #########

# Function to print current data
def ReadData():
    statusCode = ""
    for pin in DATA_PINS:
        statusCode += str(GPIO.input(pin))
    #print("Status Code: " + statusCode)
    return statusCode

PreviousDoorMask = -1
PreviousBatteryMask = -1

# Function to translate status codes to actions
def Translate(code):
    global PreviousDoorMask
    global PreviousBatteryMask
    ReturnFunctions = []
    intcode = int(code, 2)
    DoorMask = 0x02
    BatteryMask = 0x0c

    MaskedDoor = intcode & DoorMask
    MaskedBattery = intcode & BatteryMask
   
    # NOTE: The result of the door and battery maskings should NOT be 
    # equal to each other, thus equality signifies an invalid code.
    # We'll assume that an invalid code should result in no actions.
    if MaskedDoor == MaskedBattery:
#        print("Invalid Code: " + code)
        return ReturnFunctions

    if PreviousDoorMask != MaskedDoor:
        ReturnFunctions.append(codeActions[MaskedDoor])

    if PreviousBatteryMask != MaskedBattery:
        try:
            ReturnFunctions.append(codeActions[MaskedBattery])
        except:
            print("Invalid Code: " + code)
            return ReturnFunctions

    PreviousBatteryMask = MaskedBattery
    PreviousDoorMask = MaskedDoor

    return ReturnFunctions
    

# Function to Poll Pins (See if they've changed)
def PollPins():
    for pin in DATA_PINS:
        if GPIO.event_detected(pin):
            currentData = ReadData()
            actions = Translate(currentData)
            
            for action in actions:
                action()
            return

# Register callbacks
for pin in DATA_PINS:
    GPIO.add_event_detect(pin, GPIO.RISING)

#try:
while True: PollPins()
#except Exception as e:
#    print("Exception: " + str(e))
#    GPIO.cleanup()
