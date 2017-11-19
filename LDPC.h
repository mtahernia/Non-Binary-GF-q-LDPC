/*
 * LDPC.h
 *
 *  Created on: 22 Dec, 2014
 *      Author: Mehrdad Tahernia
 *		  User: mehrdad
 */

#ifndef LDPC_H_
#define LDPC_H_


#include "Definitions.h"
#include "Matrix.h" // matrix, column vector
#include "Mapping.h"
#include "Bipartite_Graph.h"
#include "Encoding.h" // Node lists

class vector;	//#include "Utils_2.h" // vector, array
class channel;	//#include "Channel.h"


//#include <cstring>  // bzero
//#include <algorithm>  // std::copy needs this but it works without it. It must have been included in other heades
//#include "TopList.h" // Not Used


////////////////////////////////////////////////////////////////////////////
//
// LDPC Code
//
////////////////////////////////////////////////////////////////////////////

//class convolutional_code; //FIXME: I really don't know why this line is here! I just commented this line

/// LDPC_Code Class definition
class LDPC_Code {
public:
	bipartite_graph Graph;					//!< Bipartite graph of underlying LDPC Code
	int lambda_degs[MAX_LAMBDAS + 1];		//!< Nonzero Degrees of variable nodes
	double lambda_wts[MAX_LAMBDAS + 1];		//!< Weights of the variable nodes with degrees specified above
	int rho_degs[MAX_RHOS + 1];				//!< Degrees of check nodes
	double rho_wts[MAX_RHOS + 1];			//!< Weights of check nodes
	int BlockLength; 						//!< Block Length that is used in monte-carlo simulations
	mapping MapInUse; 						//!< Mapping from code symbols to channel symbols
	channel *Channel; 						//!< Channel in use

	// For use in encoding
	VariableNodeList Variables; 			//!< List of variable nodes, this is used in encoding
	CheckNodeList Checks; 					//!< List of check nodes, this is used in encoding
	int Systematic;							//!< FIXME: I don't know where are these used, in the encoding process
	int Gap;								//!< FIXME: I don't know where are these used, in the encoding process
	int Triangle;							//!< FIXME: I don't know where are these used, in the encoding process
	matrix GapMatrix;						//!< FIXME: I don't know where are these used, in the encoding process
	matrix MinusPhiInverse; 				//!< FIXME: I don't know where are these used, in the encoding process

public:
	/// Constructor declaration for LDPC_Code class with 3,2 or 1 input(s). The default extra values should be specified in function call or in input file
	LDPC_Code(std::ifstream &File, int p_BlockLength = -1, channel *p_Channel =	NULL);
	/// Initialization of the constructor is called without any arguments
	LDPC_Code() : /* After colon, there is initialization of constant variables!
	 you can not initialize constant int,... inside the constructor function */
			BlockLength(-1), Channel(NULL), Systematic(-1),Gap(-1),Triangle(-1) {}
	~LDPC_Code(){/*cout <<"LDPC_Code Destructor called \n";*/}

	/// Operator overloading function declaration for = . The argument is rhs of the assignment operator
//	LDPC_Code &operator=(LDPC_Code &Code); // FIXME: This function should have explicit definition somewhere
	/// Reads the parameters of LDPC code from the input file
	void GetFromFile(std::ifstream &file);
	/// Sets the channel used for the simulation
	void SetChannel(channel &p_Channel) {Channel = &p_Channel;}
	double sigma_lambda();			//!< \f$ \sum_{i=1}^{c}\frac{\lambda_i}{i} \f$
	double sigma_rho();				//!< \f$ \sum_{j=1}^{c}\frac{\rho_j}{j} \f$

//	double Calc_Energy();				//!< FIXME: This is not used so I commented it

	/// Calculates \f$ \sum_{i=1}^{c} \lambda_i \f$
	/**
	 * This will be needed to calculate the lower bound on code rate
	 */
	double SumLambda();
	/**
	 * Calculates \f$ \sum_{j=1}^{d} \rho_j \f$
	 */
	double SumRho();
	/**
	 * \f$ Rate = 1- \frac{M}{N}=1 - \frac{ \sum_{j=1}^{d} \rho_j/j }{ \sum_{i=1}^{c} \lambda_i/i } \f$
	 */
	double Calc_Symbol_Rate();		//!< Calculate the Symbol rate
	/**
	 * Calculates bit-rate from symbol-rate
	 */
	double Calc_Bit_Rate();
	/**
	 * Normalize \f$ \lambda \f$ to make it a probability distribution
	 */
	void MakeLambdasValid();   /// Make lambdas sum = 1
	/**
	 * Normalize \f$ \rho \f$ to make it a probability distribution
	 */
	void MakeRhosValid();   /// Make rhos sum = 1
	void GetLambdasWtsFromFile(std::ifstream &file);//!< Reads lambda weights from file
	void GetRhoWtsFromFile(std::ifstream &file);//!< Reads rho weights from file

/// Calculate c
	/**
	 * c is the number of possible degrees of variable nodes
	 */
	int CountLambdaDegs();

	/// Calculate d
	/**
	 * d is the number of possible degrees of check nodes
	 */
	int CountRhoDegs();

	// Monte Carlo functions -----------------------------------------
	void Init_Messages(vector &ChannelOutput);
	void Leftbound_Iteration();
	void Rightbound_Iteration();
	void FinalIteration();
	double Belief_Propagation_Decoder(int Count_Iterations);
	void GetZeroCodeword(vector &Codeword);
	void GetCodeword(vector &Codeword);

	void Get_Symbols(GFq *Symbols);
	void Set_Symbols(GFq *Symbols);

	double Calc_Symbol_Error_Rate();
	double Calc_Rightbound_Symbol_Error_Rate();
	void ResetGraph();

	// Encoding ------------------------------------------------------
	double Calc_Source_Coding_Symbol_Rate(); //!< Calculate source coding symbol rate(FIXME:Not implemented yet)
	double Calc_Source_Coding_Bit_Rate();//!< Calculate source coding bit rate(FIXME:Not implemented yet)
	void GenerateRandomSystematic();	//!< Randomly generate systematic code
	void Encode();							//!< Encode using Urbanke method
	void GenerateEncoder();				//!< Generate encoder with gap (FIXME: )
//	void GenerateEncoder_WithoutGap();//!< Generate encoder without gap (FIXME:)
	int GenerateEncoder_WithoutGap();//!< Generate encoder without gap (FIXME:)
};

#endif /* LDPC_H_ */
