#include "common.hpp"
#include "kseq.h"

KSEQ_INIT(gzFile, gzread)

vector<string> _interleave_files1;
vector<string> _interleave_files2;
string         _interleave_out_path = "";
FILE           *_interleave_out_file;
bool 	       _interleave_isFastq = false;
char 	       _interleave_sep_char = '.';

void printHelp_interleave()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "USAGE: fastutils interleave [options] -1 lib1_1.fq -2 lib1_2.fq [-1 lib2_1.fq -2 lib2_2.fq ...]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "I/O options:\n");
    fprintf(stderr, "     -1,--in1 STR           fasta/q file containing forward (left) reads [required]\n");
    fprintf(stderr, "     -2,--in2 STR           fasta/q file containing reverse (right) reads [required]\n");
    fprintf(stderr, "     -o,--out STR           output interlaced reads in STR file [stdout]\n");
    fprintf(stderr, "More options:\n");
    fprintf(stderr, "     -q,--fastq              output reads in fastq format if possible\n");
    fprintf(stderr, "     -s,--separator CHR     separator character [.]\n");
    fprintf(stderr, "     -h,--help              print this help\n");
    fprintf(stderr, "\n");
}

int parseCommandLine_interleave(int argc, char *argv[])
{
    int index, c;

    static struct option longOptions[] =
    {
        {"in1", 		required_argument, 		0, 		'1'	},
        {"in2", 		required_argument, 		0, 		'2'	},
        {"out", 		required_argument, 		0, 		'o'	},
        {"fastq", 		no_argument, 			0, 		'q'	},
        {"separator", 	required_argument, 		0, 		's'	},
        {"help", 		no_argument, 			0, 		'h'	},
        {"version", 	no_argument, 			0, 		'v'	},
        {0,0,0,0}
    };

    while ( (c = getopt_long ( argc, argv, "1:2:o:s:qhv", longOptions, &index))!= -1 )
    {
        switch (c)
        {
            case '1':
                _interleave_files1.push_back(optarg);
                break;
            case '2':
                _interleave_files2.push_back(optarg);
                break;
            case 'o':
                _interleave_out_path = optarg;
                break;
            case 's':
                _interleave_sep_char = optarg[0];
                break;
            case 'q':
                _interleave_isFastq = true;
                break;
            case 'h':
                printHelp_interleave();
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "[ERROR] run \"fastutils interleave -h\" to see the help\n");
                fprintf(stderr, "\n");
                return 1;
        }
    }

    if(_interleave_files1.size() == 0 && _interleave_files2.size() == 0)
    {
        fprintf(stderr, "[ERROR] options -1 and -2 are required\n");
        fprintf(stderr, "\n");
        return 1;
    }

    if(_interleave_files1.size() != _interleave_files2.size())
    {
        fprintf(stderr, "[ERROR] different number of libraries for forward and reverse reads\n");
        fprintf(stderr, "\n");
        return 1;
    }

    if(_interleave_out_path == "")
    {
        _interleave_out_file = stdout;
    }
    else
    {
        _interleave_out_file = fopen(_interleave_out_path.c_str(), "w");
        if(_interleave_out_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _interleave_out_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    return 0;
}

int command_interleave(int argc, char* argv[])
{
    if(parseCommandLine_interleave(argc, argv))
    {
        return EXIT_FAILURE;
    }

    for(int i = 0; i < _interleave_files1.size(); i++)
    {
        gzFile readFile1;
        gzFile readFile2;
        kseq_t *readSeq1;
        kseq_t *readSeq2;
        readFile1 = gzopen(_interleave_files1[i].c_str(), "r");
        readFile2 = gzopen(_interleave_files2[i].c_str(), "r");
        if(readFile1==NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _interleave_files1[i].c_str());
            exit(EXIT_FAILURE);
        }
        if(readFile2==NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _interleave_files2[i].c_str());
            exit(EXIT_FAILURE);
        }
        
        readSeq1 = kseq_init(readFile1);
        readSeq2 = kseq_init(readFile2);
        while (kseq_read(readSeq1) >= 0)
        {
            kseq_read(readSeq2);
            if(_interleave_isFastq && readSeq1->qual.l>0)
            {
                fprintf(_interleave_out_file, "@%s%c1\n", readSeq1->name.s, _interleave_sep_char);
                fprintf(_interleave_out_file, "%s\n", readSeq1->seq.s);
                fprintf(_interleave_out_file, "+\n");
                fprintf(_interleave_out_file, "%s\n", readSeq1->qual.s);
                fprintf(_interleave_out_file, "@%s%c2\n", readSeq2->name.s, _interleave_sep_char);
                fprintf(_interleave_out_file, "%s\n", readSeq2->seq.s);
                fprintf(_interleave_out_file, "+\n");
                fprintf(_interleave_out_file, "%s\n", readSeq2->qual.s);
            }
            else
            {
                fprintf(_interleave_out_file, ">%s%c1\n", readSeq1->name.s, _interleave_sep_char);
                fprintf(_interleave_out_file, "%s\n", readSeq1->seq.s);
                fprintf(_interleave_out_file, ">%s%c2\n", readSeq2->name.s, _interleave_sep_char);
                fprintf(_interleave_out_file, "%s\n", readSeq2->seq.s);
            }
        }
        kseq_destroy(readSeq1);
        kseq_destroy(readSeq2);
        gzclose(readFile1);
        gzclose(readFile2);
    }

    // if(_interleave_out_path != "")
    // {
    // 	fclose(_interleave_out_file);
    // }
    // return EXIT_SUCCESS;
}
