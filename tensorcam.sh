#!/bin/sh

# to let this script work first compile and install flite:
# wget http://www.festvox.org/flite/packed/flite-2.0/flite-2.0.0-release.tar.bz2

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
