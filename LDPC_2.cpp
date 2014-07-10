#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include "Utils_1.h"
#include "LDPC_1.h"
#include "LDPC_2.h"


/*********************************************************************************
 *
 * MESSAGE
 *
 *********************************************************************************/


void message::DFT2()          // A real-valued DFT - also IDFT
{
	static message Aux;
	GFq mask, n0_index, n1_index;
	BYTE j_bit;
	double temp;

	if (!GFq::IsPrimeQ)
	{
		for (int i  = 0; i < GFq::log_2_q; i++)
		{
			Aux = *this;			// Initialize
			mask.val = 1 << i;  //Shift left i times to get powers of 2

			//			cout << "mask.val("<<i<<")="<<mask.val<<std::endl; // Just for debugging

			for (GFq j(0); j.val < q; j.val++)
			{
				j_bit = (j.val & mask.val) >> i;	// obtain value of j which is i th bit of j
				n0_index.val = j.val & (~mask.val);	// turn bit off
				n1_index.val = j.val | mask.val;    // turn bit on

				//cout << (int)j_bit;

				if (j_bit == 0)
					Probs[j.val] = Aux[n0_index] + Aux[n1_index];
				else
					Probs[j.val] = Aux[n0_index] - Aux[n1_index];
				//				cout << "Aux(" << n0_index << ")= " << Aux[n0_index] << std::endl;
			}//end for j
		}//end for i
	}//end else
	else if (GFq::IsPrimeQ) // FIXME: this is not a real FFT, here I used normal DFT for prime fields
	{
		Aux = *this;

		for (GFq j(0); j.val < q; j.val++)
		{	temp = 0;
		for (int n = 0; n < q; n++)
		{
			temp += Aux[n]*cos(2*M_PI*n*j.val/q);
//			cout << "Aux["<< n <<"]="<<Aux[n] << std::endl ;
		}
		Probs[j.val] = temp;
		}//end for j

	}
}





/*********************************************************************************
 *
 * CHANNEL
 *
 *********************************************************************************/


void channel::SimulateOutputVector(vector &InVector, vector &OutVector)
{
   OutVector.Allocate(InVector.GetSize());

   for (int i = 0; i < InVector.GetSize(); i++)   // Add noise to each component
      OutVector[i] = this->SimulateOutput( /* channel in */ InVector[i] );    
}


/*********************************************************************************
 *
 * BSC Channel
 *
 *********************************************************************************/


void BSC_Channel::ProcessMapping(LDPC_Code &Code)
{
   mapping &MapInUse = Code.MapInUse;

   for (GFq i(0); i.val < MapInUse.q; i.val++)
      if ((MapInUse.map(i) != 0) && (MapInUse.map(i) != 1))
      {
         cout << "mapping levels must be (0,1)\n";
         exit(1);
      }
}




double H2(double p)
{
   return -p*log2(p) - (1-p)*log2(1-p);
}




double H2Reverse(double H)
{
   double pmin = 0, 
          pmax = 0.5, 
          tol = 0.0000001,
          pmid;

   if (H == 0)
      return 0;
   else if (H == 1.)
      return 0.5;

   while ((pmax - pmin) > tol)
   {
      pmid = (pmin + pmax) / 2.;
      if (H2(pmid) > H)
         pmax = pmid;
      else
         pmin = pmid;
   }

   return pmid;
}





double BSC_Channel::CapacityInBits()
{
   return 1 - H2(channel_p);
}




void BSC_Channel::PrintChannelData(LDPC_Code &Code)
{
   double BitRate;

   BitRate = Code.Calc_Bit_Rate();
   cout  << "channel_p = " << channel_p
         << "\nCapacity (bits per channel use) = " << 1 - H2(channel_p) 
         << " Max channel_p for Bit rate: " << H2Reverse(1. - BitRate);
}


/*********************************************************************************
 *
 * AWGN Channel
 *
 ********************************************************************************/


double GaussGenerate(double sigma)
  // Simulate the result of passing the zero vector through the AWGN
{
  static const double pi = 3.141592653;
  double normal_random_number, x1, x2;

  x1 = my_rand();
  x2 = my_rand();

  if (x1 < EPSILON)
   x1 = EPSILON;

  normal_random_number = sqrt(-2 * log(x1)) * cos(2*pi * x2);
  
  clip(normal_random_number);
  
  return sigma * normal_random_number;
}



void AWGN_Channel::ProcessMapping(LDPC_Code &Code)
{
   Code.MapInUse.Normalize();
}

  



double AWGN_Channel::CapacityInBits()
{
   double No = pow(noise_sigma,2.);
   double SNR = 1./No;
   return 0.5 * log(1. + SNR) / log(2.);
}



void AWGN_Channel::PrintChannelData(LDPC_Code &Code)
{
   double BitRate, No, SNR, SNR_dB;
   
   BitRate = Code.Calc_Bit_Rate();
   No = pow(noise_sigma,2.);
   SNR = 1./No;
   SNR_dB = 10. * log10(SNR);

   cout << "SNR(dB) = " << SNR_dB
        << " SNR = " << SNR
        << " Noise Sigma = " << noise_sigma
        << "\nCapacity at SNR (symbols per channel use) = " 
        << 0.5 * log(1. + SNR) / log((double)GFq::q)
        << "\nCapacity at SNR (bits per channel use) = " 
        << 0.5 * log(1. + SNR) / log(2.)
        << "\nMinimum SNR for rate (dB) = " 
        << 10. * log(pow(2., 2.*BitRate) - 1.) / log(10.)
        << " (absolute value) = " << pow(2., 2.*BitRate) - 1;
}



// override virtual functions
double AWGN_Channel::CalcProbForInput( double ChannelOutput, 
				       double ChannelInput )
{
   static const double sqrt_2pi = sqrt(2*3.141592653);

   double noise_prob = (1/(sqrt_2pi * noise_sigma)*
	    exp(-pow(ChannelOutput-ChannelInput,2.)/(2.*NoiseVariance())));

   return noise_prob;
}



double AWGN_Channel::SimulateOutput(double ChannelInput)
  // Simulate the result of passing the zero vector through the AWGN
{
  return ChannelInput + GaussGenerate(noise_sigma);
}



double AWGN_Channel::NoiseVariance() 
{ 
  return pow(noise_sigma, 2); 
}




double AWGN_Channel::NoiseStdDev() 
{ 
  return noise_sigma; 
}








/****************************************************************************
 *
 * Check node
 *
 ****************************************************************************/



GFq &check_node::Value()
{
   static GFq Aux;

   Aux = 0;
   for (int i = 0; i < GetDegree(); i++)
      Aux += GetEdge(i).label * GetEdge(i).LeftNode().Symbol;

   return Aux;
}




node &check_node::AdjacentNode(int index)
{
   return GetEdge(index).LeftNode();
}



message &FastCalcLeftboundMessage(message AuxLeft[], 
                                  message AuxRight[],
			          int left_index,
                                  int degree)
{
  static message Aux;

  // Init to delta pulse 
  Aux.Set_q(GFq::q);

  Aux = AuxLeft[left_index];
  Aux *= AuxRight[left_index];
  Aux.DFT2();

  Aux.Reverse();	// estimate of symbol value = (- sum of others)

  return Aux;
}



void check_node::CalcAllLeftboundMessages( )
{
  static message Vectors[MAX_DEGREE];
  static message AuxLeft[MAX_DEGREE];
  static message AuxRight[MAX_DEGREE];
  static GFq ZERO(0);

  // Calc auxiliary DFTs
  for (int i = 0; i < GetDegree(); i++)
  {
     Vectors[i] = GetEdge(i).RightBoundMessage;
     Vectors[i].PermuteTimes(GetEdge(i).label.Inverse());
  }

  //-------------------------------------------------------------
  // If power of two - use DFT2
  //-------------------------------------------------------------
  if (true)//(!GFq::IsPrimeQ) FIXME
  {
     for (int i = 0; i < GetDegree(); i++)
	 {
         Vectors[i].DFT2();
	 }

      // Calc auxiliary values
     AuxLeft[0].Set_q(GFq::q);
     AuxLeft[0] = 1.;
     for (int i = 1; i < GetDegree(); i++)
     {
        AuxLeft[i] = AuxLeft[i - 1];
        AuxLeft[i] *= Vectors[i - 1];
     }

     AuxRight[GetDegree() - 1].Set_q(GFq::q);
     AuxRight[GetDegree() - 1] = 1.;
     for (int i = GetDegree() - 2; i >= 0; i--)
     {
        AuxRight[i] = AuxRight[i + 1];
        AuxRight[i] *= Vectors[i + 1];
     }

     // Calc leftbound messages
     for (int i = 0; i < GetDegree(); i++)
     {
       GetEdge(i).LeftBoundMessage = FastCalcLeftboundMessage(AuxLeft, AuxRight, i, GetDegree());
       GetEdge(i).LeftBoundMessage.PermuteTimes(GetEdge(i).label);
     }
  }
  else
  {
	  cout <<  "FIXME!!!" << std::endl;
  }
}



/*************************************************************************
 *
 * Node
 *
 *************************************************************************/


void node::Disconnect()
{
   // while edges remain, disconnect the first one 
   while (degree > 0) edges[0]->Disconnect();
}



/*************************************************************************
 *
 * Variable node
 *
 *************************************************************************/


node &variable_node::AdjacentNode(int index)
{
   return GetEdge(index).RightNode();
}




message &variable_node::CalcRightboundMessage( int rightbound_index )
// rightbound_index is -1 if calculation is meant for final estimate
{
  static message Aux;

  Aux = InitialMessage;

  for (int i = 0; i < GetDegree(); i++)
  {
     // ignore intrinsic information
     if (i == rightbound_index)	continue;

     Aux *= GetEdge(i).LeftBoundMessage;
  }

  Aux.Normalize();     
  
  return Aux;
}




void variable_node::CalcFinalMessage()
{
   FinalEstimate = CalcRightboundMessage( /* NO rightbound index */ -1 );
   Symbol = FinalEstimate.Decision();
}



void variable_node::CalcAllRightboundMessages()
{
   for (int i = 0; i < GetDegree(); i++)
      GetEdge(i).RightBoundMessage = CalcRightboundMessage( /* rightbound index */ i);

   CalcFinalMessage();
}




message &GenerateChannelMessage(GFq v,
                                channel &TransmitChannel, 
                                mapping &MapInUse,
                                double ChannelOut )
{
   static message InitialMessage;
   int q = MapInUse.GetQ();
   double CandidateIn, ChannelIn;
  
   ChannelIn = MapInUse.map(v);        // mapping of (0 + v) % q;
  
   // Generate InitialMessage
   InitialMessage.Set_q(q);
   for (GFq i(0); i.val < q; i.val++)
   {
      CandidateIn  = MapInUse.map(i + v);
      InitialMessage[i.val] = TransmitChannel.CalcProbForInput(
		                       ChannelOut, CandidateIn );
   }

   InitialMessage.Normalize();    // Make valid probability vector 

   return InitialMessage;
}






void variable_node::Initialize( channel &TransmitChannel, double ChannelOut )
{
   InitialMessage = GenerateChannelMessage(v, TransmitChannel, *MapInUse, ChannelOut );
   FinalEstimate = InitialMessage;
  
   // Generate Rightbound messages
   for (int j = 0; j < degree; j++)
      GetEdge(j).RightBoundMessage = InitialMessage;

}





BOOLEAN variable_node::IsRightConnectedTo( node *n )
{
  for (int i = 0; i < degree; i++)
    if (&edges[i]->RightNode() == n) return TRUE;

  return FALSE;
}

/**********************************************************************************
 *
 * Graph
 *
 **********************************************************************************/




void bipartite_graph::Reset( 
	     int p_N,                            // number of variable nodes
	     int lambda_degs[],                  // left-degrees, terminated by -1
	     double lambda_wts[],                // weights corresponding to above degrees
	     int rho_degs[],       		  // right-degree, terminated by NULL
	     double rho_wts[],                  // weights corresponding to above degrees
		 mapping &MapInUse)
{
  //-----------------------------------------------------------------------
  // Clean up
  //-----------------------------------------------------------------------
  Clear();

  //-----------------------------------------------------------------------
  // Calc M, N and E
  //-----------------------------------------------------------------------
  double Ratio;     // M/N
  double Nominator, Denominator;

  Nominator = 0;
  for (int i = 0; rho_degs[i] != -1; i++)
    Nominator += rho_wts[i] / rho_degs[i];

  Denominator = 0;
  for (int j = 0; lambda_degs[j] != -1; j++)
    Denominator += lambda_wts[j] / lambda_degs[j];

  Ratio = Nominator / Denominator;

  N = p_N;
  M = (int)ceil(Ratio * N);
  E = (int)floor(N / Denominator);
  
  //----------------------------------------------------------------------------------------
  // Create variables, checks and edges
  //----------------------------------------------------------------------------------------
  edges = new edge[E];
  variable_nodes = new variable_node[N];
  check_nodes = new check_node[M];
  EdgeStack = new edge* [E * 2];   // Buffer used to manage memory allocation for nodes

  if ((edges == NULL) ||
      (variable_nodes == NULL) ||
      (check_nodes == NULL) ||
      (EdgeStack == NULL))
    {
      cout << "Error allocating memory\n";
      exit(1);
    }

  //--------------------------------------------------------------------------
  // Generate sockets
  //--------------------------------------------------------------------------
  variable_node **left_sockets = new variable_node*[E];
  check_node **right_sockets = new check_node*[E];
  edge **EdgeStackPointer = EdgeStack;         // Auxiliary pointer to stack
  int node_index, socket_index, count_nodes_of_degree;
  
  node_index = 0;
  socket_index = 0;
  for (int i = 0; lambda_degs[i] != -1; i++)                // Loop through all left-degrees
    {
      int count_nodes_of_degree = (int)floor(lambda_wts[i] * E / lambda_degs[i]); // No. nodes of required degree
      for (int j = 0; j < count_nodes_of_degree; j++)    // Number of nodes with required left-degree
	{
	  for (int k = 0; k < lambda_degs[i]; k++)              // Number of sockets for each degree
	    left_sockets[socket_index++] = &variable_nodes[node_index];

	  variable_nodes[node_index].AllocateEdges(EdgeStackPointer, lambda_degs[i]);
	  variable_nodes[node_index].SetMapInUse(MapInUse);
	  variable_nodes[node_index].SetID(node_index);

	  node_index++;
	}
    }

  // Record no. of left sockets, may be different than E
  // due to the fact that lambda * E may not be whole numbers
  int count_left_sockets = socket_index;      

  // Modify N for same reason
  N = node_index;

  // Generate right sockets
  node_index = 0;
  socket_index = 0;
  for (int i = 0; rho_degs[i] != -1; i++)                  // Loop through all right-degrees
    {
      int CurrentDegree  = rho_degs[i];
      count_nodes_of_degree = (int)floor(rho_wts[i] * E / 
					 CurrentDegree );  // No of nodes of required degree
      for (int j = 0; j < count_nodes_of_degree; j++)         
	{
	  for (int k = 0; k < CurrentDegree; k++)      // Number of sockets for each degree
	    right_sockets[socket_index++] = &check_nodes[node_index];

	  check_nodes[node_index].AllocateEdges( EdgeStackPointer, CurrentDegree );
	  check_nodes[node_index].SetID(node_index);
	  
	  node_index++;
	}
    }
  // Record no. of right sockets, may be different than E
  // due to the fact that lambda * E may not be whole numbers
  int count_right_sockets = socket_index;      

  // Modify M,E for same reason
  M = node_index;
  E = min(count_left_sockets, count_right_sockets);

  //----------------------------------------------------------------------------------------
  // Generate permutations
  //----------------------------------------------------------------------------------------
  srand(time(NULL)); // Init random seed so that each call to function returns different set of values

  cout << "Starting bipartite graph...";

  int left_index, right_index;
  for (int i = 0; i < E; i++)
    {
      // Randomly select socket from first E - left_index (last left_index sockets represent
      // sockets that have already been selected)
      //      cout << " i = " << i << " E = " << E;
      int attempts = 0;

      do
	{
	  // It is important to select left_index randomly and not only right_index,
	  // because there is a significance to the order of bits within a code, sometimes with 
	  // first or last bits better protected.  If left_index is not selected randomly,
	  // the result would be a random tree, but in which lower degree left-nodes are of
	  // lower index within each constituent code, contrary to complete randomness.
	  left_index = uniform_random(E - i);
	  //left_index = i;
	  right_index = uniform_random(E - i);
	  if ((attempts++) > 100)
	    {
	      cout << "Warning: cycles\n";
	      break;
	    }
	}
    while (left_sockets[left_index]->IsRightConnectedTo(right_sockets[right_index]));

      if (right_index >= (E-i))
	{
	  cout << "right index or left_index exceed range\n"
	       << "right_index = " << right_index
	       << " left_index = " << left_index
               << " E = " << E 
               << " left_index = " << left_index << "\n";
	  exit(1);
	}

      edges[i].set_nodes(left_sockets[left_index], right_sockets[right_index]);

      // Overwrite current sockets with last sockets, so that they are not selected again
      right_sockets[right_index] = right_sockets[E - i - 1];
      left_sockets[left_index] = left_sockets[E - i - 1];
    }

  cout << "Done\n";

  // Clean-up
  delete left_sockets;
  delete right_sockets;
}




/************************************************************************
 *
 * LDPC code
 *
 ************************************************************************/


/**
 * Initialize Variable nodes with channel output
 */
void LDPC_Code::Init_Messages( vector &ChannelOutput )
{
  if (ChannelOutput.GetSize() != Variables.GetLength())
  {
     cout << "LDPC_Code::Init_Messages: Incompatible sizes\n";
     exit(1);
  }

  for (int i = 0; i < Variables.GetLength(); i++)
     Variables[i].Initialize( *Channel, ChannelOutput[i] );
}





void LDPC_Code::GetZeroCodeword( vector &Codeword )
{
   Codeword.Allocate(Variables.GetLength());

   for (int i = 0; i < Variables.GetLength(); i++)
      Codeword[i] = Variables[i].GetZeroSignal();
}





void LDPC_Code::Leftbound_Iteration()
{
  for (int i = 0; i < Checks.GetLength(); i++)
    Checks[i].CalcAllLeftboundMessages();
}




void LDPC_Code::Rightbound_Iteration()
{
  for (int i = 0; i < Variables.GetLength(); i++)
     Variables[i].CalcAllRightboundMessages();
}




void LDPC_Code::FinalIteration()
{
  for (int i = 0; i < Variables.GetLength(); i++)
     Variables[i].CalcFinalMessage();
}




double LDPC_Code::Calc_Symbol_Error_Rate()
{
  double acc_correct = 0;

  for (int i = 0; i < Variables.GetLength(); i++)
     acc_correct += Variables[i].FinalEstimate.Decision().IsZero();

  return 1.0 - acc_correct / (double)Variables.GetLength();
}




double LDPC_Code::Calc_Rightbound_Symbol_Error_Rate()
{
  double acc_correct = 0;

  for (int i = 0; i < Graph.E; i++)
    acc_correct += Graph.edges[i].RightBoundMessage.Decision().IsZero();

  return 1.0 - acc_correct / Graph.E;
}







double LDPC_Code::Belief_Propagation_Decoder(int Count_Iterations)
{
  static char buffer[500];
  double Func_RC=0; //=0 is for initialization of the variable so the compiler won't give warnings
  double LastMin = INF;
  int CountIncreaseIterations = 0;

  cout << "Initial symbol error rate = " << Calc_Symbol_Error_Rate() << "\n";

  for (int i = 0; i < Count_Iterations; i++)
  {

      Leftbound_Iteration( );
      Rightbound_Iteration();

      Func_RC = Calc_Rightbound_Symbol_Error_Rate();
      sprintf(buffer, "%d: Rightbound SER = %1.10f, %s", i+1, Func_RC, CharTime());
      cout << buffer;

      // Stop when Func_RC doesn't fall for some consecutive iterations
      if (Func_RC < LastMin)
      {
         LastMin = Func_RC;
         CountIncreaseIterations = 0;
      }
      else
      {
         CountIncreaseIterations++;
         if (CountIncreaseIterations > 50)
            break;
      }

      if (Func_RC < 1e-7) break;
    }

   return Func_RC;
}






