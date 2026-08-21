#ifndef WINDOWS_GUI_H_
#define WINDOWS_GUI_H_

struct Range;
struct Frac;


int isInArray(short int arr[], short int l, short int x);

void SetColorAndBackground(int ForgC, int BackC);

void SetColor(int ForgC);

void printAid(void);

void printLiveCards(short int cards[],short int size, short int boxedCards[],short int boxedSize,short int flushDraw);

void printShort(short int card);

void printShortX(short int card);

void printCard(short int card);

void printSuit(short int card);

void printBoard(short int cards[],short int size,short int key);

void printRange(struct Range* head);

void printRangeWithImprove(struct Range* range,struct Frac *val);

void printExpandedRange(struct Range *head);

void printRaw(struct Range* head);

void printHistogram(struct Range *head,struct Frac* val);


#endif // WINDOWS_GUI_H_
