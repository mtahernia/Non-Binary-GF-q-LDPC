#ifndef LDPC_2
#define LDPC_2

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include "Utils_1.h"
#include "Utils_2.h"
#include "Encoding.h"
#include <fftw3.h>


typedef unsigned char BOOLEAN;
typedef unsigned char BYTE;

#define TRUE   1
#define FALSE  0

#define MAX_DEGREE	1000

/****************************************************************************
 *
 * Channel
 *
 ****************************************************************************/

class LDPC_Code;
class mapping;

// Channel types
//! Channel Class
/**
 * General Channel Class
 * More specific channel classes will be defined as children of this classes
 */
class channel {
public:
	virtual char *GetChannelName() = 0;

	// General functions ------------------------------------------
	GFq &MaxProbableForOutput(mapping &MapInUse); // FIXME: Not used or defined in this program

	double ErrorUndefined(char *FuncName) {
		cout << GetChannelName() << "::" << FuncName << " undefined.\n";
		exit(1);
	}
	virtual void PrintChannelData(LDPC_Code &Code) = 0; /*!<This is a pure virtual function, meaning the child classes should define it or it will retun zero if not defined */
	virtual void ProcessMapping(LDPC_Code &Code) = 0;

	// Channel functions-------------------------------------------
	void SimulateOutputVector(vector &InVector, vector &OutVector);

	virtual double SimulateOutput(double ChannelInput) = 0;
	virtual double CalcProbForInput(double ChannelOutput, double ChannelInput) = 0;

	// Statistical data -------------------------------------------
	virtual double CapacityInBits() = 0;
//	virtual ~channel(void) {
//	} // Virtual destructor
};

/****************************************************************************
 *
 * BSC Channel
 *
 ****************************************************************************/

/**
 * BSC Channel, which is a child of Channel.
 * It is only useful for binary channels
 */
class BSC_Channel: public channel {
private:
	double channel_p;			///< Bit flip probability for BSC
	double source_p;				///< Input source
	double InterferenceProb; ///< Dirty paper, I guess this value is unused here

public:
	// General functions ------------------------------------------

	BSC_Channel() :
			channel_p(0), InterferenceProb(0.5) {
	}
	void SetChannel_p(double p_channel_p) {
		channel_p = p_channel_p;
	}
	int Flip(double prob) {
		return (my_rand() < prob);
	}
	virtual char *GetChannelName() {
		return "BSC_Channel";
	}

	virtual void PrintChannelData(LDPC_Code &Code);
	virtual void ProcessMapping(LDPC_Code &Code);

	// Channel coding functions -----------------------------------
	virtual double SimulateOutput(double ChannelInput) {
		return (double) (int(ChannelInput) ^ Flip(channel_p));
	}
	virtual double CalcProbForInput(double ChannelOutput, double ChannelInput) {
		if (ChannelOutput == ChannelInput)
			return 1. - channel_p;
		else
			return channel_p;
	}

	// Statistical functions -------------------------------------
	virtual double CapacityInBits();
	virtual double CalcProbForOutput(double ChannelOutput) {
		if (ChannelOutput == 1)
			return source_p;
		else if (ChannelOutput == 0)
			return 1 - source_p;
		else {
			cout << "BSC_Channel.CalcProbForOutput: Invalid parameter\n";
			exit(1);
		}
	}
};

/****************************************************************************
 *
 * AWGN Channel
 *
 ****************************************************************************/

double GaussGenerate(double sigma); // Generates a real gaussian

class AWGN_Channel: public channel {
private:
	double source_sigma;
	double noise_sigma;
public:
	// Specialized functions ----------------------------
	AWGN_Channel(double p_noise_sigma = -1) :
			noise_sigma(p_noise_sigma) {
	}

	double NoiseVariance(); // Returns noise_sigma^2
	double NoiseStdDev();	// Returns noise_sigma

	void SetNoiseSigma(double p_noise_sigma) {
		noise_sigma = p_noise_sigma;
	}

	// General functions --------------------------------
	virtual char *GetChannelName() {
		return "AWGN_Channel";
	}
	virtual void PrintChannelData(LDPC_Code &Code);
	virtual void ProcessMapping(LDPC_Code &Code);

	// Channel coding functions -------------------------
	virtual double SimulateOutput(double ChannelInput);
	virtual double CalcProbForInput(double ChannelOutput, double ChannelInput);

	// Statistical data ---------------------------------
	virtual double CapacityInBits();

};

/***************************************************************************
 *
 * mapping
 *
 ***************************************************************************/

class mapping {
public:
	int q;
	double vals[MAX_Q];
public:
	mapping(int p_q = -1, double *p_vals = NULL) {
		if (p_q != -1)
			Set_Q(p_q);

		if (p_vals != NULL)
			bcopy( /*from*/p_vals, /*to*/ vals, q * sizeof(double));
	}

	void Set_Q(int p_q) {
		if (p_q > MAX_Q) {
			cout << "Exceeded MAX_Q in mapping (simply increase)\n";
			exit(1);
		}

		q = p_q;
	}
	// Copy mapping from p_MapInUse
	mapping(mapping &p_MapInUse) :
			q(p_MapInUse.q) {
		bcopy( /*from*/p_MapInUse.vals, /*to*/vals, q * sizeof(double));
	}

	// Determine whether the mapping is a binary mapping or not!
	BOOLEAN IsBinary() {
		if (q != 2)
			return FALSE;
		if (((vals[0] != 0) && (vals[0] != 1)) || ((vals[1] != 0) && (vals[1] != 1)))
			return FALSE;
		return TRUE;
	}

	// Read mapping from input file
	void GetFromFile(std::ifstream &file);

	// Average signal power assuming all constellation points are equiprobable
	double Average_E() {
		double sum_E = 0;
		for (int i = 0; i < q; i++)
			sum_E += pow(vals[i], 2);

		return sum_E / (double) q;
	}

	void Normalize() {
		// Normalize average energy to 1

		double factor = sqrt(Average_E());

		for (int i = 0; i < q; i++)
			vals[i] /= factor;
	}

	// Operator overlad for division of a mapping
	void operator/=(double d) {
		for (int i = 0; i < q; i++)
			vals[i] /= d;
	}

	// Operator overload for multiplication of a mapping
	void operator*=(double d) {
		for (int i = 0; i < q; i++)
			vals[i] *= d;
	}

	// Returns the mapping point for a integer x
	double map(int x) {
		return vals[x];
	}

	// Returns the mapping point for g in GF(q)
	double map(GFq &g) {
		return map(g.val);
	}

	// Return q of mapping
	int GetQ() {
		return q;
	}
};


inline std::ostream &operator<<(std::ostream &s, mapping &MapInUse) { // for writing MapInUse to s
	s << "[";
	for (int i = 0; i < MapInUse.GetQ(); i++) {
		if (i != 0)
			s << " ";
		s << MapInUse.vals[i];
	}

	s << "]";

	return s;
}

/***************************************************************************
 *
 * Messages
 *
 ***************************************************************************/

class message {
public:
	signed int q;
	double Probs[MAX_Q];
public:
	message(int p_q = -1) :
			q(p_q) {
	}

	message(message &M) :
			q(M.q) {
		*this = M;
	}

	void HardMessage(GFq &g) {
		*this = 0;
		(*this)[g] = 1;
	}

	void Set_q(int p_q) {
		q = p_q;
	}

	void DFT2();

	BOOLEAN operator<(message &m2) {
		message &m1 = *this;

		for (int i = 0; i <= m1.q; i++)
			if (m1[i] >= m2[i])
				return FALSE;

		return TRUE;
	}

	double Maximum() {
		double maximum = -INF;
		for (int i = 0; i < q; i++)
			if (Probs[i] > maximum)
				maximum = Probs[i];

		return maximum;
	}

	double &operator[](int i) {
		return Probs[i];
	}
	double &operator[](GFq i) {
		return Probs[i.val];
	}
	message &operator=(message &M) {
		if (q != M.q)
			Set_q(M.q);

		bcopy(/* from */M.Probs, /* to */Probs, sizeof(double) * q);
		return *this;
	}

	message &operator=(double d) {
		for (int i = 0; i < q; i++)
			Probs[i] = d;
		return *this;
	}

	message &operator*=(message &M) {
		for (int i = 0; i < q; i++)
			Probs[i] *= M.Probs[i];

		return *this;
	}

	message &operator*(message &M2) {
		static message Aux(q);
		message &M1 = *this;

		for (int i = 0; i < q; i++)
			Aux.Probs[i] = M1.Probs[i] * M2.Probs[i];

		return Aux;
	}

	message &operator*(double d) {
		static message Aux(q);

		for (int i = 0; i < q; i++)
			Aux[i] = Probs[i] * d;

		return Aux;
	}

	BOOLEAN operator==(message &m) {
		if (q != m.q)
			return FALSE;

		for (int i = 0; i < q; i++)
			//         if (Probs[i] != m[i])
			if (fabs(Probs[i] - m[i]) > EPSILON)
				return FALSE;

		return TRUE;
	}

	BOOLEAN operator==(double d) {
		for (int i = 0; i < q; i++)
			//if (fabs(Probs[i] - d) > EPSILON)
			if (Probs[i] != d)
				return FALSE;

		return TRUE;
	}

	message &operator+=(message &M) {
		for (int i = 0; i < q; i++)
			Probs[i] += M.Probs[i];

		return *this;
	}

	message &operator+(message &M) {
		static message Aux(q);

		for (int i = 0; i < q; i++)
			Aux[i] = Probs[i] + M.Probs[i];

		return Aux;
	}

	message &operator/=(double d) {
		for (int i = 0; i < q; i++)
			Probs[i] /= d;

		return *this;
	}

	double sum() {
		double aux = 0;
		for (int i = 0; i < q; aux += Probs[i++])
			;
		return aux;
	}

	void Normalize() {
		double aux = sum();

		if (aux > 0) {
			for (int i = 0; i < q; i++)
				Probs[i] /= aux;
		} else {
			for (int i = 0; i < q; i++)
				Probs[i] = 1. / (double) q;
		}
	}

	void Clear() {
		bzero(Probs, sizeof(double) * q);
	}

	message &Convolve(message &M2) {
		message M1(*this);   // Auxiliary

		// Clear this
		Clear();

		for (GFq i(0); i.val < q; i.val++)
			for (GFq j(0); j.val < q; j.val++)
				Probs[i.val] += M1[j] * M2[i - j];

		return *this;
	}

	message &ApproxConvolve(message &M2);
	void Approximate();

	message &MaxConvolve(message &M2) {     // max-prod version
		message M1(*this);   // Auxiliary

		// Clear this
		Clear();
		double max;
		double candidate;

		for (GFq i(0); i.val < q; i.val++) {
			max = -1;
			for (GFq j(0); j.val < q; j.val++) {
				candidate = M1[j] * M2[i - j];
				if (candidate > max)
					max = candidate;
			}
			Probs[i.val] = max;
		}

		return *this;
	}

	void PermutePlus(GFq &g) {
		message Aux;

		Aux = *this;
		for (GFq i(0); i.val < q; i.val++)
			Probs[i.val] = Aux[i + g];
	}

	void PermuteTimes(GFq &g) {
		message Aux;

		Aux = *this;
		for (GFq i(0); i.val < q; i.val++)
			Probs[i.val] = Aux[i * g];
	}

	message &Reverse() {
		message Aux;

		Aux = *this;
		for (GFq i(0); i.val < q; i.val++)
			Probs[i.val] = Aux[i.Minus()];

		return *this;
	}

	GFq &Decision() {
		static GFq Candidate(0);
		double max = -1;
		int count_max = 0;

		for (GFq i(0); i.val < q; i.val++) {
			if (Probs[i.val] > max) {
				max = Probs[i.val];
				Candidate = i;
				count_max = 1;
			} else if (Probs[i.val] == max)
				count_max++;
		}

		if (count_max > 1) {     // If more than one maximum - randomly select
			int selection = uniform_random(count_max) + 1; // Between 1 and count_max
			int found_so_far = 0;

			for (GFq i(0); i.val < q; i.val++)
				if (Probs[i.val] == max) {
					Candidate = i;
					found_so_far++;
					if (found_so_far == selection)
						break;
				}
		}

		return Candidate;
	}

	double ProbCorrect() {
		int count_zero = 1;

		for (int i = 1; i < q; i++) {
			if (Probs[i] > Probs[0]) {
				return 0;
			} else if (Probs[i] == Probs[0]) {
				count_zero++;
			}
		}

		return 1. / count_zero;
	}

	double Entropy() {
		double aux = 0;

		for (int i = 0; i < q; i++) {
			if (Probs[i] != 0) {
				double aux2 = Probs[i] * log(Probs[i]);
				aux += -clip(aux2);
			}
		}

		return aux;
	}

	message &operator<<(int l) {
		static message Aux;

		Aux.q = q;
		for (int i = 0; i < q; i++)
			Aux.Probs[i] = Probs[(i + l) % q];

		return Aux;
	}

	message &LLRShift(int k) {
		message Aux = *this;

		for (int i = 0; i < q; i++)
			Probs[i] = Aux[(i + k) % q] - Aux[k % q];

		return *this;
	}

	double AverageD() {
		message m = *this;

		m.Clip();
		m /= m[0];
		m.Clip();

		m[0] = 0;     // Don't count zero
		return m.sum() / (GFq::q - 1);
	}

	void operator<<=(int l) {
		*this = *this << l;
	}

	//   ~message()
	//      { if (Probs != NULL) delete Probs; }

	void Clip(double minval = EPSILON, double maxval = INF) {
		for (int i = 0; i < q; i++) {
			clip(Probs[i], maxval);
			if (Probs[i] < minval)
				Probs[i] = minval;
		}

	}

	int RandomSelectIndex(); // Select index at random based on probabilities in message
};

// Convolve two messages
inline message &Convolve(message &M1, message &M2) {
	static message Aux;

	Aux = M1;
	Aux.Convolve(M2);
	return Aux;
}


inline std::ostream &operator<<(std::ostream &s, message &m) {
	s << ceil(m[0] * 1000.) / 1000.;
	for (int i = 1; i < m.q; i++)
		s << " " << ceil(m[i] * 1000.) / 1000.;

	return s;
}

inline message &operator-(message &m1, message &m2) {
	static message aux;

	aux.q = m1.q;

	for (int i = 0; i < m1.q; i++)
		aux[i] = m1[i] - m2[i];

	return aux;
}

inline message &operator/(message &m, double d) {
	static message aux;

	aux.q = m.q;

	for (int i = 0; i < m.q; i++)
		aux[i] = m[i] / d;

	return aux;
}

inline double pow(message &m, int l) {
	double f = 0;

	for (int i = 0; i < m.q; i++)
		f += pow(m[i], l);

	return f;
}

inline double fabs(message &m) {
	double f = 0;

	for (int i = 0; i < m.q; i++)
		f += fabs(m[i]);

	return f;
}

inline message &log(message &m) {
	static message aux;

	aux.q = m.q;
	for (int i = 0; i < m.q; i++)
		aux[i] = mylog(m[i]);

	return aux;
}

inline message &exp(message &m) {
	static message aux;

	aux.q = m.q;
	for (int i = 0; i < m.q; i++)
		aux[i] = exp(m[i]);

	return aux;
}

inline message &LLR(message &m) {
	static message aux;

	aux.q = m.q;
	double log_m0 = mylog(m[0]);

	aux[0] = 0;
	for (int i = 1; i < m.q; i++)
		aux[i] = log_m0 - mylog(m[i]);

	return aux;
}

inline message &unLLR(message &m) {
	static message aux;

	aux.q = m.q;

	aux[0] = 1;
	for (int i = 1; i < m.q; i++)
		aux[i] = exp(-m[i]);

	aux.Clip();
	aux.Normalize();
	aux.Clip();

	return aux;
}

/*****************************************************************************
 *
 * ArrayOfMessage
 *
 *****************************************************************************/

class ArrayOfMessage {
	message *Messages;
	int size;
public:

	int GetSize() {
		return size;
	}

	ArrayOfMessage() :
			Messages(NULL), size(0) {
	}

	void Allocate(int p_Size) {
		if (size != p_Size) {
			deAllocate();
			size = p_Size;
			Messages = new message[size];
		}
	}

	void deAllocate() {
		if (Messages != NULL)
			delete Messages;

		Messages = NULL;
		size = 0;
	}

	~ArrayOfMessage() {
		deAllocate();
	}

	message &operator[](int i) {
		return Messages[i];
	}
};

/*****************************************************************************
 *
 * Graphs
 *
 *****************************************************************************/

class edge;
// temporary declaration, to enable the use of the class in further declaration

class node {
public:
	int id;   // number used for identification
	int degree;
	int AuxDegree;        // Auxiliary variable for use in encoder generator
	edge **edges;
	int MaxEdges;

public:

	void SetID(int p_id) {
		id = p_id;
	}
	int GetID() {
		return id;
	}

	node() :
			degree(-1), edges(NULL) {
	}
//	virtual ~node(void) {
//	} // Virtual destructor
	void Disconnect();  // Disconnect all edges

	void DisconnectEdge(edge *e) {
		int index = 0;
		for (; index < degree; index++)
			if (edges[index] == e)
				break;

		if (index == degree) {
			cout
					<< "node::DisconnectEdge: Attempt to disconnect a nonexistent edge\n";
			exit(1);
		}

		for (int i = index + 1; i < degree; i++)
			edges[i - 1] = edges[i];

		degree--;
	}

	void add_edge(edge *e) {
		if (degree == -1) {
			cout << "Edges not yet allocated, aborting.  \n";
			exit(1);
		}

		edges[degree++] = e;

		if (degree > MaxEdges) {
			cout << "MaxEdges exceeded! " << " degree = " << degree
					<< " MaxEdges = " << MaxEdges << "\n";
		}
	}

	void AllocateEdges(edge **&EdgeStackPointer, int p_MaxEdges) {
		degree = 0;    // Indicate edges have been allocated
		edges = EdgeStackPointer;
		EdgeStackPointer += p_MaxEdges;        // Advance allocation pointer
		MaxEdges = p_MaxEdges;
	}

	int GetDegree() {
		return degree;
	}
	edge &GetEdge(int index) {
		return *edges[index];
	}

	virtual node &AdjacentNode(int index) = 0;
};

message &GenerateChannelMessage(GFq v, channel &TransmitChannel, mapping &MapInUse, double ChannelOut);

class variable_node: public node {
public:
	GFq Symbol;     // Value - for encoding
	GFq v;          // coset vector - randomly selected at variable node
	double SourceData;
	mapping *MapInUse;

	// Auxiliary for Linear Programming method
	int *LCLP_Constraints;
	int CountConstraints;

public:
	message InitialMessage;
	message FinalEstimate;
	message AllImprovementsForChange;       // For use in greedy source coding

public:
	variable_node() {
		v.val = uniform_random(GFq::q);
	}
	void Allocate_LCLP_Constraints(int **ConstraintsStack);
	int Count_LCLP_Constraints();
	void Add_LCLP_Constraint(int variable_index);

	message &CalcRightboundMessage(int rightbound_index);
	void CalcAllRightboundMessages();
	void CalcFinalMessage();
	void Initialize(channel &TransmitChannel, double ChannelOut);
	void SetMapInUse(mapping &p_MapInUse) {
		MapInUse = &p_MapInUse;
	}
	double GetZeroSignal() {
		return MapInUse->map(v);
	}

	// For use in greedy source coding

	BOOLEAN IsRightConnectedTo(node *n);
	BOOLEAN IsPath3ConnectedTo(node *n);  // Is connected by a path of at most
	virtual node &AdjacentNode(int index);
};

class complex_vector;  // FIXME: Where is the definition?

message &CalcLeftboundMessage(message Vectors[], int left_index, int degree);

class check_node: public node {
public:
	void CalcAllLeftboundMessages();
	BOOLEAN DoesFinalEstimateViolate();
	virtual node &AdjacentNode(int index);
	GFq &Element(int i); // For use in encoding - treats check like row of matrix
	GFq &Value();
};

class edge {
private:
	variable_node *left_node;
	check_node *right_node;

public:
	message LeftBoundMessage, RightBoundMessage;
	GFq label;

public:
	variable_node &LeftNode() {
		return *left_node;
	}
	check_node &RightNode() {
		return *right_node;
	}

	edge() {
		label.RandomSelect();
	}

	void set_nodes(variable_node *p_left_node, check_node *p_right_node) {
		left_node = p_left_node;
		right_node = p_right_node;
		left_node->add_edge(this);
		right_node->add_edge(this);
	}

	void Disconnect() {
		left_node->DisconnectEdge(this);
		right_node->DisconnectEdge(this);
	}
};
////////////////////////////////////////////////////////////////////////////
//
// Bipartite graph class
//
////////////////////////////////////////////////////////////////////////////
class bipartite_graph {
public:
	int N;                                // Number of variable nodes
	int M;                                // Number of check nodes
	long E;                                // Number of edges

	variable_node *variable_nodes;
	check_node *check_nodes;
	edge *edges;

	edge **EdgeStack;    // Pointer used to manage allocation of memory to nodes
	static BOOLEAN ShouldPermuteLDPCVariables;
public:
	void Reset(
			int p_N,              // number of variable nodes
			int lambda_degs[], double lambda_wts[], int rho_degs[],
			double rho_wts[], mapping &MapInUse);

	bipartite_graph() :
			N(0), M(0), E(0), variable_nodes(NULL), check_nodes(NULL), edges(
					NULL), EdgeStack(NULL) {
	}

	~bipartite_graph() {
		Clear();
	}

	void Clear() {
		if (variable_nodes != NULL)
			delete variable_nodes;
		if (check_nodes != NULL)
			delete check_nodes;
		if (edges != NULL)
			delete edges;
		if (EdgeStack != NULL)
			delete EdgeStack;
	}

	void PrintNodes(char *title = NULL) {
		if (title != NULL)
			cout << " --- " << title << "\n";
		cout << "Variable Nodes: ";
		for (int i = 0; i < N; i++)
			cout << variable_nodes[i].GetDegree() << " ";
		cout << "\n";

		cout << "Check Nodes: ";
		for (int i = 0; i < M; i++)
			cout << check_nodes[i].GetDegree() << " ";
		cout << "\n";
	}

	void SaveToFile(char *filename) {
		std::ofstream OutFile(filename);

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < check_nodes[i].GetDegree(); j++) {
				if (j != 0)
					OutFile << " ";
				OutFile << check_nodes[i].GetEdge(j).LeftNode().GetID();
			}
			OutFile << "\n";
		}

	}
};

/************************************************************************
 *
 * TopList - for use in greedy
 *
 ************************************************************************/

class TopList {
public:
	GFq *BestChange;
	int *VariableIndex;
	double *DistortionImprovement;

	int MaxSize, CurrentSize;
public:
	TopList() :
			MaxSize(-1), CurrentSize(-1) {
	}

	~TopList() {
		DeAllocate();
	}

	void DeAllocate() {
		if (MaxSize > -1) {
			delete BestChange;
			delete VariableIndex;
			delete DistortionImprovement;
		}
		MaxSize = -1;
	}

	void Init(int p_MaxSize) {
		if (MaxSize != p_MaxSize) {
			DeAllocate();

			BestChange = new GFq[p_MaxSize];
			VariableIndex = new int[p_MaxSize];
			DistortionImprovement = new double[p_MaxSize];
		}

		MaxSize = p_MaxSize;
		CurrentSize = 0;
	}

	void Add(GFq p_BestChange, int p_VariableIndex,
			double p_DistortionImprovement) {
		// Find place for new distortion
		int Place;
		for (Place = 0; Place < CurrentSize; Place++)
			if (p_DistortionImprovement > DistortionImprovement[Place])
				break;

		// Shift all the rest one place up
		if (CurrentSize < MaxSize)
			CurrentSize++;
		for (int i = CurrentSize - 1; i > Place; i--) {
			BestChange[i] = BestChange[i - 1];
			VariableIndex[i] = VariableIndex[i - 1];
			DistortionImprovement[i] = DistortionImprovement[i - 1];
		}

		// Place new entry
		if (Place < CurrentSize)      // If not passed last place in TopList
				{
			BestChange[Place] = p_BestChange;
			VariableIndex[Place] = p_VariableIndex;
			DistortionImprovement[Place] = p_DistortionImprovement;
		}
	}
};

#endif
