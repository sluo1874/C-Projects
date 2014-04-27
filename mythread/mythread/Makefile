# Makefile for UD CISC user-level thread library

CC = /opt/gcc/bin/gcc 

CFLAGS = -g

LIBOBJS = t_lib.o 

TSTOBJS = test00.o T1.o T1x.o T1a.o T2.o T4.o T7.o 

# specify the executable 

EXECS = test00 T1 T1x T1a T2 T4 T7

# specify the source files

LIBSRCS = t_lib.c

TSTSRCS = test00.c

# ar creates the static thread library

t_lib.a: ${LIBOBJS} Makefile
	ar rcs t_lib.a ${LIBOBJS}

# here, we specify how each file should be compiled, what
# files they depend on, etc.

t_lib.o: t_lib.c t_lib.h Makefile
	${CC} ${CFLAGS} -c t_lib.c

test00.o: test00.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c test00.c

test00: test00.o t_lib.a Makefile
	${CC} ${CFLAGS} test00.o t_lib.a -o test00

T1.o: T1.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c T1.c

T1: T1.o t_lib.a Makefile
	${CC} ${CFLAGS} T1.o t_lib.a -o T1

T1x.o: T1x.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c T1x.c

T1x: T1x.o t_lib.a Makefile
	${CC} ${CFLAGS} T1x.o t_lib.a -o T1x

T1a.o: T1a.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c T1a.c

T1a: T1a.o t_lib.a Makefile
	${CC} ${CFLAGS} T1a.o t_lib.a -o T1a

T2.o: T2.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c T2.c

T2: T2.o t_lib.a Makefile
	${CC} ${CFLAGS} T2.o t_lib.a -o T2

T4.o: T4.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c T4.c

T4: T4.o t_lib.a Makefile
	${CC} ${CFLAGS} T4.o t_lib.a -o T4

T7.o: T7.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c T7.c

T7: T7.o t_lib.a Makefile
	${CC} ${CFLAGS} T7.o t_lib.a -o T7
clean:
	rm -f t_lib.a ${EXECS} ${LIBOBJS} ${TSTOBJS} 

