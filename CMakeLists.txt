# cmake 2.6 requires this
cmake_minimum_required(VERSION 2.4)

PROJECT(libqdialogsolver1)
SET( PACKAGE libqdialogsolver1)
SET( CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/cmake/Modules" ${CMAKE_MODULE_PATH} )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Woverloaded-virtual -Wno-deprecated" )
SET( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -g -Wall" )
set( CMAKE_CXX_FLAGS_RELEASE   "${CMAKE_CXX_FLAGS_RELEASE} -DQT_NO_DEBUG -DNDEBUG")

SET( RPMNAME "libqdialogsolver1" )

FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(Zypp REQUIRED)

ADD_SUBDIRECTORY(src)

INCLUDE( ${CMAKE_SOURCE_DIR}/VERSION.cmake )
SET( VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )

SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${VERISON_PATCH})
SET( CPACK_GENERATOR "TBZ2")
SET( CPACK_SOURCE_GENERATOR "TBZ2")
SET( CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE}-${VERSION}" )

# The following components are regex's to match anywhere (unless anchored)
# in absolute path + filename to find files or directories to be excluded
# from source tarball.
SET (CPACK_SOURCE_IGNORE_FILES
#svn files
"\\\\.svn/"
"\\\\.cvsignore$"
# temporary files
"\\\\.swp$"
# backup files
"~$"
# eclipse files
"\\\\.cdtproject$"
"\\\\.cproject$"
"\\\\.project$"
"\\\\.settings/"
# others
"\\\\.#"
"/#"
"/build/"
"/_build/"
"/\\\\.git/"
# used before
"/CVS/"
"/\\\\.libs/"
"/\\\\.deps/"
"\\\\.o$"
"\\\\.lo$"
"\\\\.la$"
"Makefile\\\\.in$"
)

INCLUDE(CPack)

####################################################################
# RPM SPEC                                                         #
####################################################################

MACRO(SPECFILE)
  MESSAGE(STATUS "Writing spec file...")
  CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/package/libqdialogsolver1.spec.in ${CMAKE_BINARY_DIR}/package/libqdialogsolver1.spec @ONLY)
ENDMACRO(SPECFILE)

SPECFILE()

ADD_CUSTOM_TARGET( svncheck
  COMMAND cd $(CMAKE_SOURCE_DIR) && ! LC_ALL=C svn status --show-updates --quiet | grep -v '^Status against revision'
)

SET( AUTOBUILD_COMMAND
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/package/*.tar.bz2
  COMMAND ${CMAKE_MAKE_PROGRAM} package_source
  COMMAND ${CMAKE_COMMAND} -E copy ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.bz2 ${CMAKE_BINARY_DIR}/package
  COMMAND ${CMAKE_COMMAND} -E remove ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.bz2
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/package/libqdialogsolver1.changes" "${CMAKE_BINARY_DIR}/package/libqdialogsolver1.changes"
)

ADD_CUSTOM_TARGET( srcpackage_local
  ${AUTOBUILD_COMMAND}
)

ADD_CUSTOM_TARGET( srcpackage
  COMMAND ${CMAKE_MAKE_PROGRAM} svncheck
  ${AUTOBUILD_COMMAND}
)



