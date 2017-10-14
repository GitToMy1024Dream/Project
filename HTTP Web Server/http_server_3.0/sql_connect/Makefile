ROOT_PATH=$(shell pwd)
MYSQL_INCLUDE=$(ROOT_PATH)/lib/include
MYSQL_LIB=$(ROOT_PATH)/lib/lib
cc=g++



.PHONY:all
all: insert_cgi select_cgi

insert_cgi:insert_cgi.o sql_api.o
	cc -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient
select_cgi:select_cgi.o sql_api.o
	cc -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient

%.o:%.cpp
	cc -c $< -I$(MYSQL_INCLUDE)
export LD_LIBRARY_PATH=$(MYSQL_LIB)


.PHONY:clean
clean:
	rm -f insert_cgi select_cgi *.o

.PHONY:debug
debug:
	echo $(MYSQL_INCLUDE)


