#!/bin/bash

REF_DIR=/mnt/nfs/svr1/ref_log
FAIL_LIST=$CHIP_DIR/.faillist

SCRIPT_DIR=$(dirname $0)
VDU_PROC=$SCRIPT_DIR/vdu_proc.sh
VDC_PROC=$SCRIPT_DIR/vdc_proc.sh
VDS_PROC=$SCRIPT_DIR/vds_proc.sh

# file count
if [ ! -e $CHIP_DIR/.filecnt ]
then
echo 0 > $CHIP_DIR/.filecnt
fi

FILE_CNT=$(cat $CHIP_DIR/.filecnt)

# fail count
if [ ! -e $CHIP_DIR/.failcnt ]
then
echo 0 > $CHIP_DIR/.failcnt
fi

FAIL_CNT=$(cat $CHIP_DIR/.failcnt)

# write
echo Start time: $(cat $CHIP_DIR/.start_time)
echo End time: $(date)
echo Target chip: $CHIP
echo Total feeddumps: $(cat $CHIP_DIR/.total_feeddump)
echo Total test: $FILE_CNT
echo Total Fail: $FAIL_CNT
echo VDC Fail: $VDC_FAIL_CNT
echo VDU Fail: 0
echo VDS Fail: 0

if [ "$1" != "brief" ]
then
	if [ $FAIL_CNT -gt 0 ]
	then
	echo 
	echo VDC test failed file list :
	cat $VDC_FAIL_LIST
	fi
fi
