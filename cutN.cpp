#include "common.hpp"
#include "kseq.h"

KSEQ_INIT(gzFile, gzread)

string       _cutN_in_path          = "";
FILE         *_cutN_in_file;
string       _cutN_out_path         = "";
ofstream     _cutN_out_file;
ostream      *_cutN_out_pointer     = NULL;

void printHelp_cutN()
{
    cerr << endl;
    cerr << "USAGE: fastutils cutN [options]" << endl;
    cerr << endl;
    cerr << "Required options:" << endl;
    cerr << "         -i STR        input file in fastx format. Use - for stdin." << endl;
    cerr << "         -o STR        output file in fasta format. Use - for stdout." << endl;
    cerr << endl;
    cerr << "More options:" << endl;
    cerr << "         -v            print version and build date" << endl;
    cerr << "         -h            print this help" << endl;
    cerr << endl;
}

int parseCommandLine_cutN(int argc, char *argv[])
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
                _cutN_in_path = optarg;
                break;
            case 'o':
                _cutN_out_path = optarg;
                break;
            case 'h':
                printHelp_cutN();
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

    if(_cutN_in_path == "")
    {
        cerr<< "[ERROR] option -i/--in is required" << endl << endl;
        return 0;
    }
    else
    {
        if(_cutN_in_path == "-")
        {
            _cutN_in_file = stdin;
        }
        else
        {
            _cutN_in_file = fopen(_cutN_in_path.c_str(), "r");
            if(_cutN_in_file == NULL)
            {
                cerr<< "[ERROR] could not open file: " << _cutN_in_path << endl << endl;
                return 0;
            }
        }
    }

    if(_cutN_out_path == "")
    {
        cerr<< "[ERROR] option -o/--out is required" << endl << endl;
        return 0;
    }
    else
    {
        if(_cutN_out_path == "-")
        {
            _cutN_out_pointer = &cout;
        }
        else
        {
            _cutN_out_file.open(_cutN_out_path.c_str());
            if(_cutN_out_file.is_open()==false)
            {
                cerr<< "[ERROR] could not open file: " << _cutN_out_path << endl << endl;
                return 0;
            }
            _cutN_out_pointer = &_cutN_out_file;
        }
    }

    return 1;
}

int program_cutN(int argc, char* argv[])
{
    if(argc < 3)
    {
        printHelp_cutN();
        return EXIT_SUCCESS;
    }

    if(!parseCommandLine_cutN(argc, argv))
    {
        return EXIT_FAILURE;
    }
    ostream &outObj = *_cutN_out_pointer;

    uint64_t i;
    uint64_t pos_start = 0;
    uint64_t pos_n = 0;
    string str;
    uint64_t num_contig = 1;

    gzFile fp = gzdopen(fileno(_cutN_in_file), "r");
    if(fp == NULL)
    {
        cerr << "Cannot open file: " << (_cutN_in_path == "-" ? "stdin" : _cutN_in_path) << endl;
        exit(1);
    }
    kseq_t *seq = kseq_init(fp);
    while (kseq_read(seq) >= 0)
    {
        str = seq->seq.s;
        pos_n = 0;
        i=0;
        while(i < str.size())
        {
            // find first non-N base
            while(i < str.size() && toupper(str[i])=='N')
                i++;
            pos_start = i;
            // find next N base
            while(i < str.size() && toupper(str[i])!='N')
                i++;
            if(i - pos_start > 0)
            {
                outObj<< ">" << seq->name.s << "." << num_contig << " length=" << (i - pos_start) << " N_before=" <<  (pos_start - pos_n) << "\n";
                outObj<< str.substr(pos_start, i - pos_start) << "\n";
                num_contig++;
            }
            pos_n = i;
        }
    }
    kseq_destroy(seq);
    gzclose(fp);

    return 0;
}