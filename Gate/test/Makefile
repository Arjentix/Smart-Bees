# Source directory
SRCDIR = ../src

# Source directory of *Test classes
TESTSRCDIR = src

# Build directory
BUILDDIR = build

# Binary directory
BINDIR = bin

# Compiler
CC = g++

# Compiler flags
CFLAGS = -I $(SRCDIR)

# Linker flags
LDFLAGS = -lmosquitto

# Rules
$(BINDIR)/%: $(BUILDDIR)/%.o
	mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(LDFLAGS)
$(BUILDDIR)/%.o: %.cpp
	mkdir -p $(BUILDDIR)
	$(CC) -c $^ -o $@ $(CFLAGS)
$(BUILDDIR)/%.o: $(TESTSRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CC) -c $^ -o $@ $(CFLAGS)
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CC) -c $^ -o $@ $(CFLAGS)

all: $(BINDIR)/MQTTPublisherTest $(BINDIR)/TokenHandlerTest $(BINDIR)/PhotonConfigReaderTest $(BINDIR)/AliceConnectorTest_Server $(BINDIR)/AliceConnectorTest_Receiver

# MQTTPublisherTest
$(BINDIR)/MQTTPublisherTest: $(BUILDDIR)/MQTTPublisherTest.o $(BUILDDIR)/MQTTPublisher.o
$(BUILDDIR)/MQTTPublisherTest.o: $(TESTSRCDIR)/MQTTPublisherTest.cpp
$(BUILDDIR)/MQTTPublisher.o: $(SRCDIR)/MQTTPublisher.cpp

# TokenHandlerTest
$(BINDIR)/TokenHandlerTest: $(BUILDDIR)/TokenHandlerTest.o $(BUILDDIR)/TokenHandler.o
$(BUILDDIR)/TokenHandlerTest.o: $(TESTSRCDIR)/TokenHandlerTest.cpp
$(BUILDDIR)/TokenHandler.o: $(SRCDIR)/TokenHandler.cpp

# PhotonConfigReaderTest
$(BINDIR)/PhotonConfigReaderTest: $(BUILDDIR)/PhotonConfigReaderTest.o $(BUILDDIR)/PhotonConfigReader.o
$(BUILDDIR)/PhotonConfigReaderTest.o: $(TESTSRCDIR)/PhotonConfigReaderTest.cpp
$(BUILDDIR)/PhotonConfigReader.o: $(SRCDIR)/PhotonConfigReader.cpp

# AliceConnectorTest_Server
$(BINDIR)/AliceConnectorTest_Server: $(BUILDDIR)/AliceConnectorTest_Server.o
$(BUILDDIR)/AliceConnectorTest_Server.o: $(TESTSRCDIR)/AliceConnectorTest_Server.cpp

# AliceConnectorTest_Receiver
$(BINDIR)/AliceConnectorTest_Receiver: $(BUILDDIR)/AliceConnectorTest_Receiver.o $(BUILDDIR)/AliceConnector.o
$(BUILDDIR)/AliceConnectorTest_Receiver.o: $(TESTSRCDIR)/AliceConnectorTest_Receiver.cpp
$(BUILDDIR)/AliceConnector.o: $(SRCDIR)/AliceConnector.cpp

clean:
	-rm -r $(BUILDDIR)
