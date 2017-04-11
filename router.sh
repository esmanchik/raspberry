#!/bin/sh

sudo cp dnsmasq.conf /etc/dnsmasq.conf
sudo cp iptables.ipv4.nat /etc/iptables.ipv4.nat

sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
sudo iptables-restore < /etc/iptables.ipv4.nat
sudo wvdial
