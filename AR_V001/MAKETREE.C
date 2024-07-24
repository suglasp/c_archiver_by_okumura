/***********************************************************
	maketree.c -- make Huffman tree
***********************************************************/
#include "ar.h"

static short n, heapsize, heap[NC + 1];
static ushort *freq;
static uchar *len;

static void make_len(short i)  /* call with i = root */
{
	static uchar depth = 0;

	if (i < n) len[i] = depth;
	else {
		depth++;
		make_len(left [i]);
		make_len(right[i]);
		depth--;
	}
}

static void downheap(short i)
	/* priority queue; send i-th entry down heap */
{
	short j, k;

	k = heap[i];
	while ((j = 2 * i) <= heapsize) {
		if (j < heapsize && freq[heap[j]] > freq[heap[j + 1]])
		 	j++;
		if (freq[k] <= freq[heap[j]]) break;
		heap[i] = heap[j];  i = j;
	}
	heap[i] = k;
}

short make_tree(short nparm, ushort freqparm[], uchar lenparm[])
	/* make tree, calculate len[], return root */
{
	short i, j, k, avail;

	n = nparm;  freq = freqparm;  len = lenparm;
	avail = n;  heapsize = 0;  heap[1] = 0;
	for (i = 0; i < n; i++) {
		len[i] = 0;
		if (freq[i]) heap[++heapsize] = i;
	}
	if (heapsize < 2) return heap[1];
	for (i = heapsize / 2; i >= 1; i--)
		downheap(i);  /* make priority queue */
	do {  /* while queue has at least two entries */
		i = heap[1];  /* take out least-freq entry */
		heap[1] = heap[heapsize--];
		downheap(1);
		j = heap[1];  /* next least-freq entry */
		k = avail++;  /* generate new node */
		freq[k] = freq[i] + freq[j];
		heap[1] = k;  downheap(1);  /* put into queue */
		left[k] = i;  right[k] = j;
	} while (heapsize > 1);
	for (i = 0; i < n; i++) len[i] = 0;
	make_len(k);
	return k;  /* return root */
}
