#
# Makefile.am for libzypp-qdialogsolver
#
SUBDIRS =

AM_CXXFLAGS = -DQT_THREAD_SUPPORT


MOC = $(QTDIR)/bin/moc


lib_LTLIBRARIES = libzypp-qdialogsolver.la
libzypp_qdialogsolver_la_LDFLAGS = -version-info 1:0

pkginclude_HEADERS = QZyppSolverDialog.h
noinst_HEADERS = pannerview.h 			\
	drawparams.h				\
	graphtree_defines.h			\
	graphtreelabel.h			\
	resgraphview.h				\
	restreewidget.h				\
	solvertree.h

pkgincludedir = $(includedir)/zypp

libzypp_qdialogsolver_la_SOURCES =			\
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

#<INSTALL-HEADER-TARGET>

# Automatically inserted by insert_install_target
# DO NOT REMOVE THE TAGS!

# Install header files only if different to version already installed -
# prevent unnecessary installations to keep make dependency changes
# and thus rebuilds for dependent packages as low as possible.

install-pkgincludeHEADERS: $(pkginclude_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(pkgincludedir)
	@list='$(pkginclude_HEADERS)';							\
	for p in $$list; do								\
		if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; 			\
		if ! cmp -s $$d$$p $(DESTDIR)$(pkgincludedir)/$$p; then			\
			echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p";	\
			$(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p;		\
	  	fi									\
	done

#</INSTALL-HEADER-TARGET>


