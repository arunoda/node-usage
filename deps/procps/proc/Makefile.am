PACKAGE_VERSION = @PACKAGE_VERSION@
AM_CPPFLAGS = \
	-include $(top_builddir)/config.h \
	-I$(top_srcdir)/include \
	-DLOCALEDIR=\"$(localedir)\"

#
# See http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
LIBprocps_CURRENT=0
LIBprocps_REVISION=1
LIBprocps_AGE=0

lib_LTLIBRARIES = libprocps.la

libprocps_ladir = $(ladir)
libprocps_la_LIBADD = $(LIB_KPARTS)
libprocps_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	$(all_libraries) \
	-version-info $(LIBprocps_CURRENT):$(LIBprocps_REVISION):$(LIBprocps_AGE) \
	-no-undefined \
	-Wl,--version-script=$(top_srcdir)/proc/libprocps.sym

EXTRA_DIST = libprocps.sym

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libprocps.pc

libprocps_la_SOURCES = \
	alloc.c \
	alloc.h \
	devname.c \
	devname.h \
	escape.c \
	escape.h \
	ksym.c \
	procps.h \
	pwcache.c \
	pwcache.h \
	readproc.c \
	readproc.h \
	sig.c \
	sig.h \
	slab.c \
	slab.h \
	sysinfo.c \
	sysinfo.h \
	version.c \
	version.h \
	wchan.h \
	whattime.c \
	whattime.h

libprocps_la_includedir = $(includedir)/proc/
libprocps_la_include_HEADERS =  \
	alloc.h \
	devname.h \
	escape.h \
	procps.h \
	pwcache.h \
	readproc.h \
	sig.h \
	slab.h \
	sysinfo.h \
	version.h \
	wchan.h \
	whattime.h
