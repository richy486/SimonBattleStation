# playSounds.py

import pygame.mixer
from time import sleep
from sys import exit
import serial

pygame.mixer.init(44000, -16, 1, 1024) 

soundA = pygame.mixer.Sound("Sounds/PhoneRinging.wav")     # Phone 
soundB = pygame.mixer.Sound("Sounds/HornHonk.wav")         # red button
soundC = pygame.mixer.Sound("Sounds/Snorting.wav")         # Pig
soundD = pygame.mixer.Sound("Sounds/ElectricShockZap.wav") # Knife switch
soundE = pygame.mixer.Sound("Sounds/Stapler.wav")          # Stapler

soundWin = pygame.mixer.Sound("Sounds/Cheering.wav")
soundGameOver = pygame.mixer.Sound("Sounds/SadTrombone.wav")

soundChannelA = pygame.mixer.Channel(1) 
soundChannelB = pygame.mixer.Channel(2)
soundChannelC = pygame.mixer.Channel(3)
soundChannelD = pygame.mixer.Channel(4) 
soundChannelE = pygame.mixer.Channel(5)
soundChannelF = pygame.mixer.Channel(6)
soundChannelG = pygame.mixer.Channel(7)

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
        if (val == 5):
            soundChannelD.play(soundD)
        if (val == 6):
            soundChannelE.play(soundE)
        if (val == 7):
            soundChannelF.play(soundWin)
        if (val == 8):
            soundChannelF.play(soundGameOver)
        val = 0
        sleep(.01)
    except KeyboardInterrupt: 
        exit()
