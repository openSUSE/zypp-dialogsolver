## Process this file with automake to produce Makefile.in
## ##################################################

SUBDIRS = src

## ##################################################

#ACLOCAL_AMFLAGS = -I m4
ACLOCAL_AMFLAGS =

EXTRA_DIST = mkinstalldirs
#config.rpath

## ##################################################

.PHONY:	package package-local check-up-to-date

package:        check-up-to-date package-local

package-local:  dist-bzip2 qdialogsolver.spec
	@if [ `bunzip2 -c $(distdir).tar.bz2 | wc --bytes` = 0 ] ; then \
		echo "Error: $(distdir).tar.bz2 is empty" ; \
		exit 1 ; \
	fi
	rm -f package/*.tar.bz2
	rm -f package/*~
	rm -f package/*.bak
	rm -f package/*.auto
	mv $(distdir).tar.bz2 package/
	mv qdialogsolver.spec package/

check-up-to-date:
	cd $(srcdir) && ! LC_ALL=C svn status --show-updates --quiet \
	| grep -v '^Status against revision'
