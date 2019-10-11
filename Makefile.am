AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = clok
clok_SOURCES = src/main.cpp src/parse.cpp src/preasm.cpp src/headergen.cpp src/tokens.l src/nodes.cpp

EXTRA_DIST = src/cxxopts.hpp src/clok.hpp src/token.hpp src/astnode.hpp src/yydef.hpp src/tokens.h src/asmnodes.hpp

AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

clok_LDADD = $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)