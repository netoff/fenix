#!/bin/bash

case $1 in
	start)
		echo $$ >> /opt/pid/mongodb.pid;
		exec 2>&1 /opt/mongo/bin/mongod --dbpath /data/master --directoryperdb --quiet --nohttpinterface | /usr/sbin/rotatelogs -f /opt/log/mongodb.%Y-%m-%d-%H_%M_%S 10M
		;;
	stop)
		kill `cat /opt/pid/mongodb.pid`;
		rm -f /opt/pid/mongodb.pid
		;;
	*)
		echo "usage: mongodb {start|stop}";;
esac
exit 0
