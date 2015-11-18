# ecc-lib-win32 - a Win32 port of ECC-LIB

## Introduction

This is a collection of the files that I modified in order to get ECC-LIB
(version 2.0) to compile on Windows 32.  Since ECC-LIB requires the GNU
Multiple Precision Arithmetic Library (GMP) you also need my Win32 port
of GMP, which can be found at the same place you downloaded this.  Also,
I have only tested it on a Windows XP machine with MS Visual Studio .NET
2003.

## Compilation and installation

To compile it just copy the files provided here to the distribution
directory of ECC-LIB 2.0 and overwrite the existing files there, then
simply enter the directory and type nmake.  If you don't use MS VS .NET
2003 you would have to change the related include line in the Makefile.
There is no installation required, just put all the files from the include
directory to your include path and ecc.lib to your library path.  When you
are using it in MSVC++ to compile new programs don't forget to define
_NTRG_WIN32_, include the relevant header files and link against ecc.lib
and gmp.lib.

In the directory bin/ you can find a pre-compiled version.

You can find ECC-LIB at:

http://www.ceid.upatras.gr/faculty/zaro/software/ecc-lib/

## Acknowledgements

Elisavet Konstantinou, Yannis Stamatiou and Christos Zaroliagis, for a
great library and for making their work available.
