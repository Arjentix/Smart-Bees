# Source directory
SRCDIR = ../src

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
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CC) -c $^ -o $@ $(CFLAGS)

all: $(BINDIR)/MQTTPublisherTest $(BINDIR)/TokenHandlerTest

$(BINDIR)/MQTTPublisherTest: $(BUILDDIR)/MQTTPublisherTest.o $(BUILDDIR)/MQTTPublisher.o
$(BUILDDIR)/MQTTPublisherTest.o: MQTTPublisherTest.cpp
$(BUILDDIR)/MQTTPublisher.o: $(SRCDIR)/MQTTPublisher.cpp

$(BINDIR)/TokenHandlerTest: $(BUILDDIR)/TokenHandlerTest.o $(BUILDDIR)/TokenHandler.o
$(BUILDDIR)/TokenHandlerTest.o: TokenHandlerTest.cpp
$(BUILDDIR)/TokenHandler.o: $(SRCDIR)/TokenHandler.cpp

clean:
	-rm -r build/
