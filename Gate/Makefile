# Source directory
SRCDIR = src

# Build directory
BUILDDIR = build

# Binary directory
BINDIR = bin

# Compiler
CC = g++

# Linker flags
LDFLAGS = -lmosquitto

# Rules
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p build
	$(CC) -c $^ -o $@

all: $(BINDIR)/Smart-Home-Gate

$(BINDIR)/Smart-Home-Gate: $(BUILDDIR)/main.o $(BUILDDIR)/PhotonConfigReader.o $(BUILDDIR)/AliceConnector.o $(BUILDDIR)/MQTTPublisher.o $(BUILDDIR)/TokenHandler.o $(BUILDDIR)/LogPrinter.o
	mkdir -p bin
	mkdir -p log
	$(CC) $^ -o $@ $(LDFLAGS)
$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp
$(BUILDDIR)/PhotonConfigReader.o: $(SRCDIR)/PhotonConfigReader.cpp
$(BUILDDIR)/AliceConnector.o: $(SRCDIR)/AliceConnector.cpp
$(BUILDDIR)/MQTTPublisher.o: $(SRCDIR)/MQTTPublisher.cpp
$(BUILDDIR)/TokenHandler.o: $(SRCDIR)/TokenHandler.cpp
$(BUILDDIR)/LogPrinter.o: $(SRCDIR)/LogPrinter.cpp

clean:
	-rm -r $(BUILDDIR)
