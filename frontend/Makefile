include $(STREAMIT_HOME)/misc/Makefile.vars

DEBUG_OPT       = -g
JAVA_VERSION    = -source 1.5 -target 1.5
JAVA_OPT        = -nowarn $(JAVA_VERSION) 
JAVAC_FLAGS     = $(JAVA_OPT) $(DEBUG_OPT)
RM              = rm -f

STREAMIT_GENERATED = \
  laminarir/frontend/StreamItLex.java \
  laminarir/frontend/StreamItParserFE.java

all: 
	$(MAKE) java_backend laminarir_backend

java_backend:
	(cd streamit/library; `find . -name "*.java" | xargs -s20480 $(JAVAC)`)
	(cd streamit/frontend; `find . -name "*.java" | xargs -s20480 $(JAVAC)`)
	(cd streamit/scheduler2; `find . -name "*.java" | xargs -s20480 $(JAVAC)`)

laminarir_backend: $(STREAMIT_GENERATED) 
	$(MAKE) $(STREAMIT_GENERATED)
	(cd laminarir/frontend; `find . -name "*.java" | xargs -s20480 $(JAVAC)`)

%.class: %.java
	$(JAVAC) $(JAVAC_FLAGS) $<

%.java %TokenTypes.txt: %.g
	@$(RM) $*.java
	cd $(dir $<) && \
	$(JAVA) antlr.Tool $(notdir $<)

clean:
	$(FIND) . -name '*.class' -print | $(XARGS) $(RM)
	rm -rf $(STREAMIT_GENERATED) 
	rm -rf laminarir/frontend/*.txt 
	rm -rf laminarir/frontend/StreamItLex.smap 
	rm -rf laminarir/frontend/StreamItParserFE.smap
	rm -rf compile 
