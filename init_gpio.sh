#!/bin/sh

echo 66 > /sys/class/gpio/export
echo -n out > /sys/class/gpio/gpio66/direction
echo -n rising > /sys/class/gpio/gpio66/edge
