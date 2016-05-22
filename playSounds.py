# playSounds.py

import pygame.mixer
from time import sleep
from sys import exit
import serial

pygame.mixer.init(44000, -16, 1, 1024) 

soundA = pygame.mixer.Sound("Scream.wav") 
soundB = pygame.mixer.Sound("WilhelmScream.wav")
soundC = pygame.mixer.Sound("CastleThunder.wav")

soundChannelA = pygame.mixer.Channel(1) 
soundChannelB = pygame.mixer.Channel(2)
soundChannelC = pygame.mixer.Channel(3)

print "Sampler Ready."
serialFromArduino = serial.Serial("/dev/ttyACM0",9600)
serialFromArduino.flush()

while True:
    try:
        val = ord(serialFromArduino.read())
        print(val)
        if (val == 2): 
            soundChannelA.play(soundA) 
        if (val == 3):
            soundChannelB.play(soundB)
        if (val == 4):
            soundChannelC.play(soundC)
        val = 0
        sleep(.01)
    except KeyboardInterrupt: 
        exit()
