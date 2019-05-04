# Source directory
SRCDIR = ../src

# Compiler
CC = g++

# Compiler flags
CFLAGS = -I $(SRCDIR)

# Linker flags
LDFLAGS = -lmosquitto

# Rules
%:%.o
	$(CC) $^ -o $@ $(LDFLAGS)
%.o: %.cpp
	$(CC) -c $^ -o $@ $(CFLAGS)
%.o: $(SRCDIR)/%.cpp
	$(CC) -c $^ -o $@ $(CFLAGS)

all: MQTTPublisherTest, TokenHandlerTest

MQTTPublisherTest: MQTTPublisherTest.o MQTTPublisher.o
MQTTPublisherTest.o: MQTTPublisherTest.cpp
MQTTPublisher.o: $(SRCDIR)/MQTTPublisher.cpp

LDFLAGS = 

TokenHandlerTest: TokenHandlerTest.o TokenHandler.o
TokenHandlerTest.o: TokenHandlerTest.cpp
TokenHandler.o: $(SRCDIR)/TokenHandler.cpp

clean:
	-rm *.o
