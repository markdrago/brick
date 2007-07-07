#!/bin/sh

printerr() {
    echo "$@" >&2
}

glib-gettextize --copy --force
libtoolize --automake --copy --force
intltoolize --automake --copy --force
aclocal-1.7
autoconf --force
autoheader --force
automake-1.7 --add-missing --copy --force-missing --foreign

if [ "$#" = 0 ]; then
  printerr "**Warning**: I am going to run \`configure' with no arguments."
  printerr "If you wish to pass any to it, please specify them on the"
  printerr \`$0\'" command line."
  printerr
fi

./configure "$@" && echo Now type \`make\' to compile brick || exit 1