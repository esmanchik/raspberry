#!/usr/bin/python

import RPi.GPIO as GPIO
import time
import sys,tty,termios

GPIO.setmode(GPIO.BCM)
GPIO.setup(27, GPIO.OUT)
GPIO.setup(22, GPIO.OUT)
GPIO.setup(23, GPIO.OUT)
GPIO.setup(24, GPIO.OUT)

print(sys.argv)
quant = 0.025 if len(sys.argv) < 2 else float(sys.argv[1])

def right():
    GPIO.output(23, 1)
    GPIO.output(22, 1)
    time.sleep(quant)
    GPIO.output(23, 0)
    GPIO.output(22, 0)

def left():
    GPIO.output(27, 1)
    GPIO.output(24, 1)
    time.sleep(quant)
    GPIO.output(27, 0)
    GPIO.output(24, 0)

def backward():
    GPIO.output(23, 1)
    GPIO.output(27, 1)
    time.sleep(quant)
    GPIO.output(23, 0)
    GPIO.output(27, 0)

def forward():
    GPIO.output(22, 1)
    GPIO.output(24, 1)
    time.sleep(quant)
    GPIO.output(22, 0)
    GPIO.output(24, 0)

def main():
    quant = 0.025

    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    def say(msg):
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        print(msg)
        tty.setraw(sys.stdin.fileno())
        
    try:
        tty.setraw(sys.stdin.fileno())
        while True:
            ch = sys.stdin.read(1)
            if ch == '\x1b':
                key = sys.stdin.read(2)
                if key == '[A':
                    say('up')
                    forward()
                elif key == '[B':
                    say('down')
                    backward()
                elif key == '[C':
                    say('right')
                    right()
                elif key == '[D':
                    say('left')
                    left()

            else:
                say("%s: %02x" % (ch, ord(ch)))
                if ch == 'q':
                    termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
                    exit(0)
                elif ch == '=':
                    quant = quant * 2.0
                    say(quant)
                elif ch == '-':
                    quant = quant / 2.0
                    say(quant)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

if __name__=='__main__':
    main()
