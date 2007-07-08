AC_INIT(brick, 0.1)
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE

AC_CONFIG_HEADER(config.h)
AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_INTLTOOL([0.23])
AC_HEADER_STDC

BRICK_CFLAGS="$CFLAGS -O2 -Wall -Werror -ansi -pedantic"

#Check for GLib
PKG_CHECK_MODULES(GLIB, glib-2.0)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

#Check for GTK+
PKG_CHECK_MODULES(GTK, gtk+-2.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

#setup gettext
GETTEXT_PACKAGE=brick
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Gettext Domain Name])
ALL_LINGUAS="en_GB"
AM_GLIB_GNU_GETTEXT
localedir=/usr/share/locale
AC_SUBST(localedir)
AC_DEFINE_UNQUOTED(LOCALEDIR, "$localedir", [Local Directory])

#Check if Hildon (Maemo) support has been disabled
USE_HILDON="yes"
AC_ARG_ENABLE(hildon,
[  --disable-hildon Disable support for Hildon (Maemo)],
[USE_HILDON="$enableval"])

#Check for hildon libraries
if test "$USE_HILDON" == "yes"; then
        HILDON_MODULES="hildon-libs >= 0.12.0"
        PKG_CHECK_MODULES(HILDON, $HILDON_MODULES, HAVE_HILDON=yes, HAVE_HILDON=no)
	AC_SUBST(HILDON_CFLAGS)
	AC_SUBST(HILDON_LIBS)

	#get desktopentrydir for maemo install
	desktopentrydir=`$PKG_CONFIG osso-af-settings --variable=desktopentrydir`
	AC_SUBST(desktopentrydir)

fi
AM_CONDITIONAL(HAVE_HILDON, test "$HAVE_HILDON" = "yes")

if test "$HAVE_HILDON" == "yes"; then
        AC_DEFINE_UNQUOTED(HAVE_HILDON, 1, [did we find Hildon])
fi

#check if sqlite3 support is desired
USE_SQLITE3="yes"
AC_ARG_ENABLE(sqlite3,
[  --disable-sqlite3 Disable support for SQLite3],
[USE_SQLITE3="$enableval"])

#check for sqlite3
if test "$USE_SQLITE3" == "yes"; then
        SQLITE3_MODULES="sqlite3 >= 3.0"
        PKG_CHECK_MODULES(SQLITE3, $SQLITE3_MODULES, HAVE_SQLITE3=yes, HAVE_SQLITE=no)
fi
AM_CONDITIONAL(HAVE_SQLITE3, test "$HAVE_SQLITE3" = "yes")

#AC_DEFINE(PACKAGE_NAME, brick)

# Produce output files
AC_OUTPUT(
	  Makefile \
	  src/Makefile \
	  data/Makefile \
	  po/POTFILES \
	  po/Makefile.in
)
