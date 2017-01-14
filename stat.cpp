#include "common.h"
#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

using namespace std;

typedef long long llong;

string       _stat_in_path          = "";
FILE         *_stat_in_file;
string       _stat_out_path         = "";
ofstream     _stat_out_file;
ostream      *_stat_out_pointer     = NULL;
llong        _stat_min_len          = 0;
llong        _stat_max_len          = LLONG_MAX;

void printHelp_stat()
{
	cerr << endl;
	cerr << "USAGE: fastutils stat [options]" << endl;
	cerr << endl;
	cerr << "More options:" << endl;
	cerr << "         -f STR        input file in fastx format [stdin]" << endl;
	cerr << "         -o STR        output file [stdout]" << endl;
	// cerr << "         -m INT        min read length [0]" << endl;
	// cerr << "         -M INT        max read length [LLONG_MAX]" << endl;
	cerr << "         -h            print this help" << endl;
	cerr << endl;
}

int parseCommandLine_stat(int argc, char *argv[])
{
	int index, c;

	static struct option longOptions[] =
	{
		{"in",          required_argument,      0,      'i' },
		{"out",         required_argument,      0,      'o' },
		{"minLen",      required_argument,      0,      'm' },
		{"maxLen",      required_argument,      0,      'M' },
		{"help",        no_argument,            0,      'h' },
		{0,0,0,0}
	};

	while ( (c = getopt_long ( argc, argv, "i:o:m:M:h", longOptions, &index))!= -1 )
	{
		switch (c)
		{
			case 'i':
				_stat_in_path = optarg;
				break;
			case 'o':
				_stat_out_path = optarg;
				break;
			case 'm':
				_stat_min_len = str2type<long long>(optarg);
				break;
			case 'M':
				_stat_max_len = str2type<long long>(optarg);
				break;
			case 'h':
				printHelp_stat();
				exit(EXIT_SUCCESS);
			default:
				cerr << "[ERROR] please run fastutils stat -h to see the help." << endl << endl;
				return 0;
		}
	}

	if(_stat_in_path == "")
	{
		_stat_in_file = stdin;
	}
	else
	{
		_stat_in_file = fopen(_stat_in_path.c_str(), "r");
		if(_stat_in_file == NULL)
		{
			cerr<< "[ERROR] could not open file: " << _stat_in_path << endl << endl;
			return 0;
		}
	}

	if(_stat_out_path == "")
	{
		_stat_out_pointer = &cout;
	}
	else
	{
		_stat_out_file.open(_stat_out_path.c_str());
		if(_stat_out_file.is_open()==false)
		{
			cerr<< "[ERROR] could not open file: " << _stat_out_path << endl << endl;
			return 0;
		}
		_stat_out_pointer = &_stat_out_file;
	}

	if(_stat_min_len < 0)
		_stat_min_len = 0;
	if(_stat_max_len < 0)
		_stat_max_len = LLONG_MAX;
	if(_stat_min_len > _stat_max_len)
	{
		cerr<< "[ERROR] minLen cannot be greater than maxLen" << endl << endl;
		return 0;
	}

	return 1;
}

int program_stat(int argc, char* argv[])
{
	// if(argc < 3)
	// {
	// 	printHelp_stat();
	// 	return EXIT_SUCCESS;
	// }

	if(!parseCommandLine_stat(argc, argv))
	{
		return EXIT_FAILURE;
	}
	ostream &outObj = *_stat_out_pointer;

	llong cnt[128] = {0};
	llong num = 0;
	llong sLen = 0;
	int i;

	gzFile fp = gzdopen(fileno(_stat_in_file), "r");
	if(fp == NULL)
	{
		cerr << "Cannot open file: " << (_stat_in_path == "-" ? "stdin" : _stat_in_path) << endl;
		exit(1);
	}
	kseq_t *seq = kseq_init(fp);
	while (kseq_read(seq) >= 0)
	{
		num++;
		sLen += seq->seq.l;
		for(i=0; i<seq->seq.l; i++)
		{
			cnt[ seq->seq.s[i] ]++;
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