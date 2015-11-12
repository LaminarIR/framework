include ../Defines.make

LIB_PAPI=libpapi_wrapper.a
PAPI_WRAPPER_SRC=papi_hw_counter.c
OBJ=$(PAPI_WRAPPER_SRC:.c=.o)

all: $(LIB_PAPI)

$(LIB_PAPI) : $(OBJ)
	$(AR) rcs $@ $(OBJ)

%.o : %.c 
	@export LD_LIBRARY_PATH=$(LIBS); \
	$(CC) $(CFLAGS) -c -o $*.o $*.c $(INCLUDES) -I$(PAPI_INC); \
	echo "Generate dependencies for" $<

clean:
	rm -rf *.o $(LIB_PAPI)
