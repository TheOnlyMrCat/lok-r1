# clok - Lok compiler

[![Build Status](https://travis-ci.org/TheOnlyMrCat/clok.svg?branch=master)](https://travis-ci.org/TheOnlyMrCat/clok)
![Github build status](https://github.com/TheOnlyMrCat/clok/workflows/CI/badge.svg)
[![License](https://img.shields.io/github/license/TheOnlyMrCat/clok?color=yellow)](https://github.com/TheOnlyMrCat/clok/blob/master/LICENSE)

This is a compiler for the Lok programming language.

## Dependencies

* GNU Autotools (I don't know the version but you need the ability to run `autoreconf`)
* Boost >= 1.60.0 (for the filesystem and system modules)
* Flex (I again, don't know the version.)

## Building from source

To build this from the source, clone the repository and enter the following commands (at least, on a unix system):

```sh
./autogen.sh
./configure
make
```
