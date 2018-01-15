#include "common.hpp"
#include "kseq.h"

KSEQ_INIT(gzFile, gzread)


string      _format_in_path     = "";
FILE        *_format_in_file;
string      _format_out_path    = "";
FILE        *_format_out_file;
int         _format_lineWidth   = 0;
long long   _format_minLen      = 0;
long long   _format_maxLen      = LLONG_MAX;
bool        _format_isFastq     = false;
bool        _format_noN         = false;
bool        _format_noComment   = true;
bool        _format_pacbio      = false;
bool        _format_digital     = false;

void printHelp_format()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: fastutils format [options]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "I/O options:\n");
    fprintf(stderr, "     -i,--in STR            input file in fasta/q format [stdin]\n");
    fprintf(stderr, "     -o,--out STR           output file [stdout]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "More options:\n");
    fprintf(stderr, "     -w,--lineWidth INT     size of lines in fasta output. Use 0 for no wrapping [0]\n");
    fprintf(stderr, "     -m,--minLen INT        min read length [0]\n");
    fprintf(stderr, "     -M,--maxLen INT        max read length [LLONG_MAX]\n");
    fprintf(stderr, "     -q,--fastq             output reads in fastq format if possible\n");
    fprintf(stderr, "     -n,--noN               do not print entries with N's\n");
    fprintf(stderr, "     -c,--comment           print comments in headers\n");
    fprintf(stderr, "     -p,--pacbio            use pacbio's header format\n");
    fprintf(stderr, "     -d,--digital           use read index instead as read name\n");
    fprintf(stderr, "     -h,--help              print this help\n");
    fprintf(stderr, "\n");
}

int parseCommandLine_format(int argc, char *argv[])
{
    int index, c;

    static struct option longOptions[] =
    {
        {"in",          required_argument,      0,      'i' },
        {"out",         required_argument,      0,      'o' },
        {"lineWidth",   required_argument,      0,      'w' },
        {"minLen",      required_argument,      0,      'm' },
        {"maxLen",      required_argument,      0,      'M' },
        {"fastq",       no_argument,            0,      'q' },
        {"noN",         no_argument,            0,      'n' },
        {"comment",     no_argument,            0,      'c' },
        {"pacbio",      no_argument,            0,      'p' },
        {"digital",     no_argument,            0,      'd' },
        {"help",        no_argument,            0,      'h' },
        {0,0,0,0}
    };

    while ( (c = getopt_long ( argc, argv, "i:o:w:m:M:qncpdh", longOptions, &index))!= -1 )
    {
        switch (c)
        {
            case 'i':
                _format_in_path = optarg;
                break;
            case 'o':
                _format_out_path = optarg;
                break;
            case 'w':
                _format_lineWidth = str2type<int>(optarg);
                break;
            case 'm':
                _format_minLen = str2type<long long>(optarg);
                break;
            case 'M':
                _format_maxLen = str2type<long long>(optarg);
                break;
            case 'q':
                _format_isFastq = true;
                break;
            case 'n':
                _format_noN = true;
                break;
            case 'c':
                _format_noComment = false;
                break;
            case 'p':
                _format_pacbio = true;
                break;
            case 'd':
                _format_digital = true;
                break;
            case 'h':
                printHelp_format();
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "[ERROR] run \"fastutils format -h\" to see the help\n");
                fprintf(stderr, "\n");
                return 1;
        }
    }

    if(_format_in_path == "")
    {
        _format_in_file = stdin;
    }
    else
    {
        _format_in_file = fopen(_format_in_path.c_str(), "r");
        if(_format_in_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _format_in_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    if(_format_out_path == "")
    {
        _format_out_file = stdout;
    }
    else
    {
        _format_out_file = fopen(_format_out_path.c_str(), "w");
        if(_format_out_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _format_out_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    if(_format_lineWidth < 0)
        _format_lineWidth = 0;

    if(_format_minLen < 0)
        _format_minLen = 0;

    if(_format_maxLen < 0)
        _format_maxLen = LLONG_MAX;

    if(_format_minLen > _format_maxLen)
    {
        fprintf(stderr, "[ERROR] --minLen cannot be greater than --maxLen\n");
        fprintf(stderr, "\n");
        return 1;
    }

    return 0;
}

void printRead_format(FILE *fp, kseq_t *readSeq, unsigned long long cnt)
{
    int tmpPos;
    string tmpStr;
    if(_format_isFastq && readSeq->qual.l>0)
    {
        if(_format_digital)
            fprintf(fp, "@%llu", cnt);
        else                
            fprintf(fp, "@%s", readSeq->name.s);
        if(_format_pacbio)
            fprintf(fp, "/%llu/0_%zu", cnt, readSeq->seq.l);
        if(_format_noComment==false && readSeq->comment.l > 0)
            fprintf(fp, " %s\n", readSeq->comment.s);
        else
            fprintf(fp, "\n");
        fprintf(fp, "%s\n", readSeq->seq.s);
        fprintf(fp, "+\n");
        fprintf(fp, "%s\n", readSeq->qual.s);
    }
    else
    {
        if(_format_digital)
            fprintf(fp, ">%llu", cnt);
        else                
            fprintf(fp, ">%s", readSeq->name.s);
        if(_format_pacbio)
            fprintf(fp, "/%llu/0_%zu", cnt, readSeq->seq.l);
        if(_format_noComment==false && readSeq->comment.l > 0)
            fprintf(fp, " %s\n", readSeq->comment.s);
        else
            fprintf(fp, "\n");
        if(_format_lineWidth == 0)
        {
            fprintf(fp, "%s\n", readSeq->seq.s);
        }
        else
        {
            tmpPos = 0;
            tmpStr = readSeq->seq.s;
            while(readSeq->seq.l - tmpPos > _format_lineWidth)
            {
                fprintf(fp, "%s\n", tmpStr.substr(tmpPos, _format_lineWidth).c_str());
                tmpPos += _format_lineWidth;
            }
            fprintf(fp, "%s\n", tmpStr.substr(tmpPos).c_str());
        }
    }
}

bool hasNoN(string r)
{
    return (r.find('n')==string::npos && r.find('N')==string::npos);
}

int command_format(int argc, char* argv[])
{
    if(parseCommandLine_format(argc, argv))
    {
        return EXIT_FAILURE;
    }

    gzFile readFile;
    ostringstream sout;
    kseq_t *readSeq;
    readFile = gzdopen(fileno(_format_in_file), "r");
    if(readFile==NULL)
    {
        fprintf(stderr, "[ERROR] Cannot open file: %s\n", _format_in_path.c_str());
        exit(EXIT_FAILURE);
    }
    
    readSeq = kseq_init(readFile);
    unsigned long long cnt = 0;
    while (kseq_read(readSeq) >= 0)
    {
        if( ((!_format_noN) || (_format_noN && hasNoN(readSeq->seq.s))) &&
            readSeq->seq.l >= _format_minLen && 
            readSeq->seq.l <= _format_maxLen )
        {
            cnt++;
            printRead_format(_format_out_file, readSeq, cnt);
        }
    }
    kseq_destroy(readSeq);
    gzclose(readFile);

    if(_format_out_path != "")
    {
        fclose(_format_out_file);
    }

    return EXIT_SUCCESS;
}
