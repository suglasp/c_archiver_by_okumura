/***********************************************************
	maketbl.c
***********************************************************/
#include "ar.h"

static short c, n, tblsiz, len, depth, maxdepth, avail;
static ushort codeword, bit, *tbl;
static uchar *blen;

static short mktbl(void)
{
	short i;

	if (len == depth) {
		while (++c < n)
			if (blen[c] == len) {
				i = codeword;  codeword += bit;
				if (codeword > tblsiz) error("Bad table (1)");
				while (i < codeword) tbl[i++] = c;
				return c;
			}
		c = -1;  len++;  bit >>= 1;
	}
	depth++;
	if (depth < maxdepth) {
		(void) mktbl();  (void) mktbl();
	} else if (depth > USHRT_BIT) {
		error("Bad table (2)");
	} else {
		if ((i = avail++) >= 2 * n - 1) error("Bad table (3)");
		left[i] = mktbl();  right[i] = mktbl();
		if (codeword >= tblsiz) error("Bad table (4)");
		if (depth == maxdepth) tbl[codeword++] = i;
	}
	depth--;
	return i;
}

void make_table(short nchar, uchar bitlen[],
				short tablebits, ushort table[])
{
	n = avail = nchar;  blen = bitlen;  tbl = table;
	tblsiz = 1U << tablebits;  bit = tblsiz / 2;
	maxdepth = tablebits + 1;
	depth = len = 1;  c = -1;  codeword = 0;
	(void) mktbl();  /* left subtree */
	(void) mktbl();  /* right subtree */
	if (codeword != tblsiz) error("Bad table (5)");
}
