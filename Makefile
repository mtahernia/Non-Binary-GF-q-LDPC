# -Wextra 
LIBS = -lm -lfftw3 
# -static -static-libgcc -static-libstdc++
CCC = g++ -O3 -Wall -DLINUX_C

OBJ_FILES_TOOLS = GFq.o Matrix.o Report.o 
OBJ_FILES_CHANNEL = Channel.o Mapping.o BSC_Channel.o AWGN_Channel.o PNC_Channel.o
OBJ_FILES_GRAPH = Message.o Node.o Variable_Node.o Check_Node.o Edge.o Bipartite_Graph.o
OBJ_FILES = ${OBJ_FILES_TOOLS} ${OBJ_FILES_CHANNEL} ${OBJ_FILES_GRAPH} LDPC.o Encoding.o Main.o

H_FILES_TOOLS = Portability.h Report.h Definitions.h GFq.h Matrix.h Functions.h Utils_2.h
H_FILES_CHANNEL = Channel.h BSC_Channel.h AWGN_Channel.h PNC_Channel.h Mapping.h
H_FILES_GRAPH = Message.h Node.h Variable_Node.h Check_Node.h Edge.h Bipartite_Graph.h
H_FILES = ${H_FILES_TOOLS} ${H_FILES_CHANNEL} ${H_FILES_GRAPH} LDPC.h Encoding.h   

all: q_LDPC

Report.o: Report.cpp ${H_FILES}
	${CCC} -c Report.cpp -o Report.o
GFq.o: GFq.cpp ${H_FILES}
	${CCC} -c GFq.cpp -o GFq.o
Matrix.o: Matrix.cpp ${H_FILES}
	${CCC} -c Matrix.cpp -o Matrix.o
Channel.o: Channel.cpp ${H_FILES}
	${CCC} -c Channel.cpp -o Channel.o
Mapping.o: Mapping.cpp ${H_FILES}
	${CCC} -c Mapping.cpp -o Mapping.o
BSC_Channel.o: BSC_Channel.cpp ${H_FILES}
	${CCC} -c BSC_Channel.cpp -o BSC_Channel.o
AWGN_Channel.o: AWGN_Channel.cpp ${H_FILES}
	${CCC} -c AWGN_Channel.cpp -o AWGN_Channel.o
PNC_Channel.o: PNC_Channel.cpp ${H_FILES}
	${CCC} -c PNC_Channel.cpp -o PNC_Channel.o
Message.o: Message.cpp ${H_FILES}
	${CCC} -c Message.cpp -o Message.o
Node.o: Node.cpp ${H_FILES}
	${CCC} -c Node.cpp -o Node.o
Variable_Node.o: Variable_Node.cpp ${H_FILES}
	${CCC} -c Variable_Node.cpp -o Variable_Node.o
Check_Node.o: Check_Node.cpp ${H_FILES}
	${CCC} -c Check_Node.cpp -o Check_Node.o
Edge.o: Edge.cpp ${H_FILES}
	${CCC} -c Edge.cpp -o Edge.o
Bipartite_Graph.o: Bipartite_Graph.cpp ${H_FILES}
	${CCC} -c Bipartite_Graph.cpp -o Bipartite_Graph.o
Encoding.o: Encoding.cpp ${H_FILES}
	${CCC} -c Encoding.cpp -o Encoding.o
LDPC.o  : LDPC.cpp ${H_FILES}
	${CCC} -c LDPC.cpp -o LDPC.o
Main.o: Main.cpp ${H_FILES}
	${CCC} -c Main.cpp -o Main.o
q_LDPC: ${OBJ_FILES}
	${CCC} ${OBJ_FILES} ${LIBS} -o q_LDPC

doxygen:
	doxygen conf.doxyfile
latex: doxygen
	cd doxygen/latex;make
clean:
	rm -rf *o q_LDPC doxygen
