#include "common.hpp"
#include "stat.hpp"
#include "format.hpp"
#include "subseq.hpp"
#include "subsample.hpp"
#include "cutN.hpp"
#include "length.hpp"
#include "revcomp.hpp"
#include "interleave.hpp"

void printHelp_core(FILE *fp);

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printHelp_core(stderr);
        return EXIT_FAILURE;
    }
    else
    {
        string com = argv[1];
        if(com == "-h" || com == "--help")
        {
            printHelp_core(stdout);
            return EXIT_SUCCESS;
        }
        else if(com == "-v" || com == "--version")
        {
            fprintf(stdout, "%s\n", FASTUTILS_VERSION);
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
            printHelp_core(stderr);
            return EXIT_FAILURE;
        }
    }
}

void printHelp_core(FILE *fp)
{
    fprintf(fp, "\n");
    fprintf(fp, "Program: fastutils (parsing, manipulating and analysis of FASTA/FASTQ files)\n");
    fprintf(fp, "Version: %s\n", FASTUTILS_VERSION);
    fprintf(fp, "\n");
    fprintf(fp, "Usage: fastutils <command> [options]\n");
    fprintf(fp, "\n");
    fprintf(fp, "Commands:\n");
    fprintf(fp, "     stat         prints general statistics of fasta/q files\n");
    fprintf(fp, "     length       prints read ids and their length in tabular format\n");
    fprintf(fp, "     format       re-formats the fasta/q file based on user's needs\n");
    fprintf(fp, "     interleave   generate interleave paired end reads\n");
    fprintf(fp, "     revcomp      prints the reverse complement of each sequence\n");
    fprintf(fp, "     subsample    output a fraction of reads depending on the desired coverage\n");
    fprintf(fp, "     subseq       extracts a subsequence from the fasta/q file\n");
    fprintf(fp, "     cutN         breaks fasta entries into contigs (if containing N's)\n");
    fprintf(fp, "\n");
}
