#include <windows.h>
#include <stdio.h>

#include "struct_frac.h"
#include "struct_range.h"
#include "improve.h"
#include "windows_gui.h"

extern int SUIT_0;// = 0;
extern int SUIT_1;// = 1;
extern int SUIT_2;// = 2;
extern int SUIT_3;// = 3;
extern unsigned int HEIGHT;// = 54;
extern unsigned int WIDTH;//  = 80;
extern int DEFAULT_FORGC;// = 7;
extern int DEFAULT_BACKC;// = 0;

extern int face[17];//={120,97,50,51,52,53,54,55,56,57,84,74,81,75,65,101,69};			//	xa23456789TJQKAeE


int lcm(int a,int b){
	static int temp = 1;
	if(temp % b == 0 && temp % a == 0)
		return temp;
	temp++;
	lcm(a,b);
	return temp;
}

int gcd(int a,int b){
	int rem;

	if((rem = a % b)==0)
		return b;
    else
		return gcd(b,rem);
}

Frac* getImproveRange(Range *range,Range *prior){
    Range* curr;
	Range* node;
	Frac rangeCombo[4];     // .num=2*oldCount+combo (-'ive if ->key>127, i.e. flush or suited preflop) .den=combo
	Frac priorCombo[4];     // .num=2*oldCount+combo (-'ive if ->key>127, i.e. flush or suited preflop) .den=combo
	Frac* val = (Frac*)malloc(range->mod_prob*sizeof(Frac));

	unsigned int oldCount,comboCount = 0;
	short unsigned int n,i,arr,priorSize,rangeSize;
	unsigned long int total_val;

	arr=total_val=0;

	for(curr=range->next;curr!=NULL;(curr=curr->next)&&(++arr))
    {
        if(curr->holeCardHigh==-2)
			val[arr].num=curr->combo*((2*comboCount)+curr->combo)-total_val;
		else
		{
			for(n=val[arr].num=0;n<curr->holeCardSize;++n)
			{
			    oldCount=priorSize=0;
				for(node=prior->next;node!=NULL;node=node->next)
                {
                    for(i=0;i<node->holeCardSize;++i)
                        if(((node->holeCardHigh%16==curr->holeCardHigh%16)&&(node->holeCardArray[i]%16==curr->holeCardArray[n]%16))
                        || ((node->holeCardHigh%16==curr->holeCardArray[n]%16)&&(node->holeCardArray[i]%16==curr->holeCardHigh%16)))
                            {
                                priorCombo[priorSize].num=(2*(node->key>127)-1)*(oldCount+node->combo);
                                priorCombo[priorSize].den=node->holeCardArray[i]/16+1;
                                ++priorSize;
                                i=node->holeCardSize;
                            }
                    oldCount+=2*node->combo;
                }

                if(priorSize==1)
                    val[arr].num+=abs(priorCombo[0].num)*(curr->holeCardArray[n]/16+1);
                else
                {
                    oldCount=rangeSize=0;
                    for(node=range->next;node!=NULL;node=node->next)
                    {
                        if(node->holeCardHigh==-2  &&  rangeSize<4)
                        {
                            rangeCombo[rangeSize].num=-(oldCount+node->combo);
                            for(i=rangeCombo[rangeSize].den=0;i<priorSize;++i)
                                rangeCombo[rangeSize].den+=priorCombo[i].den;
                            for(i=0;i<rangeSize;++i)
                                rangeCombo[rangeSize].den-=rangeCombo[i].den;
                            if(rangeCombo[rangeSize].den)
                                ++rangeSize;
                        }
                        else
                        {
                            for(i=0;i<node->holeCardSize;++i)
                                if(((node->holeCardHigh%16==curr->holeCardHigh%16)&&(node->holeCardArray[i]%16==curr->holeCardArray[n]%16))
                                || ((node->holeCardHigh%16==curr->holeCardArray[n]%16)&&(node->holeCardArray[i]%16==curr->holeCardHigh%16)))
                                    {
                                        rangeCombo[rangeSize].num=(2*(node->key>127)-1)*(oldCount+node->combo);
                                        rangeCombo[rangeSize].den=node->holeCardArray[i]/16+1;
                                        ++rangeSize;
                                        i=node->holeCardSize;
                                    }
                            oldCount+=2*node->combo;
                        }
                    }

                    if(rangeSize==1)
                        for(i=0;i<priorSize;++i)
                            val[arr].num+=abs(priorCombo[i].num)*priorCombo[i].den;
                    else if(prior->holeCardSize==16)//monotone flop
                    {
                        if(curr->key>199)
                            val[arr].num+=priorCombo[0].num;
                        else
                            val[arr].num+=abs(priorCombo[0].num)*(priorCombo[0].den-1)+abs(priorCombo[1].num*(priorCombo[1].den));
                    }

                    else if(rangeSize==priorSize)
                    {
                        //SetColor(11);
                        //printf("#");
                        //SetColor(DEFAULT_FORGC);

                        for(i=0;(2*comboCount+curr->combo)!=((unsigned)abs(rangeCombo[i].num));++i);
                        val[arr].num+=(abs(priorCombo[i].num)*rangeCombo[i].den);
                    }

                    else if(curr->key<200)
                       val[arr].num-=priorCombo[priorSize-1].num*rangeCombo[rangeSize-1].den;

                    else    //  [1,x] [x,1] [a,a] already solved; following contains: [2,3] [2,4] [3,2] [4,2]
                    {
                        /*SetColor(5);
                        printf(" P[%d]{ ",priorSize);
                        for(i=0;i<priorSize;++i)printf("%d ",priorCombo[i].den);
                        printf("} R[%d]{ ",rangeSize);
                        for(i=0;i<rangeSize;++i)printf("%d ",rangeCombo[i].den);
                        printf("}");
                        SetColor(13);
                        printf(" %c%c:",face[curr->holeCardHigh+2],face[curr->holeCardArray[n]%13+2]);*/

                        for(i=0;(2*comboCount+curr->combo)!=((unsigned)abs(rangeCombo[i].num));++i);

                        if(!i)
                        {
                            //printf("A");
                            if(rangeCombo[0].den>priorCombo[0].den)
                              val[arr].num+=priorCombo[0].num*priorCombo[0].den+abs(priorCombo[1].num*(rangeCombo[0].den-priorCombo[0].den));
                            else
                                 val[arr].num+=priorCombo[0].num*rangeCombo[0].den;
                        }
                        else// if(i==1)
                            val[arr].num+=abs(priorCombo[1].num*rangeCombo[i].den);
                    }
                }
            }
		}

		total_val+=val[arr].num-=(curr->combo*((2*comboCount)+curr->combo));
        comboCount+=curr->combo;

		val[arr].den=2*curr->combo;
        i=gcd(abs(val[arr].num),val[arr].den);
        val[arr].num/=i;
		val[arr].den/=i;

		/*SetColor(12);
		printf("%2d %5lu:%4d/%2d\n",arr,total_val,val[arr].num,val[arr].den);
        SetColor(DEFAULT_FORGC);*/
    }
    if(total_val)
    {
        printExpandedRange(range);
        printRaw(range);
        printf("\ttotal_val: %lu",total_val);
        exit(666);
    }

    return val;
}



Frac* getImprovePrior(Range *range,Range *prior){
    Range* curr;
	Range* node;
	Frac rangeCombo[4];     // .num=2*oldCount+combo (-'ive if ->key>127, i.e. flush or suited preflop) .den=combo
	Frac priorCombo[4];     // .num=2*oldCount+combo (-'ive if ->key>127, i.e. flush or suited preflop) .den=combo
	Frac* val = (Frac*)malloc(prior->mod_prob*sizeof(Frac));

	unsigned int oldCount,comboCount = 0;
	short unsigned int n,i,arr,priorSize,rangeSize;
	unsigned long int total_val;

	arr=total_val=0;

	for(curr=prior->next;curr!=NULL;(curr=curr->next)&&(++arr))
    {
        for(n=val[arr].num=0;n<curr->holeCardSize;++n)
		{
		    oldCount=priorSize=0;
			for(node=prior->next;node!=NULL;node=node->next)
            {
                for(i=0;i<node->holeCardSize;++i)
                if(((node->holeCardHigh%13==curr->holeCardHigh%13)&&(node->holeCardArray[i]%13==curr->holeCardArray[n]%13))
                || ((node->holeCardHigh%13==curr->holeCardArray[n]%13)&&(node->holeCardArray[i]%13==curr->holeCardHigh%13)))
                        {
                            priorCombo[priorSize].num=(2*(node->key>127)-1)*(oldCount+node->combo);
                            priorCombo[priorSize].den=node->holeCardArray[i]/13;
                            ++priorSize;
                            i=node->holeCardSize;
                        }
                oldCount+=2*node->combo;
            }

                if(priorSize==1)
                    val[arr].num+=abs(priorCombo[0].num)*(curr->holeCardArray[n]/13);
                else
                {
                    oldCount=rangeSize=0;
                    for(node=range->next;node!=NULL;node=node->next)
                    {
                        if(node->holeCardHigh==-2  &&  rangeSize<4)
                        {
                            rangeCombo[rangeSize].num=-(oldCount+node->combo);
                            for(i=rangeCombo[rangeSize].den=0;i<priorSize;++i)
                                rangeCombo[rangeSize].den+=priorCombo[i].den;
                            for(i=0;i<rangeSize;++i)
                                rangeCombo[rangeSize].den-=rangeCombo[i].den;
                            if(rangeCombo[rangeSize].den)
                                ++rangeSize;
                        }
                        else
                        {
                            for(i=0;i<node->holeCardSize;++i)
                                if(((node->holeCardHigh%13==curr->holeCardHigh%13)&&(node->holeCardArray[i]%13==curr->holeCardArray[n]%13))
                                || ((node->holeCardHigh%13==curr->holeCardArray[n]%13)&&(node->holeCardArray[i]%13==curr->holeCardHigh%13)))
                                    {
                                        rangeCombo[rangeSize].num=(2*(node->key>127)-1)*(oldCount+node->combo);
                                        rangeCombo[rangeSize].den=node->holeCardArray[i]/13;
                                        ++rangeSize;
                                        i=node->holeCardSize;
                                    }
                            oldCount+=2*node->combo;
                        }
                    }

                    if(rangeSize==1)
                        for(i=0;i<priorSize;++i)
                            val[arr].num+=abs(priorCombo[i].num)*priorCombo[i].den;
                    else if(prior->holeCardSize==16)//monotone flop
                    {
                        if(curr->key>199)
                            val[arr].num+=priorCombo[0].num;
                        else
                            val[arr].num+=abs(priorCombo[0].num)*(priorCombo[0].den-1)+abs(priorCombo[1].num*(priorCombo[1].den));
                    }

                    else if(rangeSize==priorSize)
                    {
                        //SetColor(11);
                        //printf("#");
                        //SetColor(DEFAULT_FORGC);

                        for(i=0;(2*comboCount+curr->combo)!=((unsigned)abs(rangeCombo[i].num));++i);
                        val[arr].num+=(abs(priorCombo[i].num)*rangeCombo[i].den);
                    }

                    else if(curr->key<200)
                       val[arr].num-=priorCombo[priorSize-1].num*rangeCombo[rangeSize-1].den;

                    else    //  [1,x] [x,1] [a,a] already solved; following contains: [2,3] [2,4] [3,2] [4,2]
                    {
                        /*SetColor(5);
                        printf(" P[%d]{ ",priorSize);
                        for(i=0;i<priorSize;++i)printf("%d ",priorCombo[i].den);
                        printf("} R[%d]{ ",rangeSize);
                        for(i=0;i<rangeSize;++i)printf("%d ",rangeCombo[i].den);
                        printf("}");
                        SetColor(13);
                        printf(" %c%c:",face[curr->holeCardHigh+2],face[curr->holeCardArray[n]%13+2]);*/

                        for(i=0;(2*comboCount+curr->combo)!=((unsigned)abs(rangeCombo[i].num));++i);

                        if(!i)
                        {
                            //printf("A");
                            if(rangeCombo[0].den>priorCombo[0].den)
                              val[arr].num+=priorCombo[0].num*priorCombo[0].den+abs(priorCombo[1].num*(rangeCombo[0].den-priorCombo[0].den));
                            else
                                 val[arr].num+=priorCombo[0].num*rangeCombo[0].den;
                        }
                        else// if(i==1)
                            val[arr].num+=abs(priorCombo[1].num*rangeCombo[i].den);
                    }
                }
            }

		total_val+=val[arr].num-=(curr->combo*((2*comboCount)+curr->combo));
        comboCount+=curr->combo;

		val[arr].den=2*curr->combo;
        i=gcd(abs(val[arr].num),val[arr].den);
        val[arr].num/=i;
		val[arr].den/=i;

		//SetColor(10);
		//printf("\t%lu:%d/%d",total_val,val[arr].num,val[arr].den);
		//SetColor(DEFAULT_FORGC);
    }
    if(total_val)
    {
//        printf("\ttotal_val: %lu",total_val);
        exit(666);
    }
    return val;
}
