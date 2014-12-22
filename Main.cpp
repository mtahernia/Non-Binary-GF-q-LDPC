#include <cstdio> // sscanf
#include <cmath>  //pow sqrt ...
#include <cstdlib> // exit
//#include <iostream>
//#include <fstream>
//#include <ctype.h>
#include "Definitions.h"
#include "Portability.h" // my_srand
#include "LDPC.h"


BOOLEAN debug = FALSE;

/**
 *****************************************************************************
 *
 * MAIN
 *
 *****************************************************************************/

int main(int argc, char **argv) {
	//-------------------------------------------------------------------
	// Command line arguments and default values
	//-------------------------------------------------------------------
	int iterations = 5000;
	char infilename[100];
	int BlockLength = 50000;
	int count_runs = 100;
	char ChannelType = 'G';
	char *CurrentOption;
	char *CurrentValue;
	channel *Channel;
	char *OutputLogFileName = (char *)"results.txt";
	unsigned seed = RandomSeed();

	//-------------------------------------------------------------------
	// Parsing the input arguments
	//-------------------------------------------------------------------
	if (argc < 3) {		// If less
		ReportBuf.OpenFile(OutputLogFileName);  // Reportbuf is defined in Report.h
		cout << "usage: " << argv[0] // argv[0] is always the file name
				<< " <input file> <SNR (dB)/crossover> {<options>}\n"
				<< "Options: \n"
				<< "   -c : Channel (G)aussian, (B)SC default: " << ChannelType
				<< "\n" << "   -i : maximum iterations, default: " << iterations
				<< "\n" << "   -b : block length, default: " << BlockLength
				<< "\n" << "   -r : number of runs, default: " << count_runs
				<< "\n" << "   -s : seed (default = random, based on time)\n"
				<< "   -o : output log file, \"err\" means direct to stderr, default: "
				<< OutputLogFileName << "\n";

		return -1;
	}

	sscanf(argv[1], "%s", infilename);/** Get the input file name */
	// argv indices: 0(file name), 1(input file), 2(channel SNR)

	for (int i = 3; i < argc; i+=2) /** Start checking the input arguments which start with dash.*/
	{
		CurrentOption = argv[i];
		CurrentValue  = argv[i+1];
		if (CurrentOption[0] != '-') /** An Argument should start with "-" */
		{
			cout << "Invalid command line parameter #" << i << ": "
					<< CurrentOption << "\n";
			exit(1);
		}

		switch (CurrentOption[1]) {
		case 'c':
			ChannelType = CurrentValue[0];
			break;
		case 'i':
			sscanf(CurrentValue, "%d", &iterations);
			break;
		case 'b':
			sscanf(CurrentValue, "%d", &BlockLength);
			break;
		case 'r':
			sscanf(CurrentValue, "%d", &count_runs);
			break;
		case 's':
			sscanf(CurrentValue, "%d", &seed);
			break;
		case 'o':
			OutputLogFileName = CurrentValue;
			break;
		default:
			cout << "Invalid option#" << i << ": " << CurrentOption << " " << CurrentValue << "\n";
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
	// Open the input file to read the definitions
	DefinitionFile.open(infilename);

	if (!DefinitionFile) {
		cout << "Error opening file " << infilename << "\n";
		exit(1);
	}


	//-------------------------------------------------------------
	// Handle channel
	//-------------------------------------------------------------
	double channel_p = 0;  // BSC Channel Parameter
	double noise_sigma = 0;  // (\sigma)^0.5
	double SNR_dB, No, SNR;

	// Parse ChannelType input
	switch (ChannelType) {
	case 'G':

//		noise_sigma = 0;         // To avoid compiler warning (moved it to above)
		Channel = new AWGN_Channel;

		// If channel is gaussian, the next input will be SNR in dB
		sscanf(argv[2], "%lf", &SNR_dB);
		SNR = pow(10., SNR_dB / 10.);
		No = 1. / SNR;
		noise_sigma = sqrt(No);

		// Because Channel is from base class channel, we need to cast it to the real thing, which is a child class
		// Now to cast between different classes, and to be safe, we use dynamic_cast. if the casting is not safe, it will return false.
		(dynamic_cast<AWGN_Channel*>(Channel))->SetNoiseSigma(noise_sigma);
		break;

	case 'B':
		Channel = new BSC_Channel;

		sscanf(argv[2], "%lf", &channel_p);
		// Again dynamic cast to child class
		(dynamic_cast<BSC_Channel*> (Channel))->SetChannel_p(channel_p);
		break;

	default:
		cout << "Invalid channel selection\n";
		exit(1);
	}

	// Define the LDPC code based on information provided in the DefinitionFile
	LDPC_Code Code(DefinitionFile, BlockLength);

	// Connect channel with code
	Code.SetChannel(*Channel);
	Channel->ProcessMapping(Code); // Calls Normalize function of mapping of the code, looks ridiculous, we could just called: Code.MapInUse.Normalize();
	//-------------------------------------------------------------------
	// Print channel data
	//-------------------------------------------------------------------
	double Rate = Code.Calc_Symbol_Rate();
	cout
			<< "----------------------------------------------------------------------------\n"
			<< "Symbol Rate = " << Rate << " Bit Rate = "
			<< Code.Calc_Bit_Rate() << " Iterations = " << iterations
			<< "\nSum Lambda = " << Code.SumLambda() << " Sum Rhos = "
			<< Code.SumRho() << " Block length = " << BlockLength
			<< " Channel = " << ChannelType << "\nSeed = " << seed << "\n";

	Channel->PrintChannelData(Code);
	cout<< "\n----------------------------------------------------------------------------\n";

	//------------------------------------------------------------------------
	// Go
	//------------------------------------------------------------------------
	double AccumulatedSER;
	vector Codeword, ChannelOutput;

	AccumulatedSER = 0;

	for (int i = 0; i < count_runs; i++) {
		Code.ResetGraph();
		Code.GenerateEncoder_WithoutGap();
//		Code.GenerateEncoder();
		Code.GenerateRandomSystematic();
		Code.Encode();
//		Code.GetZeroCodeword(Codeword);
		Code.GetCodeword(Codeword);

		Channel->SimulateOutputVector(Codeword, ChannelOutput);
		Code.Init_Messages(ChannelOutput);
		AccumulatedSER += Code.Belief_Propagation_Decoder(iterations);
	}
	cout << "Accumulated SER = " << AccumulatedSER << "\n";
	//------------------------------------------------------------------------
	// return OK
	//------------------------------------------------------------------------
	delete Channel;
	return 0;
}
