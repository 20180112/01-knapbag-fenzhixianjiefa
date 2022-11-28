#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int NumOfitem;//物品数量
int NumOfNode;//节点数
int limit;//限制重量
int maxvalue;
int Numoforder;

struct item {
	int weight;//物品重量
	int value;//物品价值
	double rateofwv;//单位重的价值
	int id;
};
struct Node {
	int level; //节点层次
	int currentweight; //节点当前重量
	int currentvalue;  //节点当前价值
	double bound;//记录上界,用于优先级队列方法
	int solution[100];//当前解
	int active;//表示节点是否激活
	int order;//记录节点顺序，用于先进先出方法
};

void compare(struct item knap[],int NumOfitem,int*order) {
	//将物品性价比按降序排序
	for (int i = 0; i < NumOfitem; i++)
	{//遍历所有的物品，根据它们的性价比进行排序
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
			//记录变换顺序
			int tmp1 = order[i];
			order[i] = order[maxi];
			order[maxi] = tmp1;
		}
	}
	return ;
}

void compare1(struct item knap[], int NumOfitem) {
	//将物品性价比按降序排序
	for (int i = 0; i < NumOfitem; i++)
	{//遍历所有的物品，根据它们的性价比进行排序
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
	//将进入队列的节点根据上界进行降序排序
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
	//将进入队列的节点根据节点进入队列的顺序进行升序排序
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

double Bound(int t,struct item knap[], int currentwaght, int currentvalue)//计算当前节点的价值上界
{
	int leftweight = limit - currentwaght;
	double sumvalue = (double)currentvalue;
	while (t < NumOfitem && knap[t].weight <= leftweight)
	{
		//如果剩余重量大于当前节点重量，则放入节点
		leftweight = leftweight - knap[t].weight;
		sumvalue = sumvalue + (double)knap[t].value;
		t++;
	}
	if (t < NumOfitem)
	{
		//若背包没有被放满但是某整个物品放不下，则将物品切开放置
		sumvalue = sumvalue + knap[t].rateofwv * (double)leftweight;
	}
	return sumvalue;
}

void knapsack() {
	//此算法第一个输出就是0-1背包的最优选择
	printf("请输入物品数量:");
	scanf("%d", &NumOfitem);
	getchar();
	struct item* knap = (struct item*)malloc(sizeof(item) * (NumOfitem));
	int* knap_order = (int*)malloc(sizeof(int) * (NumOfitem));//记录原始顺序，保证输出装包顺序与输入顺序相同
	for (int i = 0; i < NumOfitem; i++)
	{
		printf("请输入第%d个物品的价值和重量（以空格隔开）:", i + 1);
		scanf("%d %d", &((knap + i)->value), &((knap + i)->weight));
		(knap + i)->rateofwv = (double)(knap[i].value) / (double)(knap[i].weight);
		(knap + i)->id = i + 1;
		*(knap_order + i) = i; //记下每个物品的初始位置
		getchar();
	}
	printf("请输入限制重量:");
	scanf("%d", &limit);
	getchar();	
	compare(knap, NumOfitem,knap_order);

	NumOfNode = int((pow(2, NumOfitem + 1)) - 1);
	struct Node* noding = (struct Node*)malloc(sizeof(struct Node) * NumOfNode);//创建节点数组
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
	noding[0].active = 1;//节点被激活表示进入队列中
	compare_Node(noding,NumOfNode);//排序完激活的顶点一定在最前头

	while (noding[0].active == 1)
	{//当队列中还有激活的顶点时
		struct Node top = noding[0];
		if (top.level == NumOfitem)
		{
			printf("(");
			for (int i = 0; i < NumOfitem; i++) {
				if (i == NumOfitem - 1)
				{
					for (int j = 0; j < NumOfitem; j++)
					{//找到此点交换顺序后所在位置，输出此位置的值
						if (knap_order[j] == i) {
							printf("%d", top.solution[j]);
						}
					}
				}
				else {
					for (int j = 0; j < NumOfitem; j++)
					{//找到此点交换顺序后所在位置，输出此位置的值
						if (knap_order[j] == i) {
							printf("%d,", top.solution[j]);
						}
					}
				}
			}
			printf("),the weight is %d,the value is %d\n",top.currentweight,top.currentvalue);
			if (top.currentvalue > maxvalue)
			{//记录最大价值
				maxvalue = top.currentvalue;
			}
		}
		else{
			struct Node* nextNode = (struct Node*)malloc(sizeof(struct Node) * 2);//向下扩展两个节点
			nextNode[0].level = top.level + 1;
			nextNode[0].solution[nextNode[0].level - 1] = 0;//左边节点设为不放入
			for (int i = nextNode[0].level - 2; i >= 0; i--)
			{//将之前层的节点选择记录到这一层
				nextNode[0].solution[i] = top.solution[i];
			}
			nextNode[1].level = top.level + 1;
			nextNode[1].solution[nextNode[1].level - 1] = 1;//右边节点设为放入
			for (int i = nextNode[1].level - 2; i >= 0; i--)
			{//将之前层的节点选择记录到这一层
				nextNode[1].solution[i] = top.solution[i];
			}
			nextNode[0].active = 0;
			nextNode[1].active = 0;
			nextNode[1].currentweight = top.currentweight + knap[nextNode[1].level - 1].weight;
			nextNode[0].currentweight = top.currentweight;
			//选择是否放入两个扩展节点，剪枝
			if (top.currentweight + knap[nextNode[1].level - 1].weight <= limit)
			{//当放入此节点不超过限制重量时，直接选择加入此节点
				for (int i = 0; i < 2; i++)
				{//两个节点都进入队列中
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
			{//加入新节点会超重，只能选不放入
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
		compare_Node(noding,NumOfNode);	//重新排列节点队列，保证队列第一个节点一定是上界最大的节点
	}
	return;
}

void knapsack1() {
	//此算法第一个输出就是0-1背包的最优选择
	printf("请输入物品数量:");
	scanf("%d", &NumOfitem);
	getchar();
	struct item* knap = (struct item*)malloc(sizeof(item) * (NumOfitem));
	//int* knap_order = (int*)malloc(sizeof(int) * (NumOfitem));//记录原始顺序，保证输出装包顺序与输入顺序相同
	for (int i = 0; i < NumOfitem; i++)
	{
		printf("请输入第%d个物品的价值和重量（以空格隔开）:", i + 1);
		scanf("%d %d", &((knap + i)->value), &((knap + i)->weight));
		(knap + i)->rateofwv = (double)(knap[i].value) / (double)(knap[i].weight);
		(knap + i)->id = i + 1;
		//*(knap_order + i) = i;
		getchar();
	}
	printf("请输入限制重量:");
	scanf("%d", &limit);
	getchar();
	compare1(knap, NumOfitem);
	printf("排序后，装物品的次序变为：\n");
	for (int i = 0; i < NumOfitem; i++)
	{
		printf("%d:item%d value:%d weight:%d\n", i + 1, knap[i].id, knap[i].value, knap[i].weight);
	}
	NumOfNode = int((pow(2, NumOfitem + 1)) - 1);
	struct Node* noding = (struct Node*)malloc(sizeof(struct Node) * NumOfNode);//创建节点数组
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
	noding[0].active = 1;//节点被激活表示进入队列中
	compare_Node(noding, NumOfNode);//排序完激活的顶点一定在最前头

	while (noding[0].active == 1)
	{//当队列中还有激活的顶点时
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
			struct Node* nextNode = (struct Node*)malloc(sizeof(struct Node) * 2);//向下扩展两个节点
			nextNode[0].level = top.level + 1;
			nextNode[0].solution[nextNode[0].level - 1] = 0;//左边节点设为不放入
			for (int i = nextNode[0].level - 2; i >= 0; i--)
			{//将之前层的节点选择记录到这一层
				nextNode[0].solution[i] = top.solution[i];
			}
			nextNode[1].level = top.level + 1;
			nextNode[1].solution[nextNode[1].level - 1] = 1;//右边节点设为放入
			for (int i = nextNode[1].level - 2; i >= 0; i--)
			{//将之前层的节点选择记录到这一层
				nextNode[1].solution[i] = top.solution[i];
			}
			nextNode[0].active = 0;
			nextNode[1].active = 0;
			nextNode[1].currentweight = top.currentweight + knap[nextNode[1].level - 1].weight;
			nextNode[0].currentweight = top.currentweight;
			//选择是否放入两个扩展节点，剪枝
			if (top.currentweight + knap[nextNode[1].level - 1].weight <= limit)
			{//当放入此节点不超过限制重量时，直接选择加入此节点
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
			{//加入新节点会超重，只能选不放入
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
		compare_Node(noding, NumOfNode);	//重新排列节点队列，保证队列第一个节点一定是上界最大的节点
	}
	return;
}

void knapsack2() {
	printf("请输入物品数量:");
	scanf("%d", &NumOfitem);
	getchar();
	struct item* knap = (struct item*)malloc(sizeof(item) * (NumOfitem));
	for (int i = 0; i < NumOfitem; i++)
	{
		printf("请输入第%d个物品的价值和重量（以空格隔开）:", i + 1);
		scanf("%d %d", &((knap + i)->value), &((knap + i)->weight));
		(knap + i)->rateofwv = (double)(knap[i].value) / (double)(knap[i].weight);
		getchar();
	}
	printf("请输入限制重量:");
	scanf("%d", &limit);
	getchar();
	//compare(knap, NumOfitem);//将物品根据单位价值排序放置

	NumOfNode = int((pow(2, NumOfitem + 1)) - 1);
	struct Node* noding = (struct Node*)malloc(sizeof(struct Node) * NumOfNode);//创建节点数组
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
	noding[0].active = 1;//节点被激活表示进入队列中
	Numoforder++;
	noding[0].order = Numoforder;
	compare_Node2(noding, NumOfNode);//排序完激活的顶点一定在最前头

	while (noding[0].active == 1)
	{//当队列中还有激活的顶点时
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
			struct Node* nextNode = (struct Node*)malloc(sizeof(struct Node) * 2);//向下扩展两个节点
			nextNode[0].level = top.level + 1;
			nextNode[0].solution[nextNode[0].level - 1] = 0;//一个节点设为放入
			for (int i = nextNode[0].level - 2; i >= 0; i--)
			{//将之前层的节点选择记录到这一层
				nextNode[0].solution[i] = top.solution[i];
			}
			nextNode[1].level = top.level + 1;
			nextNode[1].solution[nextNode[1].level - 1] = 1;//另一个节点设为不放入
			for (int i = nextNode[1].level - 2; i >= 0; i--)
			{//将之前层的节点选择记录到这一层
				nextNode[1].solution[i] = top.solution[i];
			}
			nextNode[0].active = 0;
			nextNode[1].active = 0;
			nextNode[1].currentweight = top.currentweight + knap[nextNode[0].level - 1].weight;
			nextNode[0].currentweight = top.currentweight;
			//选择是否放入两个扩展节点，剪枝
			if (top.currentweight + knap[nextNode[1].level - 1].weight <= limit)
			{//当放入此节点不超过限制重量时，直接选择加入此节点
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
			{//加入新节点会超重，只能选不放入
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
		compare_Node2(noding, NumOfNode);	//重新排列节点队列，保证队列第一个节点一定是上界最大的节点
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