#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build
NBTMPDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tmp-packaging
TMPDIRNAME=tmp-packaging
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/crowd3
OUTPUT_BASENAME=crowd3
PACKAGE_TOP_DIR=/usr/

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}
function copyFileToTmpDir
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

# Setup
cd "${TOP}"
mkdir -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
rm -rf ${NBTMPDIR}
mkdir -p ${NBTMPDIR}

# Copy files and create directories and links
cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/local/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}local/bin/${OUTPUT_BASENAME}" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/local/share/crowd3"
copyFileToTmpDir "crowd3.xml" "${NBTMPDIR}/${PACKAGE_TOP_DIR}local/share/crowd3/crowd3.xml" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/local/share/crowd3"
copyFileToTmpDir "backgrounds/beach.jpg" "${NBTMPDIR}/${PACKAGE_TOP_DIR}local/share/crowd3/beach.jpg" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/local/share/crowd3"
copyFileToTmpDir "backgrounds/lake.jpg" "${NBTMPDIR}/${PACKAGE_TOP_DIR}local/share/crowd3/lake.jpg" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/local/share/crowd3"
copyFileToTmpDir "backgrounds/ocean.jpg" "${NBTMPDIR}/${PACKAGE_TOP_DIR}local/share/crowd3/ocean.jpg" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/local/share/crowd3"
copyFileToTmpDir "backgrounds/sunset.jpg" "${NBTMPDIR}/${PACKAGE_TOP_DIR}local/share/crowd3/sunset.jpg" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/local/share/crowd3"
copyFileToTmpDir "backgrounds/trees.jpg" "${NBTMPDIR}/${PACKAGE_TOP_DIR}local/share/crowd3/trees.jpg" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/local/share/crowd3"
copyFileToTmpDir "icons/crowd3.png" "${NBTMPDIR}/${PACKAGE_TOP_DIR}local/share/crowd3/crowd3.png" 0644


# Create control file
cd "${TOP}"
CONTROL_FILE=${NBTMPDIR}/DEBIAN/control
rm -f ${CONTROL_FILE}
mkdir -p ${NBTMPDIR}/DEBIAN

cd "${TOP}"
echo 'Package: crowd3' >> ${CONTROL_FILE}
echo 'Version: 1.0' >> ${CONTROL_FILE}
echo 'Architecture: i386' >> ${CONTROL_FILE}
echo 'Maintainer: Dennis Damico <dennis508@yahoo.com>' >> ${CONTROL_FILE}
echo 'Installed-Size: 3500000' >> ${CONTROL_FILE}
echo 'Homepage: http://crowd3.sourceforge.net' >> ${CONTROL_FILE}
echo 'Section: graphics' >> ${CONTROL_FILE}
echo 'Description: crowd3 crowd scene maker' >> ${CONTROL_FILE}
echo ' crowd3 creates images of crowd scenes using the people in your photo collection.' >> ${CONTROL_FILE}
echo 'Depends: libstdc++6 (>=4.6.1), libgcc1 (>=1:4.6.1), libc6 (>=2.13), libsqlite3-0 (>=3.7.7), libwxbase2.8-0 (>=2.8.11), libwxgtk2.8-0 (>=2.8.11), libopencv-core2.3 (>=2.3.1), libopencv-imgproc2.3 (>=2.3.1), libopencv-highgui2.3 (>=2.3.1), libopencv-objdetect2.3 (>=2.3.1), libopencv-flann2.3 (>=2.3.1), libopencv-features2d2.3 (>=2.3.1), libopencv-calib3d2.3 (>=2.3.1)' >> ${CONTROL_FILE}

# Create Debian Package
cd "${TOP}"
cd "${NBTMPDIR}/.."
dpkg-deb  --build ${TMPDIRNAME}
checkReturnCode
cd "${TOP}"
mkdir -p  ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
mv ${NBTMPDIR}.deb ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/crowd3.deb
checkReturnCode
echo Debian: ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/crowd3.deb

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
