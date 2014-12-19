LIBS = -lm -lfftw3 -static -static-libgcc -static-libstdc++
CCC = g++ -O3 -Wall -Wno-deprecated -DLINUX_C
OBJ_FILES = Report.o Utils_1.o Utils_2.o LDPC_1.o LDPC_2.o Encoding.o Main.o
H_FILES = Utils_1.h Utils_2.h LDPC_1.h LDPC_2.h Encoding.h Portability.h Channel.h Report.h Definitions.h

all: q_LDPC

Report.o: Report.cpp ${H_FILES}
	${CCC} -c Report.cpp -o Report.o
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
latex:
	make doxygen
	cd doxygen/latex;make
clean:
	rm -rf *o q_LDPC doxygen
