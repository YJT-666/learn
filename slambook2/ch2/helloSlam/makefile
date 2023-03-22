# Top makefile for C++ program                                                                                                                                                              
# Copyright (C) 2023 JTsmileY  

# Debug option switch
export DEBUG_SYMBOLS=TRUE

# global compliation generate finall bin
export GLOBAL_BIN_SYMBOLS=TRUE

ifndef PLATFORM
	PLATFORM=linux
endif

# Source file path   
DIR=src
export MODULES=$(shell ls $(DIR))



# set build directory build/lib build/bin
ifeq ($(DEBUG_SYMBOLS),TRUE)
	BUILDDIR = build/$(PLATFORM)_dbg
else
	BUILDDIR = build/$(PLATFORM)
endif


all : $(BUILDDIR) $(MODULES)	


$(BUILDDIR):
	@echo "    Create directory $@ ..."
	mkdir -p $(BUILDDIR)/bin $(BUILDDIR)/lib $(BUILDDIR)/dmk
	mkdir -p lib
   
$(MODULES): 
	@$(MAKE) -C $(DIR)/$@

# you need to determine the order of module compilation!!!!
main : $(filter-out main,$(MODULES))
clean : 
	@for subdir in $(MODULES);do $(MAKE) -C $(DIR)/$$subdir $@;done 
dist:    
	rm -rf $(BUILDDIR)
	rm -rf build
	rm -rf lib
tags: 
	ctags -R 
    
 
help:
	@echo "===============A common Makefile for cprograms==============" 
	@echo "Copyright (C) 2023 2/21 JTsmileY" 
	@echo "The following targets are support:" 
	@echo 
	@echo " all             - (==make) compile and link" 
	@echo " obj             - just compile, without link" 
	@echo " clean           - clean target" 
	@echo " dist            - clean target and other information" 
	@echo " tags            - create ctags for vim editor" 
	@echo " help            - print help information" 
	@echo 
	@echo "To make a target, do 'make [target]'" 
	@echo "========================= Version 1.0=======================" 
    
.PHONY : all clean distclean tags help 
