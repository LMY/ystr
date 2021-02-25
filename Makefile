# exe name
EXE = ystr

# paths
SRCS    := $(wildcard src/*.c)
HEADERS := $(wildcard src/*.h)
OBJ     := $(SRCS:.c=.o)

# compiler and linker flags
CFLAGS = $(shell pkg-config --cflags glib-2.0)
LDLIBS = $(shell pkg-config --libs gio-2.0 glib-2.0) -lm
LDFLAGS += -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed

# rules
all: $(EXE)

$(OBJ): $(HEADERS)

$(EXE): $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ) $(LDLIBS)


.PHONY: all clean

clean:
	rm -f $(OBJ) $(EXE)


