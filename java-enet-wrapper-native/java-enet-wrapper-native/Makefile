# Makefile for java-enet-wrapper-native

SOURCES := org_bespin_enet_Event.c \
	org_bespin_enet_Host.c \
	org_bespin_enet_Packet.c \
	org_bespin_enet_Peer.c
	
OBJECTS := $(SOURCES:.c=.o)
SYSTEM := $(shell uname -s)
CFLAGS=-g -O2 -Iinclude
CC=gcc
CCLD=$(CC)
LIBS := -lenet

ifeq ($(SYSTEM),Darwin)
  SHLIB_EXT=jnilib
  SHARED=-dynamiclib
  EXTRA_LIBS=-lSystem
  JDK_INCLUDES=-I/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers
else
ifeq ($(SYSTEM),Linux)
  SHLIB_EXT=so
  SHARED=-shared
  EXTRA_LIBS=
  JDK_INCLUDES=-I$(shell readlink -f `which javac` | sed 's:bin/javac::')/include
else
abort Unsupported system $(SYSTEM)
endif
endif

TARGET=libjava-enet-wrapper-native.$(SHLIB_EXT)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CCLD) $(SHARED) -o $(TARGET) $(OBJECTS) $(EXTRA_LIBS) $(LIBS)

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) $(JDK_INCLUDES) -c -o $@ $<