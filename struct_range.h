#ifndef STRUCT_RANGE_H_
#define STRUCT_RANGE_H_


typedef struct Range
{
	short int holeCardHigh;	            //	REF: 200+flushDraw          LIST: made primary card, or -2 where board is best 5-card hand
	short unsigned int holeCardSize;	//	REF: 16+size of board       LIST: size of holeCardArray
	short int* holeCardArray;           //  REF: NULL                   LIST: product of 13 and subcombo + made secondary card, or NULL where holeCardHigh==-2
	short int best[5];		            //	REF: cards and 52 if n/a    LIST: best 5-card made hand
    short unsigned int key;	            //  REF: ternary flush texture of board, or 76+flushDraw (>2210 ternary) if flushSize>2
                                        //      LIST: binary mask for player hand that connects with board, or 200+flushDraw if flushSize>2
	//	ref flushSize; node key where {200+flushDraw} identifies flush, {204+flushDraw} = SF
	//short unsigned int flushKey;        //      LIST: indicates flush draws and backdoor flush draws where flushSize<3
	unsigned int combo;		            //	REF: comboCount             LIST: sum of subcombos
	unsigned int mod_prob;              //  REF: total of tiers         LIST: integer representation of likelihood of appearance within player range
    struct Range* next;
} Range;

typedef struct Range Range;

struct Range* newRangeHeadNode(int err);


void appendTier(struct Range* head,short int a,short int b[],short int b_size,short int card_0,short int card_1,short int card_2,short int card_3,short int card_4,unsigned short int key,unsigned short int combo);

void appendTierByChenKeySort(struct Range* head,short int hi,short int lo,unsigned short int modChen,unsigned short int temp);

void freeRange(struct Range** head);

void swap(short int* a, short int* b);

int howManyCards(short int cards[],short int size,short int lowerLimit,short int upperLimit);	//returns no of cards within [lower,upper]

int suitKey(short int cards[],short int size,short int bCards[],short int bSize,short int madeCard);

short int isStraight(short int cards[],short int size,short int a,short int b);	// returns madeCard-high straight on true,0 on false

void sortMade(short int cards[],short int size);

struct Range* printMade(short int cards[],short int size,short int bCards[],short int boxedSize);

#endif // STRUCT_RANGE_H_
