#!/bin/bash
ADDRESSES=$(ifconfig | grep 'inet addr' | cut -d : -f 2 | cut -d ' ' -f 1)
for IP in $ADDRESSES
do
    echo "your IP address is $IP" | festival --tts
done
