#! /bin/sh
### BEGIN INIT INFO
# Provides:          mongodb
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Mongodb startup script
# Description:       Mongodb start stop daemon sends SIGINT to terminate
#                    say man signal to see details

# Please check the startup params and replication options
#Mongo db usage:
#  run                      run db
#  msg [msg] [port]         send a request to the db server listening on port (or default)
#  msglots                  send many test messages, and then wait for answer on the last one
#  longmsg                  send a long test message to the db server
#  quicktest                just check basic assertions and exit
#  test2                    run test2() - see code
#
#Options:
# --help                    show this usage information
# --port <portno>           specify port number, default is 27017
# --dbpath <root>           directory for datafiles, default is /data/db/
# --quiet                   quieter output
# --cpu                     show cpu+iowait utilization periodically
# --noauth                  run without security
# --auth                    run with security
# --verbose
# -v+                       increase verbose level -v = --verbose
# --objcheck                inspect client data for validity on receipt
# --quota                   enable db quota management
# --appsrvpath <path>       root directory for the babble app server
# --nocursors               diagnostic/debugging option
# --nohints                 ignore query hints
# --nohttpinterface         disable http interface
# --nojni
# --oplog<n>                0=off 1=W 2=R 3=both 7=W+some reads
# --oplogSize <size_in_MB>  custom size if creating new replication operation log
# --sysinfo                 print some diagnostic system information
#
#Replication:
# --master
# --slave
# --source <server:port>    when a slave, specifies master
# --only <dbname>           when a slave, only replicate db <dbname>
# --pairwith <server:port> <arbiter>
# --autoresync

### END INIT INFO

# Author: Kunthar <kunthar@gmail.com>
#

# Do NOT "set -e"

# Check the paths and data dir with additional options at startup...
PATH=/sbin:/usr/sbin:/bin:/usr/bin:/opt/mongo
DESC="Mongodb Service"
NAME=mongod
MONGOPATH=/opt/mongo
DAEMON=$MONGOPATH/bin/$NAME
#Please do not forget to give correct path
DBPATH=/data/master
#If security matters, please change arg to --auth
DAEMON_ARGS="--dbpath $DBPATH --directoryperdb --quiet --nohttpinterface --noauth --syncdelay 180"
PIDFILE=/var/run/$NAME.pid
SCRIPTNAME=/etc/init.d/$NAME

# Exit if the package is not installed
[ -x "$DAEMON" ] || exit 0

# Load the VERBOSE setting and other rcS variables
. /lib/init/vars.sh

# Define LSB log_* functions.
# Depend on lsb-base (>= 3.0-6) to ensure that this file is present.
. /lib/lsb/init-functions

#
# Function that starts the daemon/service
#
do_start()
{
	echo -e "Starting $DESC \n"
        start-stop-daemon -Sbm -p $PIDFILE --exec  $DAEMON >/dev/null 2>&1 -- $DAEMON_ARGS
	echo -e "\n started"

}

#
# Function that stops the daemon/service
#
do_stop()
{
	echo -e "Stopping $DESC by sending ctrl+c \n"
	#Be nice send ctrl+c to mongod daemon
	start-stop-daemon --stop --signal 2 -q --pidfile $PIDFILE --name $NAME 
	# Many daemons don't delete their pidfiles when they exit.
	rm -f $PIDFILE
	echo -e "\n stopped"
}

#
# Function that sends a SIGHUP to the daemon/service
#
do_reload() {
	#
	# If the daemon can reload its configuration without
	# restarting (for example, when it is sent a SIGHUP),
	# then implement that here.
	#
	start-stop-daemon --stop --signal 1 --quiet --pidfile $PIDFILE --name $NAME
	return 0
}

case "$1" in
  start)
	[ "$VERBOSE" != no ] && log_daemon_msg "Starting $DESC" "$NAME"
	do_start
	case "$?" in
		0|1) [ "$VERBOSE" != no ] && log_end_msg 0 ;;
		2) [ "$VERBOSE" != no ] && log_end_msg 1 ;;
	esac
	;;
  stop)
	[ "$VERBOSE" != no ] && log_daemon_msg "Stopping $DESC" "$NAME"
	do_stop
	case "$?" in
		0|1) [ "$VERBOSE" != no ] && log_end_msg 0 ;;
		2) [ "$VERBOSE" != no ] && log_end_msg 1 ;;
	esac
	;;
  #reload|force-reload)
	#
	# If do_reload() is not implemented then leave this commented out
	# and leave 'force-reload' as an alias for 'restart'.
	#
	#log_daemon_msg "Reloading $DESC" "$NAME"
	#do_reload
	#log_end_msg $?
	#;;
  restart|force-reload)
	#
	# If the "reload" option is implemented then remove the
	# 'force-reload' alias
	#
	log_daemon_msg "Restarting $DESC" "$NAME"
	do_stop
	case "$?" in
	  0|1)
		do_start
		case "$?" in
			0) log_end_msg 0 ;;
			1) log_end_msg 1 ;; # Old process is still running
			*) log_end_msg 1 ;; # Failed to start
		esac
		;;
	  *)
	  	# Failed to stop
		log_end_msg 1
		;;
	esac
	;;
  *)
	#echo "Usage: $SCRIPTNAME {start|stop|restart|reload|force-reload}" >&2
	echo "Usage: $SCRIPTNAME {start|stop}" >&2
	exit 3
	;;
esac

:
