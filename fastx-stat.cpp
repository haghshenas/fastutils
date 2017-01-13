#include <iostream>
#include <fstream>
#include <iomanip>
#include <getopt.h>
#include <zlib.h>
#include "common.h"
#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

using namespace std;

typedef long long llong;

string       in_path          = "";
FILE         *in_file;
string       out_path         = "";
ofstream     out_file;
ostream      *out_pointer     = NULL;
llong        min_len          = 0;
llong        max_len          = LLONG_MAX;

void printHelp()
{
	cerr << "USAGE: fastx-stat -i (-|in.fastx) -o (-|out.txt) [options]" << endl;
	cerr << "Required options:" << endl;
	cerr << "         -f STR        input file in fastx format. Use - for stdin." << endl;
	cerr << "         -o STR        output file. Use - for stdout." << endl;
	cerr << "More options:" << endl;
	// cerr << "         -m INT        min read length [0]" << endl;
	// cerr << "         -M INT        max read length [LLONG_MAX]" << endl;
	cerr << "         -h            print this help" << endl;
	cerr << endl;
}

int parseCommandLine(int argc, char *argv[])
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
				in_path = optarg;
				break;
			case 'o':
				out_path = optarg;
				break;
			case 'm':
				min_len = str2type<long long>(optarg);
				break;
			case 'M':
				max_len = str2type<long long>(optarg);
				break;
			case 'h':
				printHelp();
				exit(EXIT_SUCCESS);
			default:
				cerr << "[ERROR] please run fastx-stat -h to see the help." << endl << endl;
				return 0;
		}
	}

	if(in_path == "")
	{
		cerr<< "[ERROR] option -i/--in is required" << endl << endl;
		return 0;
	}
	else
	{
		if(in_path == "-")
		{
			in_file = stdin;
		}
		else
		{
			in_file = fopen(in_path.c_str(), "r");
			if(in_file == NULL)
			{
				cerr<< "[ERROR] could not open file: " << in_path << endl << endl;
				return 0;
			}
		}
	}

	if(out_path == "")
	{
		cerr<< "[ERROR] option -o/--out is required" << endl << endl;
		return 0;
	}
	else
	{
		if(out_path == "-")
		{
			out_pointer = &cout;
		}
		else
		{
			out_file.open(out_path.c_str());
			if(out_file.is_open()==false)
			{
				cerr<< "[ERROR] could not open file: " << out_path << endl << endl;
				return 0;
			}
			out_pointer = &out_file;
		}
	}

	if(min_len < 0)
		min_len = 0;
	if(max_len < 0)
		max_len = LLONG_MAX;
	if(min_len > max_len)
	{
		cerr<< "[ERROR] minLen cannot be greater than maxLen" << endl << endl;
		return 0;
	}

	return 1;
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printHelp();
		return EXIT_SUCCESS;
	}

	if(!parseCommandLine(argc, argv))
	{
		return EXIT_FAILURE;
	}
	ostream &outObj = *out_pointer;

	llong cnt[128] = {0};
	llong num = 0;
	llong sLen = 0;
	int i;

	gzFile fp = gzdopen(fileno(in_file), "r");
	if(fp == NULL)
	{
		cerr << "Cannot open file: " << (in_path == "-" ? "stdin" : in_path) << endl;
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

	cout<< "# reads: " << num << endl;
	cout<< "# bases: " << sLen << endl;
	cout<< fixed << setprecision(2);
	for(i=0; i<128; i++)
	{
		if(cnt[i])
		{
			cout<< "# " << (char)i << ": " << cnt[i] << " " << (double)cnt[i]/sLen*100 << "\n";
		}
	}

	return 0;
}