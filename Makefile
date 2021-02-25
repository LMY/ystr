CFLAGS = $(shell pkg-config --cflags glib-2.0)
LDLIBS = $(shell pkg-config --libs glib-2.0) -lm
LDFLAGS += -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed

.PHONY: all clean

clean:
	rm -f ystr ystr.o
