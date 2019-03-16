#include "common.hpp"

#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

string       _subsample_in_path          = "";
string       _subsample_out_path         = "";
FILE         *_subsample_out_file;
bool         _subsample_isFastq = false;
bool         _subsample_noComment = true;
int          _subsample_depth = 0;
long long    _subsample_genomeSize = 0;
string       _subsample_genomeSize_str = "";
int          _subsample_mode = 0; // 0 top. 1 random. 2 longest
int          _subsample_seed = 0;

void printHelp_subsample()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: fastutils subsample -i input -d depth -g genomeSize\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "I/O options:\n");
    fprintf(stderr, "     -i,--in STR            input file in fasta/q format. This options is required if -r or -l are used [stdin]\n");
    fprintf(stderr, "     -o,--out STR           output file [stdout]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "More options:\n");
    fprintf(stderr, "     -d,--depth INT         coverage of the subsampled set [required]\n");
    fprintf(stderr, "     -g,--genomeSize FLT    length of the genome. Accepted suffixes are k,m,g [required]\n");
    fprintf(stderr, "     -r,--random            subsample randomly instead of selecting top reads\n");
    fprintf(stderr, "     -l,--longest           subsample longest reads instead of selecting top reads\n");
    fprintf(stderr, "     -s,--seed INT          seed for random number generator\n");
    fprintf(stderr, "     -q,--fastq             output reads in fastq format if possible\n");
    fprintf(stderr, "     -c,--comment           print comments in headers\n");
    fprintf(stderr, "     -h,--help              print this help\n");
    fprintf(stderr, "\n");
}

int parseCommandLine_subsample(int argc, char *argv[])
{
    int index, c;

    static struct option longOptions[] =
    {
        {"in",          required_argument,      0,      'i' },
        {"out",         required_argument,      0,      'o' },
        {"depth",       required_argument,      0,      'd' },
        {"genomeSize",  required_argument,      0,      'g' },
        {"seed",        required_argument,      0,      's' },
        {"random",      no_argument,            0,      'r' },
        {"longest",     no_argument,            0,      'l' },
        {"fastq",       no_argument,            0,      'q' },
        {"comment",     no_argument,            0,      'c' },
        {"help",        no_argument,            0,      'h' },
        {0,0,0,0}
    };

    while ( (c = getopt_long ( argc, argv, "i:o:d:g:s:rlqch", longOptions, &index))!= -1 )
    {
        switch (c)
        {
            case 'i':
                _subsample_in_path = optarg;
                break;
            case 'o':
                _subsample_out_path = optarg;
                break;
            case 'r':
                _subsample_mode = 1;
                break;
            case 'l':
                _subsample_mode = 2;
                break;
            case 'q':
                _subsample_isFastq = true;
                break;
            case 'c':
                _subsample_noComment = false;
                break;
            case 's':
                _subsample_seed = str2type<int>(optarg);
                break;
            case 'd':
                _subsample_depth = str2type<int>(optarg);
                break;
            case 'g':
                _subsample_genomeSize_str = optarg;
                break;
            case 'h':
                printHelp_subsample();
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "[ERROR] run \"fastutils subsample -h\" to see the help\n");
                fprintf(stderr, "\n");
                return 1;
        }
    }

    if(_subsample_depth <= 0)
    {
        fprintf(stderr, "[ERROR] option -d/--depth is required\n");
        fprintf(stderr, "\n");
        return 1;
    }

    if(_subsample_genomeSize_str == "")
    {
        fprintf(stderr, "[ERROR] option -g/--genomeSize is required\n");
        fprintf(stderr, "\n");
        return 1;
    }
    else
    {
        istringstream sin(_subsample_genomeSize_str.c_str());
        double genomeSize;
        char suffix;
        sin >> genomeSize;
        if(!sin.eof())
        {
            sin >> suffix;
            switch(suffix)
            {
                case 'k':
                    _subsample_genomeSize = (long long) (genomeSize * 1000);
                    break;
                case 'm':
                    _subsample_genomeSize = (long long) (genomeSize * 1000000);
                    break;
                case 'g':
                    _subsample_genomeSize = (long long) (genomeSize * 1000000000);
                    break;
                default:
                    fprintf(stderr, "[ERROR] unexpected suffix for -g/--genomeSize\n");
                    return 1;
            }
        }
        else
        {
            _subsample_genomeSize = (int)genomeSize;
        }
    }

    if(_subsample_in_path == "" && (_subsample_mode == 1 || _subsample_mode == 2))
    {
        fprintf(stderr, "[ERROR] option -i/--in is required when using -r/--random or -l/--longest\n");
        fprintf(stderr, "\n");
        return 1;
    }

    if(_subsample_out_path == "")
    {
        _subsample_out_file = stdout;
    }
    else
    {
        _subsample_out_file = fopen(_subsample_out_path.c_str(), "w");
        if(_subsample_out_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _subsample_out_path.c_str());
            fprintf(stderr, "\n");
            return 1;
        }
    }

    return 0;
}

void printRead_subsample(FILE *fp, kseq_t *readSeq)
{
    int tmpPos;
    string tmpStr;
    if(_subsample_isFastq && readSeq->qual.l>0)
    {
        fprintf(fp, "@%s", readSeq->name.s);
        if(_subsample_noComment==false && readSeq->comment.l > 0)
            fprintf(fp, " %s\n", readSeq->comment.s);
        else
            fprintf(fp, "\n");
        fprintf(fp, "%s\n", readSeq->seq.s);
        fprintf(fp, "+\n");
        fprintf(fp, "%s\n", readSeq->qual.s);
    }
    else
    {
        fprintf(fp, ">%s", readSeq->name.s);
        if(_subsample_noComment==false && readSeq->comment.l > 0)
            fprintf(fp, " %s\n", readSeq->comment.s);
        else
            fprintf(fp, "\n");
        fprintf(fp, "%s\n", readSeq->seq.s);
    }
}

void subsample_top()
{
    FILE *_subsample_in_file = NULL;
    if(_subsample_in_path == "")
    {
        _subsample_in_file = stdin;
    }
    else
    {
        _subsample_in_file = fopen(_subsample_in_path.c_str(), "r");
        if(_subsample_in_file == NULL)
        {
            fprintf(stderr, "[ERROR] could not open file: %s\n", _subsample_in_path.c_str());
            exit(EXIT_FAILURE);
        }
    }
    long long blen = 0;
    long long bmax = (long long)_subsample_genomeSize * _subsample_depth;
    gzFile readFile;
    kseq_t *readSeq;
    // readFile = gzopen(_subsample_in_path.c_str(), "r");
    readFile = gzdopen(fileno(_subsample_in_file), "r");
    if(readFile==NULL)
    {
        fprintf(stderr, "[ERROR] Cannot open file: %s\n", _subsample_in_path.c_str());
        exit(EXIT_FAILURE);
    }
    readSeq = kseq_init(readFile);
    while (kseq_read(readSeq) >= 0)
    {
        printRead_subsample(_subsample_out_file, readSeq);
        blen += readSeq->seq.l;
        if(blen >= bmax)
            break;
    }
    kseq_destroy(readSeq);
    gzclose(readFile);
    
    if(_subsample_in_path != "")
    {
        fclose(_subsample_in_file);
    }
    if(_subsample_out_path != "")
    {
        fclose(_subsample_out_file);
    }
}

void subsample_random()
{
    // 1st pass
    gzFile readFile;
    kseq_t *readSeq;
    vector<int> read_lens;
    readFile = gzopen(_subsample_in_path.c_str(), "r");
    if(readFile==NULL)
    {
        fprintf(stderr, "[ERROR] Cannot open file: %s\n", _subsample_in_path.c_str());
        exit(EXIT_FAILURE);
    }
    readSeq = kseq_init(readFile);
    while(kseq_read(readSeq) >= 0)
    {
        read_lens.push_back(readSeq->seq.l);
    }
    kseq_destroy(readSeq);
    gzclose(readFile);
    // select
    long long blen = 0;
    long long bmax = (long long)_subsample_genomeSize * _subsample_depth;
    uint32_t num = read_lens.size();
    unordered_set<int> selected_index;
    while(selected_index.size() < num)
    {
        int r = rand() % num;
        if(selected_index.count(r) == 0) // not in the set
        {
            selected_index.insert(r);
            blen += read_lens[r];
            if(blen >= bmax)
                break;
        }
    }
    // 2nd pass
    gzFile readFile2;
    kseq_t *readSeq2;
    readFile2 = gzopen(_subsample_in_path.c_str(), "r");
    if(readFile2==NULL)
    {
        fprintf(stderr, "[ERROR] Cannot open file: %s\n", _subsample_in_path.c_str());
        exit(EXIT_FAILURE);
    }
    int cnt = 0;
    readSeq2 = kseq_init(readFile2);
    while (kseq_read(readSeq2) >= 0)
    {
        if(selected_index.count(cnt) > 0)
            printRead_subsample(_subsample_out_file, readSeq2);
        cnt++;
    }
    kseq_destroy(readSeq2);
    gzclose(readFile2);
    
    if(_subsample_out_path != "")
    {
        fclose(_subsample_out_file);
    }
}

void subsample_longest()
{
    // 1st pass
    gzFile readFile;
    kseq_t *readSeq;
    vector<pair<int, int> > read_lens;
    readFile = gzopen(_subsample_in_path.c_str(), "r");
    if(readFile==NULL)
    {
        fprintf(stderr, "[ERROR] Cannot open file: %s\n", _subsample_in_path.c_str());
        exit(EXIT_FAILURE);
    }
    int cnt = 0;
    readSeq = kseq_init(readFile);
    while(kseq_read(readSeq) >= 0)
    {
        read_lens.push_back(make_pair(readSeq->seq.l, cnt));
        cnt++;
    }
    kseq_destroy(readSeq);
    gzclose(readFile);
    // select
    sort(read_lens.begin(), read_lens.end(), greater<pair<int, int> >());
    long long blen = 0;
    long long bmax = (long long)_subsample_genomeSize * _subsample_depth;
    unordered_set<int> selected_index;
    for(int i = 0; i < read_lens.size(); i++)
    {
        selected_index.insert(read_lens[i].second);
        blen += read_lens[i].first;
        if(blen >= bmax)
            break;
    }
    // 2nd pass
    gzFile readFile2;
    kseq_t *readSeq2;
    readFile2 = gzopen(_subsample_in_path.c_str(), "r");
    if(readFile2==NULL)
    {
        fprintf(stderr, "[ERROR] Cannot open file: %s\n", _subsample_in_path.c_str());
        exit(EXIT_FAILURE);
    }
    cnt = 0;
    readSeq2 = kseq_init(readFile2);
    while (kseq_read(readSeq2) >= 0)
    {
        if(selected_index.count(cnt) > 0)
            printRead_subsample(_subsample_out_file, readSeq2);
        cnt++;
    }
    kseq_destroy(readSeq2);
    gzclose(readFile2);
    
    if(_subsample_out_path != "")
    {
        fclose(_subsample_out_file);
    }
}

int program_subsample(int argc, char* argv[])
{
    if(argc < 3)
    {
        printHelp_subsample();
        return EXIT_SUCCESS;
    }

    if(parseCommandLine_subsample(argc, argv))
    {
        return EXIT_FAILURE;
    }

    srand(_subsample_seed);

    if(_subsample_mode == 1)
        subsample_random();
    else if(_subsample_mode == 2)
        subsample_longest();
    else
        subsample_top();

    // ostream &outObj = *_subsample_out_pointer;

    // gzFile fp = gzdopen(fileno(_subsample_in_file), "r");
    // if(fp == NULL)
    // {
    //     cerr << "Cannot open file: " << (_subsample_in_path == "-" ? "stdin" : _subsample_in_path) << endl;
    //     exit(1);
    // }

    // map<string, string> allSeq;

    // kseq_t *seq = kseq_init(fp);
    // while (kseq_read(seq) >= 0)
    // {
    //     allSeq[seq->name.s] = seq->seq.s;
    // }
    // kseq_destroy(seq);
    // gzclose(fp);

    // optind++; // to ignore subsample
    // if(optind < argc)
    // {
    //     while(optind < argc)
    //     {
    //         string region = argv[optind];
    //         string seqName = region.substr(0, region.find(':'));
    //         int seqStart = str2type<int>( region.substr(region.find(':')+1, region.find('-') - region.find(':') - 1) );
    //         int seqEnd = str2type<int>( region.substr(region.find('-')+1) );
    //         if(seqStart < seqEnd)
    //         {
    //             cout<< ">" << argv[optind] << "\n";
    //             cout<< allSeq[seqName].substr(seqStart, seqEnd-seqStart) << "\n";
    //         }

    //         optind++;
    //     }
    // }
    // else
    // {
    //     cerr<< "[ERROR] no region to extract!" << endl;
    //     return 0;
    // }

    return EXIT_SUCCESS;
}