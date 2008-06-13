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


