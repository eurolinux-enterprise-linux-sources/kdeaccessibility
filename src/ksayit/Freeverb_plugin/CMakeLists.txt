
include_directories( ${CMAKE_SOURCE_DIR}/ksayit/src  )


########### install files ###############

install( FILES   DESTINATION  ${DATA_INSTALL_DIR}/ksayit/fxplugins )




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/ksayit/src $(all_includes)
#METASOURCES = AUTO
#fxpluginsdir = $(kde_datadir)/ksayit/fxplugins
#fxplugins_LTLIBRARIES = libFreeverb_plugin.la
#libFreeverb_plugin_la_LDFLAGS = -avoid-version -module -no-undefined \
#	$(all_libraries)
#libFreeverb_plugin_la_LIBADD = $(LIB_KDECORE) $(LIB_KDEUI) -lartskde\
#	-lartsmodules
#noinst_HEADERS = freeverbsetupimpl.h ksayitfreeverblib.h kdeexportfix.h
#libFreeverb_plugin_la_SOURCES = freeverbsetupimpl.cpp freeverb_setup.ui \
#	ksayitfreeverblib.cpp
