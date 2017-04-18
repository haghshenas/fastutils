#include "common.hpp"
#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

string       _fxsubseq_in_path          = "";
FILE         *_fxsubseq_in_file;
string       _fxsubseq_out_path         = "";
ofstream     _fxsubseq_out_file;
ostream      *_fxsubseq_out_pointer     = NULL;

void printHelp_fxsubseq()
{
	cerr << endl;
	cerr << "USAGE: fastutils fxsubseq [options] <name:start-end>" << endl;
	cerr << endl;
	cerr << "Required options:" << endl;
	cerr << "         -i STR        input file in fastx format. Use - for stdin." << endl;
	cerr << "         -o STR        output file. Use - for stdout." << endl;
	cerr << endl;
	cerr << "More options:" << endl;
	cerr << "         -v            print version and build date" << endl;
	cerr << "         -h            print this help" << endl;
	cerr << endl;
}

int parseCommandLine_fxsubseq(int argc, char *argv[])
{
	int index, c;

	static struct option longOptions[] =
	{
		{"in",          required_argument,      0,      'i' },
		{"out",         required_argument,      0,      'o' },
		{"version",     no_argument,            0,      'v' },
		{"help",        no_argument,            0,      'h' },
		{0,0,0,0}
	};

	while ( (c = getopt_long ( argc, argv, "i:o:vh", longOptions, &index))!= -1 )
	{
		switch (c)
		{
			case 'i':
				_fxsubseq_in_path = optarg;
				break;
			case 'o':
				_fxsubseq_out_path = optarg;
				break;
			case 'h':
				printHelp_fxsubseq();
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				cerr << endl;
				cerr << "fastutils Version " << FASTUTILS_VERSION << endl;
				cerr << endl;
				exit(EXIT_SUCCESS);
				break;
			default:
				cerr << "[ERROR] please run fastutils stat -h to see the help." << endl << endl;
				return 0;
		}
	}

	if(_fxsubseq_in_path == "")
	{
		cerr<< "[ERROR] option -i/--in is required" << endl << endl;
		return 0;
	}
	else
	{
		if(_fxsubseq_in_path == "-")
		{
			_fxsubseq_in_file = stdin;
		}
		else
		{
			_fxsubseq_in_file = fopen(_fxsubseq_in_path.c_str(), "r");
			if(_fxsubseq_in_file == NULL)
			{
				cerr<< "[ERROR] could not open file: " << _fxsubseq_in_path << endl << endl;
				return 0;
			}
		}
	}

	if(_fxsubseq_out_path == "")
	{
		cerr<< "[ERROR] option -o/--out is required" << endl << endl;
		return 0;
	}
	else
	{
		if(_fxsubseq_out_path == "-")
		{
			_fxsubseq_out_pointer = &cout;
		}
		else
		{
			_fxsubseq_out_file.open(_fxsubseq_out_path.c_str());
			if(_fxsubseq_out_file.is_open()==false)
			{
				cerr<< "[ERROR] could not open file: " << _fxsubseq_out_path << endl << endl;
				return 0;
			}
			_fxsubseq_out_pointer = &_fxsubseq_out_file;	
		}
	}

	return 1;
}

int program_fxsubseq(int argc, char* argv[])
{
	if(argc < 3)
	{
		printHelp_fxsubseq();
		return EXIT_SUCCESS;
	}

	if(!parseCommandLine_fxsubseq(argc, argv))
	{
		return EXIT_FAILURE;
	}

	ostream &outObj = *_fxsubseq_out_pointer;

	gzFile fp = gzdopen(fileno(_fxsubseq_in_file), "r");
	if(fp == NULL)
	{
		cerr << "Cannot open file: " << (_fxsubseq_in_path == "-" ? "stdin" : _fxsubseq_in_path) << endl;
		exit(1);
	}

	map<string, string> allSeq;

	kseq_t *seq = kseq_init(fp);
	while (kseq_read(seq) >= 0)
	{
		allSeq[seq->name.s] = seq->seq.s;
	}
	kseq_destroy(seq);
	gzclose(fp);

	optind++; // to ignore fxsubseq
	if(optind < argc)
	{
		while(optind < argc)
		{
			string region = argv[optind];
			string seqName = region.substr(0, region.find(':'));
			int seqStart = str2type<int>( region.substr(region.find(':')+1, region.find('-') - region.find(':') - 1) );
			int seqEnd = str2type<int>( region.substr(region.find('-')+1) );
			if(seqStart < seqEnd)
			{
				cout<< allSeq[seqName].substr(seqStart, seqEnd-seqStart) << "\n";
			}

			optind++;
		}
	}
	else
	{
		cerr<< "[ERROR] no region to extract!" << endl;
		return 0;
	}

	return 0;
}