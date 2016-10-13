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

	if( (cfPtr = fopen("../�W�U�ӱ�S�x��(��l�ƾ�).txt","r")) == NULL)
	{
		cout << "�䤣���ɮ�";
		system("PAUSE");
	}
	else
	{
		while(!feof(cfPtr))
		{
			int n,AccPtoP,BMI;
			char c;
			StepFeature tempStepFeature;

			fscanf(cfPtr,"%d%d%c%d%d%c%d",&Classification,&n,&c,&AccPtoP,&n,&c,&BMI); //Ū�J�Ĥ@�ӼƦr�A�P�O�����@��?
			printf("��%d AccPtoP=%d BMI=%d\n",Classification,AccPtoP,BMI);
			tempStepFeature.AccelerationPeakToPeak = AccPtoP;
			tempStepFeature.BMI=BMI;

			switch(Classification)  //�w�藍�P��
			{
				case 1://�W�ӱ�
					tempStepFeature.label=1;
					StepFeature::UpperDataCounter++;
					break;
				case 2://�U�ӱ�
					tempStepFeature.label=-1;
					StepFeature::DownDataCounter++;
					break;
			}

			vTrainingSet.push_back(tempStepFeature);
		}

		for(vector<StepFeature>::iterator it=vTrainingSet.begin();it!=vTrainingSet.end();it++)
		{//�~�P
			//srand((unsigned)time(NULL));
			int i = rand()%vTrainingSet.size(); printf("%d,",i);
			StepFeature temp = *it;
			*it = vTrainingSet[i];
			vTrainingSet[i] = temp;
		}

		for(vector<StepFeature>::iterator it=vTrainingSet.begin();it!=vTrainingSet.end();it++)
		{//�~�P����
			printf("%d",it->label);
		}

		fclose(cfPtr);
	} //Ū������

	printf("\n");
	printf("��1�`�@%d�����\n",StepFeature::UpperDataCounter);
	printf("��2�`�@%d�����\n",StepFeature::DownDataCounter);
	_sleep(1000);

	int PRcounter=0;
	int Tcounter=0;

	while(1)
	{//�V�m�j��
		for(vector<StepFeature>::iterator it=vTrainingSet.begin();it!=vTrainingSet.end();it++)
		{
			PRcounter++;

			int InnerProduct = p->SumOfAction(it->AccelerationPeakToPeak,it->BMI);

			if(it->label * InnerProduct > 0)
			{//�������T
				Tcounter++;//���T�@���[�@��
			}
			//else
			//{//�������~
			//	//�ץ�
			//	if(PRcounter >= vTrainingSet.size())
			//	{
			//		p->ErrSubWeightVector((it->AccelerationPeakToPeak),(it->BMI),(it->label));
			//		Tcounter=0;//�ҥηs�����V�q, �q0�}�l�p��
			//		PRcounter=0;
			//	}
			//}
			else
			{//�������~
			 //�ץ�
				if (PRcounter >= vTrainingSet.size())
				{
					if (-1 == it->label)//�]������F,�n��(�[)����J���F��
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
					Tcounter = 0;//�ҥηs�����V�q, �q0�}�l�p��
					PRcounter = 0;
				}
			}

			if(Tcounter > Pocket.counter)//�p�G��n,��i�f�U
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