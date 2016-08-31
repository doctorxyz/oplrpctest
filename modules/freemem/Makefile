IOP_BIN  = freemem.irx
IOP_OBJS = main.o imports.o


IOP_CFLAGS  += -Wall -fno-builtin-printf -fno-builtin-memcpy -Wimplicit-int -Werror-implicit-function-declaration -Werror
IOP_LDFLAGS += -s

all: $(IOP_BIN)

clean:
	rm -f $(IOP_BIN) $(IOP_OBJS)

include $(PS2SDK)/Defs.make
include ../Rules.make
