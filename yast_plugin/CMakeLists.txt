################################################################
# Solver Dialog
################################################################

#CMake Warning (dev) at yast_plugin/CMakeLists.txt:5 (ADD_DEFINITIONS):
# Policy CMP0005 is not set: Preprocessor definition values are now escaped
#  automatically.  Run "cmake --help-policy CMP0005" for policy details.  Use
#  the cmake_policy command to set the policy and suppress this warning.
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)
ADD_DEFINITIONS(
		-DY2LOG=\\\"QT4\\\" ${QT_DEFINITIONS}
		-DICONDIR=\\"${Y2QT_ICONDIR}\\"
		-DLOCALEDIR=\\\"${Y2QT_LOCALEDIR}\\\"
		-DTHEMEDIR=\\\"${Y2QT_THEMEDIR}\\\"
		-DQTLIBDIR=\\\"${LIB_INSTALL_DIR}\\\"
		-DPREFIX=\\\"${CMAKE_INSTALL_PREFIX}\\\"
		)
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-nonliteral -fmessage-length=0" )

SET(qt_solver_dialog_plugin_SRCS

YQZyppSolverDialogPluginImpl.cc
    )

QT4_AUTOMOC(${qt_solver_dialog_plugin_SRCS})

INCLUDE_DIRECTORIES( ${QT_INCLUDES} )
INCLUDE_DIRECTORIES( ${CMAKE_INSTALL_PREFIX}/include/YaST2/yui )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
INCLUDE_DIRECTORIES( ${CMAKE_INSTALL_PREFIX}/include/YaST2/yast2-qt )
INCLUDE_DIRECTORIES( ${ZYPP_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ../src )

# CMake Warning (dev) at yast_plugin/CMakeLists.txt:30 (ADD_LIBRARY):
#   Policy CMP0003 should be set before this line.  Add code such as
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
ADD_LIBRARY( py2qt_zypp_solver_dialog SHARED ${qt_solver_dialog_plugin_SRCS} )
TARGET_LINK_LIBRARIES( py2qt_zypp_solver_dialog yui)
TARGET_LINK_LIBRARIES( py2qt_zypp_solver_dialog ${QT_QTCORE_LIBRARY} )
TARGET_LINK_LIBRARIES( py2qt_zypp_solver_dialog ${QT_QTGUI_LIBRARY} )
TARGET_LINK_LIBRARIES( py2qt_zypp_solver_dialog qdialogsolver )
SET_TARGET_PROPERTIES( py2qt_zypp_solver_dialog PROPERTIES SOVERSION 2 )
SET_TARGET_PROPERTIES( py2qt_zypp_solver_dialog PROPERTIES LINK_FLAGS "-Wl,--no-undefined -Wl,-rpath -Wl,${CMAKE_INSTALL_PREFIX}/lib/YaST2/plugin" )

INSTALL(TARGETS py2qt_zypp_solver_dialog LIBRARY DESTINATION "${LIB_INSTALL_DIR}/YaST2/plugin" )


