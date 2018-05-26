#!/bin/bash

echo printk:0 > /proc/logm/status

#VDC log enable
echo 5 -4 > /proc/lxvdec/logmask