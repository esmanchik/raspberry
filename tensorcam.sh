#!/bin/sh

TFCAM=$(ps ax | grep camera | grep -v grep)
echo $TFCAM
if [ -z "$TFCAM" ]
then
    echo "Run tensorcam" | festival --tts
    WD=$(pwd)
    cd /home/pi/Desktop/tensorflow
    tensorflow/contrib/pi_examples/camera/gen/bin/camera
    cd $WD
else
    echo "Kill tensorcam" | festival --tts
    killall camera
fi


