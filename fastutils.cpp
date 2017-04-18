#include "common.hpp"
#include "fxstat.hpp"
#include "fxsubseq.hpp"
#include "fa2contig.hpp"

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
		if(com == "fxstat")
		{
			return program_fxstat(argc, argv);	
		}
		else if(com == "fxsubseq")
		{
			return program_fxsubseq(argc, argv);
		}
		else if(com == "fa2contig")
		{
			return program_fa2contig(argc, argv);
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
	cerr<< "command: fxstat       prints general statistics of fastx files" << endl;
	cerr<< "         fxsubseq     extracts a subsequence from the fastx file" << endl;
	cerr<< "         fa2contig    splits fasta file into contigs" << endl;
	cerr<< endl;
}
