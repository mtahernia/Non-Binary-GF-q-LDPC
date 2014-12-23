# -Wextra 
LIBS = -lm -lfftw3 -static -static-libgcc -static-libstdc++
CCC = g++ -O3 -Wall -DLINUX_C
OBJ_FILES = GFq.o Matrix.o Report.o LDPC.o LDPC_2.o Channel.o Mapping.o Encoding.o BSC_Channel.o AWGN_Channel.o PNC_Channel.o Message.o Node.o Variable_Node.o Main.o
H_FILES = Functions.h Utils_2.h LDPC.h LDPC_2.h Encoding.h Portability.h Report.h Definitions.h GFq.h Matrix.h Channel.h BSC_Channel.h AWGN_Channel.h PNC_Channel.h Mapping.h Message.h Node.h Variable_Node.h 

all: q_LDPC

Report.o: Report.cpp ${H_FILES}
	${CCC} -c Report.cpp -o Report.o
GFq.o: GFq.cpp ${H_FILES}
	${CCC} -c GFq.cpp -o GFq.o
Matrix.o: Matrix.cpp ${H_FILES}
	${CCC} -c Matrix.cpp -o Matrix.o
LDPC.o  : LDPC.cpp ${H_FILES}
	${CCC} -c LDPC.cpp -o LDPC.o
LDPC_2.o: LDPC_2.cpp ${H_FILES}
	${CCC} -c LDPC_2.cpp -o LDPC_2.o
Channel.o: Channel.cpp ${H_FILES}
	${CCC} -c Channel.cpp -o Channel.o
BSC_Channel.o: BSC_Channel.cpp ${H_FILES}
	${CCC} -c BSC_Channel.cpp -o BSC_Channel.o
AWGN_Channel.o: AWGN_Channel.cpp ${H_FILES}
	${CCC} -c AWGN_Channel.cpp -o AWGN_Channel.o
PNC_Channel.o: PNC_Channel.cpp ${H_FILES}
	${CCC} -c PNC_Channel.cpp -o PNC_Channel.o
Mapping.o: Mapping.cpp ${H_FILES}
	${CCC} -c Mapping.cpp -o Mapping.o
Message.o: Message.cpp ${H_FILES}
	${CCC} -c Message.cpp -o Message.o
Node.o: Node.cpp ${H_FILES}
	${CCC} -c Node.cpp -o Node.o
Variable_Node.o: Variable_Node.cpp ${H_FILES}
	${CCC} -c Variable_Node.cpp -o Variable_Node.o
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
