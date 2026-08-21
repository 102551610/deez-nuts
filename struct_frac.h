#ifndef STRUCT_FRAC_H_
#define STRUCT_FRAC_H_

typedef struct Frac
{
	signed int num;
	unsigned short int den;

	struct Frac* next;
} Frac;

#endif // STRUCT_FRAC_H_
