CC := gcc
INSTALL := install
RM := rm
LN := ln
ORGDIR := ../..
SRCDIR := src
INCDIR := include
API_SRCDIR := api/src
API_INCDIR := api/include
CFLAGS := -O2 -I$(INCDIR) -I$(API_INCDIR) -I$(ORGDIR)/$(INCDIR) -I$(ORGDIR)/$(API_INCDIR) -Wall -fPIC
#CFLAGS := -O0 -g3 -ggdb -I$(INCDIR) -I$(API_INCDIR) -I$(ORGDIR)/$(INCDIR) -I$(ORGDIR)/$(API_INCDIR) -Wall -fPIC
LDFLAGS := -lpthread
TARGET_MAJOR_VER := 0
TARGET_MINOR_VER := 9
TARGET_PROTOCOL_VER := 0
CFLAGS := $(CFLAGS) -DESPRESS_STREAM_MAJOR_VERSION=$(TARGET_MAJOR_VER)
CFLAGS := $(CFLAGS) -DESPRESS_STREAM_MINOR_VERSION=$(TARGET_MINOR_VER)
CFLAGS := $(CFLAGS) -DESPRESS_STREAM_PROTOCOL_VERSION=$(TARGET_PROTOCOL_VER)
TARGET_NAME := libespress_stream.so
TARGET_SO_NAME := $(TARGET_NAME).$(TARGET_MAJOR_VER)
TARGET := $(TARGET_SO_NAME).$(TARGET_MINOR_VER)
INCLUDES :=
#INCLUDES := $(INCLUDES) $(shell find $(ORGDIR)/$(INCDIR) -name '*.h' -printf %P\\n | (while read l; do if [ ! -e $(INCDIR)/$$l ]; then echo $(ORGDIR)/$(INCDIR)/$$l; fi; done))
#INCLUDES := $(INCLUDES) $(shell find $(INCDIR) -name '*.h')
INCLUDES := $(INCLUDES) $(shell find $(ORGDIR)/$(API_INCDIR) -name '*.h' -printf %P\\n | (while read l; do if [ ! -e $(API_INCDIR)/$$l ]; then echo $(ORGDIR)/$(API_INCDIR)/$$l; fi; done))
INCLUDES := $(INCLUDES) $(shell find $(API_INCDIR) -name '*.h')
SOURCES :=
SOURCES := $(SOURCES) $(shell find $(ORGDIR)/$(SRCDIR) -name '*.c' -printf %P\\n | (while read l; do if [ ! -e $(SRCDIR)/$$l ]; then echo $(ORGDIR)/$(SRCDIR)/$$l; fi; done))
SOURCES := $(SOURCES) $(wildcard $(SRCDIR)/*.c)
SOURCES := $(SOURCES) $(shell find $(ORGDIR)/$(API_SRCDIR) -name '*.c' -printf %P\\n | (while read l; do if [ ! -e $(API_SRCDIR)/$$l ]; then echo $(ORGDIR)/$(API_SRCDIR)/$$l; fi; done))
SOURCES := $(SOURCES) $(windcard $(API_SRCDIR)/*.c)
OBJECTS := $(SOURCES:.c=.o)
API_TEST_TARGET := espress_stream_api_test
API_TEST_SRCDIR := api/test
API_TEST_SOURCES :=
API_TEST_SOURCES := $(API_TEST_SOURCES) $(shell find $(ORGDIR)/$(API_TEST_SRCDIR) -name '*.c' -printf %P\\n | (while read l; do if [ ! -e $(API_TEST_SRCDIR)/$$l ]; then echo $(ORGDIR)/$(API_TEST_SRCDIR)/$$l; fi; done))
API_TEST_SOURCES := $(API_TEST_SOURCES) $(wildcard $(API_TEST_SRCDIR)/*.c)
API_TEST_OBJECTS := $(API_TEST_SOURCES:.c=.o)

.PHONY: all clean depend test install uninstall

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -shared $(LDFLAGS) -Wl,-soname,$(TARGET_SO_NAME) -o $@

$(API_TEST_TARGET): $(API_TEST_OBJECTS) $(TARGET)
	$(CC) $(API_TEST_OBJECTS) $(TARGET) $(LDFLAGS) -o $@

all: clean $(TARGET) $(API_TEST_TARGET)

clean:
	$(RM) -f $(TARGET)
	$(RM) -f $(TARGET_SO_NAME)
	$(RM) -f $(OBJECTS)
	$(RM) -f $(API_TEST_TARGET)
	$(RM) -f $(API_TEST_OBJECTS)

test: $(API_TEST_TARGET)
	LD_PRELOAD=./$(TARGET) ./$(API_TEST_TARGET)

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES) -- $(API_TEST_SOURCES)

install: $(TARGET)
	$(INSTALL) -d /usr/local/include/espress_stream
	$(INSTALL) -m 644 $(INCLUDES) /usr/local/include/espress_stream/
	$(INSTALL) -m 755 $(TARGET) /usr/local/lib/
	$(LN) -s /usr/local/lib/$(TARGET) /usr/local/lib/$(TARGET_SO_NAME)
	$(LN) -s /usr/local/lib/$(TARGET_SO_NAME) /usr/local/lib/$(TARGET_NAME)
	ldconfig

uninstall:
	$(RM) -fr /usr/local/include/espress_stream
	$(RM) -f /usr/local/lib/$(TARGET_NAME)
	$(RM) -f /usr/local/lib/$(TARGET_NAME).*
	ldconfig

# DO NOT DELETE

../../src/espress_stream_timer.o: include/espress_stream/config.h
../../src/espress_stream_timer.o: include/espress_stream/depend.h
../../src/espress_stream_timer.o: ../../include/espress_stream/uint.h
../../src/espress_stream_timer.o: ../../include/espress_stream/tick.h
../../src/espress_stream_timer.o: include/espress_stream/mutex.h
../../src/espress_stream_timer.o: include/espress_stream/cond.h
../../src/espress_stream_timer.o: ../../include/espress_stream/event.h
../../src/espress_stream_timer.o: ../../include/espress_stream/timer.h
../../src/espress_stream_tick.o: include/espress_stream/config.h
../../src/espress_stream_tick.o: include/espress_stream/depend.h
../../src/espress_stream_tick.o: ../../include/espress_stream/uint.h
../../src/espress_stream_tick.o: ../../include/espress_stream/tick.h
../../src/espress_stream_trans_ring.o: include/espress_stream/config.h
../../src/espress_stream_trans_ring.o: include/espress_stream/depend.h
../../src/espress_stream_trans_ring.o: ../../include/espress_stream/uint.h
../../src/espress_stream_trans_ring.o: include/espress_stream/mutex.h
../../src/espress_stream_trans_ring.o: include/espress_stream/cond.h
../../src/espress_stream_trans_ring.o: ../../include/espress_stream/event.h
../../src/espress_stream_trans_ring.o: ../../include/espress_stream/ring.h
../../src/espress_stream_trans_ring.o: include/espress_stream/dgram.h
../../src/espress_stream_trans_ring.o: ../../include/espress_stream/trans_ring.h
../../src/espress_stream_ctr_cipher.o: include/espress_stream/config.h
../../src/espress_stream_ctr_cipher.o: include/espress_stream/depend.h
../../src/espress_stream_ctr_cipher.o: ../../include/espress_stream/uint.h
../../src/espress_stream_ctr_cipher.o: ../../include/espress_stream/block_cipher.h
../../src/espress_stream_ctr_cipher.o: ../../include/espress_stream/ctr_cipher.h
../../src/espress_stream_ec.o: include/espress_stream/config.h
../../src/espress_stream_ec.o: include/espress_stream/depend.h
../../src/espress_stream_ec.o: ../../include/espress_stream/uint.h
../../src/espress_stream_ec.o: ../../include/espress_stream/ec.h
../../src/espress_stream_ring.o: include/espress_stream/config.h
../../src/espress_stream_ring.o: include/espress_stream/depend.h
../../src/espress_stream_ring.o: ../../include/espress_stream/ring.h
../../src/espress_stream_event.o: include/espress_stream/config.h
../../src/espress_stream_event.o: include/espress_stream/depend.h
../../src/espress_stream_event.o: ../../include/espress_stream/uint.h
../../src/espress_stream_event.o: ../../include/espress_stream/tick.h
../../src/espress_stream_event.o: include/espress_stream/mutex.h
../../src/espress_stream_event.o: include/espress_stream/cond.h
../../src/espress_stream_event.o: ../../include/espress_stream/event.h
../../src/espress_stream_session.o: include/espress_stream/config.h
../../src/espress_stream_session.o: include/espress_stream/depend.h
../../src/espress_stream_session.o: ../../include/espress_stream/uint.h
../../src/espress_stream_session.o: ../../include/espress_stream/tick.h
../../src/espress_stream_session.o: include/espress_stream/mutex.h
../../src/espress_stream_session.o: include/espress_stream/cond.h
../../src/espress_stream_session.o: ../../include/espress_stream/event.h
../../src/espress_stream_session.o: ../../include/espress_stream/timer.h
../../src/espress_stream_session.o: ../../include/espress_stream/hash.h
../../src/espress_stream_session.o: ../../include/espress_stream/mac.h
../../src/espress_stream_session.o: include/espress_stream/rand.h
../../src/espress_stream_session.o: ../../include/espress_stream/block_cipher.h
../../src/espress_stream_session.o: ../../include/espress_stream/ctr_cipher.h
../../src/espress_stream_session.o: ../../include/espress_stream/hkdf.h
../../src/espress_stream_session.o: ../../include/espress_stream/ec.h
../../src/espress_stream_session.o: ../../include/espress_stream/key_ex.h
../../src/espress_stream_session.o: ../../include/espress_stream/key_exp.h
../../src/espress_stream_session.o: ../../include/espress_stream/key.h
../../src/espress_stream_session.o: ../../include/espress_stream/ring.h
../../src/espress_stream_session.o: include/espress_stream/dgram.h
../../src/espress_stream_session.o: ../../include/espress_stream/trans_ring.h
../../src/espress_stream_session.o: ../../include/espress_stream/packet.h
../../src/espress_stream_session.o: ../../include/espress_stream/trans_packet.h
../../src/espress_stream_session.o: ../../include/espress_stream/ring_buffer.h
../../src/espress_stream_session.o: ../../include/espress_stream/session.h
../../src/espress_stream_key_ex.o: include/espress_stream/config.h
../../src/espress_stream_key_ex.o: include/espress_stream/depend.h
../../src/espress_stream_key_ex.o: ../../include/espress_stream/uint.h
../../src/espress_stream_key_ex.o: ../../include/espress_stream/ec.h
../../src/espress_stream_key_ex.o: ../../include/espress_stream/key_ex.h
../../src/espress_stream_hkdf.o: include/espress_stream/config.h
../../src/espress_stream_hkdf.o: include/espress_stream/depend.h
../../src/espress_stream_hkdf.o: ../../include/espress_stream/uint.h
../../src/espress_stream_hkdf.o: ../../include/espress_stream/hash.h
../../src/espress_stream_hkdf.o: ../../include/espress_stream/mac.h
../../src/espress_stream_hkdf.o: ../../include/espress_stream/hkdf.h
../../src/espress_stream_key_exp.o: include/espress_stream/config.h
../../src/espress_stream_key_exp.o: include/espress_stream/depend.h
../../src/espress_stream_key_exp.o: ../../include/espress_stream/uint.h
../../src/espress_stream_key_exp.o: ../../include/espress_stream/hash.h
../../src/espress_stream_key_exp.o: ../../include/espress_stream/mac.h
../../src/espress_stream_key_exp.o: ../../include/espress_stream/hkdf.h
../../src/espress_stream_key_exp.o: ../../include/espress_stream/key_exp.h
../../src/espress_stream_hash.o: include/espress_stream/config.h
../../src/espress_stream_hash.o: include/espress_stream/depend.h
../../src/espress_stream_hash.o: ../../include/espress_stream/uint.h
../../src/espress_stream_hash.o: ../../include/espress_stream/hash.h
../../src/espress_stream_trans_packet.o: include/espress_stream/config.h
../../src/espress_stream_trans_packet.o: include/espress_stream/depend.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/uint.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/tick.h
../../src/espress_stream_trans_packet.o: include/espress_stream/mutex.h
../../src/espress_stream_trans_packet.o: include/espress_stream/cond.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/event.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/hash.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/mac.h
../../src/espress_stream_trans_packet.o: include/espress_stream/rand.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/block_cipher.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/ctr_cipher.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/hkdf.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/ec.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/key_ex.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/key_exp.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/key.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/ring.h
../../src/espress_stream_trans_packet.o: include/espress_stream/dgram.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/trans_ring.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/packet.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/trans_packet.h
../../src/espress_stream_trans_packet.o: ../../include/espress_stream/macro_repeat.h
../../src/espress_stream_packet.o: include/espress_stream/config.h
../../src/espress_stream_packet.o: include/espress_stream/depend.h
../../src/espress_stream_packet.o: ../../include/espress_stream/uint.h
../../src/espress_stream_packet.o: include/espress_stream/mutex.h
../../src/espress_stream_packet.o: include/espress_stream/cond.h
../../src/espress_stream_packet.o: ../../include/espress_stream/event.h
../../src/espress_stream_packet.o: ../../include/espress_stream/hash.h
../../src/espress_stream_packet.o: ../../include/espress_stream/mac.h
../../src/espress_stream_packet.o: ../../include/espress_stream/block_cipher.h
../../src/espress_stream_packet.o: ../../include/espress_stream/ctr_cipher.h
../../src/espress_stream_packet.o: ../../include/espress_stream/hkdf.h
../../src/espress_stream_packet.o: ../../include/espress_stream/key_exp.h
../../src/espress_stream_packet.o: ../../include/espress_stream/key.h
../../src/espress_stream_packet.o: ../../include/espress_stream/ring.h
../../src/espress_stream_packet.o: include/espress_stream/dgram.h
../../src/espress_stream_packet.o: ../../include/espress_stream/trans_ring.h
../../src/espress_stream_packet.o: ../../include/espress_stream/packet.h
../../src/espress_stream_uint.o: include/espress_stream/config.h
../../src/espress_stream_uint.o: include/espress_stream/depend.h
../../src/espress_stream_uint.o: ../../include/espress_stream/uint.h
../../src/espress_stream_ring_buffer.o: include/espress_stream/config.h
../../src/espress_stream_ring_buffer.o: include/espress_stream/depend.h
../../src/espress_stream_ring_buffer.o: ../../include/espress_stream/uint.h
../../src/espress_stream_ring_buffer.o: ../../include/espress_stream/ring_buffer.h
../../src/espress_stream_mac.o: include/espress_stream/config.h
../../src/espress_stream_mac.o: include/espress_stream/depend.h
../../src/espress_stream_mac.o: ../../include/espress_stream/uint.h
../../src/espress_stream_mac.o: ../../include/espress_stream/hash.h
../../src/espress_stream_mac.o: ../../include/espress_stream/mac.h
../../src/espress_stream_key.o: include/espress_stream/config.h
../../src/espress_stream_key.o: include/espress_stream/depend.h
../../src/espress_stream_key.o: ../../include/espress_stream/uint.h
../../src/espress_stream_key.o: include/espress_stream/mutex.h
../../src/espress_stream_key.o: ../../include/espress_stream/block_cipher.h
../../src/espress_stream_key.o: ../../include/espress_stream/ctr_cipher.h
../../src/espress_stream_key.o: ../../include/espress_stream/hash.h
../../src/espress_stream_key.o: ../../include/espress_stream/mac.h
../../src/espress_stream_key.o: ../../include/espress_stream/hkdf.h
../../src/espress_stream_key.o: ../../include/espress_stream/key_exp.h
../../src/espress_stream_key.o: ../../include/espress_stream/key.h
../../src/espress_stream_block_cipher.o: include/espress_stream/config.h
../../src/espress_stream_block_cipher.o: include/espress_stream/depend.h
../../src/espress_stream_block_cipher.o: ../../include/espress_stream/uint.h
../../src/espress_stream_block_cipher.o: ../../include/espress_stream/block_cipher.h
src/espress_stream_cond.o: include/espress_stream/config.h
src/espress_stream_cond.o: include/espress_stream/depend.h
src/espress_stream_cond.o: ../../include/espress_stream/uint.h
src/espress_stream_cond.o: ../../include/espress_stream/tick.h
src/espress_stream_cond.o: include/espress_stream/cond.h
src/espress_stream_dgram.o: include/espress_stream/config.h
src/espress_stream_dgram.o: include/espress_stream/depend.h
src/espress_stream_dgram.o: ../../include/espress_stream/uint.h
src/espress_stream_dgram.o: ../../include/espress_stream/tick.h
src/espress_stream_dgram.o: include/espress_stream/mutex.h
src/espress_stream_dgram.o: include/espress_stream/cond.h
src/espress_stream_dgram.o: ../../include/espress_stream/event.h
src/espress_stream_dgram.o: ../../include/espress_stream/ring.h
src/espress_stream_dgram.o: include/espress_stream/dgram.h
src/espress_stream_mutex.o: include/espress_stream/config.h
src/espress_stream_mutex.o: include/espress_stream/depend.h
src/espress_stream_mutex.o: ../../include/espress_stream/uint.h
src/espress_stream_mutex.o: include/espress_stream/mutex.h
src/espress_stream_rand.o: include/espress_stream/config.h
src/espress_stream_rand.o: include/espress_stream/depend.h
src/espress_stream_rand.o: include/espress_stream/rand.h
../../api/src/espress_stream_api.o: include/espress_stream/config.h
../../api/src/espress_stream_api.o: include/espress_stream/depend.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/uint.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/tick.h
../../api/src/espress_stream_api.o: include/espress_stream/mutex.h
../../api/src/espress_stream_api.o: include/espress_stream/cond.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/event.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/timer.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/hash.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/mac.h
../../api/src/espress_stream_api.o: include/espress_stream/rand.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/block_cipher.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/ctr_cipher.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/hkdf.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/ec.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/key_ex.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/key_exp.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/key.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/ring.h
../../api/src/espress_stream_api.o: include/espress_stream/dgram.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/trans_ring.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/packet.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/trans_packet.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/ring_buffer.h
../../api/src/espress_stream_api.o: ../../include/espress_stream/session.h
../../api/src/espress_stream_api.o: ../../api/include/espress_stream/api.h
../../api/src/espress_stream_api.o: api/include/espress_stream/api_depend.h
../../api/test/main.o: ../../api/include/espress_stream/api.h
../../api/test/main.o: api/include/espress_stream/api_depend.h
