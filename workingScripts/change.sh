#!/bin/sh
cd /sys/class/pwm/pwm-4:0
echo $1 > duty_cycle
echo 1 > enable
