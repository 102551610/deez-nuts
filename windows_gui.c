#include <stdio.h>
//#include <ncurses.h>     //getch();
#include <windows.h>
#include <math.h>

#include "struct_range.h"
#include "struct_frac.h"
#include "windows_gui.h"

extern unsigned int HEIGHT;// = 54;
extern unsigned int WIDTH;//  = 80;
extern int DEFAULT_FORGC;// = 7;
extern int DEFAULT_BACKC;// = 0;

extern int face[17];//[];//[17]={120,97,50,51,52,53,54,55,56,57,84,74,81,75,65,101,69};			//	xa23456789TJQKAeE
extern int heptadecimal[17];//[17]={48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71};	    //	0123456789ABCDEFG


int isInArray(short int arr[], short int l, short int x){
    /*returns number of incidences of x in arr[] of size l*/
    short int n;
    for(n=0;l>0;--l)
        if(arr[l-1]==x)
            ++n;
    return n;
}

void SetColorAndBackground(int ForgC, int BackC){
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}

void SetColor(int ForgC){
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

void printAid(void){
	int n,i;

	printf("\n\t\t  ");
	for(n=0;n<13;++n)	printf(" %c ",face[(n+12)%13+2]);
	printf("\n");
	for(n=0;n<3;++n)
	{
		printf("\t\t");
		SetColor(-6*n*n+14*n+4);//textcolor(-6*n*n+14*n+4);
		printf("%c",n+3);
		for(i=n;i<52;i+=4)	printf(" %2d",i);
		SetColor(DEFAULT_FORGC);//textcolor(7);
		printf("\n");
	}

	printf("\t\t%c",n+3);
	for(;n<52;n+=4)	printf(" %2d",n);
	printf("\n");

	return;
}

void printLiveCards(short int cards[],short int size, short int boxedCards[],short int boxedSize,short int flushDraw){
	int n,k;

	for(k=0;k<4;++k)
	{
		printf("\n\t     ");
		if(k==flushDraw)
		{
			SetColor(-6*k*k+14*k+4);//textcolor(-6*k*k+14*k+4);
			printf("%c ",k+3);
			SetColorAndBackground(15,12);
			//textbackground(12);
            //textcolor(15);
		}
		else	printf("  ");

		for(n=k;n<52;n+=4)
			if(isInArray(cards,size,(n+4)%52)  ||  isInArray(boxedCards,boxedSize,(n+4)%52))
					printf("   ");
			else	printf(" %c ",face[n/4+2]);
        SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
		//textbackground(0);
		//textcolor(7);

		if(k==flushDraw)
		{
			SetColor(-6*k*k+14*k+4);//textcolor(-6*k*k+14*k+4);
			printf(" %c",k+3);
			SetColor(DEFAULT_FORGC);//textcolor(7);
		}
	}
	printf("\n\n\t      ");
	SetColor(5);//textcolor(5);
	k=0;
	for(n=4;n<56;++n)
    {
        if(!isInArray(boxedCards,boxedSize,n%52)  &!  isInArray(cards,size,n%52))
            ++k;
        if(n%4==3)  printf(" %2d",k);
    }
	SetColor(DEFAULT_FORGC);//textcolor(7);
	printf("\n\n");

	return;
}

void printShort(short int card){
    // prints board card in shorthand
	printf(" %c",face[card+2]);
	return;
}

void printShortX(short int card){
    // prints hole card in shorthand
	printf(" ");
	SetColorAndBackground(15,6);
	//textbackground(6);
	//textcolor(15);
	printf("%c",face[card+2]);
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
	//textbackground(0);
	//textcolor(7);

	return;
}

void printCard(short int card){
    // prints rank in colour of suit with inverted background
	if(card%4==0)
		SetColor(4);//textcolor(4);
	else if(card%4==1)
		SetColor(12);//textcolor(12);
	else if(card%4==2)
		SetColor(8);//textcolor(8);
	else
		SetColor(0);//textcolor(0);

	printf("%c",face[(((card/4)+12)%13)+2]);

	SetColor(DEFAULT_FORGC);//textcolor(7);

	return;
}

void printSuit(short int card){
    // prints suit with inverted background
	if(card%4==0)
		SetColor(4);//textcolor(4);
	else if(card%4==1)
		SetColor(12);//textcolor(12);
	else if(card%4==2)
		SetColor(8);//textcolor(8);
	else
		SetColor(0);//textcolor(0);

	printf("%c",card%4+3);

	SetColor(DEFAULT_FORGC);//textcolor(7);

	return;
}

void printBoard(short int cards[],short int size,short int key){
	int i;

	SetColorAndBackground(15,9);//textbackground(9);
	//textcolor(15);
	for(i=0;i<size;++i)
	{
		if(key%2)
		{
			SetColor(12);//textcolor(12);
			printShort(cards[i]);
			SetColor(15);//textcolor(15);
		}
		else	printShort(cards[i]);

		key/=2;
	}
	printf(" ");
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
	//textbackground(0);
	//textcolor(7);
	printf("\t");

	return;
}

void printRange(struct Range *head){
	struct Range* node=head;
	unsigned int comboCount=0;
	short unsigned int n,i,line;

	//printBoard(node->best,node->holeCard[0]-16,node->key);
	if(node->holeCardSize>16)
	{
		printf("\n\t\t\t   ");
		for(n=5;n>node->holeCardSize-16;--n)
			printf("  ");
        SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
		printf("%c",218);
		for(i=1;i<node->holeCardSize-16;++i)
			printf("%c%c",196,194);
		printf("%c%c",196,191);
		SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
		printf("\n\t\t\t   ");
		for(n=5;n>node->holeCardSize-16;--n)
			printf("  ");
		SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
		printf("%c",179);
		for(i=0;i<node->holeCardSize-16;++i)
		{
			printCard(node->best[i]);
			printf("%c",179);
		}
		SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
		printf("\n\t\t\t   ");
		for(n=5;n>node->holeCardSize-16;--n)
			printf("  ");
        SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
		printf("%c",179);
		for(i=0;i<node->holeCardSize-16;++i)
		{
			printSuit(node->best[i]);
			printf("%c",179);
		}
		SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
		printf("\n\t\t\t   ");
		for(n=5;n>node->holeCardSize-16;--n)
			printf("  ");
        SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
		printf("%c",192);
		for(i=1;i<node->holeCardSize-16;++i)
			printf("%c%c",196,193);
		printf("%c%c",196,217);
		SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
		printf("\n\n");
	}
	node=node->next;

	for(line=0;node!=NULL;++line)
	{
		//if(line==42)	getch();//system("pause");

		if(head->holeCardSize<17)
        {
            if(node->key>63)
                SetColor(12);
            else if(node->holeCardHigh%16==node->holeCardArray[0]%16)
                SetColor(14);
        }

		printf("\t  %c ",face[node->holeCardHigh+2]);

		if(node->holeCardSize>1)
			printf(" %c \t",face[0]);
		else
			printf(" %c \t",face[node->holeCardArray[0]%16+2]);

		if(head->holeCardSize<17)
		{
			SetColor(5);//textcolor(5);
			printf("    %4.1f",(float)(node->key%64-3)/2);
			SetColor(DEFAULT_FORGC);//textcolor(7);
		}
		else if(node->key>199)
		{
			SetColorAndBackground(15,12);//textbackground(12);
			//textcolor(15);
			for(n=14;n>0;--n)
				if(isInArray(node->best,5,n-2))
					printShort(n-2);
			printf(" ");
			SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
			//textbackground(0);
			//textcolor(7);
		}
		else
		{
			i=node->key;
			for(n=0;n<5;++n)
			{
				if(i%2)		printShortX(node->best[n]);
				else		printShort(node->best[n]);
				i/=2;
			}
		}
		printf("\t%-d\t%5.2f\t",node->combo,100*(float)node->combo/(float)head->combo);
		comboCount  +=  node->combo;
		printf("%d\t%5.2f\n",comboCount,100*(float)comboCount/(float)head->combo);

		node=node->next;
	}

	//if((line>29)  &&  (line<43))	getch();//system("pause");
	return;
}

void printRangeWithImprove(struct Range *range,struct Frac *val){
    struct Range* curr = range;
	//Frac* val = getImproveRange(range,prior);

	unsigned int comboCount = 0;
	short unsigned int n,i,arr;

	printf("\n\t\t\t   ");
	for(n=5;n>curr->holeCardSize-16;--n)
		printf("  ");
	SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
	printf("%c",218);
	for(i=1;i<curr->holeCardSize-16;++i)
		printf("%c%c",196,194);
	printf("%c%c",196,191);
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
	printf("\n\t\t\t   ");
	for(n=5;n>curr->holeCardSize-16;--n)
		printf("  ");
	SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
	printf("%c",179);
	for(i=0;i<curr->holeCardSize-16;++i)
	{
		printCard(curr->best[i]);
		printf("%c",179);
	}
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
	printf("\n\t\t\t   ");
	for(n=5;n>curr->holeCardSize-16;--n)
		printf("  ");
	SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
	printf("%c",179);
	for(i=0;i<curr->holeCardSize-16;++i)
	{
		printSuit(curr->best[i]);
		printf("%c",179);
	}
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
	printf("\n\t\t\t   ");
	for(n=5;n>curr->holeCardSize-16;--n)
		printf("  ");
	SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
	printf("%c",192);
	for(i=1;i<curr->holeCardSize-16;++i)
		printf("%c%c",196,193);
	printf("%c%c",196,217);
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
	printf("\n\n");
	curr=curr->next;

	for(arr=0;curr!=NULL;++arr&&(curr=curr->next))//errata.num=
    {
		//if(line==42)	getch();//system("pause");
		printf("\t  %c ",face[curr->holeCardHigh+2]);

		if(curr->holeCardSize>1)
			printf(" %c \t",face[0]);
		else
			printf(" %c \t",face[curr->holeCardArray[0]%16+2]);

		if(curr->key>199)
		{
			SetColorAndBackground(15,12);//textbackground(12);
			//textcolor(15);
			for(n=0;n<14;++n)
                for(i=0;i<5;++i)
                    if(12-n==curr->best[i])
                        printShort(12-n);
			printf(" ");
			SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
			//textcolor(7);
		}
		else
		{
			i=curr->key;
			for(n=0;n<5;++n)
			{
				if(i%2)		printShortX(curr->best[n]);
				else		printShort(curr->best[n]);
				i/=2;
			}
			printf(" ");
		}

		if(val[arr].num)
		{
			if(val[arr].num>0)	//	hand improves
			{
				SetColor(10);	//	10=green

				if(val[arr].den>val[arr].num)
                    printf("    ");
                else
                    for(n=10000;n>val[arr].num/val[arr].den;n/=10)
                        printf(" ");

				printf("+");
			}
			else				//	hand deteriorates
			{
				SetColor(12);	//	12=red

				for(n=1000;n>(-val[arr].num)/val[arr].den;n/=10)
					printf(" ");
				if((-val[arr].num)>=val[arr].den)
					printf(" ");
			}

			printf("%.2f",(float)val[arr].num/val[arr].den);
		}
		else
		{
			SetColor(9);		//	9=blue
			printf("      =  ");
		}

		SetColor(DEFAULT_FORGC);

		printf("%5d  %5.2f",curr->combo,100*(float)curr->combo/(float)range->combo);
		comboCount  +=  curr->combo;
		printf("%6d%9.2f\n",comboCount,100*(float)comboCount/(float)range->combo);

    }

    //free(val);
	return;
}

void printExpandedRange(struct Range *head){
    struct Range* node=head;
    const short int suit[4]={4,6,2,3};
	short int n,i;
	unsigned short int key;

	printf("       %c",218);
	for(n=0;n<3;++n)	printf("%c%c%c%c%c%c",196,196,196,196,196,194);
	for(n=0;n<16;++n)	printf("%c",196);
	printf("%c",194);
	for(n=0;n<16;++n)	printf("%c",196);
	for(n=0;n<2;++n)	printf("%c%c%c%c%c%c",194,196,196,196,196,196);
	printf("%c\n",191);

	printf("       %c node%c hCH %c hCS %c       hCA      %c      best      %ccombo%cmod_p%c\n",179,179,179,179,179,179,179,179);
	printf("       %c",195);
	for(i=0;i<3;++i)	printf("%c%c%c%c%c%c",196,196,196,196,196,197);
	for(i=0;i<16;++i)	printf("%c",196);
	printf("%c",197);
	for(i=0;i<16;++i)	printf("%c",196);
	for(i=0;i<2;++i)	printf("%c%c%c%c%c%c",197,196,196,196,196,196);
	printf("%c\n",180);

	printf("       %c ",179);
	SetColor(13);
	printf("%-3d",node->mod_prob);
	SetColor(DEFAULT_FORGC);
	printf(" %c",179);
	/*if(node->key==5)
    {
        SetColor(-6*(node->holeCardHigh-200)*(node->holeCardHigh-200)+14*(node->holeCardHigh-200)+4);
        printf("%c",node->holeCardHigh-197);
    }
    else printf(" ");*/

    if(node->key)
    {
        if(node->key<5) printf(" ");

        SetColor(suit[node->holeCardHigh]);
        for(n=0;n<node->key;++n)
            printf("%c",node->holeCardHigh+3);

        if(node->key==3) printf(" ");
    }
    else
    {
        if(node->holeCardSize<21)   printf(" ");

        for(n=0;n<4;++n)
        {
            SetColor(suit[n]);
            //printf("%d%%%d/%d=",node->holeCardHigh,(short int)pow(3,n-1),(short int)pow(3,4-n));
            for(i=(node->holeCardHigh%(short int)pow(3,4-n))/pow(3,3-n);i>0;--i)
                printf("%c",n+3);
        }

        if(node->holeCardSize==19)   printf(" ");
    }

    SetColor(DEFAULT_FORGC);



    /*{
        SetColor(-6*(node->holeCardHigh-200)*(node->holeCardHigh-200)+14*(node->holeCardHigh-200)+4);
        printf("%c",node->holeCardHigh-197);
        printf("%c",node->holeCardHigh-197);
        printf("%c",node->holeCardHigh-197);
    }
    else printf("   ");
    if(node->key>3)
        printf("%c",node->holeCardHigh-197);
    else
    {
        SetColor(DEFAULT_FORGC);
        printf(" ");
    }
    SetColor(DEFAULT_FORGC);*/

	if(node->holeCardSize==19)
        printf("%c FLOP%c",179,179);
    else if(node->holeCardSize==20)
        printf("%c TURN%c",179,179);
    else if(node->holeCardSize==21)
        printf("%cRIVER%c",179,179);
    else printf("    %c     %c",179,179);

	for(i=0;i<5;++i)	printf("   ");
	printf(" %c",179);

	if(node->holeCardSize==16)  printf("    PRE-FLOP    ");
	else
    {
        //SetColorAndBackground(DEFAULT_FORGC,15);

        for(i=0;i+16<node->holeCardSize;++i)
        {//printf(" %02d",node->best[i]);
            SetColor(suit[node->best[i]%4]);
            printf(" %c%c",face[(node->best[i]/4+12)%13+2],node->best[i]%4+3);
            //printf(" ");
            //printCard(node->best[i-16]);
            //printSuit(node->best[i-16]);
        }
        while(i<5)
        {
            printf("   ");
            ++i;
        }
        printf(" ");

        SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
    }

	printf("%c ",179);
    SetColor(13);
    printf("%-4d",node->combo);
	SetColor(DEFAULT_FORGC);
	printf("%c     %c\n",179,179);

	for(n=1;node->next!=NULL;++n)
	{
		node=node->next;
		printf("       %c ",179);
		SetColor(5);
		printf("%-4d",n);

		SetColor(DEFAULT_FORGC);
		printf("%c",179);

		if(node->key>199)   SetColor(suit[head->holeCardHigh]);
		else                SetColor(15);

		printf("  %c  ",face[node->holeCardHigh+2]);
		SetColor(DEFAULT_FORGC);

        if(node->holeCardHigh==-2)
            printf("%c  x  %c",179,179);
		else if(node->holeCardSize>1)
            printf("%c  %-2d %c",179,node->holeCardSize,179);
        else printf("%c     %c",179,179);

        if(node->holeCardHigh>=0)
			for(i=0;i<5;++i)
            {
                if(i<node->holeCardSize)
                {
                    if(node->key>199)   SetColor(suit[head->holeCardHigh]);
                    else                SetColor(15);

                    printf(" %c",face[node->holeCardHigh+2]);

                    if(head->key>3)     SetColor(15);
                    printf("%c",face[node->holeCardArray[i]%16+2]);
                }
				else						printf("   ");
            }
		else
			for(i=0;i<5;++i)	printf("   ");

        SetColor(DEFAULT_FORGC);
		printf(" %c   ",179);
		key=node->key;

		if(key>199) SetColor(suit[head->holeCardHigh]);
        else        SetColor(15);
		if(key>199)
        {
            for(i=0;i<13;++i)
            {
                if(12-i==node->best[1]  &&  node->holeCardSize>1  &&  head->key==3)
                    printf("%c ",head->holeCardHigh+3);
                else if(isInArray(node->best,5,12-i))
                    printf("%c ",face[14-i]);
            }
            if(node->best[4]==-1)
                printf("%c ",face[1]);
        }
        else if(head->holeCardSize>16)
            for(i=0;i<5;++i)
            {
                if(key%2)   SetColor(13);
                printf("%c ",face[node->best[i]+2]);
                SetColor(15);
                key/=2;
            }
        else
            for(i=0;i<5;++i)
                printf("  ");

        SetColor(DEFAULT_FORGC);
		printf("   %c",179);
		SetColor(5);
		printf(" %-4d",node->combo);
		SetColor(DEFAULT_FORGC);
		printf("%c     %c\n",179,179);

		for(key=5;key<node->holeCardSize  &&  node->holeCardHigh>=0;key+=5)
		//if(node->holeCardSize>4  &&  node->holeCardHigh>=0)
		{
			printf("       %c     %c     %c     %c",179,179,179,179);

			for(i=0;i<5;++i)
            {
                if(key+i<node->holeCardSize)
                {
                    if(node->key>199)   SetColor(suit[head->holeCardHigh]);
                    else                SetColor(15);

                    printf(" %c",face[node->holeCardHigh+2]);

                    if(head->key>3)     SetColor(15);
                    printf("%c",face[node->holeCardArray[key+i]%16+2]);
                }
				else						printf("   ");
            }

            SetColor(15);
			printf(" %c                %c     %c     %c\n",179,179,179,179);
		}
	}

	printf("       %c",192);
	for(i=0;i<3;++i)	printf("%c%c%c%c%c%c",196,196,196,196,196,193);
	for(i=0;i<16;++i)	printf("%c",196);
	printf("%c",193);
	for(i=0;i<16;++i)	printf("%c",196);
	for(i=0;i<2;++i)	printf("%c%c%c%c%c%c",193,196,196,196,196,196);
	printf("%c\n",217);

	return;
}

void printoldexpandedRange(struct Range *head){
	struct Range* node=head;
	unsigned int comboCount=0;
	short unsigned int n,i,line;

	//printBoard(node->best,node->holeCard[0]-16,node->key);
	printf("\n\t\t\t   ");
	for(n=5;n>node->holeCardSize-16;--n)
		printf("  ");
	SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
	printf("%c",218);
	for(i=1;i<node->holeCardSize-16;++i)
		printf("%c%c",196,194);
	printf("%c%c",196,191);
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
	printf("\n\t\t\t   ");
	for(n=5;n>node->holeCardSize-16;--n)
		printf("  ");
	SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
	printf("%c",179);
	for(i=0;i<node->holeCardSize-16;++i)
	{
		printCard(node->best[i]);
		printf("%c",179);
	}
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
	printf("\n\t\t\t   ");
	for(n=5;n>node->holeCardSize-16;--n)
		printf("  ");
	SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
	printf("%c",179);
	for(i=0;i<node->holeCardSize-16;++i)
	{
		printSuit(node->best[i]);
		printf("%c",179);
	}
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
	printf("\n\t\t\t   ");
	for(n=5;n>node->holeCardSize-16;--n)
		printf("  ");
	SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
	printf("%c",192);
	for(i=1;i<node->holeCardSize-16;++i)
		printf("%c%c",196,193);
	printf("%c%c",196,217);
	SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
	printf("\n\n");
	node=node->next;

	for(line=0;node!=NULL;++line)
	{
		if(line==42)	getchar();//system("pause");getch();//system("pause");
		printf("\t  %c ",face[node->holeCardHigh+2]);

		if(node->holeCardSize>1)
			printf(" %c \t",face[0]);
		else
			printf(" %c \t",face[node->holeCardArray[0]%16+2]);

		if(node->key>199)
		{
			SetColorAndBackground(15,12);//textbackground(12);
			//textcolor(15);
			for(n=14;n>0;--n)
				if(isInArray(node->best,5,n-2))
					printShort(n-2);
			printf(" ");
			SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);//textbackground(0);
			//textcolor(7);
		}
		else
		{
			i=node->key;
			for(n=0;n<5;++n)
			{
				if(i%2)		printShortX(node->best[n]);
				else		printShort(node->best[n]);
				i/=2;
			}
		}
		printf("\t%-d\t%5.2f\t",node->combo,100*(float)node->combo/(float)head->combo);
		comboCount  +=  node->combo;
		printf("%d\t%5.2f\n",comboCount,100*(float)comboCount/(float)head->combo);

		if((node->holeCardSize>1)  &&  (node->holeCardHigh>-2))
		{
			++line;
			if(line==42)	getchar();//system("pause");getch();//system("pause");

			printf("\t     { ");
			for(n=0;n<node->holeCardSize;++n)
				printf("%c*%c ",heptadecimal[node->holeCardArray[n]/13],face[node->holeCardArray[n]%16+2]);
			printf("}\n");
		}

		node=node->next;
	}

	if((line>29)  &&  (line<43))	getchar();//system("pause");
	return;
}

void printRaw(struct Range* head){
    struct Range* node=head;
	short int n,i;

	printf("    %c",218);
	for(n=0;n<3;++n)	printf("%c%c%c%c%c%c",196,196,196,196,196,194);
	for(n=0;n<16;++n)	printf("%c",196);
	printf("%c",194);
	for(n=0;n<16;++n)	printf("%c",196);
	for(n=0;n<3;++n)	printf("%c%c%c%c%c%c",194,196,196,196,196,196);
	printf("%c\n",191);

	printf("    %c node%c hCH %c hCS %c       hCA      %c      best      %c key %ccombo%cmod_p%c\n",179,179,179,179,179,179,179,179,179);
	printf("    %c",195);
	for(i=0;i<3;++i)	printf("%c%c%c%c%c%c",196,196,196,196,196,197);
	for(i=0;i<16;++i)	printf("%c",196);
	printf("%c",197);
	for(i=0;i<16;++i)	printf("%c",196);
	for(i=0;i<3;++i)	printf("%c%c%c%c%c%c",197,196,196,196,196,196);
	printf("%c\n",180);

	printf("    %c REF %c %-3d %c %2d  %c ",179,179,node->holeCardHigh,179,node->holeCardSize,179);
	for(i=0;i<5;++i)	printf("   ");
	printf("%c",179);
	for(i=0;i<5;++i)	printf(" %02d",node->best[i]);
	printf(" %c %3d %c %-4d%c %-4d%c\n",179,node->key,179,node->combo,179,node->mod_prob,179);

	for(n=1;node->next!=NULL;++n)
	{
		node=node->next;
		printf("    %c %-4d%c %-4d%c %-4d%c",179,n,179,node->holeCardHigh,179,node->holeCardSize,179);
		if(node->holeCardHigh>=0)
			for(i=0;i<4;++i)
				if(i<node->holeCardSize)	printf(" %03d",node->holeCardArray[i]);
				else						printf("    ");
		else
			for(i=0;i<4;++i)	printf("    ");
		printf("%c",179);
		for(i=0;i<5;++i)	printf(" %02d",node->best[i]);
		printf(" %c %3d %c %-4d%c %-4d%c\n",179,node->key,179,node->combo,179,node->mod_prob,179);

		if(i<node->holeCardSize  &&  node->holeCardHigh>=0)
		{
			printf("    %c     %c     %c     %c",179,179,179,179);
			for(i=4;i<node->holeCardSize;++i)
				printf(" %03d",node->holeCardArray[i]);
			while(i<8)
			{
				printf("    ");
				++i;
			}
			if(node->holeCardSize<9)
					printf("%c",179);
			else	printf(" ");
			while(i<node->holeCardSize)
			{
				printf("%03d ",node->holeCardArray[i]);
				++i;
			}
			while(i<12)
			{
				printf("    ");
				++i;
			}

			if(node->holeCardSize<13)
				printf("%c     ",179);
			else
				printf("  ");

			for(i=0;i<2;++i)	printf("%c     ",179);
			printf("%c\n",179);
		}
	}

	printf("    %c",192);
	for(i=0;i<3;++i)	printf("%c%c%c%c%c%c",196,196,196,196,196,193);
	for(i=0;i<16;++i)	printf("%c",196);
	printf("%c",193);
	for(i=0;i<16;++i)	printf("%c",196);
	for(i=0;i<3;++i)	printf("%c%c%c%c%c%c",193,196,196,196,196,196);
	printf("%c\n",217);

	return;
}



void printHistogram(struct Range *head,struct Frac* val)
{
    short unsigned int n        = 0;
    unsigned int width_scale    = head->combo;
    unsigned int width_bound    = WIDTH-12-2*(head->holeCardSize==21);
    unsigned int line,column,comboCount,k;
    struct Range* node;

	for(node=head->next;node!=NULL;node=node->next)
        n+=(node->combo/width_scale);
	while(head->mod_prob+n<WIDTH-2  &&  width_scale>1)
    {
        n=0;
        width_scale/=2;
        for(node=head->next;node!=NULL;node=node->next)
            n+=(node->combo/width_scale);
        //printf("1.oC=%d\tj=%d\n",n,width_scale);
    }
    if(n)   while(head->mod_prob+n>WIDTH-2)
    {
        n=0;
        ++width_scale;
        for(node=head->next;node!=NULL;node=node->next)
            n+=(node->combo/width_scale);
        //printf("2.oC=%d\tj=%d\n",n,width_scale);
    }

	//printf("ws=%d\ti=%d\tWIDTH=%d\tarr=%d\t(WIDTH-2)/arr+(arr>WIDTH+2)=%d\n",width_scale,i,WIDTH,arr,(WIDTH-2)/arr+(arr>WIDTH+2));

	printf("\n");

	for(line=0;line<4;++line)
    {
        node=head->next;
        printf(" ");
        SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);

        for(n=comboCount=column=0;n<head->mod_prob  &&  n<width_bound;++n)//WIDTH-2;++n)
        {
            if(comboCount*HEIGHT<=line*head->combo  &&  (node->combo+comboCount)*HEIGHT+head->combo>line*head->combo)
            {
                if(val[n].num>0)	 //	hand improves
                    SetColor(10);    //	10=green
                else if(val[n].num<0)//	hand deteriorates
                    SetColor(12);    //	12=red
                else                 // no change in relative strength
                    SetColor(9);	 //	9=blue

                for(k=0;k<((node->combo/width_scale)+1);++k)  if(k+column<width_bound)    printf("%c",178);
                SetColor(DEFAULT_FORGC);
            }
            else if((comboCount+val[n].num/val[n].den)*HEIGHT<=line*head->combo  &&  (node->combo+comboCount+val[n].num/val[n].den)*HEIGHT+head->combo>line*head->combo)
            {
                if(val[n].num>0)	 //	hand improves
                    SetColor(10);    //	2=green
                else if(val[n].num<0)//	hand deteriorates
                    SetColor(12);    //	4=red

                for(k=0;k<((node->combo/width_scale)+1);++k)  if(k+column<width_bound)    printf("%c",176);//else printf("!");
                SetColor(DEFAULT_FORGC);
            }
            else for(k=0;k<((node->combo/width_scale)+1);++k)  if(k+column<width_bound)   printf(" ");//else printf("!");
        comboCount+=node->combo;
        column+=k;
        node=node->next;
        }

        SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
        while(column<width_bound)
        {
            printf(" ");
            ++column;
        }
        SetColorAndBackground(DEFAULT_FORGC,15);

        if(head->holeCardSize==19)
        {
            if(!line)
                printf("%c%c%c%c%c%c%c%c%c%c",218,196,196,194,196,196,194,196,196,191);
            else if(line==1)
                printf("%c%c%c%c%c%c%c%c%c%c",179,32,32,179,32,32,179,32,32,179);
            else if(line==2)
            {
                for(n=0;n<3;++n)
                {
                    printf("%c ",179);
                    printSuit(head->best[n]);
                }
                printf("%c",179);
            }
            else
            {
                for(n=0;n<3;++n)
                {
                    printf("%c ",179);
                    printCard(head->best[n]);
                }
                printf("%c",179);
            }
        }
        else //if(range->holeCardSize==20)
        {
            if(!line)
            {
                printf("%c",218);
                for(n=0;n<head->holeCardSize-17;++n)
                    printf("%c%c",196,194);
                printf("%c%c%c",196,196,191);
            }
            else if(line==1)
            {
                for(n=0;n<head->holeCardSize-17;++n)
                {
                    printf("%c",179);
                    printCard(head->best[n]);
                }
                printf("%c  %c",179,179);
            }
            else if(line==2)
            {
                for(n=0;n<head->holeCardSize-17;++n)
                {
                    printf("%c",179);
                    printSuit(head->best[n]);
                }
                printf("%c ",179);
                printSuit(head->best[n]);
                printf("%c",179);
            }
            else
            {
                printf("%c",192);
                for(n=0;n<head->holeCardSize-18;++n)
                    printf("%c%c",196,193);
                printf("%c%c ",196,180);
                printCard(head->best[n+1]);
                printf("%c",179);
            }
        }
        SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
        printf("\n");
    }

    width_bound+=6*(head->holeCardSize>19)+2*(head->holeCardSize>20);

    node=head->next;
    printf(" ");
    SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);
    for(n=comboCount=column=0;n<width_bound  &&  n<head->mod_prob;++n)//WIDTH-2;++n)
    {
        if(comboCount*HEIGHT<=line*head->combo  &&  (node->combo+comboCount)*HEIGHT+head->combo>line*head->combo)
        {
            if(val[n].num>0)	 //	hand improves
                SetColor(10);    //	10=green
            else if(val[n].num<0)//	hand deteriorates
                SetColor(12);    //	12=red
            else                 // no change in relative strength
                SetColor(9);	 //	9=blue

            for(k=0;k<((node->combo/width_scale)+1);++k)  if(k+column<width_bound)    printf("%c",178);
            SetColor(DEFAULT_FORGC);
        }
        else if((comboCount+val[n].num/val[n].den)*HEIGHT<=line*head->combo  &&  (node->combo+comboCount+val[n].num/val[n].den)*HEIGHT+head->combo>line*head->combo)
        {
            if(val[n].num>0)	 //	hand improves
                SetColor(10);    //	2=green
            else if(val[n].num<0)//	hand deteriorates
                SetColor(12);    //	4=red

            for(k=0;k<((node->combo/width_scale)+1);++k)  if(k+column<width_bound)    printf("%c",176);
            SetColor(DEFAULT_FORGC);
        }
        else for(k=0;k<((node->combo/width_scale)+1);++k)  if(k+column<width_bound)   printf(" ");

        comboCount+=node->combo;
        column+=k;
        node=node->next;
    }

    SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
    while(column<width_bound)
    {
        printf(" ");
        ++column;
    }
    SetColorAndBackground(DEFAULT_FORGC,15);

    if(head->holeCardSize==19)
        printf("%c%c%c%c%c%c%c%c%c%c",192,196,196,193,196,196,193,196,196,217);
    else
        printf("%c%c%c%c",192,196,196,217);
    SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
    printf("\n");
    width_bound+=((head->holeCardSize==19)*6)+4;

    for(++line;line<HEIGHT+1;++line)  //line=5
    {
        node=head->next;
        printf(" ");
        SetColorAndBackground(DEFAULT_FORGC,15);//textbackground(15);

        for(n=comboCount=column=0;n<head->mod_prob  &&  n<WIDTH-2;++n)//WIDTH-2;++n)
        {
            //printf(" %3d:%4d\t",line*100/HEIGHT,temp_val*100/head->combo);val[n].num/val[n].den
            if(comboCount*HEIGHT<=line*head->combo  &&  (node->combo+comboCount)*HEIGHT+head->combo>line*head->combo)
            {
                if(val[n].num>0)	 //	hand improves
                    SetColor(10);    //	10=green
                else if(val[n].num<0)//	hand deteriorates
                    SetColor(12);    //	12=red
                else                 // no change in relative strength
                    SetColor(9);	 //	9=blue

                for(k=0;k<((node->combo/width_scale)+1);++k)  if(k+column<width_bound)    printf("%c",178);
                SetColor(DEFAULT_FORGC);
            }
            else if((comboCount+val[n].num/val[n].den)*HEIGHT<=line*head->combo  &&  (node->combo+comboCount+val[n].num/val[n].den)*HEIGHT+head->combo>line*head->combo)
            {
                if(val[n].num>0)	 //	hand improves
                    SetColor(10);    //	2=green
                else if(val[n].num<0)//	hand deteriorates
                    SetColor(12);    //	4=red

                for(k=0;k<((node->combo/width_scale)+1);++k)  if(k+column<width_bound)    printf("%c",176);
                SetColor(DEFAULT_FORGC);
            }
            else for(k=0;k<((node->combo/width_scale)+1);++k)  if(k+column<width_bound)   printf(" ");//else printf("%d",i);
        comboCount+=node->combo;
        column+=k;
        node=node->next;
        }

        SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
        printf("\n");
    }

	return;
}
