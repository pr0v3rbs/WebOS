#!/bin/bash

. $(dirname $0)/vvs_conf.sh

IP_ADDR=$(ifconfig | awk -F':' '/inet addr/&&!/127.0.0.1/{split($2,_," ");print _[1]}')
TEST_FEEDDUMP=$NFS_DIR/bin/test_feeddump
INIT_SH=$NFS_DIR/script/vvs_init.sh

if [ $# -ne 1 ]
then
echo "No user name is inputed"
echo " -> use IP as user name :" $IP_ADDR
USER_NAME=$IP_ADDR
else
USER_NAME=$1
fi

CMD_LINE=$(cat /proc/cmdline)
CHIP="${CMD_LINE##*chip=}"
CHIP="${CHIP%% *}"

USER_DIR=$RESULT_DIR/$USER_NAME
CHIP_DIR=$USER_DIR/$CHIP
PARSER_SCRIPT=$(dirname $0)/post_proc.sh

echo Chip name : $CHIP

if [ ! -e $FEEDDUMP_DIR ]
then
	busybox mount -o port=2049,nolock,proto=tcp -t nfs $NFS_SERVER $NFS_DIR
fi

if [ ! -e $USER_DIR ]
then
	mkdir $USER_DIR
fi

if [ -e $CHIP_DIR ]
then
	echo remove $CHIP_DIR
	rm -rf $CHIP_DIR
fi

echo User Dir : $USER_DIR

. $INIT_SH

FDDUMP_LIST=$(find $FEEDDUMP_DIR -name "*.feeddump" -type f -print)
FDDUMP_NUM=$(find $FEEDDUMP_DIR -name "*.feeddump" -type f -print | wc -l)
FDDUMP_SUBDIR_LIST=$(find $FEEDDUMP_DIR -type d -print)

SAVEIFS=$IFS
IFS=$(echo -en "\n\b")
# make directories
for fddir in $FDDUMP_SUBDIR_LIST
do
	subdir=$CHIP_DIR${fddir##*$FEEDDUMP_DIR}
	if [ ! -e "$subdir" ]
	then
		echo make subdir: "$subdir"
		mkdir "$subdir"
	fi
done

echo $(date) > $CHIP_DIR/.start_time
echo $FDDUMP_NUM > $CHIP_DIR/.total_feeddump

FDDUMP_DONE_NUM=0
FDDUMP_PASS_NUM=0
FDDUMP_FAIL_NUM=0

# test feed dumps
for fddump in $FDDUMP_LIST
do
	echo 
	echo Test Start : $fddump
	echo ========================================================================
	res_file=$CHIP_DIR${fddump##*$FEEDDUMP_DIR}.res
	echo Result log path : $res_file
	cat /dev/lg/logm > $res_file &
	catpid=$(echo $!)
	$TEST_FEEDDUMP -d 1 $fddump
	sleep 1
	kill $catpid
	if [ -e $PARSER_SCRIPT ]
	then
	$PARSER_SCRIPT $res_file
	fi
	echo ========================================================================
	FDDUMP_DONE_NUM=$((FDDUMP_DONE_NUM + 1))
	echo $FDDUMP_DONE_NUM of $FDDUMP_NUM feeddump test completed
	echo 
done
IFS=$SAVEIFS

echo All tests are finished!!
echo
. $(dirname $0)/make_report.sh brief
echo

echo Check result : \\\\$(echo $SAMBA_ADDR${CHIP_DIR##*$NFS_DIR} | sed 's/\//\\/g')
