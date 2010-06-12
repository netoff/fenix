#!/bin/bash

case $1 in
	start)
		echo $$ >> /opt/pid/logger.pid;
		exec 2>&1 sudo -u www-data /opt/fenix/bin/logger | /usr/sbin/rotatelogs -f /opt/log/logger.%Y-%m-%d-%H_%M_%S 10M
		;;
	stop)
		kill `cat /opt/pid/logger.pid`;
		rm -f /opt/pid/logger.pid
		;;
	*)
		echo "usage: logger{start|stop}";;
esac
exit 0
