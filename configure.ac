dnl Process this file with autoconf to produce a configure script.
dnl ==================================================
m4_include([VERSION])
AC_INIT(libqdialogsolver1,[QDIALOGSOLVER_VERSION])
AC_CONFIG_SRCDIR(src/Makefile.am)

AC_PREFIX_DEFAULT(/usr)
AC_CANONICAL_TARGET
AC_ARG_PROGRAM

AM_INIT_AUTOMAKE(foreign dist-bzip2 no-dist-gzip tar-ustar)
AM_CONFIG_HEADER(config.h)

dnl ==================================================
dnl Versioning
dnl ==========
dnl
dnl CURRENT    The most recent interface number that this library implements.
dnl
dnl REVISION   The implementation number of the current interface.
dnl
dnl AGE        The difference between the newest and oldest interfaces that this
dnl            library implements.
dnl            In other words, the library implements all the interface numbers
dnl            in the range from number 'CURRENT - AGE' to 'CURRENT'.
dnl
dnl 1) Start with version information of `0:0:0' for each libtool library.
dnl
dnl 2) Update the version information only immediately before a public release
dnl    of your software. More frequent updates are unnecessary, and only guarantee
dnl    that the current interface number gets larger faster.
dnl
dnl 3) If the library source code has changed at all since the last update,
dnl    then increment revision (`c:r:a' becomes `c:r+1:a').
dnl
dnl 4) If any interfaces have been added, removed, or changed since the last
dnl    update, increment current, and set revision to 0.
dnl
dnl 5) If any interfaces have been added since the last public release, then
dnl    increment age.
dnl
dnl 6) If any interfaces have been removed since the last public release, then
dnl    set age to 0.
dnl
dnl QDIALOGSOLVER_CURRENT=0
dnl QDIALOGSOLVER_REVISION=0
dnl QDIALOGSOLVER_AGE=0
dnl QDIALOGSOLVER_VERSION_INFO="-version-info $QDIALOGSOLVER_CURRENT:$QDIALOGSOLVER_REVISION:$QDIALOGSOLVER_AGE"

QDIALOGSOLVER_VERSION_INFO="-version-info 0:0:0"
AC_SUBST(QDIALOGSOLVER_VERSION_INFO)

dnl ==================================================

CXXFLAGS="${CXXFLAGS} -g -O2 -Wall -Wformat"
AC_SYS_LARGEFILE

dnl ==================================================

docdir=\${datadir}/doc/packages
pkgdocdir=\${docdir}/$PACKAGE

AC_SUBST(docdir)
AC_SUBST(pkgdocdir)

zyppdir=${prefix}/share/$PACKAGE
localedir=${prefix}/share/locale

AC_SUBST(localedir)

AH_TEMPLATE([LOCALEDIR],[gettext localedir])
AC_DEFINE_UNQUOTED([LOCALEDIR], ["${localedir}"])
AH_TEMPLATE([TEXTDOMAIN],[gettext textdomain])
AC_DEFINE_UNQUOTED([TEXTDOMAIN], ["$PACKAGE"])

dnl ==================================================
dnl checks for programs

dnl   check for c++ compiler and tools
AC_PROG_CXX
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AM_PROG_LIBTOOL

dnl   do AC_CHECK_* tests using c++ (e.g. boost header)
AC_LANG([C++])

AC_PATH_PROG(XGETTEXT, xgettext)
if test -z "$XGETTEXT" ; then
    AC_MSG_ERROR(xgettext is missing; please install gettext-devel.)
fi

dnl ==================================================
dnl checks for header files
AC_CHECK_HEADERS([boost/scoped_ptr.hpp],
        [],
        AC_MSG_ERROR([boost header not found. please install boost-devel]))

dnl ==================================================
dnl checks for libraries
dnl libzypp
PKG_CHECK_MODULES(ZYPP, libzypp, pkg_libzypp=yes, pkg_libzypp=no)
if test $pkg_libzypp = no; then
  echo
  AC_MSG_ERROR([Could not find libzypp library, please install libzypp-devel])
fi

AC_MSG_CHECKING(for zypp large file support)
ZYPP_FEATURES=`$PKG_CONFIG --variable=features libzypp 2>/dev/null`
ZYPP_LFS=no
for feature in $ZYPP_FEATURES ; do
	case $feature in
    		LFS) ZYPP_LFS=yes ;;
  	esac
done
enable_largefile=$ZYPP_LFS
AC_MSG_RESULT($enable_largefile)

test -n "$ZYPP_CFLAGS" && CXXFLAGS="$CXXFLAGS $ZYPP_CFLAGS"


dnl ==================================================
dnl internationalization macros
AM_GNU_GETTEXT

dnl ==================================================
dnl checks for typedefs
dnl ==================================================
dnl checks for structures
dnl ==================================================
dnl checks for compiler characteristics
dnl ==================================================
dnl checks for library functions
dnl ==================================================
dnl checks for system services
dnl ==================================================
dnl AC_CONFIG_FILES([FILE...])
AC_OUTPUT(Makefile			\
	libqdialogsolver1.spec	\
	src/Makefile
)
dnl ==================================================

echo "
Configuration:

  PACKAGE:              ${PACKAGE}
  VERSION:              ${VERSION}

  COMPILER:             ${CXX}
  CXXFLAGS:          	${CXXFLAGS}

  PREFIX:               ${prefix}
  LIBDIR:               ${libdir}

"
echo

