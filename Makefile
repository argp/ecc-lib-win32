# NTRG ECC-LIB WIN32 -- Makefile
# Patroklos G. Argyroudis <argp@cs.tcd.ie>
#
# Makefile for compilation of ECC-LIB on Win32.
#
# $Id: Makefile,v 1.1.1.1 2004/01/11 10:24:10 argp Exp $

CC = cl
LD = link
CFLAGS = -nologo -D_NTRG_WIN32_ -DHAS_STRING_H -D__STDC__
LFLAGS = /incremental:no /nologo /subsystem:console /machine:X86 \
	    -libpath:"C:\gmp\lib" -libpath:"."
LIBS = gmp.lib ecc.lib
INCLUDE = -I"C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\Include" \
	    -I"C:\gmp\include" -I"." -I".." -I"include"
LIBCMD = lib /nologo

SRCS = lib\cm.c \
       lib\cm_prime_order.c \
       lib\complex_arithmetic.c \
       lib\create_e_and_pi.c \
       lib\ec_operations.c \
       lib\float_point_arithmetic.c \
       lib\hilbert.c \
       lib\int_arithmetic.c \
       lib\poly_arithmetic.c \
       lib\protocols.c \
       lib\rmd160.c \
       lib\weber.c

OBJS = $(SRCS:.c=.obj)
 
ECC_LIB = ecc.lib

.c.obj:
	$(CC) -c $(CFLAGS) $(INCLUDE) -Fo$@ $<

all:	$(ECC_LIB) ecdsa

$(ECC_LIB): $(OBJS)
	    $(LIBCMD) /OUT:$(ECC_LIB) $(OBJS)

ecdsa.obj:  ecdsa\ecdsa.c
	    $(CC) -c $(CFLAGS) $(INCLUDE) ecdsa\ecdsa.c

ecdsa:	ecdsa.obj
	$(LD) $(LFLAGS) -out:ecdsa.exe ecdsa.obj $(LIBS)

clean:
	del lib\*.obj
	del $(ECC_LIB)
	del ecdsa.obj
	del ecdsa.exe
	del signature
 
# EOF
