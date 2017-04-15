#!/usr/bin/python

import RPi.GPIO as GPIO
import time
import subprocess
from espeak import espeak
from glob import glob

GPIO.setmode(GPIO.BCM)
GPIO.setup(18, GPIO.IN, pull_up_down=GPIO.PUD_UP)

names = ['next', 'play', 'stop', 'halt', '85', '90', '95', '99', \
         'router', 'metal', 'add', 'shuf', 'clear', \
         'mount', 'umount', \
         'halt in 10 min', 'reboot', 'ip']
commands = [ \
    ['mpc', 'next'], ['mpc', 'play'], ['mpc', 'stop'], ['sudo', 'halt'], \
    ['mpc', 'volume', '85'], ['mpc', 'volume', '90'], \
    ['mpc', 'volume', '95'], ['mpc', 'volume', '99'], \
    ['/home/pi/Desktop/raspberry/router.sh'], \
    ['mpc', 'load', 'metalrockmania'], ['/home/pi/Desktop/raspberry/music.sh'], \
    ['mpc', 'shuffle'], ['mpc', 'clear'], \
    ['sudo', 'mount', '/dev/sda', '/home/pi/sda'], \
    ['sudo', 'umount', '/dev/sda'], \
    ['sh', '-c', 'sleep 600 && sudo halt'], ['sudo', 'reboot'],  \
    ['/home/pi/Desktop/raspberry/tellip.sh'] \
]

while True:
    j = -1
    for i in range(0, len(names)):
        input_state = GPIO.input(18)
        if input_state == False:
            j = j + 1
            print(names[j])
            #j = j % len(names[j])
            #espeak.synth('%d' % j)
            espeak.synth(names[j])
            time.sleep(1.0)

    if j >= 0:
        #espeak.synth(' '.join(commands[j]))
        subprocess.Popen(commands[j], stdout=subprocess.PIPE)

    time.sleep(1.0)
