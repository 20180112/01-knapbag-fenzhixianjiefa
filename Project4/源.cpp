#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int NumOfitem;//��Ʒ����
int NumOfNode;//�ڵ���
int limit;//��������
int maxvalue;
int Numoforder;

struct item {
	int weight;//��Ʒ����
	int value;//��Ʒ��ֵ
	double rateofwv;//��λ�صļ�ֵ
	int id;
};
struct Node {
	int level; //�ڵ���
	int currentweight; //�ڵ㵱ǰ����
	int currentvalue;  //�ڵ㵱ǰ��ֵ
	double bound;//��¼�Ͻ�,�������ȼ����з���
	int solution[100];//��ǰ��
	int active;//��ʾ�ڵ��Ƿ񼤻�
	int order;//��¼�ڵ�˳�������Ƚ��ȳ�����
};

void compare(struct item knap[],int NumOfitem,int*order) {
	//����Ʒ�Լ۱Ȱ���������
	for (int i = 0; i < NumOfitem; i++)
	{//�������е���Ʒ���������ǵ��Լ۱Ƚ�������
		double max = knap[i].rateofwv;
		int maxi = i;
		for (int j = i + 1; j < NumOfitem; j++)
		{
			if (knap[j].rateofwv > max) {
				maxi = j;
				max = knap[j].rateofwv;
			}
		}
		if (maxi != i)
		{
			struct item tmp = knap[i];
			knap[i] = knap[maxi];
			knap[maxi] = tmp;
			//��¼�任˳��
			int tmp1 = order[i];
			order[i] = order[maxi];
			order[maxi] = tmp1;
		}
	}
	return ;
}

void compare1(struct item knap[], int NumOfitem) {
	//����Ʒ�Լ۱Ȱ���������
	for (int i = 0; i < NumOfitem; i++)
	{//�������е���Ʒ���������ǵ��Լ۱Ƚ�������
		double max = knap[i].rateofwv;
		int maxi = i;
		for (int j = i + 1; j < NumOfitem; j++)
		{
			if (knap[j].rateofwv > max) {
				maxi = j;
				max = knap[j].rateofwv;
			}
		}
		if (maxi != i)
		{
			struct item tmp = knap[i];
			knap[i] = knap[maxi];
			knap[maxi] = tmp;
		}
	}
	return;
}


void compare_Node(struct Node noding[], int NumofNode) {
	//��������еĽڵ�����Ͻ���н�������
	for (int i = 0; i < NumofNode; i++)
	{
		double max = noding[i].bound;
		int maxi = i;
		for (int j = i + 1; j < NumofNode; j++)
		{
			if (noding[maxi].active == 0 && noding[j].active == 1)
			{
				maxi = j;
				max = noding[j].bound;
			}
			else if (noding[j].active ==1 && noding[j].bound > max) {
				maxi = j;
				max = noding[j].bound;
			}
		}
		if (maxi != i)
		{
			struct Node tmp = noding[i];
			noding[i] = noding[maxi];
			noding[maxi] = tmp;
		}
	}
	return;
}

void compare_Node2(struct Node noding[], int NumofNode) {
	//��������еĽڵ���ݽڵ������е�˳�������������
	for (int i = 0; i < NumofNode; i++)
	{
		int min = noding[i].order;
		int mini = i;
		for (int j = i + 1; j < NumofNode; j++)
		{
			if (noding[mini].active == 0 && noding[j].active == 1)
			{
				mini = j;
				min = noding[j].order;
			}
			else if (noding[j].active == 1 && noding[j].order < min) {
				mini = j;
				min = noding[j].order;
			}
		}
		if (mini != i)
		{
			struct Node tmp = noding[i];
			noding[i] = noding[mini];
			noding[mini] = tmp;
		}
	}
	return;
}

double Bound(int t,struct item knap[], int currentwaght, int currentvalue)//���㵱ǰ�ڵ�ļ�ֵ�Ͻ�
{
	int leftweight = limit - currentwaght;
	double sumvalue = (double)currentvalue;
	while (t < NumOfitem && knap[t].weight <= leftweight)
	{
		//���ʣ���������ڵ�ǰ�ڵ������������ڵ�
		leftweight = leftweight - knap[t].weight;
		sumvalue = sumvalue + (double)knap[t].value;
		t++;
	}
	if (t < NumOfitem)
	{
		//������û�б���������ĳ������Ʒ�Ų��£�����Ʒ�п�����
		sumvalue = sumvalue + knap[t].rateofwv * (double)leftweight;
	}
	return sumvalue;
}

void knapsack() {
	//���㷨��һ���������0-1����������ѡ��
	printf("��������Ʒ����:");
	scanf("%d", &NumOfitem);
	getchar();
	struct item* knap = (struct item*)malloc(sizeof(item) * (NumOfitem));
	int* knap_order = (int*)malloc(sizeof(int) * (NumOfitem));//��¼ԭʼ˳�򣬱�֤���װ��˳��������˳����ͬ
	for (int i = 0; i < NumOfitem; i++)
	{
		printf("�������%d����Ʒ�ļ�ֵ���������Կո������:", i + 1);
		scanf("%d %d", &((knap + i)->value), &((knap + i)->weight));
		(knap + i)->rateofwv = (double)(knap[i].value) / (double)(knap[i].weight);
		(knap + i)->id = i + 1;
		*(knap_order + i) = i; //����ÿ����Ʒ�ĳ�ʼλ��
		getchar();
	}
	printf("��������������:");
	scanf("%d", &limit);
	getchar();	
	compare(knap, NumOfitem,knap_order);

	NumOfNode = int((pow(2, NumOfitem + 1)) - 1);
	struct Node* noding = (struct Node*)malloc(sizeof(struct Node) * NumOfNode);//�����ڵ�����
	for (int i = 0; i < NumOfNode; i++)
	{
		noding[i].active = 0;
		noding[i].bound = 0;
		noding[i].currentvalue = 0;
		noding[i].currentweight = 0;
		noding[i].level = 0;
		for (int j = 0; j < 100; j++)
		{
			noding[i].solution[j] = -1;
		}
	}
	noding[0].active = 1;//�ڵ㱻�����ʾ���������
	compare_Node(noding,NumOfNode);//�����꼤��Ķ���һ������ǰͷ

	while (noding[0].active == 1)
	{//�������л��м���Ķ���ʱ
		struct Node top = noding[0];
		if (top.level == NumOfitem)
		{
			printf("(");
			for (int i = 0; i < NumOfitem; i++) {
				if (i == NumOfitem - 1)
				{
					for (int j = 0; j < NumOfitem; j++)
					{//�ҵ��˵㽻��˳�������λ�ã������λ�õ�ֵ
						if (knap_order[j] == i) {
							printf("%d", top.solution[j]);
						}
					}
				}
				else {
					for (int j = 0; j < NumOfitem; j++)
					{//�ҵ��˵㽻��˳�������λ�ã������λ�õ�ֵ
						if (knap_order[j] == i) {
							printf("%d,", top.solution[j]);
						}
					}
				}
			}
			printf("),the weight is %d,the value is %d\n",top.currentweight,top.currentvalue);
			if (top.currentvalue > maxvalue)
			{//��¼����ֵ
				maxvalue = top.currentvalue;
			}
		}
		else{
			struct Node* nextNode = (struct Node*)malloc(sizeof(struct Node) * 2);//������չ�����ڵ�
			nextNode[0].level = top.level + 1;
			nextNode[0].solution[nextNode[0].level - 1] = 0;//��߽ڵ���Ϊ������
			for (int i = nextNode[0].level - 2; i >= 0; i--)
			{//��֮ǰ��Ľڵ�ѡ���¼����һ��
				nextNode[0].solution[i] = top.solution[i];
			}
			nextNode[1].level = top.level + 1;
			nextNode[1].solution[nextNode[1].level - 1] = 1;//�ұ߽ڵ���Ϊ����
			for (int i = nextNode[1].level - 2; i >= 0; i--)
			{//��֮ǰ��Ľڵ�ѡ���¼����һ��
				nextNode[1].solution[i] = top.solution[i];
			}
			nextNode[0].active = 0;
			nextNode[1].active = 0;
			nextNode[1].currentweight = top.currentweight + knap[nextNode[1].level - 1].weight;
			nextNode[0].currentweight = top.currentweight;
			//ѡ���Ƿ����������չ�ڵ㣬��֦
			if (top.currentweight + knap[nextNode[1].level - 1].weight <= limit)
			{//������˽ڵ㲻������������ʱ��ֱ��ѡ�����˽ڵ�
				for (int i = 0; i < 2; i++)
				{//�����ڵ㶼���������
					nextNode[i].active = 1;
					nextNode[i].currentvalue = top.currentvalue + knap[nextNode[i].level - 1].value * nextNode[i].solution[nextNode[i].level - 1];
					nextNode[i].bound = Bound(nextNode[i].level, knap, nextNode[i].currentweight, nextNode[i].currentvalue);
					for (int j = 0; j < NumOfNode; j++)
					{
						if (noding[j].active == 0) {
							noding[j] = nextNode[i];
							break;
						}
					}
				}
			}
			else
			{//�����½ڵ�ᳬ�أ�ֻ��ѡ������
				nextNode[0].active = 1;
				nextNode[0].currentvalue = top.currentvalue + knap[nextNode[0].level - 1].value * nextNode[0].solution[nextNode[0].level - 1];
				nextNode[0].bound = Bound(nextNode[0].level, knap, nextNode[0].currentweight, nextNode[0].currentvalue);
				for (int j = 0; j < NumOfNode; j++)
				{
					if (noding[j].active == 0) {
						noding[j] = nextNode[0];
						break;
					}
				}
			}
		}
		noding[0].active = 0;
		noding[0].bound = 0;
		compare_Node(noding,NumOfNode);	//�������нڵ���У���֤���е�һ���ڵ�һ�����Ͻ����Ľڵ�
	}
	return;
}

void knapsack1() {
	//���㷨��һ���������0-1����������ѡ��
	printf("��������Ʒ����:");
	scanf("%d", &NumOfitem);
	getchar();
	struct item* knap = (struct item*)malloc(sizeof(item) * (NumOfitem));
	//int* knap_order = (int*)malloc(sizeof(int) * (NumOfitem));//��¼ԭʼ˳�򣬱�֤���װ��˳��������˳����ͬ
	for (int i = 0; i < NumOfitem; i++)
	{
		printf("�������%d����Ʒ�ļ�ֵ���������Կո������:", i + 1);
		scanf("%d %d", &((knap + i)->value), &((knap + i)->weight));
		(knap + i)->rateofwv = (double)(knap[i].value) / (double)(knap[i].weight);
		(knap + i)->id = i + 1;
		//*(knap_order + i) = i;
		getchar();
	}
	printf("��������������:");
	scanf("%d", &limit);
	getchar();
	compare1(knap, NumOfitem);
	printf("�����װ��Ʒ�Ĵ����Ϊ��\n");
	for (int i = 0; i < NumOfitem; i++)
	{
		printf("%d:item%d value:%d weight:%d\n", i + 1, knap[i].id, knap[i].value, knap[i].weight);
	}
	NumOfNode = int((pow(2, NumOfitem + 1)) - 1);
	struct Node* noding = (struct Node*)malloc(sizeof(struct Node) * NumOfNode);//�����ڵ�����
	for (int i = 0; i < NumOfNode; i++)
	{
		noding[i].active = 0;
		noding[i].bound = 0;
		noding[i].currentvalue = 0;
		noding[i].currentweight = 0;
		noding[i].level = 0;
		for (int j = 0; j < 100; j++)
		{
			noding[i].solution[j] = -1;
		}
	}
	noding[0].active = 1;//�ڵ㱻�����ʾ���������
	compare_Node(noding, NumOfNode);//�����꼤��Ķ���һ������ǰͷ

	while (noding[0].active == 1)
	{//�������л��м���Ķ���ʱ
		struct Node top = noding[0];
		if (top.level == NumOfitem)
		{
			printf("(");
			for (int i = 0; i < NumOfitem; i++) {
				if (i == NumOfitem - 1)
				{
					printf("%d", top.solution[i]);
				}
				else printf("%d,", top.solution[i]);
			}
			printf("),the weight is %d,the value is %d\n", top.currentweight, top.currentvalue);
			if (top.currentvalue > maxvalue)
			{
				maxvalue = top.currentvalue;
			}
		}
		else {
			struct Node* nextNode = (struct Node*)malloc(sizeof(struct Node) * 2);//������չ�����ڵ�
			nextNode[0].level = top.level + 1;
			nextNode[0].solution[nextNode[0].level - 1] = 0;//��߽ڵ���Ϊ������
			for (int i = nextNode[0].level - 2; i >= 0; i--)
			{//��֮ǰ��Ľڵ�ѡ���¼����һ��
				nextNode[0].solution[i] = top.solution[i];
			}
			nextNode[1].level = top.level + 1;
			nextNode[1].solution[nextNode[1].level - 1] = 1;//�ұ߽ڵ���Ϊ����
			for (int i = nextNode[1].level - 2; i >= 0; i--)
			{//��֮ǰ��Ľڵ�ѡ���¼����һ��
				nextNode[1].solution[i] = top.solution[i];
			}
			nextNode[0].active = 0;
			nextNode[1].active = 0;
			nextNode[1].currentweight = top.currentweight + knap[nextNode[1].level - 1].weight;
			nextNode[0].currentweight = top.currentweight;
			//ѡ���Ƿ����������չ�ڵ㣬��֦
			if (top.currentweight + knap[nextNode[1].level - 1].weight <= limit)
			{//������˽ڵ㲻������������ʱ��ֱ��ѡ�����˽ڵ�
				for (int i = 0; i < 2; i++)
				{
					nextNode[i].active = 1;
					nextNode[i].currentvalue = top.currentvalue + knap[nextNode[i].level - 1].value * nextNode[i].solution[nextNode[i].level - 1];
					nextNode[i].bound = Bound(nextNode[i].level, knap, nextNode[i].currentweight, nextNode[i].currentvalue);
					for (int j = 0; j < NumOfNode; j++)
					{
						if (noding[j].active == 0) {
							noding[j] = nextNode[i];
							break;
						}
					}
				}
			}
			else
			{//�����½ڵ�ᳬ�أ�ֻ��ѡ������
				nextNode[0].active = 1;
				nextNode[0].currentvalue = top.currentvalue + knap[nextNode[0].level - 1].value * nextNode[0].solution[nextNode[0].level - 1];
				nextNode[0].bound = Bound(nextNode[0].level, knap, nextNode[0].currentweight, nextNode[0].currentvalue);
				for (int j = 0; j < NumOfNode; j++)
				{
					if (noding[j].active == 0) {
						noding[j] = nextNode[0];
						break;
					}
				}
			}
		}
		noding[0].active = 0;
		noding[0].bound = 0;
		compare_Node(noding, NumOfNode);	//�������нڵ���У���֤���е�һ���ڵ�һ�����Ͻ����Ľڵ�
	}
	return;
}

void knapsack2() {
	printf("��������Ʒ����:");
	scanf("%d", &NumOfitem);
	getchar();
	struct item* knap = (struct item*)malloc(sizeof(item) * (NumOfitem));
	for (int i = 0; i < NumOfitem; i++)
	{
		printf("�������%d����Ʒ�ļ�ֵ���������Կո������:", i + 1);
		scanf("%d %d", &((knap + i)->value), &((knap + i)->weight));
		(knap + i)->rateofwv = (double)(knap[i].value) / (double)(knap[i].weight);
		getchar();
	}
	printf("��������������:");
	scanf("%d", &limit);
	getchar();
	//compare(knap, NumOfitem);//����Ʒ���ݵ�λ��ֵ�������

	NumOfNode = int((pow(2, NumOfitem + 1)) - 1);
	struct Node* noding = (struct Node*)malloc(sizeof(struct Node) * NumOfNode);//�����ڵ�����
	for (int i = 0; i < NumOfNode; i++)
	{
		noding[i].active = 0;
		//noding[i].bound = 0;
		noding[i].currentvalue = 0;
		noding[i].currentweight = 0;
		noding[i].level = 0;
		noding[i].order = INFINITY;
		for (int j = 0; j < 100; j++)
		{
			noding[i].solution[j] = -1;
		}
	}
	noding[0].active = 1;//�ڵ㱻�����ʾ���������
	Numoforder++;
	noding[0].order = Numoforder;
	compare_Node2(noding, NumOfNode);//�����꼤��Ķ���һ������ǰͷ

	while (noding[0].active == 1)
	{//�������л��м���Ķ���ʱ
		struct Node top = noding[0];
		if (top.level == NumOfitem)
		{
			printf("(");
			for (int i = 0; i < NumOfitem; i++) {
				if (i == NumOfitem - 1)
				{
					printf("%d", top.solution[i]);
				}
				else printf("%d,", top.solution[i]);
			}
			printf("),the weight is %d,the value is %d\n", top.currentweight, top.currentvalue);
			if (top.currentvalue > maxvalue)
			{
				maxvalue = top.currentvalue;
			}
		}
		else {
			struct Node* nextNode = (struct Node*)malloc(sizeof(struct Node) * 2);//������չ�����ڵ�
			nextNode[0].level = top.level + 1;
			nextNode[0].solution[nextNode[0].level - 1] = 0;//һ���ڵ���Ϊ����
			for (int i = nextNode[0].level - 2; i >= 0; i--)
			{//��֮ǰ��Ľڵ�ѡ���¼����һ��
				nextNode[0].solution[i] = top.solution[i];
			}
			nextNode[1].level = top.level + 1;
			nextNode[1].solution[nextNode[1].level - 1] = 1;//��һ���ڵ���Ϊ������
			for (int i = nextNode[1].level - 2; i >= 0; i--)
			{//��֮ǰ��Ľڵ�ѡ���¼����һ��
				nextNode[1].solution[i] = top.solution[i];
			}
			nextNode[0].active = 0;
			nextNode[1].active = 0;
			nextNode[1].currentweight = top.currentweight + knap[nextNode[0].level - 1].weight;
			nextNode[0].currentweight = top.currentweight;
			//ѡ���Ƿ����������չ�ڵ㣬��֦
			if (top.currentweight + knap[nextNode[1].level - 1].weight <= limit)
			{//������˽ڵ㲻������������ʱ��ֱ��ѡ�����˽ڵ�
				for (int i = 0; i < 2; i++)
				{
					Numoforder++;
					nextNode[i].order = Numoforder;
					nextNode[i].active = 1;
					nextNode[i].currentvalue = top.currentvalue + knap[nextNode[i].level - 1].value * nextNode[i].solution[nextNode[i].level - 1];
					//nextNode[i].bound = Bound(nextNode[i].level, knap, nextNode[i].currentweight, nextNode[i].currentvalue);
					for (int j = 0; j < NumOfNode; j++)
					{
						if (noding[j].active == 0) {
							noding[j] = nextNode[i];
							break;
						}
					}
				}
			}
			else
			{//�����½ڵ�ᳬ�أ�ֻ��ѡ������
				Numoforder++;
				nextNode[0].order = Numoforder;
				nextNode[0].active = 1;
				nextNode[0].currentvalue = top.currentvalue + knap[nextNode[0].level - 1].value * nextNode[0].solution[nextNode[0].level - 1];
				//nextNode[1].bound = Bound(nextNode[1].level, knap, nextNode[1].currentweight, nextNode[1].currentvalue);
				for (int j = 0; j < NumOfNode; j++)
				{
					if (noding[j].active == 0) {
						noding[j] = nextNode[0];
						break;
					}
				}
			}
		}
		noding[0].active = 0;
		//noding[0].bound = 0;
		noding[0].order = INFINITY;
		compare_Node2(noding, NumOfNode);	//�������нڵ���У���֤���е�һ���ڵ�һ�����Ͻ����Ľڵ�
	}
	return;
}

int main() {
	//maxvalue = 0;
	//knapsack1();
	//printf("The max value is %d.\n", maxvalue);
	maxvalue = 0;
	knapsack();
	printf("The max value is %d.\n", maxvalue);
	maxvalue = 0;
	Numoforder = 0;
	knapsack2();
	printf("The max value is %d.", maxvalue);
	return 1;
}