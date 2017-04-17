#include "common.hpp"
#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

string       _fxstat_in_path          = "";
FILE         *_fxstat_in_file;
string       _fxstat_out_path         = "";
ofstream     _fxstat_out_file;
ostream      *_fxstat_out_pointer     = NULL;
int64_t      _fxstat_min_len          = 0;
int64_t      _fxstat_max_len          = LLONG_MAX;

void printHelp_fxstat()
{
	cerr << endl;
	cerr << "USAGE: fastutils fxstat [options]" << endl;
	cerr << endl;
	cerr << "Required options:" << endl;
	cerr << "         -i STR        input file in fastx format. Use - for stdin." << endl;
	cerr << "         -o STR        output file. Use - for stdout." << endl;
	cerr << endl;
	cerr << "More options:" << endl;
	cerr << "         -m INT        min read length [0]" << endl;
	cerr << "         -M INT        max read length [INT64_MAX]" << endl;
	cerr << "         -v            print version and build date" << endl;
	cerr << "         -h            print this help" << endl;
	cerr << endl;
}

int parseCommandLine_fxstat(int argc, char *argv[])
{
	int index, c;

	static struct option longOptions[] =
	{
		{"in",          required_argument,      0,      'i' },
		{"out",         required_argument,      0,      'o' },
		{"minLen",      required_argument,      0,      'm' },
		{"maxLen",      required_argument,      0,      'M' },
		{"version",     no_argument,            0,      'v' },
		{"help",        no_argument,            0,      'h' },
		{0,0,0,0}
	};

	while ( (c = getopt_long ( argc, argv, "i:o:m:M:vh", longOptions, &index))!= -1 )
	{
		switch (c)
		{
			case 'i':
				_fxstat_in_path = optarg;
				break;
			case 'o':
				_fxstat_out_path = optarg;
				break;
			case 'm':
				_fxstat_min_len = str2type<int64_t>(optarg);
				break;
			case 'M':
				_fxstat_max_len = str2type<int64_t>(optarg);
				break;
			case 'h':
				printHelp_fxstat();
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

	if(_fxstat_in_path == "")
	{
		cerr<< "[ERROR] option -i/--in is required" << endl << endl;
		return 0;
	}
	else
	{
		if(_fxstat_in_path == "-")
		{
			_fxstat_in_file = stdin;
		}
		else
		{
			_fxstat_in_file = fopen(_fxstat_in_path.c_str(), "r");
			if(_fxstat_in_file == NULL)
			{
				cerr<< "[ERROR] could not open file: " << _fxstat_in_path << endl << endl;
				return 0;
			}
		}
	}

	if(_fxstat_out_path == "")
	{
		cerr<< "[ERROR] option -o/--out is required" << endl << endl;
		return 0;
	}
	else
	{
		if(_fxstat_out_path == "-")
		{
			_fxstat_out_pointer = &cout;
		}
		else
		{
			_fxstat_out_file.open(_fxstat_out_path.c_str());
			if(_fxstat_out_file.is_open()==false)
			{
				cerr<< "[ERROR] could not open file: " << _fxstat_out_path << endl << endl;
				return 0;
			}
			_fxstat_out_pointer = &_fxstat_out_file;	
		}
	}

	if(_fxstat_min_len < 0)
		_fxstat_min_len = 0;
	if(_fxstat_max_len < 0)
		_fxstat_max_len = LLONG_MAX;
	if(_fxstat_min_len > _fxstat_max_len)
	{
		cerr<< "[ERROR] minLen cannot be greater than maxLen" << endl << endl;
		return 0;
	}

	return 1;
}

int program_fxstat(int argc, char* argv[])
{
	if(argc < 3)
	{
		printHelp_fxstat();
		return EXIT_SUCCESS;
	}

	if(!parseCommandLine_fxstat(argc, argv))
	{
		return EXIT_FAILURE;
	}
	ostream &outObj = *_fxstat_out_pointer;

	uint64_t cnt[128] = {0};
	uint64_t num = 0;
	uint64_t sLen = 0;
	int i;

	gzFile fp = gzdopen(fileno(_fxstat_in_file), "r");
	if(fp == NULL)
	{
		cerr << "Cannot open file: " << (_fxstat_in_path == "-" ? "stdin" : _fxstat_in_path) << endl;
		exit(1);
	}
	kseq_t *seq = kseq_init(fp);
	while (kseq_read(seq) >= 0)
	{
		if(seq->seq.l >= _fxstat_min_len && seq->seq.l <= _fxstat_max_len)
		{
			num++;
			sLen += seq->seq.l;
			for(i=0; i<seq->seq.l; i++)
			{
				cnt[ seq->seq.s[i] ]++;
			}	
		}
	}
	kseq_destroy(seq);
	gzclose(fp);

	outObj<< "# reads: " << num << endl;
	outObj<< "# bases: " << sLen << endl;
	outObj<< fixed << setprecision(2);
	for(i=0; i<128; i++)
	{
		if(cnt[i])
		{
			outObj<< "# " << (char)i << ": " << cnt[i] << " " << (double)cnt[i]/sLen*100 << "\n";
		}
	}

	return 0;
}