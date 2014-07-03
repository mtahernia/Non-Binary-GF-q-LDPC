#ifndef LDPC_1
#define LDPC_1

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <wchar.h>
#include "Utils_1.h"
#include "LDPC_2.h"



#define MAX_LAMBDAS 1000
#define MAX_RHOS    1000


////////////////////////////////////////////////////////////////////////////
//
// LDPC Code
//
////////////////////////////////////////////////////////////////////////////

//class convolutional_code; //FIXME: I really don't know why this line is here! I just commented this line

class LDPC_Code
{
public:
  bipartite_graph Graph;
  int lambda_degs[MAX_LAMBDAS + 1];
  double lambda_wts[MAX_LAMBDAS + 1];
  int rho_degs[MAX_RHOS + 1];
  double rho_wts[MAX_RHOS + 1];
  int BlockLength;  // Used in monte-carlo simulations
  mapping MapInUse;
  channel *Channel;

  // For use in encoding
  VariableNodeList Variables;
  CheckNodeList Checks;
  int Systematic, Gap, Triangle;
  matrix GapMatrix, MinusPhiInverse;

public:

  // General functions ----------------------------------------------------
  LDPC_Code( std::ifstream &File,
             int p_BlockLength = -1,
	     channel *p_Channel = NULL );

  LDPC_Code() : 
    BlockLength(-1),
    Channel(NULL) {}

  LDPC_Code &operator=(LDPC_Code &Code);

  void GetFromFile( std::ifstream &file );
 
  void SetChannel( channel &p_Channel )
  {
     Channel = &p_Channel;
  }

  double sigma_lambda();
  double sigma_rho();

  double Calc_Energy();
  double SumLambda()
    { 
      double sum = 0;
      for (int i = 0; lambda_degs[i] != -1; i++)
	sum += lambda_wts[i];
      return sum;
    }
  double SumRho()
    { 
      double sum = 0;
      for (int i = 0; rho_degs[i] != -1; i++)
	sum += rho_wts[i];
      return sum;
    }
  double Calc_Symbol_Rate();

  double Calc_Bit_Rate()
  {return Calc_Symbol_Rate() * log((double)GFq::q)/log(2.);   }

  void MakeLambdasValid()   // Make lambdas sum = 1
    {
      double sum = SumLambda();
      for (int i = 0; lambda_degs[i] != -1; i++)
	lambda_wts[i] /= sum;
    }

  void MakeRhosValid()   // Make lambdas sum = 1
    {
      double sum = SumRho();
      for (int i = 0; rho_degs[i] != -1; i++)
	rho_wts[i] /= sum;
    }

  void GetLambdasWtsFromFile( std::ifstream &file );
  void GetRhoWtsFromFile( std::ifstream &file );
  int CountLambdaDegs()
    {
      int count;
      for (count = 0; lambda_degs[count] != -1; count++);
      return count;
    }

  int CountRhoDegs()
    {
      int count;
      for (count = 0; rho_degs[count] != -1; count++);
      return count;
    }

  // Monte Carlo functions -----------------------------------------
  void Init_Messages( vector &ChannelOutput );
  void Leftbound_Iteration( );
  void Rightbound_Iteration( );
  void FinalIteration( );
  double Belief_Propagation_Decoder(int Count_Iterations);
  void GetZeroCodeword( vector &Codeword );
  double Calc_Symbol_Error_Rate();
  double Calc_Rightbound_Symbol_Error_Rate();

  void ResetGraph()
   { 
      Graph.Reset(BlockLength, lambda_degs, lambda_wts, rho_degs, rho_wts, MapInUse); 
      Variables.Init(Graph.variable_nodes, Graph.N);
      Checks.Init(Graph.check_nodes, Graph.M);
   }

  // Encoding ------------------------------------------------------
  double Calc_Source_Coding_Symbol_Rate();
  double Calc_Source_Coding_Bit_Rate();
  void GenerateRandomSystematic();
  void Encode();
  void GenerateEncoder();
  void GenerateEncoder_WithoutGap();

} ;

#endif
