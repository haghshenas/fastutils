# fastutils

- [Quick start](#start)
- [Available commands](#commands)
- [Command details](#details)
  - [fastutils stat](#stat)
  - [fastutils length](#length)
  - [fastutils format](#format)
  - [fastutils interleave](#interleave)
  - [fastutils revcomp](#revcomp)
  - [fastutils subsample](#subsample)
  - [fastutils subseq](#subseq)
  - [fastutils cutN](#cutn)
- [Bug report](#bugs)
- [Copyright and License](#license)

## <a name="start"></a>Quick start
```
# check number of reads, number of bases, and base composition of a fasta/q file
fastutils stat -i reads.fastq
# check mean read length
fastutils length -i reads.fastq | datamash mean 1
# convert fastq to fasta
fastutils format -i reads.fastq > reads.fasta
# print reads longer than 1000 bp and format in lines of length 60 bp
fastutils format -i reads.fastq -m 1000 -w 60 > reads.1000.fasta
# interleave paired-end dataset
fastutils interleave -1 reads_1.fastq -2 reads_2.fastq -q > reads.fastq
# subsample 25x coverage of reads randomly (assuming E.coli dataset)
fastutils subsample -i reads.fastq -d 25 -g 4.6m -r > reads.subsample.fasta
# print first 1 million bp of chr1 and format in lines of length 60 bp
fastutils subseq -i hg38.fa -o - chr1:0-1000000 | fastutils format -w 60 > chr1.chunk.fasta
# compare each sequences with its reverse complement and print lexicographically smaller one
fastutils revcomp -i reads.fastq -l > reads.lex.fasta
# piping example; Get all contigs of chrX
cat hg38.fa | fastutils format | grep ">chrX" -A1 | fastutils cutN -i - > chrX.contigs.fa
```

## <a name="commands"></a>Available commands
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

## <a name="details"></a>Command details
### <a name="stat"></a>fastutils stat
Reports the number of reads, number of bases, and base composition of the input FASTA/Q file.
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

### <a name="length"></a>fastutils length
Prints the name and length of each read (separated by tab), one read per line.
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

### <a name="format"></a>fastutils format
Change the format of the input file.
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

### <a name="interleave"></a>fastutils interleave
Takes two fasta/q files of one or multiple paired-end/mate-pair library and print the sequences in interleaved/interlaced format.
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

### <a name="revcomp"></a>fastutils revcomp
Print the reverse complement of the sequences contained in the input.
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

### <a name="subsample"></a>fastutils subsample
Downsamples the input file to a desired depth of coverage. User can choose to select random reads, longest reads, or from top (default).
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

### <a name="subseq"></a>fastutils subseq
Extracts desired subsequences from input file.
```
Usage: fastutils subseq [options] <name:start-end> [<name2:start2-end2> ...]

Required options:
         -i STR        input file in fastx format. Use - for stdin.
         -o STR        output file. Use - for stdout.

More options:
         -v            print version and build date
         -h            print this help
```

### <a name="cutn"></a>fastutils cutN
Cuts fasta entries at N bases. This is useful for converting scaffolds to contigs.
```
Usage: fastutils cutN [options]

Required options:
         -i STR        input file in fastx format. Use - for stdin.
         -o STR        output file in fasta format. Use - for stdout.

More options:
         -v            print version and build date
         -h            print this help
```

## <a name="bugs"></a>Bug report
Please report the bugs through issue tracker at [https://github.com/haghshenas/fastutils/issues](https://github.com/haghshenas/fastutils/issues).

## <a name="license"></a>Copyright and License
This software is released under GNU General Public License (v3.0)

