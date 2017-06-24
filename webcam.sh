#!/bin/sh

IMAGE=$(date -Iseconds)
fswebcam --no-banner -r 1280x960 ~/Desktop/camera/$IMAGE.jpg
echo shot $IMAGE | festival --tts
