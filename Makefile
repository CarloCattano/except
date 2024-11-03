# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build

lib.name = except

class.sources = except.c

datafiles = except-help.pd

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=../pd-lib-builder/
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
