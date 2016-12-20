include include.mk

OS := $(shell uname)
ifeq ($(OS), Darwin)
	SUB_DIRS=src bin
else
	SUB_DIRS=src ld bin
endif

all: 
	@(for i in $(SUB_DIRS);do $(MAKE) -C $$i $@ || exit -1;done)

install: all
	mkdir -p ${DESTDIR}${PREFIX}
	@(for i in $(SUB_DIRS);do $(MAKE) -C $$i $@ || exit -1;done)

clean:
	@(for i in $(SUB_DIRS);do $(MAKE) -C $$i $@ || exit -1;done)
	rm -rf main
