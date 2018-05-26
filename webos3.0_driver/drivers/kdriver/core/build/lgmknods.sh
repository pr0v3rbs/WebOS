#!/bin/sh
# raxis.lim@lge.com -- simple device node creator for LG1150 kernel driver

BASE_DIR="/dev/lg"

make_node ()
{
	while read name node_type node_num dir junk ; do
		# discard comment and blank line
		[ "x${name:0:1}" == "x#" ] 	&& continue
		[ "x${name}" == "x" ] 		&& continue

		# parse /proc/devices and find major number for our device
#		local major=`grep -w $name /proc/devices | cut -d ' ' -f 1`
		local major=`grep " $name$" /proc/devices | cut -d ' ' -f 1`

		[ "x${major}" == "x" ] && echo "unknown major: skip device $dir $name" && continue

		i=0
		while [ "$i" -lt "$node_num" ]
		do
			# call mknod : <usage> mknod [OPTIONS] NAME TYPE MAJOR MINOR
			mknod $BASE_DIR/$dir/${name}${i}  ${node_type}  ${major}  ${i}
			i=`expr $i + 1`
		done
	done
}

#------------------------------------------------------------------------------
# START
#------------------------------------------------------------------------------
umask 000
rm -rf $BASE_DIR/* >& /dev/null || ( echo "[!] static /dev/lg detected..." && kill -SIGINT $$ )

echo "[!] creating device nodes..."
mkdir -p $BASE_DIR

make_node  <<__EOF
#------------------------------------------------------------------------------
# <usage>
# device_name  device_type	num_devices dir_name
#	gpio		c|b				1			.
#------------------------------------------------------------------------------
gpio	c	1	.
aad		c	1	.
adec	c	1	.
iic		c	16	.
afe		c	1	.
de		c	1	.
hdmi	c	1	.
fmt3d	c	1	.
dimm	c	1	.
denc	c	1	.
pql		c	1	.
frc		c	1	.
sdec	c	1	.
vdec	c	4	.
ci		c	1	.
ttx		c	1	.
venc	c	1	.
png		c	1	.
demod	c	1	.
gfx		c	1	.
se		c	1	.
mali	c	1	.
pvr		c	2	.
clk		c	1	.
__EOF
