#!/bin/bash
IFS=$'\n'
set -f
for song in $(find /home/pi/Music/ -type f -name "*.mp?")
do
    mpc add "file://$song"
done

for song in $(find /home/pi/sda/ -type f -name "*.mp?")
do
    mpc add "file://$song"
done
