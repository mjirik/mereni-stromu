#
# Generated Makefile - do not edit!
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
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=
AS=as.exe

# Macros
CND_PLATFORM=Cygwin-Windows
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/dlst_scr.o \
	${OBJECTDIR}/dat_scr.o \
	${OBJECTDIR}/memory.o \
	${OBJECTDIR}/serial.o \
	${OBJECTDIR}/slowloop.o \
	${OBJECTDIR}/mysql_connect.o \
	${OBJECTDIR}/error.o \
	${OBJECTDIR}/hlp_scr.o \
	${OBJECTDIR}/common.o \
	${OBJECTDIR}/log.o \
	${OBJECTDIR}/convert.o \
	${OBJECTDIR}/data.o \
	${OBJECTDIR}/measure.o \
	${OBJECTDIR}/conf.o \
	${OBJECTDIR}/keyboard.o \
	${OBJECTDIR}/stp_scr.o \
	${OBJECTDIR}/vizualizace.o \
	${OBJECTDIR}/data_serial.o \
	${OBJECTDIR}/inout.o \
	${OBJECTDIR}/buttons.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/lcd.o \
	${OBJECTDIR}/msr_scr.o \
	${OBJECTDIR}/gui_toolbox.o \
	${OBJECTDIR}/keyfcn.o \
	${OBJECTDIR}/data_file.o \
	${OBJECTDIR}/camera.o \
	${OBJECTDIR}/gui.o

# C Compiler Flags
CFLAGS=-I"C:\Program Files\MySQL\MySQL_Connector_C_6.0.2\include"

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../../../../../OpenCV1.2/lib/cxcore120.lib ../../../../../OpenCV1.2/lib/cv120.lib ../../../../../OpenCV1.2/lib/highgui120.lib -lncurses.dll -lmysql -lmysqlclient

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/Cygwin-Windows/slim_client_server.exe

dist/Debug/Cygwin-Windows/slim_client_server.exe: ../../../../../OpenCV1.2/lib/cxcore120.lib

dist/Debug/Cygwin-Windows/slim_client_server.exe: ../../../../../OpenCV1.2/lib/cv120.lib

dist/Debug/Cygwin-Windows/slim_client_server.exe: ../../../../../OpenCV1.2/lib/highgui120.lib

dist/Debug/Cygwin-Windows/slim_client_server.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/Cygwin-Windows
	${LINK.c} -L"C:\Program Files\MySQL\MySQL_Connector_C_6.0.2\lib\opt" -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/slim_client_server ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/dlst_scr.o: nbproject/Makefile-${CND_CONF}.mk dlst_scr.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/dlst_scr.o dlst_scr.c

${OBJECTDIR}/dat_scr.o: nbproject/Makefile-${CND_CONF}.mk dat_scr.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/dat_scr.o dat_scr.c

${OBJECTDIR}/memory.o: nbproject/Makefile-${CND_CONF}.mk memory.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/memory.o memory.c

${OBJECTDIR}/serial.o: nbproject/Makefile-${CND_CONF}.mk serial.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/serial.o serial.c

${OBJECTDIR}/slowloop.o: nbproject/Makefile-${CND_CONF}.mk slowloop.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/slowloop.o slowloop.c

${OBJECTDIR}/mysql_connect.o: nbproject/Makefile-${CND_CONF}.mk mysql_connect.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/mysql_connect.o mysql_connect.c

${OBJECTDIR}/error.o: nbproject/Makefile-${CND_CONF}.mk error.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/error.o error.c

${OBJECTDIR}/hlp_scr.o: nbproject/Makefile-${CND_CONF}.mk hlp_scr.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/hlp_scr.o hlp_scr.c

${OBJECTDIR}/common.o: nbproject/Makefile-${CND_CONF}.mk common.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/common.o common.c

${OBJECTDIR}/log.o: nbproject/Makefile-${CND_CONF}.mk log.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/log.o log.c

${OBJECTDIR}/convert.o: nbproject/Makefile-${CND_CONF}.mk convert.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/convert.o convert.c

${OBJECTDIR}/data.o: nbproject/Makefile-${CND_CONF}.mk data.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/data.o data.c

${OBJECTDIR}/measure.o: nbproject/Makefile-${CND_CONF}.mk measure.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/measure.o measure.c

${OBJECTDIR}/conf.o: nbproject/Makefile-${CND_CONF}.mk conf.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/conf.o conf.c

${OBJECTDIR}/keyboard.o: nbproject/Makefile-${CND_CONF}.mk keyboard.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/keyboard.o keyboard.c

${OBJECTDIR}/stp_scr.o: nbproject/Makefile-${CND_CONF}.mk stp_scr.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/stp_scr.o stp_scr.c

${OBJECTDIR}/vizualizace.o: nbproject/Makefile-${CND_CONF}.mk vizualizace.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/vizualizace.o vizualizace.c

${OBJECTDIR}/data_serial.o: nbproject/Makefile-${CND_CONF}.mk data_serial.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/data_serial.o data_serial.c

${OBJECTDIR}/inout.o: nbproject/Makefile-${CND_CONF}.mk inout.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/inout.o inout.c

${OBJECTDIR}/buttons.o: nbproject/Makefile-${CND_CONF}.mk buttons.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/buttons.o buttons.c

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/lcd.o: nbproject/Makefile-${CND_CONF}.mk lcd.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/lcd.o lcd.c

${OBJECTDIR}/msr_scr.o: nbproject/Makefile-${CND_CONF}.mk msr_scr.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/msr_scr.o msr_scr.c

${OBJECTDIR}/gui_toolbox.o: nbproject/Makefile-${CND_CONF}.mk gui_toolbox.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/gui_toolbox.o gui_toolbox.c

${OBJECTDIR}/keyfcn.o: nbproject/Makefile-${CND_CONF}.mk keyfcn.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/keyfcn.o keyfcn.c

${OBJECTDIR}/data_file.o: nbproject/Makefile-${CND_CONF}.mk data_file.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/data_file.o data_file.c

${OBJECTDIR}/camera.o: nbproject/Makefile-${CND_CONF}.mk camera.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/camera.o camera.c

${OBJECTDIR}/gui.o: nbproject/Makefile-${CND_CONF}.mk gui.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -DWIN32 -DMODUL_INTERFACE -DMODUL_COMMUNICATION -I../../../../../OpenCV1.2/include/opencv -MMD -MP -MF $@.d -o ${OBJECTDIR}/gui.o gui.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/Cygwin-Windows/slim_client_server.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
