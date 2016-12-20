include ../include.mk

OS := $(shell uname)
ifeq ($(OS), Darwin)
	LIBTOOL_CMD := glibtool 
	MVARS := -DMACOS=1
else
	LIBTOOL_CMD := libtool
	MVARS := 
endif

SUB_LIB=utils table docs check mm hash mmtable

all: ./libwordcheck.la

./libwordcheck.la: ./*.c ./wordcheck/*.h
	#createlo
	(for i in $(SUB_LIB);do $(LIBTOOL_CMD) --tag=CC --mode=compile gcc -g -O2 $(MVARS) -fPIC -Wall -c -lm `pkg-config --cflags --libs glib-2.0` $$i.c; done)
	#createla
	$(LIBTOOL_CMD) --tag=CC --mode=link gcc -g -O2 $(MVARS) -fPIC -Wall -o libwordcheck.la `(for i in $(SUB_LIB);do echo "$$i.lo"; done)` `pkg-config --cflags --libs glib-2.0` -rpath ${PREFIX}/lib/wordcheck

install:
	install -c -d ${DESTDIR}${PREFIX}/lib/wordcheck
	$(LIBTOOL_CMD) --tag=CC --mode=install install -c libwordcheck.la  ${DESTDIR}${PREFIX}/lib/wordcheck/libwordcheck.la
	install -c -d ${DESTDIR}${PREFIX}/include/wordcheck
	install -c -m 0644 wordcheck/*.h ${DESTDIR}${PREFIX}/include/wordcheck

clean:
	(for i in $(SUB_LIB);do rm -rf $$i.o; done)
	(for i in $(SUB_LIB);do rm -rf $$i.lo; done)
	rm -rf libwordcheck.*
	rm -rf .libs
