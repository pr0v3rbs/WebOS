#******************************************************************************#
#   SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA                          #
#   Copyright(c) 2013 by LG Electronics Inc.                                   #
#                                                                              #
#   This program is free software; you can redistribute it and/or              #
#   modify it under the terms of the GNU General Public License                #
#   version 2 as published by the Free Software Foundation.                    #
#                                                                              #
#   This program is distributed in the hope that it will be useful,            #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#   GNU General Public License for more details.                               #
#                                                                              #
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	hxenc_cfg.mk                                               #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	jaeseop.so (jaeseop.so@lge.com)                            #
#	DATE        :	2014.01.16                                                 #
#	DESCRIPTION	:	Makefile for building venc module 	                       #
#******************************************************************************#

#-------------------------------------------------------------------------------
# hxenc_cfg.mk
#-------------------------------------------------------------------------------

H1ENCODE_USE_POLLING		:= NO
VENC_HXENC_BUILTIN			:= NO
H1ENCODE_SUPPORT_MULTICORE	:= YES
VENC_HXENC_NONBLOCKING_MODE	:= YES

export VENC_HXENC_BUILTIN H1ENCODE_USE_POLLING
export H1ENCODE_SUPPORT_MULTICORE
export VENC_HXENC_NONBLOCKING_MODE
#EOF
