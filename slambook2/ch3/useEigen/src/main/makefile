# main moudle makefile
ifndef MODULE
	MODULE=main
endif
# project root dir
SRC_BASE = ../..
LIBS_DIR = $(SRC_BASE)/lib

CXXFLAGS += 
CPPFLAGS += -I. -I./inc -I$(SRC_BASE)/include
CPPFLAGS += -I$(SRC_BASE)/include/helloSLAM

LDFLAGS += -L$(LIBS_DIR)
LIBS:=$(shell ls $(LIBS_DIR))
LDFLAGS += $(patsubst lib%.a,-l%,$(LIBS))

# read other *.o ldflags
LDFLAGS += $(foreach ldf,$(filter-out main.ldf,$(filter %.ldf,$(shell ls $(DMKDIR)))),$(shell cat $(DMKDIR)/$(ldf)))


ifeq ($(DEBUG_SYMBOLS),)	
	DEBUG_SYMBOLS=TRUE
endif

# for ELF or EXE file
# # ELF or EXE file name, can modify according to need
SRC_BIN = bb.out
SRC_FILES := $(wildcard src/*.cpp)

MOD_SRC_DIR = src
include $(SRC_BASE)/makefile.rule
