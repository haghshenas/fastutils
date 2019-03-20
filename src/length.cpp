#include "common.hpp"
#include "kseq.h"

KSEQ_INIT(gzFile, gzread)


string      _length_in_path     = "";
FILE        *_length_in_file;
string      _length_out_path    = "";
FILE        *_length_out_file;
long long   _length_minLen      = 0;
long long   _length_maxLen      = LLONG_MAX;
bool        _length_total       = false;

void printHelp_length()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: fastutils length [options]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "I/O options:\n");
    fprintf(stderr, "     -i,--in STR            input file in fasta/q format [stdin]\n");
    fprintf(stderr, "     -o,--out STR           output file [stdout]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "More options:\n");
    fprintf(stderr, "     -m,--minLen INT        min read length [0]\n");
    fprintf(stderr, "     -M,--maxLen INT        max read length [LLONG_MAX]\n");
    fprintf(stderr, "     -t,--total             print total number of bases in third column\n");
    fprintf(stderr, "     -h,--help              print this help\n");
    fprintf(stderr, "\n");
}

int parseCommandLine_length(int argc, char *argv[])
{
    int index, c;

    static struct option longOptions[] =
    {
        {"in",          required_argument,      0,      'i' },
        {"out",         required_argument,      0,      'o' },
        {"minLen",      required_argument,      0,      'm' },
        {"maxLen",      required_argument,      0,      'M' },
        {"total",       no_argument,            0,      't' },
        {"help",        no_argument,            0,      'h' },
        {0,0,0,0}
    };

    while ( (c = getopt_long ( argc, argv, "i:o:m:M:th", longOptions, &index))!= -1 )
    {
        switch (c)
        {
            case 'i':
                _length_in_path = optarg;
                break;
            case 'o':
                _length_out_path = optarg;
                break;
            case 'm':
                _length_minLen = str2type<long long>(optarg);
                break;
            case 'M':
                _length_maxLen = str2type<long long>(optarg);
                break;
            case 't':
                _length_total = true;
                break;
            case 'h':
                printHelp_length();
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "[ERROR] run \"fastutils length -h\" to see the help\n");
                fprintf(stderr, "\n");
                return 1;
        }
    }

    if(_length_in_path == "")
    {
        _length_in_file = stdin;
    }
    else
    {
        _length_in_file = fopen(_length_in_path.c_str(), "r");
        if(_length_in_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _length_in_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    if(_length_out_path == "")
    {
        _length_out_file = stdout;
    }
    else
    {
        _length_out_file = fopen(_length_out_path.c_str(), "w");
        if(_length_out_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _length_out_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    if(_length_minLen < 0)
        _length_minLen = 0;

    if(_length_maxLen < 0)
        _length_maxLen = LLONG_MAX;

    if(_length_minLen > _length_maxLen)
    {
        fprintf(stderr, "[ERROR] --minLen cannot be greater than --maxLen\n");
        fprintf(stderr, "\n");
        return 1;
    }

    return 0;
}

int command_length(int argc, char* argv[])
{
    if(parseCommandLine_length(argc, argv))
    {
        return EXIT_FAILURE;
    }

    gzFile readFile;
    ostringstream sout;
    kseq_t *readSeq;
    readFile = gzdopen(fileno(_length_in_file), "r");
    if(readFile==NULL)
    {
        fprintf(stderr, "[ERROR] Cannot open file: %s\n", _length_in_path.c_str());
        exit(EXIT_FAILURE);
    }
    
    readSeq = kseq_init(readFile);
    unsigned long long sum = 0;
    while (kseq_read(readSeq) >= 0)
    {
        if( readSeq->seq.l >= _length_minLen && 
            readSeq->seq.l <= _length_maxLen )
        {
            sum += readSeq->seq.l;
            if(_length_total)
                fprintf(_length_out_file, "%s\t%llu\t%llu\n", readSeq->name.s, readSeq->seq.l, sum);
            else
                fprintf(_length_out_file, "%s\t%llu\n", readSeq->name.s, readSeq->seq.l);
        }
    }
    kseq_destroy(readSeq);
    gzclose(readFile);

    if(_length_out_path != "")
    {
        fclose(_length_out_file);
    }

    return EXIT_SUCCESS;
}
