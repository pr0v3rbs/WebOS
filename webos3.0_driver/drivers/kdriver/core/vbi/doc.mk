
# SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
# Copyright(c) 2013 by LG Electronics Inc.

# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
 

# stand alone makefile
# if not set DOC_CONFIG, then assumes DOC_OUTPUT_DIR/Doxyfile
MODULE_NAME=`basename ${PWD}`
DOC_PROJECT_VERSION=0.6a
DOC_OUTPUT_DIR=../../doc
DOC_INPUT="../include/${MODULE_NAME}_kapi.h ${DOC_OUTPUT_DIR}/lg1150_${MODULE_NAME}.dox"

# do not delete : default config.
DOC_CONFIG?=$(DOC_OUTPUT_DIR)/Doxyfile

all: build_doc

clean clobber: clean_doc

build_doc: clean_doc
	DOC_INPUT=${DOC_INPUT} DOC_OUTPUT_DIR=${DOC_OUTPUT_DIR} DOC_PROJECT_NAME=${DOC_PROJECT_NAME} DOC_PROJECT_VERSION=${DOC_PROJECT_VERSION} doxygen ${DOC_CONFIG}

clean_doc:
	rm -rf $(DOC_OUTPUT_DIR)/html $(DOC_OUTPUT_DIR)/rtf
