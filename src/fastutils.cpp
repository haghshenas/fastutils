#include "common.hpp"
#include "stat.hpp"
#include "format.hpp"
#include "subseq.hpp"
#include "subsample.hpp"
#include "cutN.hpp"
#include "length.hpp"
#include "revcomp.hpp"
#include "interleave.hpp"

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
        if(com == "-h" || com == "--help")
        {
            printHelp_core();
            return EXIT_SUCCESS;
        }
        else if(com == "stat")
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
        else if(com == "subseq")
        {
            return program_subseq(argc, argv);
        }
        else if(com == "subsample")
        {
            return program_subsample(argc, argv);
        }
        else if(com == "cutN")
        {
            return program_cutN(argc, argv);
        }
        else if(com == "length")
        {
            return command_length(argc, argv);
        }
        else if(com == "interleave")
        {
            return command_interleave(argc, argv);
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
    fprintf(stderr, "     interleave   generate interleave paired end reads\n");
    fprintf(stderr, "     revcomp      prints the reverse complement of each sequence\n");
    fprintf(stderr, "     subsample    output a fraction of reads depending on the desired coverage\n");
    fprintf(stderr, "     subseq       extracts a subsequence from the fasta/q file\n");
    fprintf(stderr, "     cutN         breaks fasta entries into contigs (if containing N's)\n");
    fprintf(stderr, "\n");
}
