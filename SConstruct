import os.path
import commands
import sys
import random

KEY_FILE = "./key.sec"

def secret_key():
	ret = ''
	a = []
	for i in range(16):
		key = ''
		random.seed()
		for j in range(64):			
			key += random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789')
		a.append(key)
	random.seed()
	ret = a[random.randrange(0, 15)]
	return ret

def create_key_file():
	if not os.path.exists(KEY_FILE):
		print "\nCreating key file....\n"
		fout = open(KEY_FILE, 'w')
		key = secret_key()
		fout.write(key)
		fout.close()
	else:
		print "\nKey file already there\n"
		
def get_key_from_file():
	f = open(KEY_FILE, "r")
	key = f.readline()
	f.close()
	if key == "" or not len(key) == 64:
		print "\nFATAL ERROR: No secret key!\n"
	return key
	
create_key_file()		
SECRET_KEY = get_key_from_file()

print "Secret key: [" + SECRET_KEY + "]"

#================================================
#BOOTSTRAP HERE is there a better way to do this?
def bootstrap():
	if not os.path.exists("./bin/precompiler"):
		print "Bootstrap first:"
		print "scons bootstrap"

bootstrap()
#================================================


#-----------------------------------------------
#CONFIG
INSPECT_FLAGS = ['-Wall']
INSPECTS_FLAGS = ['-Wall', '-pedantic']
DEBUG_FLAGS = ['-DDEBUG', '-w', '-g']
FINAL_FLAGS = ['-O3', '-w']

BOOST_PATH = '#ext/boost_1_42_0/'
CRYPTOPP_PATH = '#ext/crypto_pp/'
MONGO_PATH = '/opt/mongo/include/mongo/'

BOOST_LIBS = ['system/src/*.cpp', 'filesystem/src/*.cpp', 
	'date_time/src/gregorian/*.cpp', 'date_time/src/posix_time/*.cpp',
	'thread/src/pthread/*.cpp', 'program_options/src/*.cpp']

INCLUDES_PATH = ['#lib', BOOST_PATH, CRYPTOPP_PATH, MONGO_PATH]

PROG_FLAGS = ['-fvisibility=hidden', '-fpermissive', '-fPIC']
LIBS_FLAGS = ['-O3','-w'] 

BUILD_DIR = '.build/'

join = os.path.join
#----------------------------------------------

lib_env = Environment(CCFLAGS = LIBS_FLAGS, CPPPATH = INCLUDES_PATH)
prog_env = Environment(CCFLAGS = PROG_FLAGS, CPPPATH = [INCLUDES_PATH, '#src'])

#get environment from the command line
inspect = ARGUMENTS.get('inspect', 0)
inspect_strict = ARGUMENTS.get('inspect-s', 0)
debug = ARGUMENTS.get('debug', 0)
final = ARGUMENTS.get('final', 0)

#Find build directory and compilation options
if int(inspect) == 1:
        APP_BUILD_DIR = join(BUILD_DIR, "app/inspect/")
	prog_env.Append(CCFLAGS = INSPECT_FLAGS)
elif int(inspect_strict) == 1:
        APP_BUILD_DIR = join(BUILD_DIR, "app/inspect-strict/")
	prog_env.Append(CCFLAGS = INSPECTS_FLAGS)
elif int(debug) == 1:
        APP_BUILD_DIR = join(BUILD_DIR, "app/debug/")
	prog_env.Append(CCFLAGS = DEBUG_FLAGS)
elif int(final) == 1:
        APP_BUILD_DIR = join(BUILD_DIR, "app/final/")
	prog_env.Append(CCFLAGS = FINAL_FLAGS)
else: 
	APP_BUILD_DIR = join(BUILD_DIR, "app/default/")
	prog_env.Append(CCFLAGS = '-w')

BOOST_BUILD_DIR = join(BUILD_DIR, 'libs/boost/')
VariantDir(BOOST_BUILD_DIR, join(BOOST_PATH, 'libs/'), duplicate=0)

CRYPTOPP_BUILD_DIR = join(BUILD_DIR, 'libs/crypto/')
VariantDir(CRYPTOPP_BUILD_DIR, CRYPTOPP_PATH, duplicate=0)
	
#BOOST lib
boost_libs = []
for lib in BOOST_LIBS:
	boost_libs.append(Glob(join(BOOST_BUILD_DIR, lib)))
	
boost_lib = lib_env.StaticLibrary(join(BOOST_BUILD_DIR, 'boost'), 
	boost_libs, CCFLAGS = ['-DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG', '-fPIC'])

#crypto_pp lib
crypto_lib = lib_env.StaticLibrary(join(CRYPTOPP_BUILD_DIR, 'crypto'), Glob(join(CRYPTOPP_BUILD_DIR, '*.cpp')), CCFLAGS = ['-fPIC'])

Export('lib_env', 'prog_env', 'boost_lib', 'crypto_lib')

fenix_lib = SConscript('lib/SConstruct', variant_dir= join(APP_BUILD_DIR, 'lib/'), dupicate=0)
Export('fenix_lib')

apache_module = SConscript('mod/SConstruct', variant_dir=join(APP_BUILD_DIR, 'mod/'), duplicate=0)
	
precompiler = SConscript('precompiler/SConstruct', variant_dir=join(APP_BUILD_DIR, 'precompiler/'), duplicate=0)
install_precompiler = prog_env.Install('bin/', precompiler)

prog_env.Alias("bootstrap", [precompiler, install_precompiler])

precompile = prog_env.Command('src/views.h', Glob('views/**/*.html'), 'bin/precompiler')

prog_env.Requires(precompile, precompiler)
prog_env.AlwaysBuild(precompile)

#tyrant_libs = ['tokyotyrant', 'tokyocabinet', 'z', 'bz2', 'resolv', 'nsl', 'dl', 'rt', 'pthread', 'm', 'c']

#prog_env.Append(LIBPATH = ['#ext/mongo_src'])

prog_env.Append(CCFLAGS = ['-DSECRET_KEY=\\"' + SECRET_KEY + '\\"'])
application = prog_env.SharedLibrary(join(APP_BUILD_DIR, 'application'), Glob('src/*.cpp'), 
				LIBS = [fenix_lib, 'libmongoclient.a', boost_lib, crypto_lib])
				
prog_env.Depends(application, precompile)

logger = SConscript('logger/SConstruct', variant_dir=join(APP_BUILD_DIR, 'logger/'), duplicate=0)
install_logger = prog_env.InstallAs('bin/logger', logger)
install_lua_ext = prog_env.Install('/opt/lua/ext/share/lua/5.1', Glob('logger/*.lua'))

#if build only logger 'scons logger_server'
prog_env.Alias('logger_server', [logger, install_logger, install_lua_ext])

#Default(logger_server)
#-------------------------------------------------------
#INSTALL
prog_env.InstallAs('bin/mod_fenix.so', apache_module)
prog_env.InstallAs('bin/app.so', application)


