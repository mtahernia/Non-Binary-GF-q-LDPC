#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include "LDPC_1.h"
#include "LDPC_2.h"
#include "Utils_1.h"

BOOLEAN debug = FALSE;

/*****************************************************************************
 *
 * MAIN
 *
 *****************************************************************************/

int main(int argc, char **argv)
{
  //-------------------------------------------------------------------
  // Command line arguments
  //-------------------------------------------------------------------
  int iterations = 5000;
  char infilename[100];
  int BlockLength = 50000;
  int count_runs = 100;
  char ChannelType = 'G'; 
  char *CurrentOption;
  channel *Channel;
  char *OutputLogFileName = "results.txt";
  unsigned seed = RandomSeed();

  if (argc < 3)
    {
      ReportBuf.OpenFile(OutputLogFileName);
      cout << "usage: "
	   << argv[0] << " <input file> <SNR (dB)/crossover> {<options>}\n"
           << "Options: \n"
           << "   -c : Channel (G)aussian, (B)SC default: " << ChannelType << "\n"
           << "   -i : maximum iterations, default: " << iterations  << "\n"
		   << "   -b : block length, default: " << BlockLength << "\n"
           << "   -r : number of runs, default: " << count_runs << "\n"
           << "   -s : seed (default = random, based on time)\n"
           << "   -o : output log file, \"err\" means direct to stderr, default: " << OutputLogFileName << "\n";

      return -1;
    }

  sscanf(argv[1], "%s", infilename);

  for (int i = 3; i < argc; i++)
  {
     CurrentOption = argv[i];
     if (CurrentOption[0] != '-')
     {
        cout << "Invalid command line parameter #" << i << ": " << CurrentOption << "\n";
        exit(1);
     }

     switch(CurrentOption[1])
     {
     case 'c':
        ChannelType = CurrentOption[2];
        break;
     case 'i':
        sscanf(CurrentOption + 2, "%d", &iterations);
        break;
     case 'b':
        sscanf(CurrentOption + 2, "%d", &BlockLength);
        break;
     case 'r':
        sscanf(CurrentOption + 2, "%d", &count_runs);
        break;
     case 's':
        sscanf(CurrentOption + 2, "%d", &seed);
        break;
     case 'o':
        OutputLogFileName = CurrentOption + 2;
        break;
     default:
        cout << "Invalid option#" << i << ": " << CurrentOption << "\n";
        exit(1);
     }
  }

  //-------------------------------------------------------------
  // Log file
  //-------------------------------------------------------------
  ReportBuf.OpenFile(OutputLogFileName);

  //-------------------------------------------------------------
  // Init seed
  //-------------------------------------------------------------
  my_srand(seed);
  
  //-------------------------------------------------------------------
  // Code
  //-------------------------------------------------------------------
  std::ifstream DefinitionFile;
  DefinitionFile.open(infilename);

  if (!DefinitionFile)
  {
      cout << "Error opening file " << infilename << "\n";
      exit(1);
  }
  
  // Define the LDPC code based on information provided in the DefinitionFile
  LDPC_Code Code(DefinitionFile, BlockLength);

  //-------------------------------------------------------------
  // Handle channel
  //-------------------------------------------------------------
  double channel_p;
  double noise_sigma;
  double SNR_dB, No, SNR;

  switch(ChannelType)
  {
  case 'G':
     noise_sigma = 0;         // To avoid compiler warning
     Channel = new AWGN_Channel;

     sscanf(argv[2], "%lf", &SNR_dB);

     SNR = pow(10., SNR_dB / 10.);
     No = 1./SNR;
     noise_sigma = sqrt(No);
     ((AWGN_Channel*)Channel)->SetNoiseSigma(noise_sigma);
     break;
  case 'B':
     Channel = new BSC_Channel;

     sscanf(argv[2], "%lf", &channel_p);
   
     ((BSC_Channel*)Channel)->SetChannel_p(channel_p);
     break;
  default:
     cout << "Invalid channel selection\n";
     exit(1);
  }

  // Connect channel with code
  Code.SetChannel(*Channel);
  Channel->ProcessMapping(Code);

  //-------------------------------------------------------------------
  // Print channel data
  //-------------------------------------------------------------------
  double Rate = Code.Calc_Symbol_Rate();
  cout << "----------------------------------------------------------------------------\n"
       << "Symbol Rate = " << Rate 
       << " Bit Rate = " << Code.Calc_Bit_Rate()
       << " Iterations = " << iterations
       << "\nSum Lambda = " << Code.SumLambda()
       << " Sum Rhos = " << Code.SumRho() 
       << " Block length = " << BlockLength
       << " Channel = " << ChannelType 
       << "\nSeed = " << seed
       << "\n";

  Channel->PrintChannelData(Code);
  cout << "\n----------------------------------------------------------------------------\n";
  
  //------------------------------------------------------------------------
  // Go
  //------------------------------------------------------------------------
  double AccumulatedSER;
  vector Codeword, ChannelOutput;

  AccumulatedSER = 0;

  for (int i = 0; i < count_runs; i++)
  {
	Code.ResetGraph();
    Code.GetZeroCodeword( Codeword );

	Channel->SimulateOutputVector( Codeword, ChannelOutput );
    Code.Init_Messages( ChannelOutput );
	AccumulatedSER += Code.Belief_Propagation_Decoder(iterations);
  }

  //------------------------------------------------------------------------
  // return OK
  //------------------------------------------------------------------------
  delete Channel;
  return 0;
}
