import os.path
import sys

env = Environment()

#Disable warnings
env.Append(CCFLAGS = ['-fvisibility=hidden', '-fpermissive', '-w'])
env.Append(CPPPATH = ['#lib', '#ext/boost_1_40_0'])

#Boost ibraries for direct linking
boost_system_lib =                env.Object(Glob('ext/boost_1_40_0/libs/system/src/*.cpp'))
boost_filesystem_lib =            env.Object(Glob('ext/boost_1_40_0/libs/filesystem/src/*.cpp'))
boost_datetime_gregorian_lib =    env.Object(Glob('ext/boost_1_40_0/libs/date_time/src/gregorian/*.cpp'))
boost_datetime_posix_lib =        env.Object(Glob('ext/boost_1_40_0/libs/date_time/src/posix_time/*.cpp'))

VariantDir('build', 'src', duplicate=0)

fenix_lib = SConscript(['lib/SConstruct'], exports='env boost_system_lib', variant_dir='lib/build', dupicate=0)
apache_module = SConscript(['mod/SConstruct'], exports='env boost_system_lib fenix_lib', variant_dir='mod/build', duplicate=0)
precompiler = SConscript(['precompiler/SConstruct'], exports='env boost_system_lib boost_filesystem_lib',  variant_dir='precompiler/build', duplicate=0)
	
precompile = env.Command('src/views.h', Glob('views/**/*.html'), 'bin/precompiler')
env.Requires(precompile, precompiler)
env.AlwaysBuild(precompile)
tyrant_libs = ['tokyotyrant', 'tokyocabinet', 'z', 'bz2', 'resolv', 'nsl', 'dl', 'rt', 'pthread', 'm', 'c']

application = env.SharedLibrary('application', 
				Glob('src/*.cpp'), 
				LIBS = [fenix_lib, boost_system_lib, boost_datetime_gregorian_lib, boost_datetime_posix_lib, tyrant_libs])
				
env.Depends(application, precompile)

env.InstallAs('bin/app.so', application)
