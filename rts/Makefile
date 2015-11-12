include ../Defines.make

LIB_RTS=librts.a
RTS_SRC=rts.c fileIO.c
OBJ=$(RTS_SRC:.c=.o)

all: $(LIB_RTS)

$(LIB_RTS) : $(OBJ)
	$(AR) rcs $@ $(OBJ)

%.o : %.c 
	@export LD_LIBRARY_PATH=$(LIBS); \
	$(CC) $(CFLAGS) -c -o $*.o $*.c; \
	echo "Generate dependencies for" $<

clean:
	rm -rf *.o $(LIB_RTS)
