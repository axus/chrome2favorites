UNAME := $(shell uname -o)

#Make based on operating system.  Only one option :)

ifeq ($(UNAME), Msys)
include Makefile.MinGW
endif
