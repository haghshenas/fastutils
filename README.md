# fastutils
fastutils is a light toolkit for parsing, manipulating and analysis of FASTA and FASTQ files.

## Available commands
```
stat         prints general statistics of fasta/q files
length       prints read ids and their length in tabular format
format       re-formats the fasta/q file based on user's needs
interleave   generate interleave paired end reads
revcomp      prints the reverse complement of each sequence
subsample    output a fraction of reads depending on the desired coverage
subseq       extracts a subsequence from the fasta/q file
cutN         breaks fasta entries into contigs (if containing N's)
```
For details about each command enter `fastutils <command> -h`.

## Command details
### fastutils stat
```
Usage: fastutils stat [options]

I/O options:
     -i,--in STR         input file in fasta/q format [stdin]
     -o,--out STR        output file [stdout]

More options:
     -m,--minLen INT     min read length [0]
     -M,--maxLen INT     max read length [INT64_MAX]
     -h,--help           print this help
```

### fastutils length
```
Usage: fastutils length [options]

I/O options:
     -i,--in STR            input file in fasta/q format [stdin]
     -o,--out STR           output file [stdout]

More options:
     -m,--minLen INT        min read length [0]
     -M,--maxLen INT        max read length [LLONG_MAX]
     -t,--total             print total number of bases in third column
     -h,--help              print this help
```

### fastutils format
```
Usage: fastutils format [options]

I/O options:
     -i,--in STR            input file in fasta/q format [stdin]
     -o,--out STR           output file [stdout]

More options:
     -w,--lineWidth INT     size of lines in fasta output. Use 0 for no wrapping [0]
     -m,--minLen INT        min read length [0]
     -M,--maxLen INT        max read length [LLONG_MAX]
     -q,--fastq             output reads in fastq format if possible
     -n,--noN               do not print entries with N's
     -c,--comment           print comments in headers
     -d,--digital           use read index instead as read name
     -k,--keep              keep  name as a comment when using -d
     -p,--prefix STR        prepend STR to the name
     -s,--suffix STR        append STR to the name
     -P,--pacbio            use pacbio's header format
     -h,--help              print this help
```

### fastutils interleave
```
Usage: fastutils interleave [options] -1 lib1_1.fq -2 lib1_2.fq [-1 lib2_1.fq -2 lib2_2.fq ...]

I/O options:
     -1,--in1 STR           fasta/q file containing forward (left) reads [required]
     -2,--in2 STR           fasta/q file containing reverse (right) reads [required]
     -o,--out STR           output interlaced reads in STR file [stdout]
More options:
     -q,--fastq              output reads in fastq format if possible
     -s,--separator CHR     separator character [.]
     -h,--help              print this help
```

### fastutils revcomp
```
Usage: fastutils revcomp [options]

I/O options:
     -i,--in STR            input file in fasta/q format [stdin]
     -o,--out STR           output file [stdout]

More options:
     -w,--lineWidth INT     size of lines in fasta output. Use 0 for no wrapping [0]
     -q,--fastq             output reads in fastq format if possible
     -c,--comment           print comments in headers
     -l,--lex               output lexicographically smaller sequence
     -h,--help              print this help
```

### fastutils subsample
```
Usage: fastutils subsample -i input -d depth -g genomeSize

I/O options:
     -i,--in STR            input file in fasta/q format. This options is required if -r or -l are used [stdin]
     -o,--out STR           output file [stdout]

More options:
     -d,--depth INT         coverage of the subsampled set [required]
     -g,--genomeSize FLT    length of the genome. Accepted suffixes are k,m,g [required]
     -r,--random            subsample randomly instead of selecting top reads
     -l,--longest           subsample longest reads instead of selecting top reads
     -s,--seed INT          seed for random number generator
     -q,--fastq             output reads in fastq format if possible
     -c,--comment           print comments in headers
     -n,--num               use read index instead of read name
     -k,--keep              keep name as a comment when using -n
     -h,--help              print this help
```

### fastutils subseq
```
Usage: fastutils subseq [options] <name:start-end> [<name2:start2-end2> ...]

Required options:
         -i STR        input file in fastx format. Use - for stdin.
         -o STR        output file. Use - for stdout.

More options:
         -v            print version and build date
         -h            print this help
```

### fastutils cutN
```
Usage: fastutils cutN [options]

Required options:
         -i STR        input file in fastx format. Use - for stdin.
         -o STR        output file in fasta format. Use - for stdout.

More options:
         -v            print version and build date
         -h            print this help
```
