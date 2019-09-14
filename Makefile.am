AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = clok
clok_SOURCES = src/main.cpp src/parse.cpp src/tokens.l

EXTRA_DIST = src/cxxopts.hpp src/clok.hpp src/token.hpp src/trees.hpp src/yydef.hpp src/tokens.h

AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

clok_LDADD = $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)