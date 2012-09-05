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
ENET_VERSION = 1.3.5
ENET_A = enet-$(ENET_VERSION)/installed/lib/libenet.a
ENET_CFLAGS = -Ienet-$(ENET_VERSION)/installed/include
ENET_LDFLAGS = -Lenet-$(ENET_VERSION)/installed/lib

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

$(ENET_A): enet-$(ENET_VERSION)
	mkdir -p enet-$(ENET_VERSION)/build
	(cd enet-$(ENET_VERSION)/build; CFLAGS=-fPIC ../configure --enable-static --disable-shared --prefix=$(PWD)/enet-$(ENET_VERSION)/installed)
	$(MAKE) -C enet-$(ENET_VERSION)/build install

enet-$(ENET_VERSION):
	tar xzf enet-$(ENET_VERSION).tar.gz

$(TARGET): $(ENET_A) $(OBJECTS)
	$(CCLD) $(SHARED) -o $(TARGET) $(OBJECTS) $(ENET_LDFLAGS) $(EXTRA_LIBS) $(LIBS)

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) $(JDK_INCLUDES) $(ENET_CFLAGS) -c -o $@ $<
