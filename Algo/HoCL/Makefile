SRCS=
GEN_OPTS=-dump_typed
DOT_OPTS=-target_dir ./dot -dot_slotted_boxes
SYSTEMC_OPTS=-target_dir ./systemc -sc_stop_time 100
PREESM_OPTS=-target_dir ./preesm
PREESM_REP=/home/ggauthie/dev/workspace_preesm
PREESM_PROJ=simple

HOCL=/usr/local/hocl
HOCLC=$(HOCL)/bin/hoclc
DOTVIEWER=dot
VCDVIEWER=gtkwave
TXTVIEWER=nano

all: dot.show

dot: $(SRCS)
	$(HOCLC) $(HOCL_OPTS) $(GEN_OPTS) -dot $(DOT_OPTS) $(SRCS)

dot.show: dot
	@for f in ./dot/*.dot; do $(DOTVIEWER) $$f; done

.PHONY: dot preesm systemc preesm.proj

preesm: $(SRCS)
	$(HOCLC) $(HOCL_OPTS) $(GEN_OPTS) -preesm $(PREESM_OPTS) $(SRCS)

preesm.proj: 
	cp preesm/*.pi ..

xdf: $(SRCS)
	$(HOCLC) $(HOCL_OPTS) $(GEN_OPTS) -xdf $(XDF_OPTS) $(SRCS)

clean:
	@\rm -f *.dot
	@\rm -f *.xdf
	@\rm -f *.output

clobber: clean
	@\rm -f *~
