#!/bin/bash

. $(dirname $0)/vvs_conf.sh
USER_CHIP=$(dirname "$1")
USER_CHIP=${USER_CHIP##*$RESULT_DIR}
USER_NAME=$(echo "$USER_CHIP" | awk -F'/' '{printf $2}')
CHIP=$(echo "$USER_CHIP" | awk -F'/' '{printf $3}')

USER_DIR=$RESULT_DIR/$USER_NAME
CHIP_DIR=$USER_DIR/$CHIP

FAIL_LIST=$CHIP_DIR/.faillist
VDC_FAIL_LIST=$CHIP_DIR/.vdc_faillist
OUT_FILE=$CHIP_DIR/test_report.txt

SCRIPT_DIR=$(dirname $0)
VDU_PROC=$SCRIPT_DIR/vdu_proc.sh
VDC_PROC=$SCRIPT_DIR/vdc_proc.sh
VDS_PROC=$SCRIPT_DIR/vds_proc.sh
MAKE_REPORT=$SCRIPT_DIR/make_report.sh

get_variable () {
if [ ! -e $1 ]
	then
	echo 0 > $1
	fi
echo $(cat $1)
}

# file count
FILE_CNT=$(get_variable $CHIP_DIR/.filecnt)
FAIL_CNT=$(get_variable $CHIP_DIR/.failcnt)
VDC_FAIL_CNT=$(get_variable $CHIP_DIR/.vdc_failcnt)

FILE_CNT=$((FILE_CNT + 1))
echo $FILE_CNT > $CHIP_DIR/.filecnt

echo \#$FILE_CNT Post Processing for $(basename "$1")

# do vdc log test
. $VDC_PROC "$1"

TEST_RESULT=ok
if [ "$VDC_RESULT" != "ok" ]
then
TEST_RESULT=fail
VDC_FAIL_CNT=$((VDC_FAIL_CNT + 1))
echo $VDC_FAIL_CNT > $CHIP_DIR/.vdc_failcnt
echo $1 >> $VDC_FAIL_LIST
fi

if [ "$TEST_RESULT" != "ok" ]
then
FAIL_CNT=$((FAIL_CNT + 1))
echo $FAIL_CNT > $CHIP_DIR/.failcnt
echo $1 >> $FAIL_LIST
fi

echo $FAIL_CNT/$FILE_CNT feeddumps failed
. $MAKE_REPORT > $OUT_FILE
