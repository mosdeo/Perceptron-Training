#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include "Perceptron.h"

using namespace std;

FILE *cfPtr;

int Classification;

class StepFeature
{
public:
	StepFeature()
	{
	}
	static int UpperDataCounter;
	static int DownDataCounter;
	int AccelerationPeakToPeak;
	int BMI;
	int label;
};
int StepFeature::UpperDataCounter=0;
int StepFeature::DownDataCounter=0;

struct Pocket
{
	int Vector[3];
	int counter;
}Pocket;

vector<StepFeature> vTrainingSet;

int main(void)
{
	cout << "Pocket Perceptron Training\n\n" ;
	srand((unsigned)time(NULL));
	//Perceptron* p = new Perceptron(-2508+rand()%500,8926+rand()%500,3764+rand()%500);
	Perceptron* p = new Perceptron(-130, -61, 11710);
	printf("First,%s\n",p->GetWeightVector().c_str());
	_sleep(1000);

	if( (cfPtr = fopen("../上下樓梯特徵集(原始數據).txt","r")) == NULL)
	{
		cout << "找不到檔案";
		system("PAUSE");
	}
	else
	{
		while(!feof(cfPtr))
		{
			int n,AccPtoP,BMI;
			char c;
			StepFeature tempStepFeature;

			fscanf(cfPtr,"%d%d%c%d%d%c%d",&Classification,&n,&c,&AccPtoP,&n,&c,&BMI); //讀入第一個數字，判別為哪一類?
			printf("類%d AccPtoP=%d BMI=%d\n",Classification,AccPtoP,BMI);
			tempStepFeature.AccelerationPeakToPeak = AccPtoP;
			tempStepFeature.BMI=BMI;

			switch(Classification)  //針對不同類
			{
				case 1://上樓梯
					tempStepFeature.label=1;
					StepFeature::UpperDataCounter++;
					break;
				case 2://下樓梯
					tempStepFeature.label=-1;
					StepFeature::DownDataCounter++;
					break;
			}

			vTrainingSet.push_back(tempStepFeature);
		}

		for(vector<StepFeature>::iterator it=vTrainingSet.begin();it!=vTrainingSet.end();it++)
		{//洗牌
			//srand((unsigned)time(NULL));
			int i = rand()%vTrainingSet.size(); printf("%d,",i);
			StepFeature temp = *it;
			*it = vTrainingSet[i];
			vTrainingSet[i] = temp;
		}

		for(vector<StepFeature>::iterator it=vTrainingSet.begin();it!=vTrainingSet.end();it++)
		{//洗牌驗證
			printf("%d",it->label);
		}

		fclose(cfPtr);
	} //讀取完成

	printf("\n");
	printf("類1總共%d筆資料\n",StepFeature::UpperDataCounter);
	printf("類2總共%d筆資料\n",StepFeature::DownDataCounter);
	_sleep(1000);

	int PRcounter=0;
	int Tcounter=0;

	while(1)
	{//訓練迴圈
		for(vector<StepFeature>::iterator it=vTrainingSet.begin();it!=vTrainingSet.end();it++)
		{
			PRcounter++;

			int InnerProduct = p->SumOfAction(it->AccelerationPeakToPeak,it->BMI);

			if(it->label * InnerProduct > 0)
			{//分類正確
				Tcounter++;//正確一次加一分
			}
			//else
			//{//分類錯誤
			//	//修正
			//	if(PRcounter >= vTrainingSet.size())
			//	{
			//		p->ErrSubWeightVector((it->AccelerationPeakToPeak),(it->BMI),(it->label));
			//		Tcounter=0;//啟用新分類向量, 從0開始計算
			//		PRcounter=0;
			//	}
			//}
			else
			{//分類錯誤
			 //修正
				if (PRcounter >= vTrainingSet.size())
				{
					if (-1 == it->label)//因為算錯了,要減(加)當初輸入的東西
					{
						p->ErrSubWeightVector((it->AccelerationPeakToPeak), (it->BMI), 1);
					}
					else if (1 == it->label)
					{
						p->ErrAddWeightVector((it->AccelerationPeakToPeak), (it->BMI), 1);
					}
					else
					{
 						system("PAUSE");
					}

					//p->SubErrWeightVector((it->AccelerationPeakToPeak),(it->BMI),(it->label));
					Tcounter = 0;//啟用新分類向量, 從0開始計算
					PRcounter = 0;
				}
			}

			if(Tcounter > Pocket.counter)//如果更好,放進口袋
			{
				Pocket.counter = Tcounter;
				Pocket.Vector[0]=p->GetWeightValue(0);
				Pocket.Vector[1]=p->GetWeightValue(1);
				Pocket.Vector[2]=p->GetWeightValue(2);
				printf("Pocket.counter=%d, Pocket.Vector=[Acc=%d,BMI=%d,W0=%d]\n",Pocket.counter,Pocket.Vector[0],Pocket.Vector[1],Pocket.Vector[2]);
			}
		}
	}
}