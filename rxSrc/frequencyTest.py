#!/usr/bin/python

import pigpio
import os
import time

GPIO = pigpio.pi()

COUNT = 0

# Pins used for recieving data
DATA_PINS = [6]

START_TIME = time.time()
GPIO.set_mode( DATA_PINS[0], pigpio.INPUT)
GPIO.set_pull_up_down(DATA_PINS[0], pigpio.PUD_UP)

# Function to print current data
def PrintData(channel):
    os.system('clear')
    print("Current Data")
    print("____________")
    global COUNT
    COUNT += 1
    freq = (time.time() - START_TIME) / COUNT * 2
    print(str((freq)) + " bps COUNT: " + str(COUNT))
    print("")

# Function to Poll Pins (See if they've changed)
PREV_STATE = GPIO.read(DATA_PINS[0])
def PollPins():
    global PREV_STATE
    curState = GPIO.read(DATA_PINS[0])
    os.system('clear')
    if curState != PREV_STATE:
        PrintData(pin)
    PREV_STATE = curState


if GPIO.wait_for_edge(DATA_PINS[0]):
       print("Rising edge detected")
else:
  print("wait for edge timed out")

if GPIO.wait_for_edge(DATA_PINS[0], pigpio.FALLING_EDGE, 5.0):
     print("Falling edge detected")
else:
    print("wait for falling edge timed out")

try:
    while True: PollPins()
except Exception as e:
    print(str(e))
    GPIO.stop()
