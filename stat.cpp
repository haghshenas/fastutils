#include "common.hpp"
#include "kseq.h"

KSEQ_INIT(gzFile, gzread)

string      _fxstat_in_path          = "";
FILE        *_fxstat_in_file;
string      _fxstat_out_path         = "";
FILE        *_fxstat_out_file;
int64_t     _fxstat_min_len          = 0;
int64_t     _fxstat_max_len          = LLONG_MAX;

void printHelp_stat()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: fastutils stat [options]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "I/O options:\n");
    fprintf(stderr, "     -i,--in STR         input file in fasta/q format [stdin]\n");
    fprintf(stderr, "     -o,--out STR        output file [stdout]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "More options:\n");
    fprintf(stderr, "     -m,--minLen INT     min read length [0]\n");
    fprintf(stderr, "     -M,--maxLen INT     max read length [INT64_MAX]\n");
    fprintf(stderr, "     -h,--help           print this help\n");
    fprintf(stderr, "\n");
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
                printHelp_stat();
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "[ERROR] please run \"fastutils stat -h\" to see the help.\n");
                fprintf(stderr, "\n");
                return 1;
        }
    }

    if(_fxstat_in_path == "")
    {
        _fxstat_in_file = stdin;
    }
    else
    {
        _fxstat_in_file = fopen(_fxstat_in_path.c_str(), "r");
        if(_fxstat_in_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _fxstat_in_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    if(_fxstat_out_path == "")
    {
        _fxstat_out_file = stdout;
    }
    else
    {
        _fxstat_out_file = fopen(_fxstat_out_path.c_str(), "w");
        if(_fxstat_out_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _fxstat_out_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    if(_fxstat_min_len < 0)
        _fxstat_min_len = 0;
    if(_fxstat_max_len < 0)
        _fxstat_max_len = LLONG_MAX;
    if(_fxstat_min_len > _fxstat_max_len)
    {
        fprintf(stderr, "[ERROR] --minLen cannot be greater than --maxLen\n");
        fprintf(stderr, "\n");
        return 1;
    }

    return 0;
}

int command_stat(int argc, char* argv[])
{
    if(parseCommandLine_stat(argc, argv))
    {
        return EXIT_FAILURE;
    }

    uint64_t cnt[128] = {0};
    uint64_t num = 0;
    uint64_t sLen = 0;
    int i;

    gzFile fp = gzdopen(fileno(_fxstat_in_file), "r");
    if(fp == NULL)
    {
        cerr << "Cannot open file: " << (_fxstat_in_path == "-" ? "stdin" : _fxstat_in_path) << endl;
        exit(EXIT_FAILURE);
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

    fprintf(_fxstat_out_file, "# reads: %llu\n", num);
    fprintf(_fxstat_out_file, "# bases: %llu\n", sLen);
    for(i=0; i<128; i++)
    {
        if(cnt[i])
        {
            fprintf(_fxstat_out_file, "# %c: %llu %.2lf\n", i, cnt[i], (double)cnt[i]/sLen*100);
        }
    }

    return EXIT_SUCCESS;
}