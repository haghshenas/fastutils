#include "common.hpp"
#include "stat.hpp"
#include "fxsubseq.hpp"
#include "fa2contig.hpp"

void printHelp_core();

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printHelp_core();
    }
    else
    {
        string com = argv[1];
        if(com == "stat")
        {
            return command_stat(argc, argv);
        }
        else if(com == "fxsubseq")
        {
            return program_fxsubseq(argc, argv);
        }
        else if(com == "fa2contig")
        {
            return program_fa2contig(argc, argv);
        }
        else
        {
            printHelp_core();
        }
    }
    return EXIT_SUCCESS;
}

void printHelp_core()
{
    fprintf(stdout, "\n");
    fprintf(stderr, "Program: fastutils (parsing, manipulating and analysis of FASTA/FASTQ files) %s\n");
    fprintf(stderr, "Version: %s\n", FASTUTILS_VERSION);
    fprintf(stderr, "\n");
    fprintf(stdout, "Usage: fastutils <command> [options]\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Commands:\n");
    fprintf(stdout, "     stat         prints general statistics of fastx files\n");
    fprintf(stdout, "     fxsubseq     extracts a subsequence from the fastx file\n");
    fprintf(stdout, "     fa2contig    splits fasta file into contigs\n");
    fprintf(stdout, "\n");
}
