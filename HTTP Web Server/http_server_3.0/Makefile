
COMM_MAKE = 1
COMM_ECHO = 1
version=0.5
v=release
include co.mk

########## options ##########
CFLAGS += -g -fno-strict-aliasing -O2 -Wall -export-dynamic \
		-Wall -pipe  -D_GNU_SOURCE -D_REENTRANT -fPIC -Wno-deprecated 

LINKS += -g -L./lib -lcolib -lpthread -ldl

COLIB_OBJS=co_routine.o co_hook_sys_call.o coctx_swap.o coctx.o
#co_swapcontext.o

PROGS = colib http_server

all:$(PROGS) 

colib:libcolib.a libcolib.so

libcolib.a: $(COLIB_OBJS)
	$(ARSTATICLIB) 
libcolib.so: $(COLIB_OBJS)
	$(BUILDSHARELIB) 

http_server:http_server.o httpd.o
	$(BUILDEXE)

.PHONY:cgi
cgi:
	cd /home/bit-class/http_server/wwwroot/cgi-bin;\
	make;\
	cp math-cgi ../;\
	cd -
	cd /home/bit-class/project/sql_connect;\
	make;\
	cp insert_cgi .;\
	cp select_cgi .;\
	cd -

dist: clean libco-$(version).src.tar.gz

libco-$(version).src.tar.gz:
	@find . -type f | grep -v CVS | grep -v .svn | sed s:^./:libco-$(version)/: > MANIFEST
	@(cd ..; ln -s libco_pub libco-$(version))
	(cd ..; tar cvf - `cat libco_pub/MANIFEST` | gzip > libco_pub/libco-$(version).src.tar.gz)
	@(cd ..; rm libco-$(version))

clean:
	$(CLEAN) *.o $(PROGS)
	rm -fr MANIFEST lib solib libco-$(version).src.tar.gz libco-$(version)

