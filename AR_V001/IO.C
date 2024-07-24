/***********************************************************
	io.c -- input/output
***********************************************************/
#include "ar.h"
#include <stdlib.h>
#include <stdarg.h>

#define USHRT_BIT  (CHAR_BIT * sizeof(ushort))
#define CRCPOLY  0x8408U  /* CCITT */
#define UPDATE_CRC(c) \
	crc = crctable[(crc ^ (c)) & 0xFF] ^ (crc >> CHAR_BIT)

FILE *arcfile, *infile, *outfile;
ushort crc, bitbuf;

static ushort crctable[UCHAR_MAX + 1];
static uchar  subbitbuf, bitcount;

void error(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	putc('\n', stderr);
	vfprintf(stderr, fmt, args);
	putc('\n', stderr);
	va_end(args);
	exit(EXIT_FAILURE);
}

void make_crctable(void)
{
	uint i, j, r;

	for (i = 0; i <= UCHAR_MAX; i++) {
		r = i;
		for (j = 0; j < CHAR_BIT; j++)
			if (r & 1) r = (r >> 1) ^ CRCPOLY;
			else       r >>= 1;
		crctable[i] = r;
	}
}

void fillbuf(uchar n)  /* Shift bitbuf n bits left, read n bits */
{
	while (n > bitcount) {
		n -= bitcount;
		bitbuf = (bitbuf << bitcount) + (subbitbuf >> (CHAR_BIT - bitcount));
		if (compsize != 0) {
			compsize--;  subbitbuf = (uchar) getc(arcfile);
		} else subbitbuf = 0;
		bitcount = CHAR_BIT;
	}
	bitcount -= n;
	bitbuf = (bitbuf << n) + (subbitbuf >> (CHAR_BIT - n));
	subbitbuf <<= n;
}

ushort getbits(uchar n)
{
	ushort x;

	x = bitbuf >> (2 * CHAR_BIT - n);  fillbuf(n);
	return x;
}

#if 0  /* not used in this version */
void putbit(uchar bit)  /* Write 1 bit (bit = 0 or 1) */
{
	bitcount--;
	if (bit) subbitbuf |= (1U << bitcount);
	if (bitcount == 0) {
		putc(subbitbuf, outfile);
		subbitbuf = 0;  bitcount = CHAR_BIT;  compsize++;
	}
}
#endif

void putbits(uchar n, ushort x)  /* Write rightmost n bits of x */
{
	x <<= USHRT_BIT - n;
	while (n >= bitcount) {
		n -= bitcount;
		subbitbuf += x >> (USHRT_BIT - bitcount);
		x <<= bitcount;
		if (compsize < origsize) {
			putc(subbitbuf, outfile);  compsize++;
		} else unpackable = 1;
		subbitbuf = 0;  bitcount = CHAR_BIT;
	}
	subbitbuf += x >> (USHRT_BIT - bitcount);
	bitcount -= n;
}

int fread_crc(uchar *p, int n, FILE *f)
{
	int i;

	i = n = fread(p, 1, n, f);  origsize += n;
	while (--i >= 0) UPDATE_CRC(*p++);
	return n;
}

void fwrite_crc(uchar *p, int n, FILE *f)
{
	if (fwrite(p, 1, n, f) < n) error("Unable to write");
	while (--n >= 0) UPDATE_CRC(*p++);
}

void init_getbits(void)
{
	bitbuf = 0;  subbitbuf = 0;  bitcount = 0;
	fillbuf(2 * CHAR_BIT);
}

void init_putbits(void)
{
	bitcount = CHAR_BIT;  subbitbuf = 0;
}
