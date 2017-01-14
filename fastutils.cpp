#include "common.h"
#include "stat.h"

void printHelp_core();

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printHelp_core();
	}
	else
	{
		// initCommon();
		string com = argv[1];
		if(com == "stat")
		{
			return program_stat(argc, argv);	
		}
		// if(com == "shuffle")
		// {
		// 	return program_shuffle(argc, argv);
		// }
		// else if(com == "toLower")
		// {
		// 	return program_toLower(argc, argv);
		// }
		// else if(com == "split")
		// {
		// 	return program_split(argc, argv);
		// }
		// else if(com == "getStat")
		// {
		// 	return program_getStat(argc, argv);
		// }
		// else if(com == "format")
		// {
		// 	return program_format(argc, argv);
		// }
		// else if(com == "revComp")
		// {
		// 	return program_revComp(argc, argv);
		// }
		else
		{
			printHelp_core();
		}
	}
	return 0;
}

void printHelp_core()
{
	cerr<< endl;
	cerr<< "USAGE: fastUtils <command> [options]" << endl;
	cerr<< endl;
	cerr<< "command: stat         prints general statistics of fastx files" << endl;
	// cerr<< "         format       re-format the fasta/q file based on user's needs" << endl;
	// cerr<< "         shuffle      interleave/interlace fasta/q files" << endl;
	// cerr<< "         toLower      convert fasta/q reads to lowercase" << endl;
	// cerr<< "         revComp      convert each fasta/q reads to its reverse-complement" << endl;
	// cerr<< "         split        split a fasta/q file into smaller files" << endl;
	cerr<< endl;
}
