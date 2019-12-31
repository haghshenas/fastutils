#include "common.hpp"
#include "kseq.h"

KSEQ_INIT(gzFile, gzread)


string      _revcomp_in_path     = "";
FILE        *_revcomp_in_file;
string      _revcomp_out_path    = "";
FILE        *_revcomp_out_file;
int         _revcomp_lineWidth   = 0;
bool        _revcomp_isFastq     = false;
bool        _revcomp_noComment   = true;
bool        _revcomp_lex         = false;

void printHelp_revcomp()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: fastutils revcomp\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "I/O options:\n");
    fprintf(stderr, "     -i,--in STR            input file in fasta/q format [stdin]\n");
    fprintf(stderr, "     -o,--out STR           output file [stdout]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "More options:\n");
    fprintf(stderr, "     -w,--lineWidth INT     size of lines in fasta output. Use 0 for no wrapping [0]\n");
    fprintf(stderr, "     -q,--fastq             output reads in fastq format if possible\n");
    fprintf(stderr, "     -c,--comment           print comments in headers\n");
    fprintf(stderr, "     -l,--lex               output lexicographically smaller sequence\n");
    fprintf(stderr, "     -h,--help              print this help\n");
    fprintf(stderr, "\n");
}

int parseCommandLine_revcomp(int argc, char *argv[])
{
    int index, c;

    static struct option longOptions[] =
    {
        {"in",          required_argument,      0,      'i' },
        {"out",         required_argument,      0,      'o' },
        {"lineWidth",   required_argument,      0,      'w' },
        {"fastq",       no_argument,            0,      'q' },
        {"comment",     no_argument,            0,      'c' },
        {"lex",         no_argument,            0,      'l' },
        {"help",        no_argument,            0,      'h' },
        {0,0,0,0}
    };

    while ( (c = getopt_long ( argc, argv, "i:o:w:qclh", longOptions, &index))!= -1 )
    {
        switch (c)
        {
            case 'i':
                _revcomp_in_path = optarg;
                break;
            case 'o':
                _revcomp_out_path = optarg;
                break;
            case 'w':
                _revcomp_lineWidth = str2type<int>(optarg);
                break;
            case 'q':
                _revcomp_isFastq = true;
                break;
            case 'c':
                _revcomp_noComment = false;
                break;
            case 'l':
                _revcomp_lex = false;
                break;
            case 'h':
                printHelp_revcomp();
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "[ERROR] run \"fastutils revcompt -h\" to see the help\n");
                fprintf(stderr, "\n");
                return 1;
        }
    }

    if(_revcomp_in_path == "")
    {
        _revcomp_in_file = stdin;
    }
    else
    {
        _revcomp_in_file = fopen(_revcomp_in_path.c_str(), "r");
        if(_revcomp_in_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _revcomp_in_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    if(_revcomp_out_path == "")
    {
        _revcomp_out_file = stdout;
    }
    else
    {
        _revcomp_out_file = fopen(_revcomp_out_path.c_str(), "w");
        if(_revcomp_out_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _revcomp_out_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    if(_revcomp_lineWidth < 0)
        _revcomp_lineWidth = 0;

    return 0;
}

void printRead_revcomp(FILE *fp, kseq_t *readSeq)
{
    int tmpPos;
    string tmpStr;
    if(_revcomp_isFastq && readSeq->qual.l>0)
    {
        string rev_seq;
        revComplement(rev_seq, readSeq->seq.s);
        string rev_qual;
        revString(rev_qual, readSeq->qual.s);
        if(_revcomp_lex)
        {
            fprintf(fp, "@%s", readSeq->name.s);
            if(_revcomp_noComment==false && readSeq->comment.l > 0)
                fprintf(fp, " %s\n", readSeq->comment.s);
            else
                fprintf(fp, "\n");
            if(rev_seq.compare(readSeq->seq.s) < 0)
            {
                fprintf(fp, "%s\n", rev_seq.c_str());
                fprintf(fp, "+\n");
                fprintf(fp, "%s\n", rev_qual.c_str());
            }
            else
            {
                fprintf(fp, "%s\n", readSeq->seq.s);
                fprintf(fp, "+\n");
                fprintf(fp, "%s\n", readSeq->qual.s);
            }
        }
        else
        {
            fprintf(fp, "@%s", readSeq->name.s);
            if(_revcomp_noComment==false && readSeq->comment.l > 0)
                fprintf(fp, " %s\n", readSeq->comment.s);
            else
                fprintf(fp, "\n");
            fprintf(fp, "%s\n", rev_seq.c_str());
            fprintf(fp, "+\n");
            fprintf(fp, "%s\n", rev_qual.c_str());
        }
    }
    else
    {
        string rev_seq;
        revComplement(rev_seq, readSeq->seq.s);
        // string rev_qual;
        // revString(rev_qual, readSeq->qual.s);
        if(_revcomp_lex)
        {
            fprintf(fp, ">%s", readSeq->name.s);
            if(_revcomp_noComment==false && readSeq->comment.l > 0)
                fprintf(fp, " %s\n", readSeq->comment.s);
            else
                fprintf(fp, "\n");
            if(rev_seq.compare(readSeq->seq.s) < 0)
            {
                if(_revcomp_lineWidth == 0)
                {
                    fprintf(fp, "%s\n", rev_seq.c_str());
                }
                else
                {
                    tmpPos = 0;
                    tmpStr = rev_seq;
                    while(readSeq->seq.l - tmpPos > _revcomp_lineWidth)
                    {
                        fprintf(fp, "%s\n", tmpStr.substr(tmpPos, _revcomp_lineWidth).c_str());
                        tmpPos += _revcomp_lineWidth;
                    }
                    fprintf(fp, "%s\n", tmpStr.substr(tmpPos).c_str());
                }
            }
            else
            {
                if(_revcomp_lineWidth == 0)
                {
                    fprintf(fp, "%s\n", readSeq->seq.s);
                }
                else
                {
                    tmpPos = 0;
                    tmpStr = readSeq->seq.s;
                    while(readSeq->seq.l - tmpPos > _revcomp_lineWidth)
                    {
                        fprintf(fp, "%s\n", tmpStr.substr(tmpPos, _revcomp_lineWidth).c_str());
                        tmpPos += _revcomp_lineWidth;
                    }
                    fprintf(fp, "%s\n", tmpStr.substr(tmpPos).c_str());
                }
            }
        }
        else
        {
            fprintf(fp, ">%s", readSeq->name.s);
            if(_revcomp_noComment==false && readSeq->comment.l > 0)
                fprintf(fp, " %s\n", readSeq->comment.s);
            else
                fprintf(fp, "\n");
            if(_revcomp_lineWidth == 0)
            {
                fprintf(fp, "%s\n", rev_seq.c_str());
            }
            else
            {
                tmpPos = 0;
                tmpStr = rev_seq;
                while(readSeq->seq.l - tmpPos > _revcomp_lineWidth)
                {
                    fprintf(fp, "%s\n", tmpStr.substr(tmpPos, _revcomp_lineWidth).c_str());
                    tmpPos += _revcomp_lineWidth;
                }
                fprintf(fp, "%s\n", tmpStr.substr(tmpPos).c_str());
            }
        }
    }
}

int command_revcomp(int argc, char* argv[])
{
    if(parseCommandLine_revcomp(argc, argv))
    {
        return EXIT_FAILURE;
    }

    gzFile readFile;
    ostringstream sout;
    kseq_t *readSeq;
    readFile = gzdopen(fileno(_revcomp_in_file), "r");
    if(readFile==NULL)
    {
        fprintf(stderr, "[ERROR] Cannot open file: %s\n", _revcomp_in_path.c_str());
        exit(EXIT_FAILURE);
    }
    
    readSeq = kseq_init(readFile);
    while (kseq_read(readSeq) >= 0)
    {
        // cnt++;
        printRead_revcomp(_revcomp_out_file, readSeq);
    }
    kseq_destroy(readSeq);
    gzclose(readFile);

    if(_revcomp_out_path != "")
    {
        fclose(_revcomp_out_file);
    }

    return EXIT_SUCCESS;
}
