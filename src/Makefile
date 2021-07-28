# fbd
# Date:2021.07.26
# File:/src/Makefile
# Copyright (c) 2021 Suote127.All rights reserved.

# Configure
CONF_DEBUG_OPTION:=-g -O0

# Tool
CC:=gcc
AR:=ar
REMOVE:=rm
ECHO:=echo
COPY:=cp -rf
MAKEDIR:=mkdir

# Flags
FLAG_AR:=rcu
FLAG_REMOVE:=-rf
FLAG_CC:=-Wall -Werror $(CONF_DEBUG_OPTION) -pedantic -Wextra -I./

# Files
FILES_OBJS:=fbd.o
FILES_LIB:=libfbd.a
FILES_HEADERS:=fbd
DIR_INSTALL_INCLUDE:=/usr/include
DIR_INSTALL_LIB:=/usr/lib

default:build

%.o:%.c
	@$(ECHO) [C99] $< $@
	@$(CC) $< -c $(FLAG_CC)

build:$(FILES_OBJS)
	@$(ECHO) [AR] $(FILES_LIB) $(FILES_OBJS)
	@$(AR) $(FLAG_AR) $(FILES_LIB) $(FILES_OBJS)

disbuild:
	@$(ECHO) [REMOVE] $(FILES_OBJS) $(FILES_LIB)
	@$(REMOVE) $(FILES_OBJS) $(FILES_LIB)

install:build
	@$(ECHO) [COPY] $(FILES_HEADERS) $(DIR_INSTALL_INCLUDE)
	@$(COPY) $(FILES_HEADERS) $(DIR_INSTALL_INCLUDE)
	@$(ECHO) [COPY] $(FILES_LIB)
	@$(COPY) $(FILES_LIB) $(DIR_INSTALL_LIB)

uninstall:
	@$(ECHO) [REMOVE] $(DIR_INSTALL_INCLUDE)/fbd
	@$(REMOVE) $(FLAG_REMOVE) $(DIR_INSTALL_HEADERS)/fbd
	@$(ECHO) [REMOVE] $(DIR_INSTALL_LIB)/libfbd.a
	@$(REMOVE) $(DIR_INSTALL_LIB)/libfbd.a
