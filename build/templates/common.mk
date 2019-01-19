
#=============================================================================
#
# This Makefile template is part of the SoftFloat IEEE Floating-Point
# Arithmetic Package, Release 3e, by John R. Hauser.
#
# Copyright 2011, 2012, 2013, 2014, 2015, 2016, 2017 The Regents of the
# University of California.  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions, and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions, and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  3. Neither the name of the University nor the names of its contributors
#     may be used to endorse or promote products derived from this software
#     without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS", AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ARE
# DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#=============================================================================

ifndef SPECIALIZE_TYPE
$(error this makefile is a template, do not use directly)
endif

dir_of_makefile = $(dir $(word $(shell expr $(words $(MAKEFILE_LIST)) - $(1)),$(MAKEFILE_LIST)))
# the chain of include is
# Makefile -> (not-)FAST_INT64/template.mk -> common.mk
MAKEFILE_DIR = $(call dir_of_makefile,2)
TEMPLATE_DIR = $(call dir_of_makefile,1)

SOURCE_DIR ?= $(MAKEFILE_DIR)/../../source

CFLAGS += -DSOFTFLOAT_ROUND_ODD -DINLINE_LEVEL=5
CFLAGS += -I$(TEMPLATE_DIR) -I$(SOURCE_DIR)/$(SPECIALIZE_TYPE) -I$(SOURCE_DIR)/include

OTHER_HEADERS ?=

ifdef SHARED
       MAKELIB = $(CC) $(LDFLAGS) -shared $^ -o $@ $(LOADLIBES) $(LDLIBS)
       LIB = .so
       CFLAGS += -fPIC
else
       MAKELIB = $(AR) crs $@ $^
       LIB = .a
endif

OBJ = .o

.PHONY: all
all: softfloat$(LIB)

OBJS_ALL = $(OBJS_PRIMITIVES) $(OBJS_SPECIALIZE) $(OBJS_OTHERS)

$(OBJS_ALL): \
  $(OTHER_HEADERS) $(TEMPLATE_DIR)/platform.h $(SOURCE_DIR)/include/primitiveTypes.h \
  $(SOURCE_DIR)/include/primitives.h
$(OBJS_SPECIALIZE) $(OBJS_OTHERS): \
  $(SOURCE_DIR)/include/softfloat_types.h $(SOURCE_DIR)/include/internals.h \
  $(SOURCE_DIR)/$(SPECIALIZE_TYPE)/specialize.h \
  $(SOURCE_DIR)/include/softfloat.h

$(OBJS_PRIMITIVES) $(OBJS_OTHERS): %$(OBJ): $(SOURCE_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJS_SPECIALIZE): %$(OBJ): $(SOURCE_DIR)/$(SPECIALIZE_TYPE)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

softfloat$(LIB): $(OBJS_ALL)
	$(MAKELIB)

.PHONY: clean
clean:
	$(RM) $(OBJS_ALL) softfloat$(LIB)
