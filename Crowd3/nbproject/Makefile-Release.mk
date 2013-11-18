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
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/HelpFrame.o \
	${OBJECTDIR}/Settings.o \
	${OBJECTDIR}/crowd3.o \
	${OBJECTDIR}/CrowdMaker.o \
	${OBJECTDIR}/forms/HelpPanel.o \
	${OBJECTDIR}/MakerFrame.o \
	${OBJECTDIR}/wxImagePanel.o \
	${OBJECTDIR}/ImageDB.o \
	${OBJECTDIR}/Tools.o \
	${OBJECTDIR}/ImageTree.o \
	${OBJECTDIR}/PeopleFinder.o \
	${OBJECTDIR}/AppFrame.o \
	${OBJECTDIR}/Icon.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`wx-config --cflags --cxxflags` 
CXXFLAGS=`wx-config --cflags --cxxflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib/gtk-2.0/2.10.0 `pkg-config --libs opencv` `wx-config --libs --cxxflags --debug=no` -lsqlite3  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/crowd3

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/crowd3: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/crowd3 -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/HelpFrame.o: HelpFrame.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/HelpFrame.o HelpFrame.cpp

${OBJECTDIR}/Settings.o: Settings.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/Settings.o Settings.cpp

${OBJECTDIR}/crowd3.o: crowd3.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/crowd3.o crowd3.cpp

${OBJECTDIR}/CrowdMaker.o: CrowdMaker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/CrowdMaker.o CrowdMaker.cpp

${OBJECTDIR}/forms/HelpPanel.o: forms/HelpPanel.cpp 
	${MKDIR} -p ${OBJECTDIR}/forms
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/forms/HelpPanel.o forms/HelpPanel.cpp

${OBJECTDIR}/MakerFrame.o: MakerFrame.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/MakerFrame.o MakerFrame.cpp

${OBJECTDIR}/wxImagePanel.o: wxImagePanel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/wxImagePanel.o wxImagePanel.cpp

${OBJECTDIR}/ImageDB.o: ImageDB.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/ImageDB.o ImageDB.cpp

${OBJECTDIR}/Tools.o: Tools.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/Tools.o Tools.cpp

${OBJECTDIR}/ImageTree.o: ImageTree.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/ImageTree.o ImageTree.cpp

${OBJECTDIR}/PeopleFinder.o: PeopleFinder.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/PeopleFinder.o PeopleFinder.cpp

${OBJECTDIR}/AppFrame.o: AppFrame.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/AppFrame.o AppFrame.cpp

${OBJECTDIR}/Icon.o: Icon.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -s -D__cplusplus -I/usr/include -I/usr/include/wx-2.8 -I/usr/include/c++/4.6 -I/usr/include/i386-linux-gnu -I/usr/lib/wx/include/gtk2-unicode-release-2.8 `pkg-config --cflags opencv` `wx-config --cflags --cxxflags --debug=no`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/Icon.o Icon.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/crowd3

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
