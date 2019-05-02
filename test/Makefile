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

# Targets
MQTTPublisherTest : MQTTPublisherTest.o MQTTPublisher.o
MQTTPublisherTest.o : MQTTPublisherTest.cpp
MQTTPublisher.o : $(SRCDIR)/MQTTPublisher.cpp

clean:
	-rm *.o
