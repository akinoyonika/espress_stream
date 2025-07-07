CC := gcc
INSTALL := install
RM := rm
SRCDIR := src
ADD_SRCDIR := cipher/src
CFLAGS := -O2 -Icipher/include -Wall
#CFLAGS := -O0 -g3 -ggdb -Icipher/include -Wall
LDFLAGS := -lpthread -lespress_stream
TARGET := es_gw
SOURCES := $(wildcard $(SRCDIR)/*.c) $(wildcard $(ADD_SRCDIR)/*.c)
OBJECTS := $(SOURCES:.c=.o)
LIBS := 

.PHONY: clean all install uninstall

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	$(RM) -f $(TARGET)
	$(RM) -f $(OBJECTS)

all: clean $(TARGET)

install: $(TARGET)
	$(INSTALL) -m 755 $(TARGET) /usr/local/bin

uninstall:
	$(RM) -f /usr/local/bin/$(TARGET)
