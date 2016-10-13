#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <cmath>
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
};

vector<StepFeature> vTrainingSet;

int main(void)
{
	cout << "Pocket Perceptron Training\n\n" ;
	srand((unsigned)time(NULL));
	int RandRange = 9999;
	Perceptron* p00 = new Perceptron(rand()%RandRange-RandRange,rand()%RandRange-RandRange,rand()%RandRange-RandRange);
	Pocket* pocket00 = new Pocket;

	Perceptron* p01 = new Perceptron(rand()%RandRange-RandRange,rand()%RandRange-RandRange,rand()%RandRange-RandRange);
	Pocket* pocket01 = new Pocket;

	Perceptron* p10 = new Perceptron(rand()%RandRange-RandRange,rand()%RandRange-RandRange,rand()%RandRange-RandRange);
	Pocket* pocket10 = new Pocket;

	printf("%s\n",p00->GetWeightVector().c_str());
	printf("%s\n",p01->GetWeightVector().c_str());
	printf("%s\n",p10->GetWeightVector().c_str());

	_sleep(2000);

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

			//輸入層運算
			p00->InnerProductSum = p00->SumOfAction(it->AccelerationPeakToPeak, it->BMI);
			p01->InnerProductSum = p01->SumOfAction(it->AccelerationPeakToPeak, it->BMI);

			//輸出層運算
			p10->SumOfAction(p00->InnerProductSum, p01->InnerProductSum);

			if(it->label * p10->InnerProductSum > 0)
			{//分類正確
				Tcounter++;//正確一次加一分
			}
			else
			{//分類錯誤
				//修正
				if(PRcounter >= vTrainingSet.size())
				{
					if (0 == it->label)//因為算錯了,要減(加)當初輸入的東西
					{
						//倒傳遞第一關
						p10->ErrSubWeightVector(p00->InnerProductSum*(it->label - p10->InnerProductSum), p01->InnerProductSum, 1);

						//倒傳遞第二關
						p00->ErrSubWeightVector((it->AccelerationPeakToPeak), (it->BMI), 1);
						p01->ErrSubWeightVector((it->AccelerationPeakToPeak), (it->BMI), 1);
					}
					else if (1 == it->label)
					{
						//倒傳遞第一關
						p10-> ErrAddWeightVector(p00->InnerProductSum*(it->label - p10->InnerProductSum), p01->InnerProductSum, 1);

						//倒傳遞第二關
						p00->ErrAddWeightVector((it->AccelerationPeakToPeak), (it->BMI), 1);
						p01->ErrAddWeightVector((it->AccelerationPeakToPeak), (it->BMI), 1);
					}
					else
					{
						system("PAUSE");
					}

					//p->SubErrWeightVector((it->AccelerationPeakToPeak),(it->BMI),(it->label));
					Tcounter=0;//啟用新分類向量, 從0開始計算
					PRcounter=0;
				}
			}

			if(Tcounter > pocket00->counter)//如果更好,放進口袋
			{
				pocket00->counter = Tcounter;
				pocket01->counter = Tcounter;
				pocket10->counter = Tcounter;

				for(int i=0;i<3;i++)
				{
					pocket00->Vector[i]=p00->GetWeightValue(i);
					pocket01->Vector[i]=p00->GetWeightValue(i);
					pocket10->Vector[i]=p00->GetWeightValue(i);
				}

				printf("pocket00->counter=%d, pocket00->Vector[]=[Acc=%d,BMI=%d,W0=%d]\n",pocket00->counter,pocket00->Vector[0],pocket00->Vector[1],pocket00->Vector[2]);
				printf("pocket01->counter=%d, pocket01->Vector[]=[Acc=%d,BMI=%d,W0=%d]\n",pocket01->counter,pocket01->Vector[0],pocket01->Vector[1],pocket01->Vector[2]);
				printf("pocket10->counter=%d, pocket10->Vector[]=[Acc=%d,BMI=%d,W0=%d]\n",pocket10->counter,pocket10->Vector[0],pocket10->Vector[1],pocket10->Vector[2]);
			}
		}
	}
}