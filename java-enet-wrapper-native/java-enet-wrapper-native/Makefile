# Makefile for java-enet-wrapper-native

SOURCES := org_bespin_enet_Event.c \
	org_bespin_enet_Host.c \
	org_bespin_enet_Packet.c \
	org_bespin_enet_Peer.c

OBJECTS := $(SOURCES:.c=.o)
SYSTEM := $(shell uname -s)
CFLAGS=-g -O2 -Iinclude -fPIC
CC=gcc
CCLD=$(CC)
LIBS := -lenet
ENET_CFLAGS =
ENET_LDFLAGS =

ifeq ($(SYSTEM),Darwin)
  SHLIB_EXT=jnilib
  SHARED=-dynamiclib
  EXTRA_LIBS=-lSystem
  JAVA_HOME = /System/Library/Frameworks/JavaVM.framework/Versions/Current
  JDK_INCLUDES = -I$(JAVA_HOME)/Headers
  CFLAGS=$(CFLAGS) -arch x86_64 -arch i386
else
ifeq ($(SYSTEM),Linux)
  SHLIB_EXT=so
  SHARED=-shared
  EXTRA_LIBS=
  JAVA_HOME = $(shell readlink -f `which javac` | sed 's:bin/javac::')
  JDK_INCLUDES=-I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
else
abort Unsupported system $(SYSTEM)
endif
endif

TARGET=libjava-enet-wrapper-native.$(SHLIB_EXT)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CCLD) $(SHARED) -o $(TARGET) $(OBJECTS) $(ENET_LDFLAGS) $(EXTRA_LIBS) $(LIBS)

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) $(JDK_INCLUDES) $(ENET_CFLAGS) -c -o $@ $<
