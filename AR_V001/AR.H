/***********************************************************
	ar.h
***********************************************************/
#ifndef AR_H
#define AR_H

#include <stdio.h>
#include <limits.h>
typedef unsigned char  uchar;   /*  8 bits or more */
typedef unsigned int   uint;    /* 16 - 32 bits or more */
typedef unsigned short ushort;  /* 16 bits or more */
typedef unsigned long  ulong;   /* 32 bits or more */

/* ar.c */

extern int unpackable;
extern ulong origsize, compsize;

/* io.c */

extern FILE *arcfile, *infile, *outfile;
extern ushort crc, bitbuf;

void error(char *fmt, ...);
void make_crctable(void);
void fillbuf(uchar n);
ushort getbits(uchar n);
/* void putbit(uchar bit); */
void putbits(uchar n, ushort x);
int fread_crc(uchar *p, int n, FILE *f);
void fwrite_crc(uchar *p, int n, FILE *f);
void init_getbits(void);
void init_putbits(void);

/* slide.c */

#define DICBIT    12    /* 12 or 13 */
#define DICSIZ (1U << DICBIT)
#define MATCHBIT   8    /* bits for MAXMATCH - THRESHOLD */
#define MAXMATCH 256    /* formerly F (not more than UCHAR_MAX + 1) */
#define THRESHOLD  3    /* choose optimal value */

void encode(void);
void decode(void);

/* huf.c */

#define NC (UCHAR_MAX + MAXMATCH + 2 - THRESHOLD)
	/* alphabet = {0, 1, 2, ..., NC - 1} */
#define CBIT 9  /* $\lfloor \log_2 NC \rfloor + 1$ */
#define USHRT_BIT (CHAR_BIT * sizeof(ushort))

extern short left[], right[];

void encode_start(void);
void decode_start(void);
ushort decode_c(void);
ushort decode_p(void);
void output(ushort c, ushort p);
void encode_end(void);

/* maketbl.c */

void make_table(short nchar, uchar bitlen[],
				short tablebits, ushort table[]);

/* maketree.c */

short make_tree(short nparm, ushort freqparm[], uchar lenparm[]);
#endif