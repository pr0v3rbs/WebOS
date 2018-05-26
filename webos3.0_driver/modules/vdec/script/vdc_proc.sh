#!/bin/bash

SRC_FILE="$1"
SRC_DIR=$(dirname "$SRC_FILE")
TRG_DIR="$SRC_DIR"/vdc
TRG_FILE="$TRG_DIR"/$(basename "$SRC_FILE").vdc
REF_FILE="$REF_DIR"${TRG_FILE##*"$USER_DIR"}
REPORT_FILE="$TRG_FILE".report

echo VDC post-processing

if [ ! -e "$TRG_DIR" ]
then
mkdir "$TRG_DIR"
fi

#awk -F'[(): ]+' '/PUSH/{ print "PTS : " $39}' $SRC_FILE > $TRG_FILE
#sed 's/(0x/ /g' $SRC_FILE | awk -F'[(): ]+' '/PUSH/{ print "PTS : " $38}' > $TRG_FILE
#awk -F'\(0x|\)' '/_VDC_PrintPushInfo/{ split($4,_," ");print "PTS : " _[1]}' $SRC_FILE > $TRG_FILE
awk -F'\(0x' '/PUSH/{ gsub(/[ )]/, "", $2); print "PTS: "$2}' "$SRC_FILE" > "$TRG_FILE"

if [ ! -e "$REF_FILE" ]
then
echo No reference file : "$REF_FILE"
echo Register new reference file with $(basename "$TRG_FILE")
mkdir -p "$(dirname "$REF_FILE")"
cp "$TRG_FILE" "$REF_FILE"
fi

diff "$REF_FILE" "$TRG_FILE" > "$REPORT_FILE"
comp_value=$?
if [ $comp_value -eq 1 ]
	then
	echo Mismatch with reference log!!!
	VDC_RESULT=fail
	else
	rm "$REPORT_FILE"
	VDC_RESULT=ok
fi
