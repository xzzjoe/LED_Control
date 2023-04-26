#!/bin/sh
echo 0 > /sys/class/pwm/pwmchip4/pwm-4:0/enable
echo 0 > /sys/class/pwm/pwmchip4/unexport
echo 66 > /sys/class/gpio/unexport 