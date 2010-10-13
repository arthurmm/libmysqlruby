#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=f77

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Debug/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/libmysqlruby.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lmysqlclient -lruby -lz

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/GNU-Linux-x86/libmysqlruby.so

dist/Debug/GNU-Linux-x86/libmysqlruby.so: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.c} -rdynamic -shared -o dist/Debug/GNU-Linux-x86/libmysqlruby.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/libmysqlruby.o: libmysqlruby.c 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -g -Werror -D_GNU_SOURCE -D_DEBUG_ -I/usr/include/mysql -I/usr/lib/ruby/1.8/i486-linux -fPIC  -o ${OBJECTDIR}/libmysqlruby.o libmysqlruby.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/libmysqlruby.so

# Subprojects
.clean-subprojects:
