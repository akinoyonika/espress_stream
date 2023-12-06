CC := gcc
INSTALL := install
RM := rm
SRCDIR := src
INCDIR := include
API_SRCDIR := api/src
API_INCDIR := api/include
CFLAGS := -O2 -I$(INCDIR) -I$(API_INCDIR) -Wall -fPIC
#CFLAGS := -O0 -g3 -ggdb -I$(INCDIR) -I$(API_INCDIR) -Wall -fPIC
LDFLAGS :=
TARGET := libespress_stream.so
SOURCES := $(wildcard $(SRCDIR)/*.c) $(wildcard $(API_SRCDIR)/*.c)
OBJECTS := $(SOURCES:.c=.o)
UNIT_TEST_TARGET := espress_stream_unit_test
UNIT_TEST_SRCDIR := test
UNIT_TEST_SOURCES := $(wildcard $(UNIT_TEST_SRCDIR)/*.c)
UNIT_TEST_OBJECTS := $(UNIT_TEST_SOURCES:.c=.o)
API_TEST_TARGET := espress_stream_api_test
API_TEST_SRCDIR := api/test
API_TEST_SOURCES := $(wildcard $(API_TEST_SRCDIR)/*.c)
API_TEST_OBJECTS := $(API_TEST_SOURCES:.c=.o)

.PHONY: all clean test depend

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -shared $(LDFLAGS) -o $@

$(UNIT_TEST_TARGET): $(TARGET) $(UNIT_TEST_OBJECTS)
	$(CC) $(UNIT_TEST_OBJECTS) $(TARGET) $(LDFLAGS) -o $@

$(API_TEST_TARGET): $(TARGET) $(API_TEST_OBJECTS)
	$(CC) $(API_TEST_OBJECTS) $(TARGET) $(LDFLAGS) -o $@

all: clean $(TARGET) $(UNIT_TEST_TARGET) $(API_TEST_TARGET)

clean:
	$(RM) -f $(TARGET)
	$(RM) -f $(OBJECTS)
	$(RM) -f $(UNIT_TEST_TARGET)
	$(RM) -f $(UNIT_TEST_OBJECTS)
	$(RM) -f $(API_TEST_TARGET)
	$(RM) -f $(API_TEST_OBJECTS)

test: $(UNIT_TEST_TARGET) $(API_TEST_TARGET)
	LD_LIBRARY_PATH=./ ./$(UNIT_TEST_TARGET)
	LD_LIBRARY_PATH=./ ./$(API_TEST_TARGET)

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES) -- $(UNIT_TEST_SOURCES) -- $(API_TEST_SOURCES)

# DO NOT DELETE

src/espress_stream_block_cipher.o: include/espress_stream/config.h
src/espress_stream_block_cipher.o: include/espress_stream/depend.h
src/espress_stream_block_cipher.o: include/espress_stream/uint.h
src/espress_stream_block_cipher.o: include/espress_stream/block_cipher.h
src/espress_stream_cond.o: include/espress_stream/config.h
src/espress_stream_cond.o: include/espress_stream/depend.h
src/espress_stream_cond.o: include/espress_stream/uint.h
src/espress_stream_cond.o: include/espress_stream/tick.h
src/espress_stream_cond.o: include/espress_stream/mutex.h
src/espress_stream_cond.o: include/espress_stream/cond.h
src/espress_stream_ctr_cipher.o: include/espress_stream/config.h
src/espress_stream_ctr_cipher.o: include/espress_stream/depend.h
src/espress_stream_ctr_cipher.o: include/espress_stream/uint.h
src/espress_stream_ctr_cipher.o: include/espress_stream/block_cipher.h
src/espress_stream_ctr_cipher.o: include/espress_stream/ctr_cipher.h
src/espress_stream_dgram.o: include/espress_stream/config.h
src/espress_stream_dgram.o: include/espress_stream/depend.h
src/espress_stream_dgram.o: include/espress_stream/uint.h
src/espress_stream_dgram.o: include/espress_stream/tick.h
src/espress_stream_dgram.o: include/espress_stream/mutex.h
src/espress_stream_dgram.o: include/espress_stream/cond.h
src/espress_stream_dgram.o: include/espress_stream/event.h
src/espress_stream_dgram.o: include/espress_stream/ring.h
src/espress_stream_dgram.o: include/espress_stream/dgram.h
src/espress_stream_ec.o: include/espress_stream/config.h
src/espress_stream_ec.o: include/espress_stream/depend.h
src/espress_stream_ec.o: include/espress_stream/uint.h
src/espress_stream_ec.o: include/espress_stream/ec.h
src/espress_stream_event.o: include/espress_stream/config.h
src/espress_stream_event.o: include/espress_stream/depend.h
src/espress_stream_event.o: include/espress_stream/uint.h
src/espress_stream_event.o: include/espress_stream/tick.h
src/espress_stream_event.o: include/espress_stream/mutex.h
src/espress_stream_event.o: include/espress_stream/cond.h
src/espress_stream_event.o: include/espress_stream/event.h
src/espress_stream_hash.o: include/espress_stream/config.h
src/espress_stream_hash.o: include/espress_stream/depend.h
src/espress_stream_hash.o: include/espress_stream/uint.h
src/espress_stream_hash.o: include/espress_stream/hash.h
src/espress_stream_hkdf.o: include/espress_stream/config.h
src/espress_stream_hkdf.o: include/espress_stream/depend.h
src/espress_stream_hkdf.o: include/espress_stream/uint.h
src/espress_stream_hkdf.o: include/espress_stream/hash.h
src/espress_stream_hkdf.o: include/espress_stream/mac.h
src/espress_stream_hkdf.o: include/espress_stream/hkdf.h
src/espress_stream_key.o: include/espress_stream/config.h
src/espress_stream_key.o: include/espress_stream/depend.h
src/espress_stream_key.o: include/espress_stream/uint.h
src/espress_stream_key.o: include/espress_stream/mutex.h
src/espress_stream_key.o: include/espress_stream/block_cipher.h
src/espress_stream_key.o: include/espress_stream/ctr_cipher.h
src/espress_stream_key.o: include/espress_stream/hash.h
src/espress_stream_key.o: include/espress_stream/mac.h
src/espress_stream_key.o: include/espress_stream/hkdf.h
src/espress_stream_key.o: include/espress_stream/key_exp.h
src/espress_stream_key.o: include/espress_stream/key.h
src/espress_stream_key_ex.o: include/espress_stream/config.h
src/espress_stream_key_ex.o: include/espress_stream/depend.h
src/espress_stream_key_ex.o: include/espress_stream/uint.h
src/espress_stream_key_ex.o: include/espress_stream/ec.h
src/espress_stream_key_ex.o: include/espress_stream/key_ex.h
src/espress_stream_key_exp.o: include/espress_stream/config.h
src/espress_stream_key_exp.o: include/espress_stream/depend.h
src/espress_stream_key_exp.o: include/espress_stream/uint.h
src/espress_stream_key_exp.o: include/espress_stream/hash.h
src/espress_stream_key_exp.o: include/espress_stream/mac.h
src/espress_stream_key_exp.o: include/espress_stream/hkdf.h
src/espress_stream_key_exp.o: include/espress_stream/key_exp.h
src/espress_stream_mac.o: include/espress_stream/config.h
src/espress_stream_mac.o: include/espress_stream/depend.h
src/espress_stream_mac.o: include/espress_stream/uint.h
src/espress_stream_mac.o: include/espress_stream/hash.h
src/espress_stream_mac.o: include/espress_stream/mac.h
src/espress_stream_mutex.o: include/espress_stream/config.h
src/espress_stream_mutex.o: include/espress_stream/depend.h
src/espress_stream_mutex.o: include/espress_stream/uint.h
src/espress_stream_mutex.o: include/espress_stream/mutex.h
src/espress_stream_packet.o: include/espress_stream/config.h
src/espress_stream_packet.o: include/espress_stream/depend.h
src/espress_stream_packet.o: include/espress_stream/uint.h
src/espress_stream_packet.o: include/espress_stream/mutex.h
src/espress_stream_packet.o: include/espress_stream/cond.h
src/espress_stream_packet.o: include/espress_stream/event.h
src/espress_stream_packet.o: include/espress_stream/hash.h
src/espress_stream_packet.o: include/espress_stream/mac.h
src/espress_stream_packet.o: include/espress_stream/block_cipher.h
src/espress_stream_packet.o: include/espress_stream/ctr_cipher.h
src/espress_stream_packet.o: include/espress_stream/hkdf.h
src/espress_stream_packet.o: include/espress_stream/key_exp.h
src/espress_stream_packet.o: include/espress_stream/key.h
src/espress_stream_packet.o: include/espress_stream/ring.h
src/espress_stream_packet.o: include/espress_stream/dgram.h
src/espress_stream_packet.o: include/espress_stream/trans_ring.h
src/espress_stream_packet.o: include/espress_stream/packet.h
src/espress_stream_rand.o: include/espress_stream/config.h
src/espress_stream_rand.o: include/espress_stream/depend.h
src/espress_stream_rand.o: include/espress_stream/uint.h
src/espress_stream_rand.o: include/espress_stream/mutex.h
src/espress_stream_rand.o: include/espress_stream/tick.h
src/espress_stream_rand.o: include/espress_stream/rand.h
src/espress_stream_ring.o: include/espress_stream/config.h
src/espress_stream_ring.o: include/espress_stream/depend.h
src/espress_stream_ring.o: include/espress_stream/ring.h
src/espress_stream_ring_buffer.o: include/espress_stream/config.h
src/espress_stream_ring_buffer.o: include/espress_stream/depend.h
src/espress_stream_ring_buffer.o: include/espress_stream/uint.h
src/espress_stream_ring_buffer.o: include/espress_stream/ring_buffer.h
src/espress_stream_session.o: include/espress_stream/config.h
src/espress_stream_session.o: include/espress_stream/depend.h
src/espress_stream_session.o: include/espress_stream/uint.h
src/espress_stream_session.o: include/espress_stream/tick.h
src/espress_stream_session.o: include/espress_stream/mutex.h
src/espress_stream_session.o: include/espress_stream/cond.h
src/espress_stream_session.o: include/espress_stream/event.h
src/espress_stream_session.o: include/espress_stream/timer.h
src/espress_stream_session.o: include/espress_stream/hash.h
src/espress_stream_session.o: include/espress_stream/mac.h
src/espress_stream_session.o: include/espress_stream/rand.h
src/espress_stream_session.o: include/espress_stream/block_cipher.h
src/espress_stream_session.o: include/espress_stream/ctr_cipher.h
src/espress_stream_session.o: include/espress_stream/hkdf.h
src/espress_stream_session.o: include/espress_stream/ec.h
src/espress_stream_session.o: include/espress_stream/key_ex.h
src/espress_stream_session.o: include/espress_stream/key_exp.h
src/espress_stream_session.o: include/espress_stream/key.h
src/espress_stream_session.o: include/espress_stream/ring.h
src/espress_stream_session.o: include/espress_stream/dgram.h
src/espress_stream_session.o: include/espress_stream/trans_ring.h
src/espress_stream_session.o: include/espress_stream/packet.h
src/espress_stream_session.o: include/espress_stream/trans_packet.h
src/espress_stream_session.o: include/espress_stream/ring_buffer.h
src/espress_stream_session.o: include/espress_stream/session.h
src/espress_stream_tick.o: include/espress_stream/config.h
src/espress_stream_tick.o: include/espress_stream/depend.h
src/espress_stream_tick.o: include/espress_stream/uint.h
src/espress_stream_tick.o: include/espress_stream/tick.h
src/espress_stream_timer.o: include/espress_stream/config.h
src/espress_stream_timer.o: include/espress_stream/depend.h
src/espress_stream_timer.o: include/espress_stream/uint.h
src/espress_stream_timer.o: include/espress_stream/tick.h
src/espress_stream_timer.o: include/espress_stream/mutex.h
src/espress_stream_timer.o: include/espress_stream/cond.h
src/espress_stream_timer.o: include/espress_stream/event.h
src/espress_stream_timer.o: include/espress_stream/timer.h
src/espress_stream_trans_packet.o: include/espress_stream/config.h
src/espress_stream_trans_packet.o: include/espress_stream/depend.h
src/espress_stream_trans_packet.o: include/espress_stream/uint.h
src/espress_stream_trans_packet.o: include/espress_stream/tick.h
src/espress_stream_trans_packet.o: include/espress_stream/mutex.h
src/espress_stream_trans_packet.o: include/espress_stream/cond.h
src/espress_stream_trans_packet.o: include/espress_stream/event.h
src/espress_stream_trans_packet.o: include/espress_stream/hash.h
src/espress_stream_trans_packet.o: include/espress_stream/mac.h
src/espress_stream_trans_packet.o: include/espress_stream/rand.h
src/espress_stream_trans_packet.o: include/espress_stream/block_cipher.h
src/espress_stream_trans_packet.o: include/espress_stream/ctr_cipher.h
src/espress_stream_trans_packet.o: include/espress_stream/hkdf.h
src/espress_stream_trans_packet.o: include/espress_stream/ec.h
src/espress_stream_trans_packet.o: include/espress_stream/key_ex.h
src/espress_stream_trans_packet.o: include/espress_stream/key_exp.h
src/espress_stream_trans_packet.o: include/espress_stream/key.h
src/espress_stream_trans_packet.o: include/espress_stream/ring.h
src/espress_stream_trans_packet.o: include/espress_stream/dgram.h
src/espress_stream_trans_packet.o: include/espress_stream/trans_ring.h
src/espress_stream_trans_packet.o: include/espress_stream/packet.h
src/espress_stream_trans_packet.o: include/espress_stream/trans_packet.h
src/espress_stream_trans_packet.o: include/espress_stream/macro_repeat.h
src/espress_stream_trans_ring.o: include/espress_stream/config.h
src/espress_stream_trans_ring.o: include/espress_stream/depend.h
src/espress_stream_trans_ring.o: include/espress_stream/uint.h
src/espress_stream_trans_ring.o: include/espress_stream/mutex.h
src/espress_stream_trans_ring.o: include/espress_stream/cond.h
src/espress_stream_trans_ring.o: include/espress_stream/event.h
src/espress_stream_trans_ring.o: include/espress_stream/ring.h
src/espress_stream_trans_ring.o: include/espress_stream/dgram.h
src/espress_stream_trans_ring.o: include/espress_stream/trans_ring.h
src/espress_stream_uint.o: include/espress_stream/config.h
src/espress_stream_uint.o: include/espress_stream/depend.h
src/espress_stream_uint.o: include/espress_stream/uint.h
api/src/espress_stream_api.o: include/espress_stream/config.h
api/src/espress_stream_api.o: include/espress_stream/depend.h
api/src/espress_stream_api.o: include/espress_stream/uint.h
api/src/espress_stream_api.o: include/espress_stream/tick.h
api/src/espress_stream_api.o: include/espress_stream/mutex.h
api/src/espress_stream_api.o: include/espress_stream/cond.h
api/src/espress_stream_api.o: include/espress_stream/event.h
api/src/espress_stream_api.o: include/espress_stream/timer.h
api/src/espress_stream_api.o: include/espress_stream/hash.h
api/src/espress_stream_api.o: include/espress_stream/mac.h
api/src/espress_stream_api.o: include/espress_stream/rand.h
api/src/espress_stream_api.o: include/espress_stream/block_cipher.h
api/src/espress_stream_api.o: include/espress_stream/ctr_cipher.h
api/src/espress_stream_api.o: include/espress_stream/hkdf.h
api/src/espress_stream_api.o: include/espress_stream/ec.h
api/src/espress_stream_api.o: include/espress_stream/key_ex.h
api/src/espress_stream_api.o: include/espress_stream/key_exp.h
api/src/espress_stream_api.o: include/espress_stream/key.h
api/src/espress_stream_api.o: include/espress_stream/ring.h
api/src/espress_stream_api.o: include/espress_stream/dgram.h
api/src/espress_stream_api.o: include/espress_stream/trans_ring.h
api/src/espress_stream_api.o: include/espress_stream/packet.h
api/src/espress_stream_api.o: include/espress_stream/trans_packet.h
api/src/espress_stream_api.o: include/espress_stream/ring_buffer.h
api/src/espress_stream_api.o: include/espress_stream/session.h
api/src/espress_stream_api.o: api/include/espress_stream/api.h
api/src/espress_stream_api.o: api/include/espress_stream/api_depend.h
test/espress_stream_block_cipher_test.o: include/espress_stream/config.h
test/espress_stream_block_cipher_test.o: include/espress_stream/depend.h
test/espress_stream_block_cipher_test.o: include/espress_stream/uint.h
test/espress_stream_block_cipher_test.o: include/espress_stream/block_cipher.h
test/espress_stream_ctr_cipher_test.o: include/espress_stream/config.h
test/espress_stream_ctr_cipher_test.o: include/espress_stream/depend.h
test/espress_stream_ctr_cipher_test.o: include/espress_stream/uint.h
test/espress_stream_ctr_cipher_test.o: include/espress_stream/block_cipher.h
test/espress_stream_ctr_cipher_test.o: include/espress_stream/ctr_cipher.h
test/espress_stream_dgram_test.o: include/espress_stream/config.h
test/espress_stream_dgram_test.o: include/espress_stream/depend.h
test/espress_stream_dgram_test.o: include/espress_stream/uint.h
test/espress_stream_dgram_test.o: include/espress_stream/mutex.h
test/espress_stream_dgram_test.o: include/espress_stream/cond.h
test/espress_stream_dgram_test.o: include/espress_stream/event.h
test/espress_stream_dgram_test.o: include/espress_stream/ring.h
test/espress_stream_dgram_test.o: include/espress_stream/dgram.h
test/espress_stream_ec_test.o: include/espress_stream/config.h
test/espress_stream_ec_test.o: include/espress_stream/depend.h
test/espress_stream_ec_test.o: include/espress_stream/uint.h
test/espress_stream_ec_test.o: include/espress_stream/ec.h
test/espress_stream_event_test.o: include/espress_stream/config.h
test/espress_stream_event_test.o: include/espress_stream/depend.h
test/espress_stream_event_test.o: include/espress_stream/uint.h
test/espress_stream_event_test.o: include/espress_stream/mutex.h
test/espress_stream_event_test.o: include/espress_stream/cond.h
test/espress_stream_event_test.o: include/espress_stream/event.h
test/espress_stream_hash_test.o: include/espress_stream/config.h
test/espress_stream_hash_test.o: include/espress_stream/depend.h
test/espress_stream_hash_test.o: include/espress_stream/uint.h
test/espress_stream_hash_test.o: include/espress_stream/hash.h
test/espress_stream_hkdf_test.o: include/espress_stream/config.h
test/espress_stream_hkdf_test.o: include/espress_stream/depend.h
test/espress_stream_hkdf_test.o: include/espress_stream/uint.h
test/espress_stream_hkdf_test.o: include/espress_stream/hash.h
test/espress_stream_hkdf_test.o: include/espress_stream/mac.h
test/espress_stream_hkdf_test.o: include/espress_stream/hkdf.h
test/espress_stream_key_ex_test.o: include/espress_stream/config.h
test/espress_stream_key_ex_test.o: include/espress_stream/depend.h
test/espress_stream_key_ex_test.o: include/espress_stream/uint.h
test/espress_stream_key_ex_test.o: include/espress_stream/ec.h
test/espress_stream_key_ex_test.o: include/espress_stream/key_ex.h
test/espress_stream_key_exp_test.o: include/espress_stream/config.h
test/espress_stream_key_exp_test.o: include/espress_stream/depend.h
test/espress_stream_key_exp_test.o: include/espress_stream/uint.h
test/espress_stream_key_exp_test.o: include/espress_stream/hash.h
test/espress_stream_key_exp_test.o: include/espress_stream/mac.h
test/espress_stream_key_exp_test.o: include/espress_stream/hkdf.h
test/espress_stream_key_exp_test.o: include/espress_stream/key_exp.h
test/espress_stream_key_test.o: include/espress_stream/config.h
test/espress_stream_key_test.o: include/espress_stream/depend.h
test/espress_stream_key_test.o: include/espress_stream/uint.h
test/espress_stream_key_test.o: include/espress_stream/mutex.h
test/espress_stream_key_test.o: include/espress_stream/block_cipher.h
test/espress_stream_key_test.o: include/espress_stream/ctr_cipher.h
test/espress_stream_key_test.o: include/espress_stream/hash.h
test/espress_stream_key_test.o: include/espress_stream/mac.h
test/espress_stream_key_test.o: include/espress_stream/hkdf.h
test/espress_stream_key_test.o: include/espress_stream/key_exp.h
test/espress_stream_key_test.o: include/espress_stream/key.h
test/espress_stream_mac_test.o: include/espress_stream/config.h
test/espress_stream_mac_test.o: include/espress_stream/depend.h
test/espress_stream_mac_test.o: include/espress_stream/uint.h
test/espress_stream_mac_test.o: include/espress_stream/hash.h
test/espress_stream_mac_test.o: include/espress_stream/mac.h
test/espress_stream_macro_repeat_test.o: include/espress_stream/macro_repeat.h
test/espress_stream_packet_test.o: include/espress_stream/config.h
test/espress_stream_packet_test.o: include/espress_stream/depend.h
test/espress_stream_packet_test.o: include/espress_stream/uint.h
test/espress_stream_packet_test.o: include/espress_stream/mutex.h
test/espress_stream_packet_test.o: include/espress_stream/cond.h
test/espress_stream_packet_test.o: include/espress_stream/event.h
test/espress_stream_packet_test.o: include/espress_stream/hash.h
test/espress_stream_packet_test.o: include/espress_stream/mac.h
test/espress_stream_packet_test.o: include/espress_stream/block_cipher.h
test/espress_stream_packet_test.o: include/espress_stream/ctr_cipher.h
test/espress_stream_packet_test.o: include/espress_stream/hkdf.h
test/espress_stream_packet_test.o: include/espress_stream/key_exp.h
test/espress_stream_packet_test.o: include/espress_stream/key.h
test/espress_stream_packet_test.o: include/espress_stream/ring.h
test/espress_stream_packet_test.o: include/espress_stream/dgram.h
test/espress_stream_packet_test.o: include/espress_stream/trans_ring.h
test/espress_stream_packet_test.o: include/espress_stream/packet.h
test/espress_stream_rand_test.o: include/espress_stream/config.h
test/espress_stream_rand_test.o: include/espress_stream/depend.h
test/espress_stream_rand_test.o: include/espress_stream/uint.h
test/espress_stream_rand_test.o: include/espress_stream/mutex.h
test/espress_stream_rand_test.o: include/espress_stream/tick.h
test/espress_stream_rand_test.o: include/espress_stream/rand.h
test/espress_stream_ring_buffer_test.o: include/espress_stream/config.h
test/espress_stream_ring_buffer_test.o: include/espress_stream/depend.h
test/espress_stream_ring_buffer_test.o: include/espress_stream/uint.h
test/espress_stream_ring_buffer_test.o: include/espress_stream/ring_buffer.h
test/espress_stream_ring_test.o: include/espress_stream/config.h
test/espress_stream_ring_test.o: include/espress_stream/depend.h
test/espress_stream_ring_test.o: include/espress_stream/ring.h
test/espress_stream_session_test.o: include/espress_stream/config.h
test/espress_stream_session_test.o: include/espress_stream/depend.h
test/espress_stream_session_test.o: include/espress_stream/uint.h
test/espress_stream_session_test.o: include/espress_stream/tick.h
test/espress_stream_session_test.o: include/espress_stream/mutex.h
test/espress_stream_session_test.o: include/espress_stream/cond.h
test/espress_stream_session_test.o: include/espress_stream/event.h
test/espress_stream_session_test.o: include/espress_stream/timer.h
test/espress_stream_session_test.o: include/espress_stream/hash.h
test/espress_stream_session_test.o: include/espress_stream/mac.h
test/espress_stream_session_test.o: include/espress_stream/rand.h
test/espress_stream_session_test.o: include/espress_stream/block_cipher.h
test/espress_stream_session_test.o: include/espress_stream/ctr_cipher.h
test/espress_stream_session_test.o: include/espress_stream/hkdf.h
test/espress_stream_session_test.o: include/espress_stream/ec.h
test/espress_stream_session_test.o: include/espress_stream/key_ex.h
test/espress_stream_session_test.o: include/espress_stream/key_exp.h
test/espress_stream_session_test.o: include/espress_stream/key.h
test/espress_stream_session_test.o: include/espress_stream/ring.h
test/espress_stream_session_test.o: include/espress_stream/dgram.h
test/espress_stream_session_test.o: include/espress_stream/trans_ring.h
test/espress_stream_session_test.o: include/espress_stream/packet.h
test/espress_stream_session_test.o: include/espress_stream/trans_packet.h
test/espress_stream_session_test.o: include/espress_stream/ring_buffer.h
test/espress_stream_session_test.o: include/espress_stream/session.h
test/espress_stream_tick_test.o: include/espress_stream/config.h
test/espress_stream_tick_test.o: include/espress_stream/depend.h
test/espress_stream_tick_test.o: include/espress_stream/uint.h
test/espress_stream_tick_test.o: include/espress_stream/tick.h
test/espress_stream_timer_test.o: include/espress_stream/config.h
test/espress_stream_timer_test.o: include/espress_stream/depend.h
test/espress_stream_timer_test.o: include/espress_stream/uint.h
test/espress_stream_timer_test.o: include/espress_stream/tick.h
test/espress_stream_timer_test.o: include/espress_stream/mutex.h
test/espress_stream_timer_test.o: include/espress_stream/cond.h
test/espress_stream_timer_test.o: include/espress_stream/event.h
test/espress_stream_timer_test.o: include/espress_stream/timer.h
test/espress_stream_trans_packet_test.o: include/espress_stream/config.h
test/espress_stream_trans_packet_test.o: include/espress_stream/depend.h
test/espress_stream_trans_packet_test.o: include/espress_stream/uint.h
test/espress_stream_trans_packet_test.o: include/espress_stream/tick.h
test/espress_stream_trans_packet_test.o: include/espress_stream/mutex.h
test/espress_stream_trans_packet_test.o: include/espress_stream/cond.h
test/espress_stream_trans_packet_test.o: include/espress_stream/event.h
test/espress_stream_trans_packet_test.o: include/espress_stream/hash.h
test/espress_stream_trans_packet_test.o: include/espress_stream/mac.h
test/espress_stream_trans_packet_test.o: include/espress_stream/rand.h
test/espress_stream_trans_packet_test.o: include/espress_stream/block_cipher.h
test/espress_stream_trans_packet_test.o: include/espress_stream/ctr_cipher.h
test/espress_stream_trans_packet_test.o: include/espress_stream/hkdf.h
test/espress_stream_trans_packet_test.o: include/espress_stream/ec.h
test/espress_stream_trans_packet_test.o: include/espress_stream/key_ex.h
test/espress_stream_trans_packet_test.o: include/espress_stream/key_exp.h
test/espress_stream_trans_packet_test.o: include/espress_stream/key.h
test/espress_stream_trans_packet_test.o: include/espress_stream/ring.h
test/espress_stream_trans_packet_test.o: include/espress_stream/dgram.h
test/espress_stream_trans_packet_test.o: include/espress_stream/trans_ring.h
test/espress_stream_trans_packet_test.o: include/espress_stream/packet.h
test/espress_stream_trans_packet_test.o: include/espress_stream/trans_packet.h
test/espress_stream_trans_ring_test.o: include/espress_stream/config.h
test/espress_stream_trans_ring_test.o: include/espress_stream/depend.h
test/espress_stream_trans_ring_test.o: include/espress_stream/uint.h
test/espress_stream_trans_ring_test.o: include/espress_stream/mutex.h
test/espress_stream_trans_ring_test.o: include/espress_stream/cond.h
test/espress_stream_trans_ring_test.o: include/espress_stream/event.h
test/espress_stream_trans_ring_test.o: include/espress_stream/ring.h
test/espress_stream_trans_ring_test.o: include/espress_stream/dgram.h
test/espress_stream_trans_ring_test.o: include/espress_stream/trans_ring.h
test/espress_stream_uint_test.o: include/espress_stream/config.h
test/espress_stream_uint_test.o: include/espress_stream/depend.h
test/espress_stream_uint_test.o: include/espress_stream/uint.h
test/main.o: include/espress_stream/config.h include/espress_stream/depend.h
test/main.o: include/espress_stream/uint.h include/espress_stream/tick.h
test/main.o: include/espress_stream/rand.h
api/test/main.o: api/include/espress_stream/api.h
api/test/main.o: api/include/espress_stream/api_depend.h
