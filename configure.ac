AC_INIT(Makefile.am)
AM_INIT_AUTOMAKE(brick, `date +%Y%m%d`)

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_PROG_INTLTOOL([0.23])
AC_HEADER_STDC

CFLAGS="$CFLAGS -O2 -Wall -Werror -ansi -pedantic"

# Hildon library dependsncies
PKG_CHECK_MODULES(HILDON, hildon-libs >= 0.9.50)
AC_SUBST(HILDON_LIBS)
AC_SUBST(HILDON_CFLAGS)

# To make application visible in maemo Task Navigator it needs a Desktop
# file for the application.
desktopentrydir=`$PKG_CONFIG osso-af-settings --variable=desktopentrydir`

# Define as variables in Makefiles
AC_SUBST(desktopentrydir)

# Produce output files
AC_OUTPUT(
	  Makefile \
	  src/Makefile \
	  data/Makefile
)
