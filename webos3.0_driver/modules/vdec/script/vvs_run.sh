#!/bin/bash

RUNSH_VERSION=7
NFS_SERVER=10.178.36.95:/volume1/VVS
NFS_SERVER2=10.178.36.90:/nfsroot/columbus
NFS_DIR=/mnt/nfs/svr1
NFS_DIR2=/mnt/nfs/columbus
NFS_SCRIPT_DIR=$NFS_DIR/script
IP_ADDR=$(ifconfig | awk -F':' '/inet addr/&&!/127.0.0.1/{split($2,_," ");print _[1]}')

if [ $# -ne 1 ]
then
echo "No user name is inputed"
echo " -> use IP as user name :" $IP_ADDR
USER_NAME=$IP_ADDR
else
USER_NAME=$1
fi

if [ "$1" = "help" ]
then
echo version $RUNSH_VERSION
echo $(basename $0) \[user name\] : run with the user name, result log will be stored in user name based location
echo $(basename $0) ver : print version
echo $(basename $0) up : update script from server
echo $(basename $0) mount : just mount nfs dir \& exit
exit
fi

if [ "$1" = "ver" ]
then
echo $RUNSH_VERSION
exit
fi

# mount NFSs
if ! grep -qs $NFS_DIR /proc/mounts;
then
	if [ ! -e $NFS_DIR ]
	then
		mkdir $NFS_DIR
	fi
	busybox mount -o port=2049,nolock,proto=tcp -t nfs $NFS_SERVER $NFS_DIR
fi

if ! grep -qs $NFS_DIR2 /proc/mounts;
	then
	if [ ! -e $NFS_DIR2 ]
	then
		mkdir $NFS_DIR2
	fi
	busybox mount -o port=2049,nolock,proto=tcp -t nfs $NFS_SERVER2 $NFS_DIR2
fi

if [ "$1" = "mount" ]
then
exit
fi

# use remote localtion for scripts if there are no main 
#  scripts in the same directory
if [ -e ./vvs.sh ]
then
echo Use local VVS script
SCRIPT_LOC="LOCAL"
VVS_SH=./vvs.sh
else
echo Use remote VVS script
SCRIPT_LOC="REMOTE"
VVS_SH=$NFS_SCRIPT_DIR/vvs.sh
fi

# copy scripts to VVS NFS server
if [ "$1" = "push" ]
then
	if [ $SCRIPT_LOC = "LOCAL" ]
	then
	echo push scripts to VVS server
	cp $(dirname $0)/*.sh $NFS_SCRIPT_DIR/
	else
	echo Fail!! It\'s not local mode
	fi
exit
fi

# update current script from VVS NFS server
runsh_ver_server=$(/bin/bash $NFS_SCRIPT_DIR/vvs_run.sh ver)
if [ "$1" = "up" ]
then
echo Update to ver.$runsh_ver_server for $0
cp $NFS_SCRIPT_DIR/vvs_run.sh $0
exit
fi

if [ $runsh_ver_server -gt $RUNSH_VERSION ]
then
echo There are new version\(Current:$RUNSH_VERSION New:$runsh_ver_server\)
echo Update to ver.$runsh_ver_server to $0
cp $NFS_SCRIPT_DIR/vvs_run.sh $0
echo Launch new version...
/bin/bash $0 $USER_NAME
exit
fi

/bin/bash $VVS_SH $USER_NAME
