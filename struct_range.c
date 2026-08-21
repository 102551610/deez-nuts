#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "struct_range.h"


struct Range* newRangeHeadNode(int err)
{
    struct Range* ptr=(struct Range*)malloc(sizeof(struct Range));

    if(ptr==NULL)
	{
		//printf("Out of memory!\n");
		exit(700+err);
    }

	ptr->key=63;      //
	ptr->next=NULL;

	return ptr;
}



void appendTier(struct Range* head,short int a,short int b[],short int b_size,short int card_0,short int card_1,short int card_2,short int card_3,short int card_4,unsigned short int key,unsigned short int combo)
{
	struct Range* temp	= (struct Range*)malloc(sizeof(struct Range));
	struct Range* curr	= head;
	short int n;

    if(temp==NULL)
	{
		//printf("Out of memory!\n");
    	exit(700);
    }

    if(a<0)
    {
    	temp->holeCardHigh	= a;
    	temp->holeCardSize	= b_size;	// number of total unique hands
    	temp->holeCardArray	= NULL;
    }
    else
    {
    	temp->holeCardArray	= (short int*)malloc(b_size * sizeof(short int));

		if(temp->holeCardArray==NULL)
		{
//			printf("Out of memory!\n");
	    	exit(701);
	    }

		if(b_size==1)
		{
			temp->holeCardSize		= 1;
			b[0]%=16;

			if(a<b[0])
			{
				temp->holeCardHigh		= b[0];
				temp->holeCardArray[0]	= a;
			}
			else
			{
				temp->holeCardHigh		= a;
				temp->holeCardArray[0]	= b[0];
			}
			temp->holeCardArray[0]	+= 16*combo-16;
		}
		else
		{
			for(n=0;n<b_size;++n)
				temp->holeCardArray[n] = b[n];
			temp->holeCardSize		= b_size;
			temp->holeCardHigh		= a;
		}
	}

	temp->best[0]		= card_0;
	temp->best[1]		= card_1;
	temp->best[2]		= card_2;
	temp->best[3]		= card_3;
	temp->best[4]		= card_4;
	temp->key			= key;
//	temp->flushKey      = flushKey;
	temp->combo			= combo;
	temp->mod_prob		= 0;
	temp->next			= NULL;

	++head->mod_prob;//		+= combo;

	while((curr->next)!=NULL)
		curr			= curr->next;

	curr->next			= temp;
	return;
}



void appendTierByChenKeySort(struct Range* head,short int hi,short int lo,unsigned short int modChen,unsigned short int temp)
{
	//modChen = 2*F(Chen formula)+3
	struct Range* node	= (struct Range*)malloc(sizeof(struct Range));
	struct Range* curr	= head;
	short int i;
		//appendTier(head,n,x,1,13,13,13,13,13,k,temp);

	if(node==NULL)
	{
//		printf("Out of memory!\n");
    	exit(702);
	}
	node->holeCardHigh	= hi;
	node->holeCardSize	= 1;
	node->holeCardArray	= (short int*)malloc(sizeof(short int));

	if(node->holeCardArray==NULL)
	{
//		printf("Out of memory!\n");
	    exit(703);
	}

	for(i=0;i<5;++i)
		node->best[i]	=  13;
	node->key				=  modChen;
	node->combo				=  temp;
	node->mod_prob			=  0;
	--temp;
	node->holeCardArray[0]	=  16*temp+lo;
	++head->mod_prob;//			+= node->combo;

	while((curr->next)!=NULL  &&  curr->next->key%128>=node->key%128)
		curr			= curr->next;

	node->next			= curr->next;
	curr->next			= node;
}



void freeRange(struct Range** head)
{
	struct Range* temp;

	while (*head  !=  NULL)
	{
		temp=*head;
		*head=(*head)->next;
		free(temp->holeCardArray);
		free(temp);
	}

	return;
}



void swap(short int* a, short int* b)
{
	short int temp;
	temp=*a;
	*a=*b;
	*b=temp;
}



int howManyCards(short int cards[],short int size,short int lowerLimit,short int upperLimit)	//returns no of cards within [lower,upper]
{
	int i;
	int count;
	if(upperLimit==lowerLimit)
		for(i=count=0;i<size;++i)
		{
			if(cards[i]==upperLimit)
				++count;
		}
	else
		for(i=count=0;i<size;++i)
			if((cards[i]>=lowerLimit)  &&  (cards[i]<=upperLimit))
				++count;

	//printf("minorOuts: %d\tl:%d\tu:%d\n",count,lowerLimit,upperLimit);
	return count;
}



int suitKey(short int cards[],short int size,short int bCards[],short int bSize,short int madeCard)
{
    short int n=0;
    int result=0;

    for(madeCard%=13*4;n<4;++n)
        result*=2+1-howManyCards(cards,size,madeCard+n,madeCard+n)-howManyCards(bCards,bSize,madeCard+n,madeCard+n);

    return result;
}



short int isStraight(short int cards[],short int size,short int a,short int b)	// returns madeCard-high straight on true,0 on false
{
	short int n,count,i;

	if(b>a) {n=a;a=b;b=n;}

	if(a>cards[0])			{n=a-1;	a=b;b=n+1;	i=0;}
	else if(a==cards[0])	{n=a-1;	a=b;b=n+1;	i=1;}
	else					{n=cards[0]-1;		i=1;}

	for(count=1;n>-2;--n)
		if((n+13)%13==cards[i%size])
		{
	//		printf("a");
			if(count>3)
				return n+4;
	//			{printf("-%c\n",face[n+6]);return n+4;}

			if(i<size)	++i;
			if(a==n)a=b;
			++count;
		}
		else if((n+13)%13==a)
		{
	//		printf("b");
			if(count>3)
				return n+4;
	//			{printf("-%c\n",face[n+6]);return n+4;}

			a=b;
			++count;
		}
		else if(n>3)
			count=0;
	//		{count=0;printf("%c",face[n+2]);}
		else
			return 0;
	//		{printf("f\n");return 0;}

	exit(704);	//inaccessible code
}



void sortMade(short int cards[],short int size)
{

	/*		takes 3-5 cards, prints poker hand, e.g. AACCB where A>B>C	*
	 *	if pair+: check for trips+ otherwise highest pair				*
	 *	swap to start of array, check for 2nd pair, otherwise kicker	*
	 *	if trips+: put trips at front, higher kicker first				*
	 *	if 2nd pair: swap relevant cards								*/
	int n,i,count;
	short int sorted[size];

	for(n=count=0;n<14;++n)
	{
		for(i=0;i<size;++i)
		if((((cards[i]/4)+12)%13)==(13-n))
		{
			sorted[count]=13-n;
			++count;
		}
	}

	//	1:AAA	2:AAB	1:ABB	2:ABC
	//	1:AAAA	2:AAAB	2:AABB	2:AABC	1:ABBB	4:ABBC	3:ABCC	5:ABCD
	//	2:AAAAB	2:AAABB	2:AAABC	3:AABBB	5:AABBC	5:AABCC	5:AABCD
	//	1:ABBBB	4:ABBBC	6:ABBCC	6:ABBCD	3:ABCCC	:ABCCD	:ABCDD	:ABCDE

	if(sorted[1]==sorted[size-1])			//AAA,ABB,AAAA,ABBB,ABBBB
		swap(&sorted[0],&sorted[size-1]);

	else if(sorted[0]==sorted[size-3]);		//AAB,ABC,AAAB,AABB,AABC,AAAAB,AAABB,AAABC

	else if(sorted[2]==sorted[size-1])		//ABCC,AABBB,ABCCC
	{
		swap(&sorted[0],&sorted[size-2]);		//CBAC,BABAB,CBCAC
		swap(&sorted[1],&sorted[size-1]);
	}

	else if(sorted[1]==sorted[size-2])		//ABBC,ABBBC
		swap(&sorted[0],&sorted[size-2]);

	else if(sorted[0]==sorted[size-4])		//ABCD,AABBC,AABCC,AABCD
	{
		if(sorted[size-2]==sorted[size-1])
			swap(&sorted[size-3],&sorted[size-1]);
	}

	else if(sorted[1]==sorted[2])			//ABBCC,ABBCD
	{
		swap(&sorted[0],&sorted[2]);		//BBACC,BBACD

		if(sorted[3]==sorted[4])
			swap(&sorted[2],&sorted[4]);
	}

	else if(sorted[2]==sorted[3])			//ABCCD
	{
		swap(&sorted[0],&sorted[2]);
		swap(&sorted[1],&sorted[3]);
	}

	else if(sorted[3]==sorted[4])			//ABCDD
	{
		swap(&sorted[2],&sorted[4]);
		swap(&sorted[0],&sorted[2]);
		swap(&sorted[1],&sorted[3]);
	}

	for(n=0;n<size;++n)	cards[n]=sorted[n];

	return;
}



struct Range* printMade(short int cards[],short int size,short int bCards[],short int boxedSize)
/*	prints hierarchy to Range						        */
/*	A>B>C>D>E, a!= b!= c!= d!= e							*/
/*	x= any non-relevant card (doesn't improve best five)	*/
{
	//unsigned int comboCount 	= 0;	// when size==5 should count to 47*46 == combo
	short int madeCards[size];			// 0123456789ABC	henceforth referred to as madeCard format
										// 23456789TJQKA
	short int boxedCards[boxedSize];
	short int boxedFlushCards[10];
	unsigned long int temp		= 0;
	short int majorOuts			= 0;	// number of 'high' possibilities
	short int minorOuts			= 0;	// number of 'low' possibilities
	short int miniOuts			= 0;	// number of other possibilities
	short int highestLiveCard,lowestLiveCard;
	short int i,j,k,n;
	short int x[13];					// array passed to appendTier
	short int x_size			= 1;	// size of array x[] to dynamically allocate when function appendTier() is called
	short int straightCards[size];		// ignores dupes, madeCard format
	short int straightSize		= 0;	// number of unique cards
	short int flushCards[size];			// made cards of suit flushDraw
	short int flushSize;				// number of unique cards of suit flushDraw if flush, else 0
	short int majorFlush,minorFlush;	// bit to indicate presence of particular card or lack thereof
	short int boxedFlushSize;           // number of unique cards of suit flushDraw in boxedCards prior to made sort
	short int flushDraw;                // corresponds to modulus of card[] (suit) if flush, or ternary representation of number of each suit if >3
	struct Range *head			= (struct Range*)malloc(sizeof(struct Range));

	if(head==NULL)
	{
//		printf("Out of memory!\n");
    	exit(705);
    }

	for(n=0;n<size;++n)			madeCards[n]=head->best[n]=cards[n];
	while(n<5)
	{
		head->best[n]=52;
		++n;
	}
	for(n=0;n<boxedSize;++n)	boxedCards[n]=bCards[n];

	if(size)	sortMade(madeCards,size);

	//figuring out all the flush outs/blockers in respective arrays

	for(n=flushDraw=flushSize=boxedFlushSize=0;n<4;++n)  //  records suits in ternary (0,1,2 reps none, one, and two of given suit respectively)
    {
        flushDraw*=9;
        for(i=0;i<size;++i)
            if(cards[i]%4==n)
                ++flushDraw;

        //printf("\nflushSize: %d",flushSize);
        if((flushDraw%9)/3)         // least significant digit overflows into 0*3^1 if flushSize%9>2
        {
            flushDraw=n;

            for(i=12;i>=0;--i)
			{
				for(j=0;j<size;++j)
					if(cards[j]==(i*4+4+flushDraw)%52)
					{
						flushCards[flushSize]=i;
						++flushSize;
						j=size;
					}

				for(j=0;j<boxedSize;++j)
					if(boxedCards[j]==(i*4+4+flushDraw)%52)
					{
						boxedFlushCards[boxedFlushSize]=i;
						++boxedFlushSize;
						j=boxedSize;
					}
			}

            n=4;
        }
        else    //{printf("\t%d %d %d %d %d",flushSize/81%3,(flushSize/27)%3,(flushSize/9)%3,flushSize/3%3,flushSize%3);flushSize=(flushSize/3)+(flushSize%3);printf("\t\t%d %d %d %d = %d",(flushSize/27)%9,(flushSize/9)%3,flushSize/3%3,flushSize%3,flushSize);}
            flushDraw=(flushDraw/3)+(flushDraw%3);
    }
    if(0)//flushDraw>3)
    {
        flushDraw=-1;
        //flushSize=boxedFlushSize=0;
    }

    /*for(i=0;i<size-2;++i)
	{
		flushSize=1;
		for(j=i+1;j<size;++j)
			if(cards[i]%4==cards[j]%4)
				++flushSize;
		if(flushSize>2)
		{
			flushDraw=cards[i]%4;

			boxedFlushSize=flushSize=0;

			for(i=12;i>=0;--i)
			{
				for(j=0;j<size;++j)
					if(cards[j]==(i*4+4+flushDraw)%52)
					{
						flushCards[flushSize]=i;
						++flushSize;
						j=size;
					}

				for(j=0;j<boxedSize;++j)
					if(boxedCards[j]==(i*4+4+flushDraw)%52)
					{
						boxedFlushCards[boxedFlushSize]=i;
						++boxedFlushSize;
						j=boxedSize;
					}
			}

			i=size;
		}
		else
		{
			flushDraw=-1;
			flushSize=boxedFlushSize=0;
		}
	}*/

	for(i=12;i>=0;--i)
		for(j=0;j<size;++j)
			if(madeCards[j]==i)
			{
				straightCards[straightSize]=i;
				++straightSize;
				j=size;
			}

	for(n=0;12-n==straightCards[n]  &&  n<straightSize;++n);
	highestLiveCard=12-n;
	for(n=0;n==straightCards[straightSize-1-n]  &&  n<straightSize;++n);
	lowestLiveCard=n;

	//initial node
	head->holeCardSize	= 16+size;
	head->holeCardHigh	= flushDraw;//200+flushDraw;
	head->holeCardArray = NULL;
	head->combo			= ((52-size-boxedSize)*(52-size-boxedSize-1))/2;
	head->mod_prob		= 0; // REF node contains tier
	head->key			= flushSize;
	head->next			= NULL;

	for(n=0;n<boxedSize;++n)	boxedCards[n]=((boxedCards[n]/4)+12)%13;

	//	populate preflop range according to Chen formula, j, as follows:
	//	A high = 10 points (12->20)
	//	K high = 8 points (11->16)
	//	Q high = 7 points (10->14)
	//	J high = 6 points (9->12)
	//	T through 2 high = half of face value (i.e. T = 5, 9 = 4.5)
	//	Pairs, multiply score by 2 (i.e. KK = 16), minimum score for a pair is 5 (so pairs of 2 through 4 get a 5 score)
	//	Suited cards, add two points to highest card score
	//	Connectors add 1 point (i.e. KQ)
	//	One gap, subtract 1 point (i.e. T8)
	//	Two gap, subtract 2 points (i.e. AJ)
	//	Three gap, subtract 4 points (i.e. J7)
	//	Four or more gap, subtract 5 points (i.e. A4)
	//
	//	note that node->key is unsigned int so hands like 72o return -1.5
	//	(i.e. negative non-integer) so j is offset by k=2*j+3 such that
	//	max range of k is {72o,AA}:{0,43}
	//	suited bonus affects k but duplicates hole cards so k+=128 if suited

	if(!size)
	{
		head->key=0;

		for(n=12;n>=0;--n)
			if( (majorOuts=4-howManyCards(boxedCards,boxedSize,n,n)) )    //double brackets added after first instance of -Wparentheses for assignment '=' invoked
			{
				j=n+2+(n>8)+(n>9)+(n>10)+((n==12)*3);
				if((temp=(majorOuts*majorOuts-majorOuts)/2))
				{
					if(j<5)	k=13;
					else	k=2*j+3;

					//appendTier(head,n,x,1,13,13,13,13,13,k,temp);
					appendTierByChenKeySort(head,n,n,k,temp);
				}

				for(i=n-1;i>=0;--i)
					if( (minorOuts=4-howManyCards(boxedCards,boxedSize,i,i)) )
					{
						temp=majorOuts*minorOuts;
						//x[0]=i;

						k=2*((n==i+1)-(n>i+1)-(n>i+2)-(n>i+4)-(2*(n>i+3)))+j+3;

						for(x_size=0;x_size<4;++x_size)
							if(!(howManyCards(bCards,boxedSize,(n+1)%13*4+x_size,(n+1)%13*4+x_size)  ||  howManyCards(bCards,boxedSize,(i+1)%13*4+x_size,(i+1)%13*4+x_size)))
								++size;

						if(size)
						{
							//appendTier(head,n,x,1,13,13,13,13,13,k+68,size);
							appendTierByChenKeySort(head,n,i,k+132,size);
							temp-=size;
							size=0;
						}
						if(temp)
							//appendTier(head,n,x,1,13,13,13,13,13,k,temp);
							appendTierByChenKeySort(head,n,i,k,temp);
					}
			}

		return head;
	}

	//STRAIGHT FLUSHES (inc. ROYAL)

	if(flushSize)
		for(n=j=miniOuts=0;n<flushSize-1;++n)
			if(flushCards[n]+1-(flushCards[(n+2)%flushSize]+1)%13<5)
				for(k=(flushCards[(n+2)%flushSize]+1)%13+3;k>=flushCards[n]  &&  k<=flushCards[n]+2  &&  k>2;--k)
					if((k<13)  &!  (n  &&  j  &&  k>=flushCards[n-1])  &!  howManyCards(flushCards,flushSize,k+1,k+1))
					{
						for(i=0;(k-i+13)%13==(flushCards[(n+i)%flushSize]+13)%13;++i);
						for(j=i+1;(k-j+13)%13==(flushCards[(n+j-1)%flushSize]+13)%13;++j);


						if(j<5)
						{
							if(!howManyCards(boxedFlushCards,boxedFlushSize,k-i,k-i)  &!  howManyCards(boxedFlushCards,boxedFlushSize,(k-j+13)%13,(k-j+13)%13))
							{
								//printTest("sf1");
								x[0]=(k-j+13)%13+pow(2,flushDraw+4);
								printf("%d ",x[0]);
								appendTier(head,pow(2,flushDraw+4)+k-i,x,1,k,k-1,k-2,k-3,k-4,204+flushDraw,1);

								++miniOuts;
							}
						}
						else if(i<5)
						{
							if(!howManyCards(boxedFlushCards,boxedFlushSize,(k-i+13)%13,(k-i+13)%13))
							{
								x_size=temp=0;
								for(j=12;j>=0;--j)
									if( (majorOuts  =  4  -  howManyCards(madeCards,size,j,j)  -  howManyCards(boxedCards,boxedSize,j,j)  -  (j==((k-i+13)%13))  -  ((j==(k+1))  &!  howManyCards(boxedFlushCards,boxedFlushSize,k+1,k+1))) )
									{
										x[x_size]=majorOuts*16+j-16;
										++x_size;
										temp+=majorOuts;
									}

								if(temp)
								{
									//printTest("sf2");
									appendTier(head,(k-i+13)%13,x,x_size,k,k-1,k-2,k-3,k-4,204+flushDraw,temp);

									miniOuts+=temp;
								}
							}
						}
						else
						{
							temp=((52-size-boxedSize)*(52-size-boxedSize-1))/2-miniOuts;

							if(temp)
							{
								for(x_size=minorOuts=n=0;n<14;++n)
									if( (majorOuts  =  4  -  howManyCards(madeCards,size,n,n)  -  howManyCards(boxedCards,boxedSize,n,n)) )
									{
										++minorOuts;
										if(majorOuts>1)	++x_size;
									}

								x_size+=(minorOuts*minorOuts-minorOuts)/2;

								//printTest("sf3");
								appendTier(head,-2,x,x_size,k,k-1,k-2,k-3,k-4,204+flushDraw,temp);
							}

							return head;
						}
					}

	//QUADS AND BOOKS
	if(madeCards[0]==madeCards[1])	//aaxxx
	{
		if(madeCards[1]==madeCards[2])	//aaaxx
		{
			if(madeCards[2]==madeCards[3]  &&  size>3)	//aaaab
			{
				if(size==5)
				{
					for(n=highestLiveCard;n>madeCards[4];--n)
					{
						if((n!=madeCards[0])  &&  (majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n)))
						{
							temp=x_size=0;

							if(majorOuts>1)
							{
								x[x_size]  =  16*(majorOuts*majorOuts-majorOuts)/2+n-16;
								++x_size;
								temp+=(majorOuts*majorOuts-majorOuts)/2;
							}

							for(i=n-1;i>=0;--i)
							{
								minorOuts=4-howManyCards(boxedCards,boxedSize,i,i)-(i==madeCards[4]);
								if((i!=madeCards[0])  &&  minorOuts)
								{
									x[x_size]  =  16*majorOuts*minorOuts+i-16;
									++x_size;
									temp+=majorOuts*minorOuts;
								}
							}

							if(temp)	//aaaab+ overcard to b
							{
								//printTest("quad+n");
								appendTier(head,n,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],n,16,temp);
							}
						}
					}

					minorOuts=3-howManyCards(boxedCards,boxedSize,madeCards[4],madeCards[4]);
					for((n=madeCards[4]-1)  &&  (x_size=0);n>=lowestLiveCard;--n)
						if((n!=madeCards[0])  &&  (j=4-howManyCards(boxedCards,boxedSize,n,n)))
						{
							minorOuts +=  j;
							x[x_size]  =  n;
							++x_size;
						}
					temp  =  (minorOuts*minorOuts-minorOuts)/2;

					if(temp)
					{
						for(x_size=minorOuts=n=0;n<14;++n)
							if( (majorOuts  =  4  -  howManyCards(madeCards,size,n,n)  -  howManyCards(boxedCards,boxedSize,n,n)) )
							{
								++minorOuts;
								if(majorOuts>1)	++x_size;
							}

						x_size+=(minorOuts*minorOuts-minorOuts)/2;

						//printTest("quad+b");
						appendTier(head,-2,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],madeCards[4],0,temp);
					}
				}
				else //if(size==4)
				{
					for(n=highestLiveCard;n>=lowestLiveCard;--n)
					{
						if((n!=madeCards[0])  &&  (majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n)))
						{
							x_size=temp=0;

							if(majorOuts>1)
							{
								x[x_size] = 16*(majorOuts*majorOuts-majorOuts)/2+n-16;
								++x_size;
								temp+=(majorOuts*majorOuts-majorOuts)/2;
							}
							for(i=n-1;i>=lowestLiveCard;--i)
								if((i!=madeCards[0])  &&  (minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i)))
								{
									x[x_size] = 16*majorOuts*minorOuts+i-16;
									++x_size;
									temp += majorOuts*minorOuts;
								}

							if(temp)
							{
								//printTest("quad+c");
								appendTier(head,n,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],n,16,temp);
							}
						}
					}
				}

				return head;
			}
			else if(madeCards[3]==madeCards[4]  &&  size==5)	//aaabb
			{
				majorOuts =  1  -  howManyCards(boxedCards,boxedSize,madeCards[0],madeCards[0]);
				minorOuts =  2  -  howManyCards(boxedCards,boxedSize,madeCards[3],madeCards[3]);
				miniOuts  =  52 -  boxedSize  -  size  -  majorOuts  -  minorOuts;

				if(madeCards[3]>madeCards[0]  &&  minorOuts==2)	//BBBAA+AA
				{
					//printTest("BfoA+AA");
					x[0]=madeCards[3];
					appendTier(head,madeCards[3],x,1,madeCards[3],madeCards[3],madeCards[3],madeCards[3],madeCards[0],3,1);
				}
				if(majorOuts  &&  minorOuts+miniOuts)	//aaabb+ax
				{
					for(n=x_size=0;n<=12;++n)
						if(howManyCards(madeCards,size,12-n,12-n)  +  howManyCards(boxedCards,boxedSize,12-n,12-n)  +  (12-n==madeCards[0])  <  4)
						{
							x[x_size]=16*(4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n))+12-n-16;
							++x_size;
						}
					//printTest("afob+ax");
					appendTier(head,madeCards[0],x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[0],madeCards[3],1,minorOuts+miniOuts);
				}
				if(madeCards[3]<madeCards[0]  &&  minorOuts==2)	//AAABB+BB
				{
					//printTest("AfoB+BB");
					x[0]=madeCards[3];
					appendTier(head,madeCards[3],x,1,madeCards[3],madeCards[3],madeCards[3],madeCards[3],madeCards[0],3,1);
				}
				if(madeCards[3]>madeCards[0]  &&  minorOuts  &&  miniOuts)	//BBBAA+Ax
				{
					for(n=x_size=0;n<=12;++n)
						if((12-n!=madeCards[0])  &&  (12-n!=madeCards[3])  &&  (howManyCards(madeCards,size,12-n,12-n)  +  howManyCards(boxedCards,boxedSize,12-n,12-n)  <  4))
						{
							x[x_size]=16*minorOuts*(4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n))+12-n-16;
							++x_size;
						}
					//printTest("BfoA+Ax");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[0]+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,madeCards[3],x,x_size,madeCards[3],madeCards[3],madeCards[3],madeCards[0],madeCards[0],1,minorOuts*miniOuts);
				}

				k=temp=0;
				for(n=highestLiveCard;n>madeCards[3];--n)	//aaabb+overpair to b
					if(n!=madeCards[0])
					{
						temp  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
						temp  =  (temp*temp-temp)/2;
						k+=temp;

						if(temp)
						{
							//printTest("afob+nn");
							x[0]=n;
							appendTier(head,n,x,1,madeCards[0],madeCards[0],madeCards[0],n,n,24,temp);
						}
					}

				temp=(miniOuts*miniOuts-miniOuts)/2-k;
				if(madeCards[3]<madeCards[0])	temp+=minorOuts*miniOuts;

				if(temp)
				{
					for(x_size=minorOuts=n=0;n<14;++n)
						if( (majorOuts  =  4  -  howManyCards(madeCards,size,n,n)  -  howManyCards(boxedCards,boxedSize,n,n)) )
						{
							++minorOuts;
							if(majorOuts>1)	++x_size;
						}

					x_size+=(minorOuts*minorOuts-minorOuts)/2;

					//printTest("aaabbXX");
					appendTier(head,-2,x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[3],madeCards[3],0,temp);
				}

				return head;
			}
			else //aaabc,aaab,aaa
			{
				majorOuts =  1  -  howManyCards(boxedCards,boxedSize,madeCards[0],madeCards[0]);
				minorOuts =  3  -  howManyCards(boxedCards,boxedSize,madeCards[3],madeCards[3]);
				miniOuts  =  3  -  howManyCards(boxedCards,boxedSize,madeCards[4],madeCards[4]);

				if(majorOuts  &&  ((size+boxedSize)<51))	//aaabc+ ax
				{
					for(n=x_size=0;n<=12;++n)
						if(howManyCards(madeCards,size,12-n,12-n)  +  howManyCards(boxedCards,boxedSize,12-n,12-n)  +  (12-n==madeCards[0])  <  4)
						{
							x[x_size]=16*(4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n))+12-n-16;
							++x_size;
						}
					//printTest("aaabc+a");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[0]+2],face[x[i]%13+2]);printf("\n");
					if(size==3)
						appendTier(head,madeCards[0],x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[0],lowestLiveCard,1,51-size-boxedSize);
					else
						appendTier(head,madeCards[0],x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[0],madeCards[3],1,51-size-boxedSize);
				}

				if((madeCards[3]>madeCards[0])  &&  (minorOuts>1)  &&  size>3)	//BBBAC+,CCCAB+ AA
				{
					//printTest("BBB+AA");
					x[0]=madeCards[3];
					appendTier(head,madeCards[3],x,1,madeCards[3],madeCards[3],madeCards[3],madeCards[0],madeCards[0],3,(minorOuts*minorOuts-minorOuts)/2);
				}

				if((madeCards[4]>madeCards[0])  &&  (miniOuts>1)  &&  size>4)	//CCCAB+ BB
				{
					//printTest("CCC+AA");
					x[0]=madeCards[4];
					appendTier(head,madeCards[4],x,1,madeCards[4],madeCards[4],madeCards[4],madeCards[0],madeCards[0],3,(miniOuts*miniOuts-miniOuts)/2);
				}

				for(n=12;n>=0;--n)	//aaabc+ pockets
				{
					if(n==madeCards[3]  &&  size>3)	//aaabc+ bx
					{
						for(i=x_size=temp=0;i<=12;++i)
							if(12-i==madeCards[3])
							{
								if(12-i<madeCards[0]  &&  minorOuts>1)
								{
									x[x_size]=16*(minorOuts*minorOuts-minorOuts)/2+12-i-16;
									++x_size;
									temp+=(minorOuts*minorOuts-minorOuts)/2;
								}
							}
							else if((12-i!=madeCards[0])  &&  (k=4-howManyCards(boxedCards,boxedSize,12-i,12-i)-((12-i==madeCards[4])&&size==5)))
							{
								x[x_size]=16*minorOuts*k+12-i-16;
								++x_size;
								temp+=minorOuts*k;
							}

						if(temp)
						{
							//printTest("AAA+BB");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
							appendTier(head,n,x,x_size,madeCards[0],madeCards[0],madeCards[0],n,n,8,temp);
						}
					}
					else if(n==madeCards[4]  &&  size==5)	//AAABC+,BBBAC+ Cx
					{
						for(i=x_size=temp=0;i<=12;++i)
							if(12-i==madeCards[4])
							{
								if(12-i<madeCards[0]  &&  miniOuts>1)
								{
									x[x_size]=16*(miniOuts*miniOuts-miniOuts)/2+12-i-16;
									++x_size;
									temp+=(miniOuts*miniOuts-miniOuts)/2;
								}
							}
							else if((12-i!=madeCards[0])  &&  (12-i!=madeCards[3])  &&  (k=4-howManyCards(boxedCards,boxedSize,12-i,12-i)))
							{
								x[x_size]=16*miniOuts*k+12-i-16;
								++x_size;
								temp+=miniOuts*k;
							}

						if(temp)
						{
							//printTest("aaa+CC");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
							appendTier(head,n,x,x_size,madeCards[0],madeCards[0],madeCards[0],n,n,8,temp);
						}
					}
					else if(n!=madeCards[0])
					{
						k=4-howManyCards(boxedCards,boxedSize,n,n);
						temp=(k*k-k)/2;

						if(temp)
						{
							//printTest("aaa+nn");
							x[0]=n;
							appendTier(head,n,x,1,madeCards[0],madeCards[0],madeCards[0],n,n,24,temp);
						}
					}
				}
			}
		}
		else if(madeCards[2]==madeCards[3]  &&  size>3)	//aabbc,aabb
		{
			majorOuts  =  2  -  howManyCards(boxedCards,boxedSize,madeCards[0],madeCards[0]);
			minorOuts  =  2  -  howManyCards(boxedCards,boxedSize,madeCards[2],madeCards[2]);

			if(size==5)	miniOuts  =  3  -  howManyCards(boxedCards,boxedSize,madeCards[4],madeCards[4]);
			else		miniOuts  =  0;

			k  =  52 -   size  -  boxedSize  -  majorOuts  -  minorOuts  -  miniOuts;

			if(majorOuts==2)	//aabbc+ aa
			{
				//printTest("1ai");
				x[0]=madeCards[0];
				appendTier(head,madeCards[0],x,1,madeCards[0],madeCards[0],madeCards[0],madeCards[0],madeCards[2],3,1);
			}

			if(minorOuts==2)	//aabbc+ bb
			{
				//printTest("1aii");
				x[0]=madeCards[2];
				appendTier(head,madeCards[2],x,1,madeCards[2],madeCards[2],madeCards[2],madeCards[2],madeCards[0],3,1);
			}

			if(size==4)
			{
				if(majorOuts  &&  (k  ||  minorOuts))
				{
					for(n=x_size=temp=0;n<=12;++n)
						if((12-n!=madeCards[0])  &&  (j=4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n)))
						{
							x[x_size]=16*j*majorOuts+12-n-16;
							++x_size;
							temp+=j*majorOuts;
						}
					//printTest("1aiii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[0]+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,madeCards[0],x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[2],madeCards[2],1,temp);
				}
				if(k  &&  minorOuts)
				{
					for(n=x_size=temp=0;n<=12;++n)
						if((12-n!=madeCards[0])  &&  (12-n!=madeCards[2])  &&  (j=4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n)))
						{
							x[x_size]=16*j*minorOuts+12-n-16;
							++x_size;
							temp+=j*minorOuts;
						}
					//printTest("1aiv");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[2]+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,madeCards[2],x,x_size,madeCards[2],madeCards[2],madeCards[2],madeCards[0],madeCards[0],1,temp);
				}
			}
			else if(madeCards[2]>madeCards[4])	//AABBC
			{
				if(majorOuts  &&  (k  ||  minorOuts  ||  miniOuts))
				{
					for(n=x_size=temp=0;n<=12;++n)
						if((12-n!=madeCards[0])  &&  (j=4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n)))
						{
							x[x_size]=16*j*majorOuts+12-n-16;
							++x_size;
							temp+=j*majorOuts;
						}
					//printTest("1bi");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[0]+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,madeCards[0],x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[2],madeCards[2],1,temp);
				}

				if(minorOuts  &&  (k  ||  miniOuts))
				{
					for(n=x_size=temp=0;n<=12;++n)
						if((12-n!=madeCards[0])  &&  (12-n!=madeCards[2])  &&  (j=4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n)))
						{
							x[x_size]=16*j*minorOuts+12-n-16;
							++x_size;
							temp+=j*minorOuts;
						}
					//printTest("1bii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[2]+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,madeCards[2],x,x_size,madeCards[2],madeCards[2],madeCards[2],madeCards[0],madeCards[0],1,temp);
				}

				if(miniOuts>1)
				{
					//printTest("1biii");
					x[0]=madeCards[4];
					appendTier(head,madeCards[4],x,1,madeCards[4],madeCards[4],madeCards[4],madeCards[0],madeCards[0],3,(miniOuts*miniOuts-miniOuts)/2);
				}
			}
			else if(madeCards[0]>madeCards[4])	//AACCB
			{
				if(majorOuts  &&  miniOuts)
				{
					//printTest("1ci");
					x[0]=madeCards[4];
					appendTier(head,madeCards[0],x,1,madeCards[0],madeCards[0],madeCards[0],madeCards[4],madeCards[4],9,majorOuts*miniOuts);
				}

				if(majorOuts  &&  k+minorOuts>0)
				{
					for(n=x_size=temp=0;n<=12;++n)
						if((12-n!=madeCards[0])  &&  (12-n!=madeCards[4])  &&  (j=4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n)))
						{
							x[x_size]=16*j*majorOuts+12-n-16;
							++x_size;
							temp+=j*majorOuts;
						}
					//printTest("1cii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[0]+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,madeCards[0],x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[2],madeCards[2],1,temp);
				}

				if(miniOuts>1)
				{
					//printTest("1ciii");
					x[0]=madeCards[4];
					appendTier(head,madeCards[4],x,1,madeCards[4],madeCards[4],madeCards[4],madeCards[0],madeCards[0],3,(miniOuts*miniOuts-miniOuts)/2);
				}

				if(minorOuts  &&  k+miniOuts>0)
				{
					for(n=x_size=temp=0;n<=12;++n)
						if((12-n!=madeCards[0])  &&  (12-n!=madeCards[2])  &&  (j=4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n)))
						{
							x[x_size]=16*j*minorOuts+12-n-16;
							++x_size;
							temp+=j*minorOuts;
						}
					//printTest("1civ");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[2]+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,madeCards[2],x,x_size,madeCards[2],madeCards[2],madeCards[2],madeCards[0],madeCards[0],1,temp);
				}
			}
			else	//BBCCA
			{
				if(miniOuts>1)
				{
					//printTest("1di");
					x[0]=madeCards[4];
					appendTier(head,madeCards[4],x,1,madeCards[4],madeCards[4],madeCards[4],madeCards[0],madeCards[0],3,(miniOuts*miniOuts-miniOuts)/2);
				}

				if(majorOuts  &&  miniOuts)
				{
					//printTest("1dii");
					x[0]=madeCards[0];
					appendTier(head,madeCards[4],x,1,madeCards[0],madeCards[0],madeCards[0],madeCards[4],madeCards[4],9,majorOuts*miniOuts);
				}

				if(majorOuts  &&  minorOuts+k>0)
				{
					for(n=x_size=temp=0;n<=12;++n)
						if((12-n!=madeCards[0])  &&  (12-n!=madeCards[4])  &&  (j=4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n)))
						{
							x[x_size]=16*j*majorOuts+12-n-16;
							++x_size;
							temp+=j*majorOuts;
						}
					//printTest("1diii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[0]+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,madeCards[0],x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[2],madeCards[2],1,temp);
				}

				if(minorOuts  &&  miniOuts)
				{
					//printTest("1div");
					x[0]=madeCards[2];
					appendTier(head,madeCards[4],x,1,madeCards[2],madeCards[2],madeCards[2],madeCards[4],madeCards[4],9,minorOuts*miniOuts);
				}
				if(minorOuts  &&  k)
				{
					for(n=x_size=temp=0;n<=12;++n)
						if((12-n!=madeCards[0])  &&  (12-n!=madeCards[2])  &&  (12-n!=madeCards[4])  &&  (j=4-howManyCards(madeCards,size,12-n,12-n)-howManyCards(boxedCards,boxedSize,12-n,12-n)))
						{
							x[x_size]=16*j*minorOuts+12-n-16;
							++x_size;
							temp+=j*minorOuts;
						}
					//printTest("1dv");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[2]+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,madeCards[2],x,x_size,madeCards[2],madeCards[2],madeCards[2],madeCards[0],madeCards[0],1,temp);
				}
			}
		}
		else	//aabcd
		{
			majorOuts  =  2  -  howManyCards(boxedCards,boxedSize,madeCards[0],madeCards[0]);
			minorOuts  =  3  -  howManyCards(boxedCards,boxedSize,madeCards[2],madeCards[2]);

			if(size>3)	miniOuts  =  3  -  howManyCards(boxedCards,boxedSize,madeCards[3],madeCards[3]);
			else		miniOuts  =  0;

			if(size>4)	k  =  3  -  howManyCards(boxedCards,boxedSize,madeCards[4],madeCards[4]);
			else		k  =  0;

			if(majorOuts==2)	//aabcd+ aa
			{
				//printTest("2a");
				x[0]=madeCards[0];
				appendTier(head,madeCards[0],x,1,madeCards[0],madeCards[0],madeCards[0],madeCards[0],madeCards[2],3,1);
			}

			if(madeCards[2]>madeCards[0]  &&  minorOuts>1)
			{
				//printTest("2bi");
				x[0]=madeCards[2];
				appendTier(head,madeCards[2],x,1,madeCards[2],madeCards[2],madeCards[2],madeCards[0],madeCards[0],3,(minorOuts*minorOuts-minorOuts)/2);
			}
			if(madeCards[3]>madeCards[0]  &&  miniOuts>1)
			{
				//printTest("2bii");
				x[0]=madeCards[3];
				appendTier(head,madeCards[3],x,1,madeCards[3],madeCards[3],madeCards[3],madeCards[0],madeCards[0],3,(miniOuts*miniOuts-miniOuts)/2);
			}
			if(madeCards[4]>madeCards[0]  &&  k>1)
			{
				//printTest("2biii");
				x[0]=madeCards[4];
				appendTier(head,madeCards[4],x,1,madeCards[4],madeCards[4],madeCards[4],madeCards[0],madeCards[0],3,(k*k-k)/2);
			}

			if(majorOuts  &&  minorOuts)
			{
				//printTest("2ci");
				x[0]=madeCards[2];
				appendTier(head,madeCards[0],x,1,madeCards[0],madeCards[0],madeCards[0],madeCards[2],madeCards[2],9,majorOuts*minorOuts);
			}
			if(majorOuts  &&  miniOuts)
			{
				//printTest("2cii");
				x[0]=madeCards[3];
				appendTier(head,madeCards[0],x,1,madeCards[0],madeCards[0],madeCards[0],madeCards[3],madeCards[3],9,majorOuts*miniOuts);
			}
			if(majorOuts  &&  k)
			{
				//printTest("2ciii");
				x[0]=madeCards[4];
				appendTier(head,madeCards[0],x,1,madeCards[0],madeCards[0],madeCards[0],madeCards[4],madeCards[4],9,majorOuts*k);
			}

			if(madeCards[2]<madeCards[0]  &&  minorOuts>1)
			{
				//printTest("2di");
				x[0]=madeCards[2];
				appendTier(head,madeCards[2],x,1,madeCards[2],madeCards[2],madeCards[2],madeCards[0],madeCards[0],3,(minorOuts*minorOuts-minorOuts)/2);
			}
			if(madeCards[3]<madeCards[0]  &&  miniOuts>1)
			{
				//printTest("2dii");
				x[0]=madeCards[3];
				appendTier(head,madeCards[3],x,1,madeCards[3],madeCards[3],madeCards[3],madeCards[0],madeCards[0],3,(miniOuts*miniOuts-miniOuts)/2);
			}
			if(madeCards[4]<madeCards[0]  &&  k>1)
			{
				//printTest("2diii");
				x[0]=madeCards[4];
				appendTier(head,madeCards[4],x,1,madeCards[4],madeCards[4],madeCards[4],madeCards[0],madeCards[0],3,(k*k-k)/2);
			}
		}
	}

	//FLUSHES

	if(flushSize)
	{
		for(k=0;howManyCards(boxedFlushCards,boxedFlushSize,k,k)  ||  howManyCards(flushCards,flushSize,k,k);++k);

		if(flushSize==3)
		{
			for(n=12;n>k;--n)
			{
				temp=x_size=0;

				if(!howManyCards(boxedFlushCards,boxedFlushSize,n,n)  &!  howManyCards(flushCards,flushSize,n,n))
				{
					for(i=n-1;i>=k;--i)
						if(!howManyCards(boxedFlushCards,boxedFlushSize,i,i)  &!  howManyCards(flushCards,flushSize,i,i)  &!  isStraight(flushCards,flushSize,n,i))
						{
							x[x_size]=i;
							++x_size;
							++temp;
						}

					if(temp)
					{
						//printTest("flush1");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n%13+2],face[x[i]%13+2]);printf("\n");
						appendTier(head,n,x,x_size,n,k,flushCards[0],flushCards[1],flushCards[2],200+flushDraw,temp);
					}
				}
			}
		}
		else if(flushSize==4)
		{
			for(n=12;n>=0;--n)
				if(!(howManyCards(boxedFlushCards,boxedFlushSize,n,n)  ||  howManyCards(flushCards,flushSize,n,n)  ||  isStraight(flushCards,flushSize,n,n)))
				{
					temp=x_size=0;

					for(i=12;i>=0;--i)
						if( (majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i)  -  howManyCards(madeCards,size,i,i)  -  (((i>=n)||(isStraight(flushCards,flushSize,n,i)))&!(howManyCards(boxedFlushCards,boxedFlushSize,i,i)||howManyCards(flushCards,flushSize,i,i)))) )
						{
							x[x_size]=16*majorOuts+i-16;
							++x_size;
							temp+=majorOuts;
						}

					if(temp)
					{
						//printTest("flush2");
						appendTier(head,n,x,x_size,n,flushCards[0],flushCards[1],flushCards[2],flushCards[3],200+flushDraw,temp);
					}
				}
		}
		else //flushSize==5
		{
			for(n=12;n>flushCards[4];--n)
				if(!howManyCards(boxedFlushCards,boxedFlushSize,n,n)  &!  howManyCards(flushCards,flushSize,n,n)  &!  isStraight(flushCards,flushSize,n,n))
				{
					x_size=temp=0;
					for(i=12;i>=0;--i)
						if(!howManyCards(boxedFlushCards,boxedFlushSize,n,n)  &!  howManyCards(flushCards,flushSize,n,n))
						{
							majorOuts  =  4  -  howManyCards(madeCards,size,i,i)  -  howManyCards(boxedCards,boxedSize,i,i)  -  (n==i)  -  (((i>n)  ||  (isStraight(flushCards,flushSize,n,i)))  &!  (howManyCards(boxedFlushCards,boxedFlushSize,i,i)  ||  howManyCards(flushCards,flushSize,i,i)));

							if(majorOuts)
							{
								x[x_size]=16*majorOuts+i-16;
								++x_size;
								temp+=majorOuts;
							}
						}

					if(temp)
					{
						//printTest("flush3");
						appendTier(head,n,x,x_size,n,flushCards[0],flushCards[1],flushCards[2],flushCards[3],200+flushDraw,temp);

						miniOuts+=temp;
					}
				}

			majorOuts  =  52  -  size  -  boxedSize;
			temp=(majorOuts*majorOuts-majorOuts)/2-miniOuts;

			if(temp)
			{
				for(x_size=minorOuts=n=0;n<14;++n)
					if( (majorOuts  =  4  -  howManyCards(madeCards,size,n,n)  -  howManyCards(boxedCards,boxedSize,n,n)) )
					{
						++minorOuts;
						if(majorOuts>1)	++x_size;
					}

				x_size+=(minorOuts*minorOuts-minorOuts)/2;

				//printTest("flXX");
				appendTier(head,-2,x,x_size,flushCards[0],flushCards[1],flushCards[2],flushCards[3],flushCards[4],200+flushDraw,temp);
			}

			return head;
		}
	}

	//STRAIGHTS

	for(n=j=0;n<straightSize-1;++n)
		if(straightCards[n]+1-(straightCards[(n+2)%straightSize]+1)%13<5)
			for(k=(straightCards[(n+2)%straightSize]+1)%13+3;k>=straightCards[n]  &&  k<=straightCards[n]+2  &&  k>2;--k)
				if(((k<13)  &!  (n  &&  j  &&  k>=straightCards[n-1]))  &!  howManyCards(madeCards,size,k+1,k+1))
				{
					for(i=0;(k-i+13)%13==(straightCards[(n+i)%straightSize]+13)%13;++i);
					for(j=i+1;(k-j+13)%13==(straightCards[(n+j-1)%straightSize]+13)%13;++j);

					if(j<5)
					{
						majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,k-i,k-i);
						minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,(k-j+13)%13,(k-j+13)%13);

						majorFlush =  1  -  howManyCards(flushCards,flushSize,k-i,k-i)  -  howManyCards(boxedFlushCards,boxedFlushSize,k-i,k-i);
						minorFlush =  1  -  howManyCards(flushCards,flushSize,(k-j+13)%13,(k-j+13)%13)  -  howManyCards(boxedFlushCards,boxedFlushSize,(k-j+13)%13,(k-j+13)%13);

						if(flushSize==3)
							temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							temp  =  majorOuts  *  minorOuts;

						if(temp)
						{
							//printTest("str1");
							x[0]=(k-j+13)%13;
							appendTier(head,k-i,x,1,k,k-1,k-2,k-3,k-4,(short)pow(2,i)+pow(2,j),temp);
						}
					}
					else if(i<5)
					{
						temp=x_size=0;

						if( (majorOuts=4-howManyCards(boxedCards,boxedSize,(k-i+13)%13,(k-i+13)%13)) )
						{
							majorFlush =  1  -  howManyCards(flushCards,flushSize,(k-i+13)%13,(k-i+13)%13)  -  howManyCards(boxedFlushCards,boxedFlushSize,(k-i+13)%13,(k-i+13)%13);

							for(j=12;j>=0;--j)
							{
								if((j==(k-i+13)%13))
								{
									miniOuts=(majorOuts*majorOuts-majorOuts)/2;

									if(!(flushSize<4  ||  howManyCards(boxedFlushCards,boxedFlushSize,j,j)  ||  howManyCards(flushCards,flushSize,j,j)))
										miniOuts/=2;
								}
								else if((j!=k+1)  &&  (minorOuts=4-howManyCards(boxedCards,boxedSize,j,j)-howManyCards(madeCards,size,j,j)))
								{
									minorFlush =  1  -  howManyCards(boxedFlushCards,boxedFlushSize,j,j)  -  howManyCards(flushCards,flushSize,j,j);

									if(flushSize==3)
										miniOuts  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
									else if(flushSize==4)
										miniOuts  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
									else
										miniOuts  =  majorOuts  *  minorOuts;
								}
								else miniOuts=0;

								if(miniOuts)
								{
									x[x_size]=16*miniOuts+j-16;
									++x_size;
									temp+=miniOuts;
								}
							}

							if(temp)
							{
								//printTest("str2");for(j=0;j<x_size;++j)printf("%2d*%c%c ",x[j]/13,face[(k-i+15)%13],face[x[j]%13+2]);printf("\n");
								appendTier(head,(k-i+13)%13,x,x_size,k,k-1,k-2,k-3,k-4,(short)pow(2,i),temp);
							}
						}
					}
					else
					{
						if(k<12)
						{
							majorOuts  =  48  -  size  -  boxedSize  +  howManyCards(boxedCards,boxedSize,k+1,k+1);
							majorFlush =  12  -  flushSize  -  boxedFlushSize  +  howManyCards(flushCards,flushSize,k+1,k+1)  +  howManyCards(boxedFlushCards,boxedFlushSize,k+1,k+1);
						}
						else
						{
							majorOuts  =  52  -  size  -  boxedSize;
							majorFlush =  13  -  flushSize  -  boxedFlushSize;
						}

						if(flushSize==3)
							temp  =  (majorOuts*majorOuts-majorOuts)/2  -  (majorFlush*majorFlush-majorFlush)/2;
						else if(flushSize==4)
							temp  =  ((majorOuts-majorFlush)*(majorOuts-majorFlush)-(majorOuts-majorFlush))/2;
						else
							temp  =  (majorOuts*majorOuts-majorOuts)/2;

						if(temp)
						{
							for(x_size=minorOuts=n=0;n<14;++n)
								if( (majorOuts  =  4  -  howManyCards(madeCards,size,n,n)  -  howManyCards(boxedCards,boxedSize,n,n)) )
								{
									++minorOuts;
									if(majorOuts>1)	++x_size;
								}

							x_size+=(minorOuts*minorOuts-minorOuts)/2;

							//printTest("strX");
							appendTier(head,-2,x,x_size,k,k-1,k-2,k-3,k-4,(short)pow(2,i)+pow(2,j),temp);
						}
					}

					if(i>4)
					{
						return head;
					}
				}

	//TRIPS

	if(madeCards[0]==madeCards[2])	//aaabc,aaab
	{
		if(size>3)
		{
			for(n=highestLiveCard;n>madeCards[3];--n)
			{
				majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
				majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

				if(n!=madeCards[0]  &&  majorOuts)
				{
					for(i=n-1;i>madeCards[3];--i)
						if((i!=madeCards[0])  &!  isStraight(straightCards,straightSize,n,i))
						{
							minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
							minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

							if(flushSize==3)
								temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else
								temp  =  majorOuts*minorOuts;

							if(temp)
							{
								//printTest("aaabc1");
								x[0]=i;
								appendTier(head,n,x,1,madeCards[0],madeCards[0],madeCards[0],n,i,24,temp);
							}
						}

					i=madeCards[3]-1;
					for(temp=x_size=0;i>=lowestLiveCard;--i)
						if((i!=madeCards[0]  &&  i!=madeCards[size-1])  &!  isStraight(straightCards,straightSize,n,i))
						{
							minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
							minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

							if(flushSize==3)
								miniOuts  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else
								miniOuts  =  majorOuts*minorOuts;

							if(miniOuts)
							{
								x[x_size]=16*miniOuts+i-16;
								++x_size;
								temp+=miniOuts;
							}
						}

					if(temp)
					{
						//printTest("aaabc2");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
						appendTier(head,n,x,x_size,madeCards[0],madeCards[0],madeCards[0],n,madeCards[3],8,temp);
					}
				}
			}
			for(n=madeCards[3]-1;n>madeCards[size-1];--n)
			{
				majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
				majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

				if(n!=madeCards[0]  &&  majorOuts)
				{
					i=n-1;
					for(temp=x_size=0;i>=lowestLiveCard;--i)
						if((i!=madeCards[0]  &&  i!=madeCards[4])  &!  isStraight(straightCards,straightSize,n,i))
						{
							minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
							minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

							if(flushSize==3)
								miniOuts  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else
								miniOuts  =  majorOuts*minorOuts;

							if(miniOuts)
							{
								x[x_size]=16*miniOuts+i-16;
								++x_size;
								temp+=miniOuts;
							}
						}

					if(temp)
					{
						//printTest("aaabc3");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
						appendTier(head,n,x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[3],n,16,temp);
					}
				}
			}

			if(size==5)
			{
				majorOuts=minorOuts=majorFlush=minorFlush=temp=0;
				for(n=i=lowestLiveCard;n<madeCards[size-1];++n)
					if(n!=madeCards[0])
					{
						if(!isStraight(straightCards,straightSize,n,i))
						{
							minorOuts  +=  majorOuts;
							minorFlush +=  majorFlush;

							majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
							majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

							if(flushSize==3)
								temp  +=  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else
								temp  +=  majorOuts  *  minorOuts;

							i=n;
						}
						else
						{
							majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
							majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

							if(flushSize==3)
								temp  +=  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else
								temp  +=  majorOuts  *  minorOuts;

							minorOuts  +=  majorOuts;
							minorFlush +=  majorFlush;
						}
					}

				if(temp)
				{
					for(x_size=minorOuts=n=0;n<14;++n)
						if( (majorOuts  =  4  -  howManyCards(madeCards,size,n,n)  -  howManyCards(boxedCards,boxedSize,n,n)) )
						{
							++minorOuts;
							if(majorOuts>1)	++x_size;
						}

					x_size+=(minorOuts*minorOuts-minorOuts)/2;

					//printTest("aaabcX");
					appendTier(head,-2,x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[3],madeCards[4],0,temp);
				}
			}
			else// if(size==4)
			{
				for(n=madeCards[3]-1;n>lowestLiveCard;--n)
					if((n!=madeCards[0])  &&  (majorOuts=4-howManyCards(boxedCards,boxedSize,n,n)))
					{
						x_size=temp=0;

						for(i=n-1;i>=lowestLiveCard;--i)
							if((i!=madeCards[0])  &&  (minorOuts=4-howManyCards(boxedCards,boxedSize,i,i)))
							{
								x[x_size]=16*majorOuts*minorOuts+i-16;
								++x_size;
								temp+=majorOuts*minorOuts;
							}

						if(temp)
						{
							//printTest("aaabx");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
							appendTier(head,n,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],n,16,temp);
						}
					}
			}
		}
		else //if(size==3)
		{
			for(n=highestLiveCard;n>lowestLiveCard;--n)
				if(n!=madeCards[0]  &&  (majorOuts=4-howManyCards(boxedCards,boxedSize,n,n)))
					for(i=n-1;i>=lowestLiveCard;--i)
						if(i!=madeCards[0]  &&  (minorOuts=4-howManyCards(boxedCards,boxedSize,i,i)))
						{
							temp=majorOuts*minorOuts;

							if(temp)
							{
								//printTest("aaaxx");
								x[0]=i;
								appendTier(head,n,x,1,madeCards[0],madeCards[1],madeCards[2],n,i,24,temp);
							}
						}
		}

		return head;
	}
	else if(madeCards[0]==madeCards[1]  &&  (madeCards[2]!=madeCards[3]  &&  size>3))	//aabcd,aabc
	{
		majorOuts  =  2  -  howManyCards(boxedCards,boxedSize,madeCards[0],madeCards[0]);
		majorFlush =  1  -  howManyCards(flushCards,flushSize,madeCards[0],madeCards[0])  -  howManyCards(boxedFlushCards,boxedFlushSize,madeCards[0],madeCards[0]);

		for(n=highestLiveCard;n>madeCards[3];--n)
			if((n!=madeCards[0]  &&  n!=madeCards[2])  &!  isStraight(straightCards,straightSize,n,n))
			{
				minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
				minorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

				if(flushSize==3)
					temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
				else if(flushSize==4)
					temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
				else
					temp  =  majorOuts  *  minorOuts;

				if(temp)
				{
					//printTest("3ai");
					x[0]=madeCards[0];
					if(madeCards[2]>n)	appendTier(head,n,x,1,madeCards[0],madeCards[0],madeCards[0],madeCards[2],n,17,temp);
					else				appendTier(head,n,x,1,madeCards[0],madeCards[0],madeCards[0],n,madeCards[2],9,temp);
				}
			}

		minorOuts=minorFlush=temp=x_size=0;
		for(n=madeCards[3]-1;n>=lowestLiveCard;--n)
			if((n!=madeCards[0]  &&  n!=madeCards[size-1])  &!  isStraight(straightCards,straightSize,n,n))
			{
				minorOuts  = 4  -  howManyCards(boxedCards,boxedSize,n,n);
				minorFlush = 1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

				if(flushSize==3)
					k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
				else if(flushSize==4)
					k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
				else
					k  =  majorOuts  *  minorOuts;

				if(k)
				{
					temp+=k;
					x[x_size]=16*k+n-16;
					++x_size;
				}
			}

		if(temp)
		{
			//printTest("3aii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[madeCards[0]+2],face[x[i]%13+2]);printf("\n");
			appendTier(head,madeCards[0],x,x_size,madeCards[0],madeCards[0],madeCards[0],madeCards[2],madeCards[3],1,temp);
		}
	}
	else if(madeCards[0]==madeCards[1]  &&  size==3)	//aab
	{
		majorOuts  =  2  -  howManyCards(boxedCards,boxedSize,madeCards[0],madeCards[0]);

		if(majorOuts)
			for(n=highestLiveCard;n>=lowestLiveCard;--n)
				if(n!=madeCards[0]  &&  n!=madeCards[2])
				{
					minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
					temp  =  majorOuts*minorOuts;

					if(temp)
					{
						//printTest("3aiii");
						x[0]=n;
						if(n>madeCards[2])	appendTier(head,madeCards[0],x,1,madeCards[0],madeCards[0],madeCards[0],n,madeCards[2],9,temp);
						else				appendTier(head,madeCards[0],x,1,madeCards[0],madeCards[0],madeCards[0],madeCards[2],n,17,temp);
					}
				}
	}
	else if(madeCards[0]!=madeCards[1])	//abcde
	{
		for(n=0;n<size;++n)
		{
			majorOuts  =  3  -  howManyCards(boxedCards,boxedSize,madeCards[n],madeCards[n]);
			majorFlush =  1  -  howManyCards(flushCards,flushSize,madeCards[n],madeCards[n])  -  howManyCards(boxedFlushCards,boxedFlushSize,madeCards[n],madeCards[n]);
			if(flushSize==4)	majorOuts-=majorFlush;
			temp=(majorOuts*majorOuts-majorOuts)/2;

			if(temp)
			{
				//printTest("3b");
				x[0]=madeCards[n];
				if(!n)			appendTier(head,madeCards[n],x,1,madeCards[n],madeCards[n],madeCards[n],madeCards[1],madeCards[2],3,temp);
				else if(n==1)	appendTier(head,madeCards[n],x,1,madeCards[n],madeCards[n],madeCards[n],madeCards[0],madeCards[2],3,temp);
				else			appendTier(head,madeCards[n],x,1,madeCards[n],madeCards[n],madeCards[n],madeCards[0],madeCards[1],3,temp);
			}
		}
	}

	//2PAIRS

	if(madeCards[0]==madeCards[1])
	{
		if(madeCards[2]==madeCards[3]  &&  size>3)	//aabbc,aabb
		{
			for(n=12;n>madeCards[2];--n)
			{
				majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
				majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

				if(n==madeCards[4]  &&  majorOuts>1  &&  size==5)	//BBCCA+ Ax;AACCB+ Bx
				{
					--majorOuts;

					for(i=highestLiveCard;i>madeCards[2];--i)
					{
						minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
						minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else
							temp  =  majorOuts  *  minorOuts;

						if(i!=madeCards[0]  &&  i!=madeCards[4]  &&  temp)
						{
							//printTest("4ai");
							x[0]=i;
							if(n>madeCards[0])	appendTier(head,n,x,1,n,n,madeCards[0],madeCards[0],i,17,temp);
							else				appendTier(head,n,x,1,madeCards[0],madeCards[0],n,n,i,20,temp);
						}
					}

					temp=x_size=0;
					for(i=madeCards[2]-1;i>=lowestLiveCard;--i)
					{
						minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
						minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
						k=0;

						if(flushSize==3)
							k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else
							k  =  majorOuts  *  minorOuts;

						if(k)
						{
							temp+=k;
							x[x_size]=16*k+i-16;
							++x_size;
						}
					}

					if(temp)	//aabbc(BBCCA)+ cx
					{
						//printTest("4aii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
						if(n>madeCards[0])	appendTier(head,n,x,x_size,n,n,madeCards[0],madeCards[0],madeCards[2],3,temp);
						else				appendTier(head,n,x,x_size,madeCards[0],madeCards[0],n,n,madeCards[2],12,temp);
					}
				}
				else if(n!=madeCards[0]  &&  n!=madeCards[2]  &&  majorOuts>1)
				{
					if(madeCards[2]>madeCards[4]  ||  size<5)	i=madeCards[2];
					else										i=madeCards[4];
					temp=(majorOuts*majorOuts-majorOuts)/2;

					//printTest("4aiii");
					x[0]=n;
					if(n>madeCards[0])	appendTier(head,n,x,1,n,n,madeCards[0],madeCards[0],i,3,temp);
					else				appendTier(head,n,x,1,madeCards[0],madeCards[0],n,n,i,12,temp);
				}
			}
			if(size==5)
			{
				for(n=highestLiveCard;n>madeCards[4];--n)
				{
					majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
					majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);
					x_size=temp=0;

					if(n!=madeCards[0]  &&  n!=madeCards[2]  &&  majorOuts)
					{
						if(majorOuts>1  &&  n<madeCards[2])
						{
							temp=(majorOuts*majorOuts-majorOuts)/2;
							x[x_size]=16*temp+n-16;
							++x_size;
						}

						for(i=n-1;i>=0;--i)
						{
							minorOuts=minorFlush=k=0;
							if(i==madeCards[4]  &&  i<madeCards[2])
							{
								minorOuts  =  3  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
							}
							else if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i))
							{
								minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
							}

							if(flushSize==3)
								k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else
								k  =  majorOuts  *  minorOuts;

							if(k)
							{
								x[x_size]=16*k+i-16;
								++x_size;
								temp+=k;
							}
						}

						if(temp)
						{
							//printTest("4b");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
							appendTier(head,n,x,x_size,madeCards[0],madeCards[0],madeCards[2],madeCards[2],n,16,temp);
						}
					}
				}

				majorOuts=minorOuts=majorFlush=minorFlush=temp=0;
				for(n=i=lowestLiveCard;n<madeCards[4];++n)
					if(n!=madeCards[0]  &&  n!=madeCards[2])
					{
						if(!isStraight(straightCards,straightSize,n,i))
						{
							minorOuts  +=  majorOuts;
							minorFlush +=  majorFlush;

							majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
							majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

							if(flushSize==3)
								temp  +=  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else
								temp  +=  majorOuts  *  minorOuts;

							i=n;
						}
						else
						{
							majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
							majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

							if(flushSize==3)
								temp  +=  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else
								temp  +=  majorOuts  *  minorOuts;

							if(n<madeCards[2]  ||  madeCards[0]<5)	//potential wheels and aabbc where a-c=2 double majorOuts in terminating calculation
							{
								minorOuts  +=  4  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush +=  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
							}
						}

						if(n<madeCards[2])
							temp  +=  (majorOuts*majorOuts-majorOuts)/2;
					}
				if(madeCards[4]<madeCards[2])	temp+=(3-howManyCards(boxedCards,boxedSize,n,n))  *  (minorOuts+majorOuts);

				if(temp)
				{
					for(x_size=minorOuts=n=0;n<14;++n)
						if( (majorOuts  =  4  -  howManyCards(madeCards,size,n,n)  -  howManyCards(boxedCards,boxedSize,n,n)) )
						{
							++minorOuts;
							if(majorOuts>1)	++x_size;
						}

					x_size+=(minorOuts*minorOuts-minorOuts)/2;

					//printTest("aabbcX");
					appendTier(head,-2,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],madeCards[4],0,temp);
				}
			}
			else
			{
				for(n=highestLiveCard;n>=lowestLiveCard;--n)
					if(n!=madeCards[0]  &&  n!=madeCards[2]  &&  (majorOuts=4-howManyCards(boxedCards,boxedSize,n,n)))
					{
						if(n<madeCards[2]  &&  majorOuts>1)
						{
							temp=(majorOuts*majorOuts-majorOuts)/2;
							x[0]=16*temp+n-16;
							x_size=1;
						}
						else temp=x_size=0;

						for(i=n-1;i>=lowestLiveCard;--i)
							if(i!=madeCards[0]  &&  i!=madeCards[2]  &&  (minorOuts=4-howManyCards(boxedCards,boxedSize,i,i)))
							{
								x[x_size]=16*majorOuts*minorOuts+i-16;
								++x_size;
								temp+=majorOuts*minorOuts;
							}

						if(temp)
						{
							//printTest("aabbX");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
							appendTier(head,n,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],n,16,temp);
						}
					}
			}

			return head;
		}
		else if(madeCards[1]!=madeCards[2])	//aabcd,aabc,aab
		{
			for(n=12;n>=0;--n)
			{
				for(majorOuts=4-howManyCards(boxedCards,boxedSize,n,n);isStraight(straightCards,straightSize,n,n);majorOuts=4-howManyCards(boxedCards,boxedSize,n,n))
					--n;
				majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

				if(n==madeCards[2]  &&  majorOuts>1)
				{
					--majorOuts;
					if(size>3)
					{
						if(madeCards[0]<madeCards[4]  &&  size==5)	//DDABC+ AB,AC
						{
							minorOuts =  3  -  howManyCards(boxedCards,boxedSize,madeCards[3],madeCards[3]);
							miniOuts  =  3  -  howManyCards(boxedCards,boxedSize,madeCards[4],madeCards[4]);

							if(minorOuts)	//AB
							{
								//printTest("5ai");
								x[0]=madeCards[3];
								appendTier(head,n,x,1,n,n,madeCards[3],madeCards[3],madeCards[4],5,majorOuts*minorOuts);
							}
							if(miniOuts)	//AC
							{
								//printTest("5aii");
								x[0]=madeCards[4];
								appendTier(head,n,x,1,n,n,madeCards[4],madeCards[4],madeCards[3],5,majorOuts*miniOuts);
							}
						}
						else if(madeCards[0]<madeCards[3])	//CCABD+ AB
						{
							minorOuts=3-howManyCards(boxedCards,boxedSize,madeCards[3],madeCards[3]);

							if(minorOuts)
							{
								//printTest("5aiii");
								x[0]=madeCards[3];
								appendTier(head,n,x,1,n,n,madeCards[3],madeCards[3],madeCards[0],5,majorOuts*minorOuts);
							}
						}

						for(i=highestLiveCard;i>madeCards[3];--i)
						{
							if((i!=madeCards[0]  &&  i!=madeCards[2])  &!  isStraight(straightCards,straightSize,n,i))
							{
								minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

								if(flushSize==3)
									temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
								else if(flushSize==4)
									temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
								else
									temp  =  majorOuts*minorOuts;

								if(temp)	//aabcd+ bi
								{
									//printTest("5bi");
									x[0]=i;
									if(n>madeCards[0])	appendTier(head,n,x,1,n,n,madeCards[0],madeCards[0],i,17,temp);
									else				appendTier(head,n,x,1,madeCards[0],madeCards[0],n,n,i,20,temp);
								}
							}
						}

						k=x_size=temp=0;
						if(madeCards[0]>madeCards[3])	//AABCD+ bx,bc,bd;BBACD+ Ax,Ac,Ad
						{
							for(i=madeCards[3];i>=0;--i)
							{
								if(i==madeCards[3]  ||  i==madeCards[size-1])
								{
									minorOuts  =  3  -  howManyCards(boxedCards,boxedSize,i,i);
									minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
								}
								else if(!isStraight(straightCards,straightSize,n,i))
								{
									minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
									minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
								}
								else minorOuts=minorFlush=0;

								if(flushSize==3)
									k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
								else if(flushSize==4)
									k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
								else
									k  =  majorOuts*minorOuts;

								if(k)
								{
									x[x_size]=16*k+i-16;
									++x_size;
									temp+=k;
								}
							}
						}
						else if(madeCards[0]>madeCards[4]  &&  size==5)	//CCABD+ Ax,AD
						{
							for(i=madeCards[3]-1;i>=0;--i)
							{
								if(i==madeCards[4])
								{
									minorOuts  =  3  -  howManyCards(boxedCards,boxedSize,i,i);
									minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
								}
								else if((i!=madeCards[0])  &!  isStraight(straightCards,straightSize,n,i))
								{
									minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
									minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
								}
								else minorOuts=minorFlush=0;

								if(flushSize==3)
									k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
								else if(flushSize==4)
									k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
								else
									k  =  majorOuts*minorOuts;

								if(k)
								{
									x[x_size]=16*k+i-16;
									++x_size;
									temp+=k;
								}
							}
						}
						else	//DDABC+ Ax
						{
							for(i=madeCards[3]-1;i>=lowestLiveCard;--i)
							{
								if((i!=madeCards[0]  &&  i!=madeCards[size-1])  &!  isStraight(straightCards,straightSize,n,i))
								{
									minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
									minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
								}
								else minorOuts=minorFlush=0;

								if(flushSize==3)
									k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
								else if(flushSize==4)
									k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
								else
									k  =  majorOuts*minorOuts;

								if(k)
								{
									x[x_size]=16*k+i-16;
									++x_size;
									temp+=k;
								}
							}
						}

						if(temp)	//aabcd+ bx
						{
							//printTest("5bii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
							if(n>madeCards[0])	appendTier(head,n,x,x_size,n,n,madeCards[0],madeCards[0],madeCards[3],1,temp);
							else				appendTier(head,n,x,x_size,madeCards[0],madeCards[0],n,n,madeCards[3],4,temp);
						}
					}
					else //if(size==3)
						for(i=highestLiveCard;i>=lowestLiveCard;--i)
							if(i!=madeCards[0]  &&  i!=n)
							{
								minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush =  1  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

								if(flushSize==3)
									temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
								else
									temp  =  majorOuts*minorOuts;

								if(temp)
								{
									//printTest("5biii");
									x[0]=i;
									if(n>madeCards[0])	appendTier(head,n,x,1,n,n,madeCards[0],madeCards[0],i,17,temp);
									else				appendTier(head,n,x,1,madeCards[0],madeCards[0],n,n,i,20,temp);
								}
							}
				}
				else if(n==madeCards[3]  &&  size>3  &&  majorOuts>1)
				{
					--majorOuts;

					if(madeCards[0]<madeCards[4]  &&  size==5)	//DDABC+ BC
					{
						temp  =  majorOuts  *  (3  -  howManyCards(boxedCards,boxedSize,madeCards[4],madeCards[4]));

						if(temp)
						{
							//printTest("5c");
							x[0]=madeCards[4];
							appendTier(head,madeCards[3],x,1,madeCards[3],madeCards[3],madeCards[4],madeCards[4],madeCards[2],5,temp);
						}
					}

					for(i=highestLiveCard;i>madeCards[2];--i)
					{
						if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i))
						{
							minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
							minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

							if(flushSize==3)
								temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else if(flushSize==4)
								temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
							else
								temp  =  majorOuts*minorOuts;

							if(temp)	//aabcd+ ci
							{
								//printTest("5di");
								x[0]=i;
								if(n>madeCards[0])	appendTier(head,n,x,1,n,n,madeCards[0],madeCards[0],i,5,temp);
								else				appendTier(head,n,x,1,madeCards[0],madeCards[0],n,n,i,20,temp);
							}
						}
					}

					x_size=temp=0;
					if(madeCards[0]>madeCards[4]  &&  size==5)	//AABCD+,BBACD+,CCABD+ cD,cx
					{
						for(i=madeCards[2]-1;i>=0;--i)
						{
							if(i==madeCards[4])
							{
								minorOuts  =  3  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
							}
							else if((i!=madeCards[0]  &&  i!=n)  &!  isStraight(straightCards,straightSize,n,i))
							{
								minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
							}
							else minorOuts=minorFlush=0;

							if(flushSize==3)
								k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else if(flushSize==4)
								k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
							else
								k  =  majorOuts*minorOuts;

							if(k)
							{
								x[x_size]=16*k+i-16;
								++x_size;
								temp+=k;
							}
						}
					}
					else	//DDABC+ Bx
					{
						for(i=madeCards[2]-1;i>=lowestLiveCard;--i)
						{
							if((i!=madeCards[0]  &&  i!=n  &&  i!=madeCards[size-1])  &!  isStraight(straightCards,straightSize,n,i))
							{
								minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
							}
							else minorOuts=minorFlush=0;

							if(flushSize==3)
								k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else if(flushSize==4)
								k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
							else
								k  =  majorOuts*minorOuts;

							if(k)
							{
								x[x_size]=16*k+i-16;
								++x_size;
								temp+=k;
							}
						}
					}

					if(temp)	//aabcd+ cx
					{
						//printTest("5dii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
						if(n>madeCards[0])	appendTier(head,n,x,x_size,n,n,madeCards[0],madeCards[0],madeCards[2],1,temp);
						else				appendTier(head,n,x,x_size,madeCards[0],madeCards[0],n,n,madeCards[2],4,temp);
					}
				}
				else if(n==madeCards[4]  &&  size==5  &&  majorOuts>1)
				{
					--majorOuts;

					for(i=highestLiveCard;i>madeCards[2];--i)
					{
						if((i!=madeCards[0])  &!  isStraight(straightCards,straightSize,n,i))
						{
							minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
							minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

							if(flushSize==3)
								temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else if(flushSize==4)
								temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
							else
								temp  =  majorOuts*minorOuts;

							if(temp)	//aabcd+ di
							{
								//printTest("5ei");
								x[0]=i;
								if(n>madeCards[0])	appendTier(head,n,x,1,n,n,madeCards[0],madeCards[0],i,17,temp);
								else				appendTier(head,n,x,1,madeCards[0],madeCards[0],n,n,i,20,temp);
							}
						}
					}

					x_size=temp=0;
					for(i=madeCards[2]-1;i>=lowestLiveCard;--i)
					{
						if(!isStraight(straightCards,straightSize,n,i)  &&  i!=madeCards[0]  &&  i!=madeCards[3]  &&  i!=n)
						{
							minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
							minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
						}
						else minorOuts=minorFlush=0;

						if(flushSize==3)
							k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							k  =  majorOuts*minorOuts;

						if(k)
						{
							x[x_size]=16*k+i-16;
							++x_size;
							temp+=k;
						}
					}

					if(temp)	//aabcd+ dx
					{
						//printTest("5eii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
						if(n>madeCards[0])	appendTier(head,n,x,x_size,n,n,madeCards[0],madeCards[0],madeCards[2],1,temp);
						else				appendTier(head,n,x,x_size,madeCards[0],madeCards[0],n,n,madeCards[2],4,temp);
					}
				}
				else if(n!=madeCards[0]  &&  n>=0)
				{
					if(flushSize==4  &&  majorFlush)	--majorOuts;
					temp=(majorOuts*majorOuts-majorOuts)/2;

					if(temp)
					{
						//printTest("5f");
						x[0]=n;
						if(n>madeCards[0])	appendTier(head,n,x,1,n,n,madeCards[0],madeCards[0],madeCards[2],3,temp);
						else				appendTier(head,n,x,1,madeCards[0],madeCards[0],n,n,madeCards[2],12,temp);
					}
				}
			}
		}
	}
	else if(madeCards[0]!=madeCards[1])//abcde
	{
		for(n=0;n<size-1;++n)
			if( (majorOuts=3-howManyCards(boxedCards,boxedSize,madeCards[n],madeCards[n])) )
				for(i=n+1;i<size;++i)
					if( (minorOuts=3-howManyCards(boxedCards,boxedSize,madeCards[i],madeCards[i])) )
					{
						majorFlush =  1  -  howManyCards(flushCards,flushSize,madeCards[n],madeCards[n])  -  howManyCards(boxedFlushCards,boxedFlushSize,madeCards[n],madeCards[n]);
						minorFlush =  1  -  howManyCards(flushCards,flushSize,madeCards[i],madeCards[i])  -  howManyCards(boxedFlushCards,boxedFlushSize,madeCards[i],madeCards[i]);

						if(flushSize==3)
							temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							temp  =  majorOuts*minorOuts;

						if(temp)
						{
							//printTest("6");
							x[0]=madeCards[i];
							if(i==1)		appendTier(head,madeCards[n],x,1,madeCards[n],madeCards[n],madeCards[i],madeCards[i],madeCards[2],5,temp);
							else if(n==0)	appendTier(head,madeCards[n],x,1,madeCards[n],madeCards[n],madeCards[i],madeCards[i],madeCards[1],5,temp);
							else			appendTier(head,madeCards[n],x,1,madeCards[n],madeCards[n],madeCards[i],madeCards[i],madeCards[0],5,temp);
						}
					}
	}

	//1PAIRS on PAIRED BOARD

	if(madeCards[0]==madeCards[1])	//aabcd,aabc,aab
	{
		if(size==3)	//aab
		{
			for(n=highestLiveCard;n>lowestLiveCard;--n)
				if(!howManyCards(madeCards,size,n,n))
				{
					majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);

					for(i=n-1;i>=lowestLiveCard;--i)
						if(!howManyCards(madeCards,size,i,i))
						{
							minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
							temp  =  majorOuts*minorOuts;

							if(temp)
							{
								//printTest("7bii");
								x[0]=i;
								if(i>madeCards[2])		appendTier(head,n,x,1,madeCards[0],madeCards[0],n,i,madeCards[2],12,temp);
								else if(n>madeCards[2])	appendTier(head,n,x,1,madeCards[0],madeCards[0],n,madeCards[2],i,20,temp);
								else					appendTier(head,n,x,1,madeCards[0],madeCards[0],madeCards[2],n,i,24,temp);
							}
						}
				}

			return head;
		}

		else
		{
			for(n=highestLiveCard;n>madeCards[size-1];--n)
			{
				if(!howManyCards(madeCards,size,n,n))
				{
					majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
					majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

					for(i=n-1;i>madeCards[3];--i)
					{
						if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i))
						{
							minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
                            minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

							if(flushSize==3)
								temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else if(flushSize==4)
								temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
							else
								temp  =  majorOuts*minorOuts;

							if(temp)
							{
								//printTest("7ai");
								x[0]=i;
								if(i>madeCards[2])		appendTier(head,n,x,1,madeCards[0],madeCards[0],n,i,madeCards[2],12,temp);
								else if(n>madeCards[2])	appendTier(head,n,x,1,madeCards[0],madeCards[0],n,madeCards[2],i,20,temp);
								else					appendTier(head,n,x,1,madeCards[0],madeCards[0],madeCards[2],n,i,24,temp);
							}
						}
					}

					x_size=temp=0;
					if(n>madeCards[3])
					{
						for(i=madeCards[3]-1;i>=lowestLiveCard;--i)
						{
							if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i))
							{
								minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
							}
							else minorOuts=minorFlush=0;

							if(flushSize==3)
								k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else if(flushSize==4)
								k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
							else
								k  =  majorOuts*minorOuts;

							if(k)
							{
								x[x_size]=16*k+i-16;
								++x_size;
								temp+=k;
							}
						}
					}
					else
					{
						for(i=n-1;i>=lowestLiveCard;--i)
						{
							if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i))
							{
								minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
								minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);
							}
							else minorOuts=minorFlush=0;

							if(flushSize==3)
								k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
							else if(flushSize==4)
								k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
							else
								k  =  majorOuts*minorOuts;

							if(k)
							{
								x[x_size]=16*k+i-16;
								++x_size;
								temp+=k;
							}
						}
					}

					if(temp)
					{
						//printTest("7bi");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
						if(n>madeCards[2])		appendTier(head,n,x,x_size,madeCards[0],madeCards[0],n,madeCards[2],madeCards[3],4,temp);
						else if(n>madeCards[3])	appendTier(head,n,x,x_size,madeCards[0],madeCards[0],madeCards[2],n,madeCards[3],8,temp);
						else					appendTier(head,n,x,x_size,madeCards[0],madeCards[0],madeCards[2],madeCards[3],n,16,temp);
					}
				}
			}
		}
		temp=0;
		for(n=madeCards[size-1]-1;n>lowestLiveCard;--n)
			if((n!=madeCards[0])  &&  (majorOuts=4-howManyCards(boxedCards,boxedSize,n,n)))
			{
				majorFlush  =  1  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);
				x_size=0;
				for(i=n-1;i>=lowestLiveCard;--i)
					if((i!=madeCards[0])  &&  (minorOuts=4-howManyCards(boxedCards,boxedSize,i,i))  &&  (!isStraight(straightCards,straightSize,n,i)))
					{
						minorFlush  =  1  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							k  =  majorOuts  *  minorOuts;

						if(k)
						{
							x[x_size]=16*k+i-16;
							++x_size;
							temp+=k;
						}
					}
				if(size==4  &&  temp)
				{
					//printTest("7d");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,n,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],n,16,temp);
					temp=0;
				}
			}

		if(size==5  &&  temp)
		{
			for(x_size=minorOuts=n=0;n<14;++n)
				if( (majorOuts  =  4  -  howManyCards(madeCards,size,n,n)  -  howManyCards(boxedCards,boxedSize,n,n)) )
				{
					++minorOuts;
					if(majorOuts>1)	++x_size;
				}

				x_size+=(minorOuts*minorOuts-minorOuts)/2;

			//printTest("7e");
			appendTier(head,-2,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],madeCards[4],0,temp);
		}

		return head;
	}

	//1PAIRS and HIGHCARDS abc

	else if(size==3)	//abc
	{
		for(n=12;n>=0;--n)
		{
			while(isStraight(straightCards,straightSize,n,n))	--n;
			majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
			majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

			if(howManyCards(madeCards,size,n,n))
			{
				--majorOuts;

				for(i=highestLiveCard;i>=lowestLiveCard;--i)
				{
					minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
					minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

					if(flushSize==3)
						temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
					else
						temp  =  majorOuts  *  minorOuts;

					if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i)  &&  temp)
					{
						//printTest("abc1");
						x[0]=i;
						if(n==madeCards[0])
						{
							if(i>madeCards[1])		appendTier(head,n,x,1,n,n,i,madeCards[1],madeCards[2],5,temp);
							else if(i>madeCards[2])	appendTier(head,n,x,1,n,n,madeCards[1],i,madeCards[2],9,temp);
							else					appendTier(head,n,x,1,n,n,madeCards[1],madeCards[2],i,17,temp);
						}
						else if(n==madeCards[1])
						{
							if(i>madeCards[0])		appendTier(head,n,x,1,n,n,i,madeCards[0],madeCards[2],5,temp);
							else if(i>madeCards[2])	appendTier(head,n,x,1,n,n,madeCards[0],i,madeCards[2],9,temp);
							else					appendTier(head,n,x,1,n,n,madeCards[0],madeCards[2],i,17,temp);
						}
						else
						{
							if(i>madeCards[0])		appendTier(head,n,x,1,n,n,i,madeCards[0],madeCards[1],5,temp);
							else if(i>madeCards[1])	appendTier(head,n,x,1,n,n,madeCards[0],i,madeCards[1],9,temp);
							else					appendTier(head,n,x,1,n,n,madeCards[0],madeCards[1],i,17,temp);
						}
					}
				}
			}
			else if(n>=0)
			{
				temp=(majorOuts*majorOuts-majorOuts)/2;

				if(temp)
				{
					//printTest("abc2");
					x[0]=n;
					appendTier(head,n,x,1,n,n,madeCards[0],madeCards[1],madeCards[2],3,temp);
				}
			}
		}

		//HIGHCARDS abc

		for(n=highestLiveCard;n>lowestLiveCard;--n)
			if(!howManyCards(madeCards,size,n,n)  &&  (majorOuts=4-howManyCards(boxedCards,boxedSize,n,n)))
			{
				majorFlush  =  1  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

				for(i=n-1;i>=lowestLiveCard;--i)
					if((minorOuts=4-howManyCards(boxedCards,boxedSize,i,i))  &&  (!howManyCards(madeCards,size,i,i))  &!  isStraight(straightCards,straightSize,n,i))
					{
						minorFlush=1-howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else
							temp  =  majorOuts  *  minorOuts;

						if(temp)
						{
							//printTest("abc+de");
							x[0]=i;
							if(n>madeCards[0])
							{
								if(i>madeCards[0])		appendTier(head,n,x,1,n,i,madeCards[0],madeCards[1],madeCards[2],3,temp);
								else if(i>madeCards[1])	appendTier(head,n,x,1,n,madeCards[0],i,madeCards[1],madeCards[2],5,temp);
								else if(i>madeCards[2])	appendTier(head,n,x,1,n,madeCards[0],madeCards[1],i,madeCards[2],9,temp);
								else					appendTier(head,n,x,1,n,madeCards[0],madeCards[1],madeCards[2],i,17,temp);
							}
							else if(n>madeCards[1])
							{
								if(i>madeCards[1])		appendTier(head,n,x,1,madeCards[0],n,i,madeCards[1],madeCards[2],6,temp);
								else if(i>madeCards[2])	appendTier(head,n,x,1,madeCards[0],n,madeCards[1],i,madeCards[2],10,temp);
								else					appendTier(head,n,x,1,madeCards[0],n,madeCards[1],madeCards[2],i,18,temp);
							}
							else if(n>madeCards[2])
							{
								if(i>madeCards[2])		appendTier(head,n,x,1,madeCards[0],madeCards[1],n,i,madeCards[2],12,temp);
								else					appendTier(head,n,x,1,madeCards[0],madeCards[1],n,madeCards[2],i,20,temp);
							}
							else						appendTier(head,n,x,1,madeCards[0],madeCards[1],madeCards[2],n,i,24,temp);
						}
					}
			}

		return head;
	}

	//1PAIRS and HIGHCARDS abcde,abcd

	else	//abcde,abcd
	{
		for(n=12;n>=0;--n)
		{
			while(isStraight(straightCards,straightSize,n,n))	--n;
			majorOuts  =  4  -  howManyCards(boxedCards,boxedSize,n,n);
			majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);

			if(n==madeCards[0])
			{
				--majorOuts;
				for(i=highestLiveCard;i>madeCards[3];--i)
				{
					minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
					minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

					if(flushSize==3)
						temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
					else if(flushSize==4)
						temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
					else
						temp  =  majorOuts  *  minorOuts;

					if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i)  &&  temp)
					{
						//printTest("8ai");
						x[0]=i;
						if(i>madeCards[1])		appendTier(head,n,x,1,n,n,i,madeCards[1],madeCards[2],5,temp);
						else if(i>madeCards[2])	appendTier(head,n,x,1,n,n,madeCards[1],i,madeCards[2],9,temp);
						else					appendTier(head,n,x,1,n,n,madeCards[1],madeCards[2],i,17,temp);
					}
				}

				x_size=temp=0;
				for(i=madeCards[3]-1;i>=lowestLiveCard;--i)
				{
					if((i!=madeCards[size-1])  &!  isStraight(straightCards,straightSize,n,i))
					{
						minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
						minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							k  =  majorOuts  *  minorOuts;

						if(k)
						{
							x[x_size]=16*k+i-16;
							++x_size;
							temp+=k;
						}
					}
				}

				if(temp)
				{
					//printTest("8aii");for(i=0;i<x_size;++i)printf("%2d*%c%c ",x[i]/13,face[n+2],face[x[i]%13+2]);printf("\n");
					appendTier(head,n,x,x_size,n,n,madeCards[1],madeCards[2],madeCards[3],1,temp);
				}
			}
			else if(n==madeCards[1])
			{
				--majorOuts;
				for(i=highestLiveCard;i>madeCards[3];--i)
				{
					minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
					minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

					if(flushSize==3)
						temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
					else if(flushSize==4)
						temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
					else
						temp  =  majorOuts  *  minorOuts;

					if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i)  &&  temp)
					{
						//printTest("8bi");
						x[0]=i;
						if(i>madeCards[0])		appendTier(head,n,x,1,n,n,i,madeCards[0],madeCards[2],5,temp);
						else if(i>madeCards[2])	appendTier(head,n,x,1,n,n,madeCards[0],i,madeCards[2],9,temp);
						else					appendTier(head,n,x,1,n,n,madeCards[0],madeCards[2],i,17,temp);
					}
				}

				x_size=temp=0;
				for(i=madeCards[3]-1;i>=lowestLiveCard;--i)
					if((i!=madeCards[size-1])  &!  isStraight(straightCards,straightSize,n,i))
					{
						minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
						minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							k  =  majorOuts  *  minorOuts;

						if(k)
						{
							x[x_size]=16*k+i-16;
							++x_size;
							temp+=k;
						}
					}

				if(temp)
				{
					//printTest("8bii");
					appendTier(head,n,x,x_size,n,n,madeCards[0],madeCards[2],madeCards[3],1,temp);
				}
			}
			else if(n==madeCards[2])
			{
				--majorOuts;
				for(i=highestLiveCard;i>madeCards[3];--i)
				{
					minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
					minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

					if(flushSize==3)
						temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
					else if(flushSize==4)
						temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
					else
						temp  =  majorOuts  *  minorOuts;

					if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i)  &&  temp)
					{
						//printTest("8ci");
						x[0]=i;
						if(i>madeCards[0])		appendTier(head,n,x,1,n,n,i,madeCards[0],madeCards[1],5,temp);
						else if(i>madeCards[1])	appendTier(head,n,x,1,n,n,madeCards[0],i,madeCards[1],9,temp);
						else					appendTier(head,n,x,1,n,n,madeCards[0],madeCards[1],i,17,temp);
					}
				}

				x_size=temp=0;
				for(i=madeCards[3]-1;i>=lowestLiveCard;--i)
					if((i!=madeCards[size-1])  &!  isStraight(straightCards,straightSize,n,i))
					{
						minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
						minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							k  =  majorOuts  *  minorOuts;

						if(k)
						{
							x[x_size]=16*k+i-16;
							++x_size;
							temp+=k;
						}
					}

				if(temp)
				{
					//printTest("8cii");
					appendTier(head,n,x,x_size,n,n,madeCards[0],madeCards[1],madeCards[3],1,temp);
				}
			}
			else if(n==madeCards[3]  ||  n==madeCards[size-1])
			{
				--majorOuts;
				for(i=highestLiveCard;i>madeCards[2];--i)
				{
					minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
					minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

					if(flushSize==3)
						temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
					else if(flushSize==4)
						temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
					else
						temp  =  majorOuts  *  minorOuts;

					if(!howManyCards(madeCards,size,i,i)  &!  isStraight(straightCards,straightSize,n,i)  &&  temp)
					{
						//printTest("8di");
						x[0]=i;
						if(i>madeCards[0])		appendTier(head,n,x,1,n,n,i,madeCards[0],madeCards[1],5,temp);
						else if(i>madeCards[1])	appendTier(head,n,x,1,n,n,madeCards[0],i,madeCards[1],9,temp);
						else					appendTier(head,n,x,1,n,n,madeCards[0],madeCards[1],i,17,temp);
					}
				}

				x_size=temp=0;
				for(i=madeCards[2]-1;i>=lowestLiveCard;--i)
					if((i!=madeCards[3]  &&  i!=madeCards[size-1])  &!  isStraight(straightCards,straightSize,n,i))
					{
						minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
						minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							k  =  majorOuts  *  minorOuts;

						if(k)
						{
							x[x_size]=16*k+i-16;
							++x_size;
							temp+=k;
						}
					}

				if(temp)
				{
					//printTest("8dii");
					appendTier(head,n,x,x_size,n,n,madeCards[0],madeCards[1],madeCards[2],1,temp);
				}
			}
			else if(n>=0)
			{
				if(flushSize==4)	majorOuts-=majorFlush;
				temp  =  (majorOuts*majorOuts-majorOuts)/2;

				if(temp)
				{
					//printTest("8e");
					x[0]=n;
					appendTier(head,n,x,1,n,n,madeCards[0],madeCards[1],madeCards[2],3,temp);
				}
			}
		}

		//HIGHCARDS abcde,abcd

		for(n=highestLiveCard;n>madeCards[size-1];--n)
		{
			if((!howManyCards(madeCards,size,n,n))  &&  (majorOuts=4-howManyCards(boxedCards,boxedSize,n,n)))
			{
				majorFlush =  1  -  howManyCards(flushCards,flushSize,n,n)  -  howManyCards(boxedFlushCards,boxedFlushSize,n,n);
				for(i=n-1;i>madeCards[3];--i)
				{
					if((!howManyCards(madeCards,size,i,i))  &&  (minorOuts=4-howManyCards(boxedCards,boxedSize,i,i))  &&  (!isStraight(straightCards,straightSize,n,i)))
					{
						minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							temp  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							temp  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							temp  =  majorOuts  *  minorOuts;

						if(temp)
						{
							//printTest("9a");
							x[0]=i;
							if(n>madeCards[0])
							{
								if(i>madeCards[0])		appendTier(head,n,x,1,n,i,madeCards[0],madeCards[1],madeCards[2],3,temp);
								else if(i>madeCards[1])	appendTier(head,n,x,1,n,madeCards[0],i,madeCards[1],madeCards[2],5,temp);
								else if(i>madeCards[2])	appendTier(head,n,x,1,n,madeCards[0],madeCards[1],i,madeCards[2],9,temp);
								else					appendTier(head,n,x,1,n,madeCards[0],madeCards[1],madeCards[2],i,17,temp);
							}
							else if(n>madeCards[1])
							{
								if(i>madeCards[1])		appendTier(head,n,x,1,madeCards[0],n,i,madeCards[1],madeCards[2],6,temp);
								else if(i>madeCards[2])	appendTier(head,n,x,1,madeCards[0],n,madeCards[1],i,madeCards[2],10,temp);
								else					appendTier(head,n,x,1,madeCards[0],n,madeCards[1],madeCards[2],i,18,temp);
							}
							else if(n>madeCards[2])
							{
								if(i>madeCards[2])		appendTier(head,n,x,1,madeCards[0],madeCards[1],n,i,madeCards[2],12,temp);
								else					appendTier(head,n,x,1,madeCards[0],madeCards[1],n,madeCards[2],i,20,temp);
							}
							else						appendTier(head,n,x,1,madeCards[0],madeCards[1],madeCards[2],n,i,24,temp);
						}
					}
				}

				for(x_size=temp=0;i>=lowestLiveCard;--i)
					if((i!=madeCards[3])  &&  (i!=madeCards[size-1])  &!  isStraight(straightCards,straightSize,n,i))
					{
						minorOuts  =  4  -  howManyCards(boxedCards,boxedSize,i,i);
						minorFlush =  1  -  howManyCards(flushCards,flushSize,i,i)  -  howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							k  =  majorOuts  *  minorOuts;

						if(k)
						{
							x[x_size]=16*k+i-16;
							++x_size;
							temp+=k;
						}
					}

				if(temp)
				{
					//printTest("9b");
					if(n>madeCards[0])		appendTier(head,n,x,x_size,n,madeCards[0],madeCards[1],madeCards[2],madeCards[3],1,temp);
					else if(n>madeCards[1])	appendTier(head,n,x,x_size,madeCards[0],n,madeCards[1],madeCards[2],madeCards[3],2,temp);
					else if(n>madeCards[2])	appendTier(head,n,x,x_size,madeCards[0],madeCards[1],n,madeCards[2],madeCards[3],4,temp);
					else if(n>madeCards[3])	appendTier(head,n,x,x_size,madeCards[0],madeCards[1],madeCards[2],n,madeCards[3],8,temp);
					else					appendTier(head,n,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],n,16,temp);
				}
			}
		}

		j=0;
		for(n=madeCards[size-1]-1;n>lowestLiveCard;--n)
			if( (majorOuts=4-howManyCards(boxedCards,boxedSize,n,n)) )
			{
				x_size=temp=0;
				majorFlush=1-howManyCards(boxedFlushCards,boxedFlushSize,n,n);

				for(i=n-1;i>=lowestLiveCard;--i)
					if((minorOuts=4-howManyCards(boxedCards,boxedSize,i,i))  &&  (!isStraight(straightCards,straightSize,n,i)))
					{
						minorFlush=1-howManyCards(boxedFlushCards,boxedFlushSize,i,i);

						if(flushSize==3)
							k  =  (majorOuts*minorOuts)  -  (majorFlush*minorFlush);
						else if(flushSize==4)
							k  =  (majorOuts-majorFlush)  *  (minorOuts-minorFlush);
						else
							k  =  majorOuts  *  minorOuts;

						if(k)
						{
							x[x_size]=16*k+i-16;
							++x_size;
							temp+=k;
						}
					}

				j+=temp;
				if(size==4  &&  temp)
				{
					//printTest("9c");
					appendTier(head,n,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],n,16,temp);
				}
			}

		if(size==5  &&  j)
		{
			for(x_size=minorOuts=n=0;n<14;++n)
				if( (majorOuts  =  4  -  howManyCards(madeCards,size,n,n)  -  howManyCards(boxedCards,boxedSize,n,n)) )
				{
					++minorOuts;
					if(majorOuts>1)	++x_size;
				}

			x_size+=(minorOuts*minorOuts-minorOuts)/2;

			//printTest("9d");
			appendTier(head,-2,x,x_size,madeCards[0],madeCards[1],madeCards[2],madeCards[3],madeCards[4],0,j);
		}

		return head;
	}
}
