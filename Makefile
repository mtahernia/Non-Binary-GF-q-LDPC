LIBS = -lm -lfftw3 -static -static-libgcc -static-libstdc++
CCC = g++ -O3 -Wall -Wno-deprecated -DLINUX_C
OBJ_FILES = GFq.o Matrix.o Report.o LDPC_1.o LDPC_2.o Encoding.o Main.o
H_FILES = Functions.h Utils_2.h LDPC_1.h LDPC_2.h Encoding.h Portability.h Channel.h Report.h Definitions.h GFq.h Matrix.h

all: q_LDPC

Report.o: Report.cpp ${H_FILES}
	${CCC} -c Report.cpp -o Report.o
GFq.o: GFq.cpp ${H_FILES}
	${CCC} -c GFq.cpp -o GFq.o
Matrix.o: Matrix.cpp ${H_FILES}
	${CCC} -c Matrix.cpp -o Matrix.o
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
latex: doxygen
	cd doxygen/latex;make
clean:
	rm -rf *o q_LDPC doxygen
