#include "common.hpp"
#include "stat.hpp"
#include "format.hpp"
#include "fxsubseq.hpp"
#include "fa2contig.hpp"
#include "length.hpp"
#include "revcomp.hpp"

void printHelp_core();

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printHelp_core();
        return EXIT_SUCCESS;
    }
    else
    {
        string com = argv[1];
        if(com == "stat")
        {
            return command_stat(argc, argv);
        }
        else if(com == "format")
        {
            return command_format(argc, argv);
        }
        else if(com == "revcomp")
        {
            return command_revcomp(argc, argv);
        }
        else if(com == "fxsubseq")
        {
            return program_fxsubseq(argc, argv);
        }
        else if(com == "fa2contig")
        {
            return program_fa2contig(argc, argv);
        }
        else if(com == "length")
        {
            return command_length(argc, argv);
        }
        else
        {
            printHelp_core();
            return EXIT_FAILURE;
        }
    }
}

void printHelp_core()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Program: fastutils (parsing, manipulating and analysis of FASTA/FASTQ files)\n");
    fprintf(stderr, "Version: %s\n", FASTUTILS_VERSION);
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: fastutils <command> [options]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Commands:\n");
    fprintf(stderr, "     stat         prints general statistics of fasta/q files\n");
    fprintf(stderr, "     length       prints read ids and their length in tabular format\n");
    fprintf(stderr, "     format       re-formats the fasta/q file based on user's needs\n");
    fprintf(stderr, "     revcomp      prints the reverse complement of each sequence\n");
    fprintf(stderr, "     fxsubseq     extracts a subsequence from the fasta/q file\n");
    fprintf(stderr, "     fa2contig    breaks fasta entries into contigs (if containing N's)\n");
    fprintf(stderr, "\n");
}
