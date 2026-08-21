#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
//#include <graphics.h> //-lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
//#include <conio.h>
//#include <curses.h>
#include <windows.h>
#include <time.h>

#include "struct_frac.h"
#include "struct_range.h"
#include "improve.h"
#include "windows_gui.h"

int SUIT_0 = 0;
int SUIT_1 = 1;
int SUIT_2 = 2;
int SUIT_3 = 3;
unsigned int HEIGHT = 54;
unsigned int WIDTH  = 80;
int DEFAULT_FORGC = 7;
int DEFAULT_BACKC = 0;

int face[17]={120,97,50,51,52,53,54,55,56,57,84,74,81,75,65,101,69};			//	xa23456789TJQKAeE
int heptadecimal[17]={48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71};	//	0123456789ABCDEFG


int GetRand(int min, int max)	//random value [min,max]
{
  static int Init = 0;
  int rc;

  if (Init == 0)
  {
    /*
     *  As Init is static, it will remember it's value between
     *  function calls.  We only want srand() run once, so this
     *  is a simple way to ensure that happens.
     */
    srand(time(NULL));
    Init = 1;
  }

  /*
   * Formula:
   *    rand() % N   <- To get a number between 0 - N-1
   *    Then add the result to min, giving you
   *    a random number between min - max.
   */
  rc = (rand() % (max - min + 1) + min);

  return (rc);
}



int clean_stdin()
{
    while (getchar()!='\n');
    return 1;
}



void randomBoard(short int cards[],short int size)
{
	short int i,n,temp;

	for(n=0;n<size;++n)
	{
		temp=GetRand(0,51);

		for(i=0;i<n;++i)
			if(temp==cards[i])
			{
				temp=GetRand(0,51);
				i=-1;
			}

		cards[n]=temp;
	}
}



void setBoard(short int cards[],short int size)
{
	short int n,i;
	char command;

	printf("\nPlease enter value for community cards [0-51]");

	for(i=0;i<size;++i)
	{
		printf("\n\t%d/%d: ",i+1,size);

		while (((scanf("%hd%c", &cards[i], &command)!=2 || command!='\n') && clean_stdin()) || cards[i]<0 || cards[i]>51);

		for(n=i;n>0;--n)
			if(cards[i]==cards[n-1])
			{
				n=0;
				--i;
			}
	}
	return;
}



void initBoxedCards(short int madeCards[],short int size,short int boxedCards[],short int boxedSize)
{
	int i,n,j,temp;

	for(n=0;n<boxedSize;++n)
	{
		temp=GetRand(0, 51);

		for(i=0;i<n;++i)
			if(temp==boxedCards[i])
			{
				temp=GetRand(0, 51);
				i=-1;
			}
		for(j=0;j<size;++j)
			if(temp==madeCards[j])
			{
				i=-1;
				j=size;
			}

		if(i+1)	boxedCards[n]=temp;
		else	--n;
	}
}



void printTest(const char* str)
{
	printf("%s",str);
	return;
}



void saveHandToFile(short int cards[],short int size,short int boxedCards[],short int boxedSize)
{
    short int n;
    FILE *fp=fopen("history.txt","w+");

    fprintf(fp,"%3d",size);
    for(n=0;n<size;++n)
        fprintf(fp,"%3d",cards[n]);
    fprintf(fp,"%3d",boxedSize);
    for(n=0;n<boxedSize;++n)
        fprintf(fp,"%3d",boxedCards[n]);
    fclose(fp);

    return;
}



void loadHandFromFile(short int cards[],short int *size,short int **boxedCards,short int *boxedSize)
{
    FILE *fp;
    short int n;

    if((fp=fopen("history.txt","r")) == NULL)
        {
            printf("Cannot open file!\n");
            exit(901);
        }

    fscanf(fp,"%hd",size);
    for(n=0;n<*size;++n)    fscanf(fp,"%hd",&cards[n]);
    fscanf(fp,"%hd",boxedSize);

    free(*boxedCards);

    *boxedCards=(short int*)malloc((*boxedSize+5)*sizeof(short int));
    if(*boxedCards==NULL)
    {
        printf("Out of memory!\n");
        exit(904);
    }

    for(n=0;n<*boxedSize;++n)
    {
        fscanf(fp,"%hd",&(*boxedCards)[n]);
        //printf("%d:%d\t",n,(*boxedCards)[n]);
    }
    fclose(fp);

    return;
}






int main()
{
	int i,n,temp;
//    int gd = DETECT, gm;
	char command;
//	FILE *fp;
    short int cards[5];
	short int size			= -1;
	short int size_min		= 3;
	short int size_max		= 5;
//	short int preset		= 0;
	short int boxedSize;
	short int bSize_min		= 0;
	short int bSize_max		= 47;
	short int* boxedCards	= NULL;
	unsigned long int test	= 25;
	unsigned long int test_n;
	struct Range *head				= NULL;//newRangeHeadNode(1);//(Range*)malloc(sizeof(Range));	//initial node contains total combos in head->combo
	struct Range *prior			= NULL;
	struct Frac  *improve          = NULL;

/*if(head==NULL)
{
	printf("Out of memory!\n");
   	exit(900);
}*/

/*initscr();
cbreak();
noecho();
nonl();
intrflush(stdscr, FALSE);
keypad(stdscr, TRUE);*/

/* Resize the terminal to something larger than the physical screen */
//resize_term(2000, 2000);

/*head->holeCardArray	= NULL;
head->holeCardSize	= 16;
head->next			= NULL;
*/
while(1)
{
	printf(	"\n\n\t\t\t   MAIN MENU\n\n"
				"\t\t\tr  -  Randomise cards\n"
				"\t\t\ta  -  Assign cards\n"
				"\t\t\tt  -  Test\n\n");
    if(head!=NULL)
        printf(	"\t\t\tb  -  redo Boxed cards\n"
                "\t\t\tm  -  Modify boxed cards\n"
				"\t\t\tp  -  Print list\n"
				"\t\t\tw  -  print raW\n"
				"\t\t\tx  -  eXpand raw\n"
				"\t\t\ts  -  Save list\n"
				"\t\t\td  -  Delete list\n");
    else
        printf( "\t\t\tl  -  Load cards from file\n");
	printf(		"\t\t\tq  -  Quit\n");

    fflush(stdin);
	command=getchar();

	if(command=='q')
	{
		free(boxedCards);
		freeRange(&head);
//		endwin();

		return 10;
	}

	else if(command=='p'  &&  head!=NULL)
	{
		printf("\n");
		printLiveCards(cards,size,boxedCards,boxedSize,head->holeCardHigh);
		printRange(head);
	}

	else if(command=='w'  &&  head!=NULL)
	{
		printf("\n");
		printRaw(head);
	}

	else if(command=='x'  &&  head!=NULL)
	{
		printf("\n");
		printLiveCards(cards,size,boxedCards,boxedSize,head->holeCardHigh);
		printExpandedRange(head);
	}

	else if(command=='s'  &&  head!=NULL)
    {
        saveHandToFile(cards,size,boxedCards,boxedSize);
        printf("\nData saved to file!\n");
    }

	else if(command=='d'  &&  head!=NULL)
	{
		free(boxedCards);
		freeRange(&head);
	}

	else if(command=='l'  &&  head==NULL)
	{
	    loadHandFromFile(cards,&size,&boxedCards,&boxedSize);

        printf("c[%d]:\t",size);
		for(n=0;n<size;++n)			printf("%d ",cards[n]);
		printf("\nb[%d]:\t",boxedSize);
		for(n=0;n<boxedSize;++n)	printf("%d ",boxedCards[n]);
		printf("\n");

        head=printMade(cards,size,boxedCards,boxedSize);
		printLiveCards(cards,size,boxedCards,boxedSize,head->holeCardHigh);
		printRange(head);
	}

	else if(command=='b'  &&  head!=NULL)
	{
		free(boxedCards);
		freeRange(&head);

		boxedCards=(short int*)malloc((boxedSize+5)*sizeof(short int));

		if(boxedCards==NULL)
		{
			printf("Out of memory!\n");
	    	exit(902);
	    }

		initBoxedCards(cards,size,boxedCards,boxedSize);

		printf("\n");

		head=printMade(cards,size,boxedCards,boxedSize);
		printLiveCards(cards,size,boxedCards,boxedSize,head->holeCardHigh);
		printRange(head);
	}

	else if(command=='m'  &&  head!=NULL)
	{
	    while(command!='q')
        {
            printLiveCards(cards,size,boxedCards,boxedSize,head->holeCardHigh);

            for(n=0;5*n<boxedSize;++n)
            {
                printf("\n\t      ");
                SetColorAndBackground(7,15);

                for(i=0;5*n+i<boxedSize  &&  i<5;++i)
                {
                    printf("%2d:",5*n+i);
                    printCard(boxedCards[5*n+i]);
                    printSuit(boxedCards[5*n+i]);
                    printf("\t");
                }
                SetColorAndBackground(DEFAULT_FORGC,DEFAULT_BACKC);
            }

	        printf("\n\nSelect an element to modify [0,%d] (enter %d to return to main menu) ",boxedSize-1,boxedSize);
            while (((scanf("%d%c", &i, &command)!=2 || command!='\n') && clean_stdin()) || i<0 || i>boxedSize);

            if(i==boxedSize)
                command='q';
            else
            {
                printAid();
                printf("\nPlease enter value for boxed card %d [0-51] ",i);
                while (((scanf("%d%c", &temp, &command)!=2 || command!='\n') && clean_stdin()) || temp<0 || temp>51);

                for(n=test_n=0;n<boxedSize;++n)
                    if(boxedCards[n]==temp  &&  n!=i)
						++test_n;
				for(n=0;n<size;++n)
                    if(boxedCards[n]==temp)
						++test_n;

                if(!test_n)
                {
                    printf("\nElement modified successfully!\n");
                    boxedCards[i]=temp;
                }
                else
                    printf("\nError!\n");
            }
        }
        command='b';
	}

    else if(command=='r')
	{
		free(boxedCards);
		freeRange(&head);

		size=GetRand(size_min,size_max);
		randomBoard(cards,5);

		boxedSize=GetRand(bSize_min,bSize_max);
		boxedCards=(short int*)malloc((boxedSize+5)*sizeof(short int));

		if(boxedCards==NULL)
		{
			printf("Out of memory!\n");
	    	exit(903);
	    }
		initBoxedCards(cards,5,boxedCards,boxedSize);

		printf("c[%d]:\t",size);
		for(n=0;n<size;++n)			printf("%d ",cards[n]);
		printf("\nb[%d]:\t",boxedSize);
		for(n=0;n<boxedSize;++n)	printf("%d ",boxedCards[n]);
		printf("\n\n");

		head=printMade(cards,size,boxedCards,boxedSize);
		printLiveCards(cards,size,boxedCards,boxedSize,head->holeCardHigh);
		printRange(head);
	}

	else if(command=='a')
	{
		free(boxedCards);
		freeRange(&head);

		printf("\n\nHow many community cards? [3-5] ");
		while (((scanf("%hd%c", &size, &command)!=2 || command!='\n') && clean_stdin()) || size<3 || size>5);

		printAid();
		setBoard(cards,size);

		printf("\n\nHow many boxed cards? [%d,%d] ",bSize_min,bSize_max);
		while (((scanf("%hd%c", &boxedSize, &command)!=2 || command!='\n') && clean_stdin()) || boxedSize<bSize_min || boxedSize>bSize_max);
		boxedCards=(short int*)malloc((boxedSize+5)*sizeof(short int));
		if(boxedCards==NULL)
		{
			printf("Out of memory!\n");
	    	exit(904);
	    }

		if(boxedSize)
		{
			printAid();
			printf("\nPlease enter value for boxed cards [0-51]");
			for(i=0;i<boxedSize;++i)
			{
				printf("\n\t%d/%d: ",i+1,boxedSize);

				while (((scanf("%hd%c", &boxedCards[i], &command)!=2 || command!='\n') && clean_stdin()) || boxedCards[i]<0 || boxedCards[i]>51);

				for(n=i;n>0;--n)
					if(boxedCards[i]==boxedCards[n-1])
					{
						n=0;
						--i;
					}
				if(n+1)
					for(n=size;n>0;--n)
						if(boxedCards[i]==cards[n-1])
						{
							n=0;
							--i;
						}
			}
		}

		printf("c[%d]:\t",size);
		for(n=0;n<size;++n)			printf("%d ",cards[n]);
		printf("\nb[%d]:\t",boxedSize);
		for(n=0;n<boxedSize;++n)	printf("%d ",boxedCards[n]);
		printf("\n");

		head=printMade(cards,size,boxedCards,boxedSize);
		printLiveCards(cards,size,boxedCards,boxedSize,head->holeCardHigh);
		printRange(head);
	}

	else if(command=='t')
	{
		printf(	"\n\nChoose a test:\n"
				"\t1  -  series of random hierarchies\n"
				"\t2  -  random flop with improve stats as hand progresses\n"
				"\t3  -  populate preflop database\n"
				"\t4  -  random preflop with improve stats as hand progresses\n"
				"\t5  -  replay histogram\n"
				"\t6  -  histogram test preflop to flop to turn to river\n"
				"\t7  -  histogram test flop to river\n"
				"\tq  -  return to menu\n");

		while(command!='q')
		{
			command=getchar();
			fflush(stdin);

			if(command=='1')
			{
				printf("\n\nHow many tests? [0-99999] ");
				while (((scanf("%lu%c", &test, &command)!=2 || command!='\n') && clean_stdin()) || test<0 || test>99999);

				printf( "\n"
						"\n"
						"Current limitations:\n"
						"\t- none, i.e. should run to completion for size=[3,5]\n"
						"\t- executes %lu times to prevent malloc leaks"
						"\n"
						"Current test:\n"
						"\t- size      = [%d,%d]\n"
						"\t- boxedSize = [%d,%d]\n",test,size_min,size_max,bSize_min,bSize_max);

				for(test_n=temp=1;test_n<=test;(++test_n)  &&  (head=printMade(cards,size,boxedCards,boxedSize)))   //(head->next==NULL  ||  head->combo==head->mod_prob)  &&  (
				{
					free(boxedCards);
					freeRange(&head);

					size=GetRand(size_min,size_max);
					randomBoard(cards,size);

					boxedSize=GetRand(bSize_min,bSize_max);
					boxedCards=(short int*)malloc((boxedSize+5)*sizeof(short int));
					if(boxedCards==NULL)
					{
						printf("Out of memory!\n");
	    				exit(905);
	    			}
					initBoxedCards(cards,size,boxedCards,boxedSize);
					saveHandToFile(cards,size,boxedCards,boxedSize);
				}

				if(test_n>test)
					printf("\n\n\tTest completed successfully!\n");
				else
				{
					printf("\n\n\tTest failed!\n\ntest no.%lu/%lu\nc[%d]:\t",test_n,test,size);
					for(n=0;n<size;++n)			printf("%d ",cards[n]);
					printf("\nb[%d]:\t",boxedSize);
					for(n=0;n<boxedSize;++n)	printf("%d ",boxedCards[n]);
					printf("\n\n");
					printLiveCards(cards,size,boxedCards,boxedSize,head->holeCardHigh);
					printRange(head);
				}

				command='q';
			}
			else if(command=='2')
			{
				printf( "\n"
						"\n"
						"Current limitations:\n"
						"\t- gets freaky when flush completes, especially straight flushes\n"
						"\n"
						"Current test:\n"
						"\t- boxedSize = %d\n",boxedSize=GetRand(0,8));

//				getch();//system("pause");

				free(boxedCards);
				freeRange(&head);

				randomBoard(cards,5);

				boxedCards=(short int*)malloc((boxedSize+5)*sizeof(short int));
				if(boxedCards==NULL)
				{
					printf("Out of memory!\n");
	    			exit(907);
	    		}
				initBoxedCards(cards,5,boxedCards,boxedSize);
				//boxedCards[0]=11;
				//boxedCards[1]=14;

				printf("\n");
				head=printMade(cards,3,boxedCards,boxedSize);
				printLiveCards(cards,3,boxedCards,boxedSize,head->holeCardHigh);
				printRange(head);

				for(size=3;size<5;++size)
				{
					freeRange(&head);
					boxedCards[boxedSize]=cards[size];
					prior=printMade(cards,size,boxedCards,boxedSize+1);
					//printf("\n");
					//printLiveCards(cards,size,boxedCards,boxedSize+1,prior->holeCardHigh-200);
					//printRange(prior);

					printf("\n");
					head=printMade(cards,size+1,boxedCards,boxedSize);
					printLiveCards(cards,size+1,boxedCards,boxedSize,head->holeCardHigh);
					improve=getImproveRange(head,prior);
					printRangeWithImprove(head,improve);
					freeRange(&prior);
					free(improve);
				}

				command='q';
			}
			else if(command=='3')
			{
				if(head==NULL)
                {
                    free(boxedCards);

                    randomBoard(cards,5);

                    boxedSize=GetRand(0,47);
                    boxedCards=(short int*)malloc((boxedSize+5)*sizeof(short int));
                    if(boxedCards==NULL)
                    {
                        printf("Out of memory!\n");
                        exit(908);
                    }
                    initBoxedCards(cards,5,boxedCards,boxedSize);
                }

				printf("\n");
				head=printMade(cards,0,boxedCards,boxedSize);
				printLiveCards(cards,0,boxedCards,boxedSize,head->holeCardHigh);
				printRange(head);

				command='q';
			}
			else if(command=='4')
			{
				printf( "\n"
						"\n"
						"Current limitations:\n"
						"\t- gets freaky when flush completes, especially straight flushes\n"
						"\n"
						"Current test:\n"
						"\t- boxedSize = %d\n",boxedSize=GetRand(0,8));

//				getch();//system("pause");

				free(boxedCards);

				randomBoard(cards,5);
				boxedCards=(short int*)malloc((boxedSize+5)*sizeof(short int));
				if(boxedCards==NULL)
				{
					printf("Out of memory!\n");
	    			exit(910);
	    		}
				initBoxedCards(cards,5,boxedCards,boxedSize);

				freeRange(&head);
				for(n=0;n<3;++n)	boxedCards[boxedSize+n]=cards[n];
				prior=printMade(cards,0,boxedCards,boxedSize+3);
				head=printMade(cards,3,boxedCards,boxedSize);

				printf("\n");
				printLiveCards(cards,3,boxedCards,boxedSize,head->holeCardHigh);
				improve=getImproveRange(head,prior);
                printRangeWithImprove(head,improve);
				freeRange(&prior);
				free(improve);

				for(size=3;size<5;++size)
				{
					freeRange(&head);
					boxedCards[boxedSize]=cards[size];
					prior=printMade(cards,size,boxedCards,boxedSize+1);
					head=printMade(cards,size+1,boxedCards,boxedSize);

					printf("\n");
					printLiveCards(cards,size+1,boxedCards,boxedSize,head->holeCardHigh);
					improve=getImproveRange(head,prior);
                    printRangeWithImprove(head,improve);
                    freeRange(&prior);
                    free(improve);
				}

				command='q';
			}
			else if(command=='5'  &&  head!=NULL)
			{
				printf( "\n"
						"\n"
						"Current limitations:\n"
						"\t- prints in ASCII form, mostly accurate results\n"
						"\n"
						"Current test:\n"
						"\t- boxedSize = %d\n",boxedSize);//=2*GetRand(1,8));

//				getch();//system("pause");

				freeRange(&head);
				for(n=0;n<3;++n)	boxedCards[boxedSize+n]=cards[n];
				prior=printMade(cards,0,boxedCards,boxedSize+3);
				head=printMade(cards,3,boxedCards,boxedSize);

				printf("\n");
				printRange(prior);
                printLiveCards(cards,3,boxedCards,boxedSize,head->holeCardHigh);
				improve=getImproveRange(head,prior);

				printRangeWithImprove(head,improve);
				//getch();system("pause");
                freeRange(&prior);
                free(improve);

				for(size=3;size<5;++size)
				{
					freeRange(&head);
					boxedCards[boxedSize]=cards[size];
					prior=printMade(cards,size,boxedCards,boxedSize+1);
					head=printMade(cards,size+1,boxedCards,boxedSize);

					printf("\n");
					printLiveCards(cards,size+1,boxedCards,boxedSize,head->holeCardHigh);
					improve=getImproveRange(head,prior);

                    printRangeWithImprove(head,improve);
                    printHistogram(head,improve);
					//getch();system("pause");
					freeRange(&prior);
					free(improve);
				}

				command='q';
			}
			else if(command=='6')
            {
                printf("\n\nHow many tests? [0-99999] ");
				while (((scanf("%lu%c", &test, &command)!=2 || command!='\n') && clean_stdin()) || test<0 || test>99999);

                for(test_n=0;test_n<test;++test_n)
                {
                    boxedSize=2*GetRand(12,23);
                    free(boxedCards);
                    freeRange(&head);

                    randomBoard(cards,5);
                    boxedCards=(short int*)malloc((boxedSize+5)*sizeof(short int));
                    if(boxedCards==NULL)
                    {
                        printf("Out of memory!\n");
                        exit(914);
                    }
                    initBoxedCards(cards,5,boxedCards,boxedSize);
                    saveHandToFile(cards,size,boxedCards,boxedSize);

                    for(n=0;n<3;++n)	boxedCards[boxedSize+n]=cards[n];
                    prior=printMade(cards,0,boxedCards,boxedSize+3);
                    head=printMade(cards,3,boxedCards,boxedSize);

                    /*printf("\n");
                    printLiveCards(cards,0,boxedCards,boxedSize+3,head->holeCardHigh-200);
                    printRange(prior);*/

                    //printf("\n");
                    //printLiveCards(cards,3,boxedCards,boxedSize,head->holeCardHigh-200);
                    improve=getImproveRange(head,prior);
                    //printRangeWithImprove(head,improve);
                    //getch();system("pause");
                    freeRange(&prior);
                    free(improve);

                    for(size=3;size<5;++size)
                    {
                        freeRange(&head);
                        boxedCards[boxedSize]=cards[size];
                        prior=printMade(cards,size,boxedCards,boxedSize+1);
                        head=printMade(cards,size+1,boxedCards,boxedSize);

                        //printLiveCards(cards,size+1,boxedCards,boxedSize,head->holeCardHigh-200);
                        improve=getImproveRange(head,prior);
                        //printRangeWithImprove(head,improve);
                        //getch();system("pause");
                        freeRange(&prior);
                        free(improve);
                    }
                }
                printf("%ld tests completed successfully!\n",test);

                command='q';
            }
            else if(command=='7'  &&  head!=NULL)
            {
                //saveHandToFile(cards,size,boxedCards,boxedSize);
                size=5;

                for(temp=4;temp<55;++temp)
                {
                    for(i=0;i<3;++i)
                        if(temp%52==cards[i])
                            i=4;
                    if(i<4)
                    {
                        for(i=0;i<boxedSize;++i)
                            if(temp%52==boxedCards[i])
                                i=boxedSize+1;
                        if(i<=boxedSize)
                        {
                            cards[3]=boxedCards[boxedSize]=temp%52;

                            for(n=temp+1;n<56;++n)
                            {
                                for(i=0;i<3;++i)
                                    if(n%52==cards[i])
                                        i=4;
                                if(i<4)
                                {
                                    for(i=0;i<boxedSize;++i)
                                        if(n%52==boxedCards[i])
                                            i=boxedSize+1;
                                    if(i<=boxedSize)
                                    {
                                        cards[4]=boxedCards[boxedSize+1]=n%52;

                                        saveHandToFile(cards,size,boxedCards,boxedSize);
                                        prior=printMade(cards,3,boxedCards,boxedSize+2);
                                        head=printMade(cards,5,boxedCards,boxedSize);
                                        improve=getImproveRange(head,prior);
                                        //printLiveCards(cards,5,boxedCards,boxedSize,head->holeCardHigh-200);
                                        printRangeWithImprove(head,improve);
                                        //printHistogram(head,improve);

                                        freeRange(&prior);
                                        free(improve);
                                    }
                                }
                            }
                        }
                    }
                }
                loadHandFromFile(cards,&size,&boxedCards,&boxedSize);
                head=printMade(cards,size,boxedCards,boxedSize);

                command='q';
            }


                /*initgraph(&gd, &gm, "C:\\TC\\BGI"); // "X:\\TC\\BGI");
                SetColor(RED);
                printf("\ntest!");
                SetColor(DEFAULT_FORGC);

                //settextstyle(BOLD_FONT,HORIZ_DIR,2);
                //outtextxy(275,0,"BAR GRAPH");

                //setlinestyle(SOLID_LINE,0,2);
                // Draw X and Y Axis
                line(90,410,90,50);
                //line(90,410,590,410);
//                line(85,60,90,50);
                //line(95,60,90,50);
                //line(585,405,590,410);
                //line(585,415,590,410);

                //outtextxy(65,60,"Y");
                //outtextxy(570,420,"X");
                //outtextxy(70,415,"O");
                // Draw bars on screen
                //setfillstyle(XHATCH_FILL, RED);
                //bar(150,80,200,410);
                //bar(225,100,275,410);
                //bar(300,120,350,410);
                //bar(375,170,425,410);
                bar(450,135,500,410);

                system("pause");
                closegraph();
                return 0;*/
		}
	}
}

exit(99);	//inaccessible code
}
