LIBS = -lm -lfftw3
CCC = g++ -O3 -Wall -Wno-deprecated -DLINUX_C
OBJ_FILES = Utils_1.o Utils_2.o LDPC_1.o LDPC_2.o Encoding.o Main.o
H_FILES = Utils_1.h Utils_2.h LDPC_1.h LDPC_2.h Encoding.h Portability.h

all: q_LDPC

Utils_1.o: Utils_1.cpp ${H_FILES}
	${CCC} -c Utils_1.cpp -o Utils_1.o
Utils_2.o: Utils_2.cpp ${H_FILES}
	${CCC} -c Utils_2.cpp -o Utils_2.o
LDPC_1.o: LDPC_1.cpp ${H_FILES}
	${CCC} -c LDPC_1.cpp -o LDPC_1.o
LDPC_2.o: LDPC_2.cpp ${H_FILES}
	${CCC} -c LDPC_2.cpp -o LDPC_2.o
Encoding.o: Encoding.cpp ${H_FILES}
	${CCC} -c Encoding.cpp -o Encoding.o
Main.o: Main.cpp ${H_FILES}
	${CCC} -c Main.cpp -o Main.o
q_LDPC: ${OBJ_FILES}
	${CCC} ${OBJ_FILES} ${LIBS} -o q_LDPC

doxygen:
	doxygen conf.dox

clean:
	rm -rf *o q_LDPC doxygen
