# keep preprocessor defines escaped
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)
  

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

ADD_DEFINITIONS(
		${QT_DEFINITIONS}
		-DQT3_SUPPORT
		-DQT_LOCALEDIR=\\\"${CMAKE_INSTALL_PREFIX}/share/qt4/translations\\\"
		-DQTLIBDIR=\\\"${LIB_INSTALL_DIR}\\\"
		-DLOCALEDIR=\\\"${CMAKE_INSTALL_PREFIX}/share/locale\\\" -DTEXTDOMAIN=\\\"dialogsolver\\\"
		)

SET( libqdialogsolver_SRCS

     drawparams.cpp
     graphtreelabel.cpp
     pannerview.cpp
     QZyppSolverDialog.cpp
     resgraphview.cpp
     restreewidget.cpp
     solvertree.cpp
     getText.cc
     )

SET( libqdialogsolver_HEADERS
     QZyppSolverDialog.h
)

INSTALL(  FILES ${libqdialogsolver_HEADERS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/zypp" )

QT4_AUTOMOC(${libqdialogsolver_SRCS})

INCLUDE_DIRECTORIES( ${QT_INCLUDES} )

ADD_LIBRARY( qdialogsolver SHARED ${libqdialogsolver_SRCS})
TARGET_LINK_LIBRARIES( qdialogsolver ${QT_QTCORE_LIBRARY} )
TARGET_LINK_LIBRARIES( qdialogsolver ${QT_QTGUI_LIBRARY} )
TARGET_LINK_LIBRARIES( qdialogsolver ${QT_QT3SUPPORT_LIBRARY} )
TARGET_LINK_LIBRARIES( qdialogsolver ${ZYPP_LIBRARY} )
SET_TARGET_PROPERTIES( qdialogsolver PROPERTIES SOVERSION 1 )
SET_TARGET_PROPERTIES( qdialogsolver PROPERTIES LINK_FLAGS "-Wl,--no-undefined" )

INSTALL(TARGETS qdialogsolver LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" )

