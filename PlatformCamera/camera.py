#!/usr/bin/python3

import RPi.GPIO as GPIO
from picamera import PiCamera
from time import sleep
import os
import re

lights_pin = 17
n_images = 10
image_interval = 10

camera = PiCamera(resolution=(1920, 1080))
camera.start_preview()
sleep(5)

GPIO.setmode(GPIO.BCM)
GPIO.setup(lights_pin, GPIO.OUT)

# Determine the next empty image index.
index = 0
for image in os.listdir('images'):
    match = re.match(r'image_(\d+).jpg', image)
    if match is not None:
        if int(match.group(1)) >= index:
            index = int(match.group(1)) + 1

# Capture images
for i in range(n_images):
    GPIO.output(lights_pin, GPIO.HIGH)
    filename = 'images/image_{0:06d}.jpg'.format(index)
    index += 1
    print("Capturing image", filename)
    camera.capture(filename)
    GPIO.output(lights_pin, GPIO.LOW)
    sleep(image_interval)

GPIO.cleanup()
