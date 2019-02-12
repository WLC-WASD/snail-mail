#!/usr/bin/python

import RPi.GPIO as GPIO
import os

# Pins used for recieving data
DATA_PINS = [6, 13, 19, 26]

GPIO.setmode(GPIO.BCM)

# Set Up DATA_PINS to be INPUT pins
for pin in DATA_PINS:
    GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
# Function to print current data
def PrintData(channel):
    os.system('clear')
    print("Current Data")
    print("____________")
    for pin in DATA_PINS:
        print(str(GPIO.input(pin))),

    print("")

# Function to Poll Pins (See if they've changed)
def PollPins():
    for pin in DATA_PINS:
        if GPIO.event_detected(pin):
            PrintData(pin)
            return

# Register callbacks (Print data when we get data!)
for pin in DATA_PINS:
    GPIO.add_event_detect(pin, GPIO.RISING)


PrintData(0)

try:
    while True: PollPins()
except Exception as e:
    print(str(e))
    GPIO.cleanup()
