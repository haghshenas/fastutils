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
		string com = argv[1];
		if(com == "stat")
		{
			return program_stat(argc, argv);	
		}
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
	cerr<< "USAGE: fastutils <command> [options]" << endl;
	cerr<< endl;
	cerr<< "command: stat         prints general statistics of fastx files" << endl;
	cerr<< endl;
}
