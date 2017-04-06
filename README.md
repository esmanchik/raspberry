# raspberry
Various scripts and tools I created on my Raspberry PIs

# add following to ~/.bashrc

```
OBC=$(ps ax | grep one-button-control | grep -v grep)
if [ -z "$OBC" ]
then
    echo "No OBC" # | festival --tts
    ~/Desktop/one-button-control.py &
else
    echo "OBC runs" # | festival --tts &
fi
```
