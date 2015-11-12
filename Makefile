include Defines.make

ANTLR_JAR=bin/antlr.jar
EXPORT=export LD_LIBRARY_PATH=$(LIBS)

all: parser/laminarParser.py parser/laminarLexer.py librts.a libpapi_wrapper.a 

parser/laminarParser.py parser/laminarLexer.py: parser/laminar.g
	java -cp ${ANTLR_JAR} org.antlr.Tool parser/laminar.g

librts.a:
	make -C rts/

libpapi_wrapper.a:
	make -C papi_wrapper/

%.s: %.c
	$(CC) $(CFLAGS) -S $< -I$(PAPI_INC)

%.ll: %.c
	$(CC) $(CFLAGS) -S -emit-llvm -o $@ $< -I$(PAPI_INC)

%: %.c librts.a libpapi_wrapper.a
	$(EXPORT);$(CC) $(CFLAGS) -o $@ $< papi_wrapper/libpapi_wrapper.a rts/librts.a -I$(PAPI_INC) -L$(PAPI_LIB) -lpapi -lm

%: %.s librts.a libpapi_wrapper.a
	$(EXPORT);$(CC) $(CFLAGS) -o $@ $< papi_wrapper/libpapi_wrapper.a rts/librts.a -L$(PAPI_LIB) -lpapi -lm

run-%: 
	$(EXPORT);$(RUN_OPT) ./$*

print-%:
	@echo -n $($*)

clean:
	@echo "clean up."
	@rm -rf *.pyc
	@rm -f parser/*.py*
	@rm -f *.c *.out *.result *.ll *.s
	@cd rts;make clean
	@cd streamit;find . -name "a.out" -exec rm -f "{}" \;
	@cd streamit;find . -name "combined_threads.o" -exec rm -f "{}" \;
	@cd papi_wrapper;make clean
	@cd frontend;make clean
	@find . -name '*.py.bak' -exec rm -f "{}" \;

.PHONY: clean all
