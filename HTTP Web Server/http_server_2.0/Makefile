ROOT_PATH=$(shell pwd)
CONF=$(ROOT_PATH)/conf
PLUGIN=$(ROOT_PATH)/plugin
WWWROOT=$(ROOT_PATH)/wwwroot
SQL=$(ROOT_PATH)/sql_connect
FLAGS=-D_STDOUT_  #-D_DEBUG_#
LDFLAGS=-lpthread #-static
cc=gcc
bin=httpd
src=httpd.c main.c
obj=$(shell echo $(src) | sed 's/\.c/\.o/g')

.PHONY:all
all:$(bin) cgi
$(bin):$(obj)
	$(cc) -o $@ $^ $(LDFLAGS)
%.o:%.c
	$(cc) -c $< $(FLAGS)
cgi:
	cd $(WWWROOT)/cgi-bin;\
	make;\
	cp math-cgi $(ROOT_PATH);\
	cd -
	cd $(SQL);\
	make;\
	cp insert_cgi $(WWWROOT)/cgi-bin/;\
	cp select_cgi $(WWWROOT)/cgi-bin/;\
	cd -

.PHONY:clean
clean:
	rm -rf *.o $(bin) output;\
	cd $(WWWROOT)/cgi-bin;\
	make clean;\
	cd -
	cd $(SQL);\
	make clean;\
	cd -;\
	rm -f insert_cgi select_cgi math-cgi cgi-bin

.PHONY:output
output:
	mkdir output
	cp -rf conf output/
	cp -rf log output/
	cp -rf wwwroot output/
	mv  httpd output/
	rm -f math-cgi
	cp $(PLUGIN)/ctl_server.sh output
	cp -rf $(CONF) output
