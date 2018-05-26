# stand alone makefile or configuration for mk_doc.sh
# if not set DOC_CONFIG, then assumes DOC_OUTPUT_DIR/Doxyfile
DOC_PROJECT_NAME=lg1150/sys
DOC_PROJECT_VERSION=v1.0
DOC_OUTPUT_DIR=../../doc

# do not delete : default config.
DOC_CONFIG?=$(DOC_OUTPUT_DIR)/Doxyfile

all: build_doc

clean clobber: clean_doc

build_doc: clean_doc
	DOC_OUTPUT_DIR=${DOC_OUTPUT_DIR} DOC_PROJECT_NAME=${DOC_PROJECT_NAME} DOC_PROJECT_VERSION=${DOC_PROJECT_VERSION} doxygen ${DOC_CONFIG}

clean_doc:
	rm -rf $(DOC_OUTPUT_DIR)/html $(DOC_OUTPUT_DIR)/rtf
