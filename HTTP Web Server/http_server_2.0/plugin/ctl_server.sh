#!/bin/bash

ROOT_PATH=$(pwd)
CONF=$ROOT_PATH/conf/server.conf
BIN=$ROOT_PATH/httpd

server_pid=''
proc=$(basename $0)
function usage()
{
	printf "%s [start(-s)|stop(-t)|restart(-rs)]\n" "$proc"
}
function is_exist()
{
	name=$(basename $BIN)
	server_pid=$(pidof $name)
	if [ $? -eq 0 ];then
		return 0
	else
		return 1
	fi
}

function start_server()
{
	if is_exist; then
		echo "server is exist,pid is : $server_pid "
	else
		ip=$(awk -F: '/^IP/{print $NF}' $CONF)
		port=$(awk -F: '/^PORT/{print $NF;}' $CONF)

		$BIN $ip $port
		echo "start...done"
	fi
}

function stop_server()
{
	if is_exist; then
		kill -9 $server_pid
		echo "stop...done"
	else
		echo "server not exist,no need to stop!"
	fi
}

if [ $# -ne 1 ];then
	usage
	exit 1
fi

case $1 in
	start | -s )
		start_server
		;;
	stop | -t )
		stop_server
		;;
	restart | -rs )
		stop_server
		start_server
		;;
	* )
		usage
		exit 2
		;;
esac
