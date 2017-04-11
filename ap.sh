#!/bin/bash
sudo apt-get install usb-modeswitch wvdial hostapd dnsmasq
sudo cp 40-modemswitch.rules /etc/udev/rules.d/40-modemswitch.rules
sudo cp wvdial.conf /etc/wvdial.conf

