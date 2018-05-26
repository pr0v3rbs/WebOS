#!/bin/sh
CURR_DATE=`date +%Y.%m.%d`
MOD_L=`echo $1 |awk '{printf(tolower($1))}'`
MOD_U=`echo $1 |awk '{printf(toupper($1))}'`
MODULE=$MOD_L

BASE_DIR=..
TEMPL_DIR=${BASE_DIR}/template
TEMPL_DOC_DIR=${BASE_DIR}/../doc
TEMPL_INC_DIR=${BASE_DIR}/include

DEST_DIR=${BASE_DIR}/$1
DEST_DOC_DIR=${BASE_DIR}/../doc
DEST_INC_DIR=${BASE_DIR}/include

# valid author list for a module.
# Shell environment variable $USER shall be checked for module author.
# module author shall be automatically replaced in every source file
# and documentation file(e.g. .dox)
author_list=`echo -n eunjoo.kim wonsik.do sh.myoung dongho7.park ingyu.yang jongsang.oh jeongpil.yun seokjoo.lee raxis.lim donghyeon.joh gauius jasonlee.lee jaemo.kim srinivasan.shanmugam ks.hyun`

function check_author()
{
	#r=1
	#while [ $r -le 3 ]
	while [ 1 ]
	do
		for w in $author_list
		do
			if [[ ! -z $w && $w = $AUTHOR ]]; then
			AUTHOR="$AUTHOR ($AUTHOR@lge.com)"
			return 0
			fi
		done

		#echo "AUTHOR is not vald please valid author LGE SSO ID only  :"
		read -p "enter Valid Author name(LGE SSO ID)(Ctrl+C to exit): " AUTHOR
		#r=`expr $r + 1`
	done
	echo bad author. Aborting!
	exit 1
}

# $1 src file (with path)
# $2 module name.
# $2 destination directory
function sed_script ()
{
	MOD_L=`basename $2 |awk '{printf(tolower($1))}'`
	MOD_U=`basename $2 |awk '{printf(toupper($1))}'`

	srcfile=`basename $1`
	dstfile=`echo $srcfile | sed -e s/template/$MOD_L/g`
	postfix=`echo $srcfile | sed -e s/template_//g`

	if [ ! -f $1 ] ; then
		echo "source template not found : $1"
		return;
	fi

	if [ -f "$3/$dstfile" ]; then
		echo " file exists $3/$dstfile, skipping..."
		return
	else
		echo "$3/$dstfile"
	fi

	sed -e s/template/$MOD_L/g 											\
		-e s/tempalte/$MOD_L/g 											\
		-e s/TEMPLATE/$MOD_U/g 											\
		-e s/Template/$MOD_U/g 											\
		-e "/*  author.*modifier$/d"									\
		-e "s/*  author.*$/*  author		$AUTHOR/"					\
		-e "s/*  @author.*$/*  @author		$AUTHOR/"					\
		-e "s/raxis/$AUTHOR/"											\
		-e "s/*  date		2009.11.*$/*  date		$CURR_DATE/"		\
 		-e "s/*  @date		2009.11.*$/*  @date		$CURR_DATE/"		\
		-e "s/*  date		2010.*$/*  date		$CURR_DATE/"		\
 		-e "s/*  @date		2010.*$/*  @date		$CURR_DATE/"		\
		-e "s/#	AUTHOR		:	.*$/#	AUTHOR		:	$AUTHOR/"		\
		-e "s/#	DATE        :	.*$/#	DATE        :	$CURR_DATE/"	\
		-e "s/(c) 1998-2007 by LG E/(c) 1998-2010 by LG E/"				\
		 $1  > $3/$dstfile
}

#########################################################################
# Actual Starts Here.
# generate from template to module skelenton files.
# check condition.
if [ -z "$1" ]; then
	echo Usage : `basename $0` module_name
	echo "      ---- skelenton file maker for given module (.c, .h, .dox) ----"
	echo "      this shall make..."
	echo "      .c, .h into $BASE_DIR/'module_name' using $TEMPL_DIR/.c,.h"
	echo "      'module_name'_kapi.h into $DEST_INC_DIR"
	echo "      .dox into $DEST_DOC_DIR"
	exit 0
fi

if [ ! -d $TEMPL_DIR ]; then
	echo cannot find "$TEMPL_DIR" in current directory.
	exit 0
fi

AUTHOR=$USER
check_author;

if [ ! -d $DEST_DIR ]; then
	mkdir -p $DEST_DIR
fi

for src in $TEMPL_DIR/*.[ch] $TEMPL_DIR/doc.mk $TEMPL_DIR/Makefile
do
	sed_script $src $MODULE $DEST_DIR
done

for src in $TEMPL_DOC_DIR/*_template.dox
do
	sed_script $src $MODULE $DEST_DOC_DIR
done

for i in $DEST_DIR/*kapi.h
do
	target=${DEST_INC_DIR}/`basename $i`
	if [[ -f $i ]]; then
		if [[ ! -f $target ]]; then
			echo moving $i to $target
			mv $i $target
		else
			echo skipping $i
			rm -f $i
		fi
	fi
done

