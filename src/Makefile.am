#
# Makefile.am for qdialogsolver
#
SUBDIRS =

AM_CXXFLAGS = -DY2LOG=\"ui-qt\"					\
	-DQT_THREAD_SUPPORT					\
	-DLOCALEDIR=\"${localedir}\"				\
	-DQT_LOCALEDIR=\"$(QTDIR)/translations\"		\
	-DICONDIR=\"${themedir}/current\"			\
	-DTHEMEDIR=\"${themedir}/current\"			\
	-DHELPIMAGEDIR=\"${imagedir}\"				\
	-DQTLIBDIR=\"$(QTLIBDIR)\"				\
	-fmessage-length=0


MOC = $(QTDIR)/bin/moc


# plugin, libtool forces 'lib' prefix
plugin_LTLIBRARIES = libqdialogsolver.la
libqdialogsolver_la_LDFLAGS = -version-info 1:0

libqdialogsolver_la_LIBADD =			\
	-L$(libdir) 			\
	-L$(QTLIBDIR) -lqt-mt		\
	-lX11 -lXext -lSM -lICE


libqdialogsolver_la_SOURCES =			\
	drawparams.cpp				\
	pannerview.cpp				\
	resgraphview.cpp			\
	solvertree.cpp				\
	graphtreelabel.cpp			\
	QZyppSolverDialog.cpp			\
	restreewidget.cpp


BUILT_SOURCES =					\
	pannerview.moc				\
	QZyppSolverDialog.moc			\
	resgraphview.moc			\
	restreewidget.moc

pannerview.o pannerview.lo:			pannerview.moc
QZyppSolverDialog.o QZyppSolverDialog.lo:	QZyppSolverDialog.moc
resgraphview.o resgraphview.lo:			resgraphview.moc
restreewidget.o restreewidget.lo:		restreewidget.moc

pannerview.moc:					pannerview.h
QZyppSolverDialog.moc:				QZyppSolverDialog.h
resgraphview.moc:				resgraphview.h
restreewidget.moc:				restreewidget.h

%.moc: %.h
	$(MOC) $< -o $@

CLEANFILES = *.moc

INCLUDES =  -I$(QTDIR)/include
