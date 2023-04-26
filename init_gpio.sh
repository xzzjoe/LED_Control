#!/bin/sh

echo 66 > /sys/class/gpio/export
cd /sys/class/gpio/gpio66
echo -n out > direction
echo -n rising > edge
