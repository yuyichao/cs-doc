#include<stdio.h>
#include<malloc.h>

#define LEN  sizeof(struct area)
#define LEN_POINTER_LIST sizeof(struct AreaPointer_list)

struct area{
	int start;	//分区的其始地址
	int length;	//分区的长度
	int job;	//若被作业占用值为作业号，若空闲值为0
	struct area * next;
};

//分区指针的链表
//当把空闲分区链表和占用分区链表按照地址先后顺序合并
//以显示整个内存情况的时候使用
struct AreaPointer_list{
	struct area * data;
	struct AreaPointer_list * next;
};

struct area * idle;						//全局变量，空闲分区链表头指针
struct area * used;						//全局变量，占用分区链表头指针
struct AreaPointer_list * whole = NULL;	//全局变量，分区指针链表头指针










//p(previcious) n(next)指出在链表中的何处插入新生成的元素
//p==NULL  在链表头插入，返回头指针
//p!=NULL  在链表中或链表尾插入，返回当前插入的元素的指针
struct area * insert(int s,int l,int j,struct area * p,struct area * n)
{
	struct area * current = (struct area * )malloc(LEN);
	current->start = s;
	current->length = l;
	current->job = j;
	if(p == NULL){//在链表头插入
		current->next = n;
		return current;
	}
	else{
		if(p->next == NULL){//在链表尾插入
			current->next = NULL;
			p->next = current;
		}
		else{//在链表中插入
			current->next = p->next;
			p->next = current;
		}
		return current;
	}
}
//此模块居于次要地位，只被使用一次










//打印分区链表
void print(struct area * head)
{
	if(head == NULL){
		printf("Area list is null...\n");
	}
	else{
		while(head != NULL)
		{
			if(head->job == 0)
				printf("begin:%dK\tlength:%dK\t空闲\t\t|\n",head->start,head->length);
			else
				printf("begin:%dK\tlength:%dK\tuse:Job%d\t|\n",head->start,head->length,head->job);
			head = head->next;
		}
	}
}
void file_print(struct area * head,FILE * file)
{
	if(head == NULL){
		fprintf(file,"Area list is null...\n");
	}
	else{
		while(head != NULL)
		{
			if(head->job == 0)
				fprintf(file,"begin:%dK\tlength:%dK\t空闲\t\t|\n",head->start,head->length);
			else
				fprintf(file,"begin:%dK\tlength:%dK\tuse:Job%d\t|\n",head->start,head->length,head->job);
			head = head->next;
		}
	}
}
//打印分区链表


//释放分区链表空间
void free_AreaList(struct area * head)
{
	struct area * temp;
	while(head != NULL){
		temp = head;
		head = head->next;
		free(temp);
	}
}
//释放分区链表空间


//在分区链表中搜索插入位置
//flag==0 表明分区链表按起始地址从小到大排列
//flag==1 表明分区链表按分区长度从小到大排列
//输入参数 element 不能为NULL
struct area * search_pos(struct area * element,struct area * head,int flag)
{
	struct area * p = NULL;
	while(head != NULL){
		if(flag == 0)
		{
			if(element->start < head->start)
				break;
		}
		else
		{
			if(element->length < head->length)
				break;
		}
		p = head;
		head = head->next;
	}
	return p;
}
//返回值p==NULL表明插入位置在链表头
//返回值p!=NULL表明插入位置在p 之后


//进行分区链表的实际插入工作
//flag==0 表明分区链表按起始地址从小到大排列
//flag==1 表明分区链表按分区长度从小到大排列
//输入参数 element->next 要为NULL
struct area * insert_list(struct area * element,struct area * list,int flag)
{
	if(list == NULL)
		list = element;
	else{
		struct area * pos = search_pos(element,list,flag);
		if(pos == NULL){
			element->next = list;
			list = element;
		}
		else{
			element->next = pos->next;
			pos->next = element;
		}
	}
	return list;
}//返回插入元素之后新链表的头指针


//进行查询空闲分区链表动态分配分区的实际工作，算法步骤：
//1。查询空闲分区链表中是否有长度大于或等于申请长度的分区，若没有返回FALSE
//2。若查找到符合条件的分区，把它从空闲链表中取出
//3。根据请求把取出的空闲分区分块，把新的占用分区和剩余空闲分区分别插入链表
//注意：插入占用分区链表按照固定的地址先后顺序，插入空闲分区链表的方式要根据flag的值
int memory_alloc(int length,int job,int flag)
{
	struct area * used_element;
	struct area * free_element;

	struct area * head = idle;
	struct area * head_temp = used;
	struct area * p = NULL;


	//检测输入的作业号是否存在
	while(head_temp != NULL)
	{
		if(head_temp->job == job)
			return 2;
		head_temp = head_temp->next;
	}

	//在空闲分区链表中查找
	while(head != NULL)
	{
		if(head->length >= length)
			break;
		p = head;
		head = head->next;
	}

	if(head != NULL)
	{
		//从空闲区链表中取出
		if(p == NULL)//链表中的第一个分区符合条件
		{
			idle = idle->next;
		}
		else
		{
			p->next = head->next;
		}
		head->next = NULL;
	}
	else return 0;

	//生成新的占用区链表元素并插入占用区链表
	used_element = (struct area * )malloc(LEN);
	used_element->start = head->start;
	used_element->length = length;
	used_element->job = job;
	used_element->next = NULL;

	used = insert_list(used_element,used,0);


	//若空闲分区分块后有剩余，生成新的空闲区链表元素并插入空闲区链表
	if(head->length > length){
		free_element = (struct area * )malloc(LEN);
		free_element->start = head->start + length;
		free_element->length = head->length - length;
		free_element->job = 0;
		free_element->next = NULL;

		idle = insert_list(free_element,idle,flag);
	}
	
	//释放空间
	free(head);

	return 1;
}


//进行查询占用分区链表动态释放分区的实际工作，算法步骤：
//1。根据作业号查询到占用分区链表中要释放的分区，若没有返回FALSE
//2。若查找到要释放的分区，把它从空闲链表中取出
//3。根据取出的分区的数据建立新的空闲分区
//4。在空闲分区链表中查询是否有和新空闲分区相邻的空闲分区，有则合并
//5。根据flag的取值按照特定方式插入空闲分区链表
int memory_free(int job,int flag)
{
	struct area * used_element;
	struct area * free_element;
	struct area * head = used;
	struct area * p = NULL;

	struct area * previcious1 = NULL;
	struct area * current1 = NULL;
	struct area * previcious2 = NULL;
	struct area * current2 = NULL;

	//根据作业号在占用分区链表中查找
	while(head != NULL)
	{
		if(head->job == job)
			break;
		p = head;
		head = head->next;
	}

	if(head != NULL)
	{
		//从占用区链表中取出
		if(p == NULL)//链表中的第一个分区符合条件
		{
			used = used->next;
		}
		else
		{
			p->next = head->next;
		}
		head->next = NULL;
	}
	else return 0;

	//建立新的空闲分区
	used_element = head;
	free_element = (struct area * )malloc(LEN);
	free_element->start = used_element->start;
	free_element->length = used_element->length;
	free_element->job = 0;
	free_element->next = NULL;

	
	//从空闲区链表查找和新的空闲分区相邻分区
	head = idle;
	p = NULL;

	while(head != NULL)
	{
		if(head->start + head->length == used_element->start)
		{
			previcious1 = p;
			current1 = head;
		}
		if(used_element->start + used_element->length == head->start)
		{
			previcious2 = p;
			current2 = head;
		}
		p = head;
		head = head->next;
	}
	

	//合并相邻空闲分区
	if( current1 != NULL )
	{
		//把和新分区相邻的分区从空闲分区链表中取出
		if( previcious1 == NULL )
			idle = idle->next;
		else
			previcious1->next = current1->next;
		current1->next = NULL;

		//修改新空闲分区的相关数据
		free_element->start = current1->start;
		free_element->length = free_element->length + current1->length;
	}
	if( current2 != NULL )
	{
		//把和新分区相邻的分区从空闲分区链表中取出
		if( previcious2 == NULL )
			idle = idle->next;
		else
			previcious2->next = current2->next;
		current2->next = NULL;

		//修改新空闲分区的相关数据
		free_element->length = free_element->length + current2->length;
	}

	//根据flag的取值按照特定方式插入空闲分区链表
	idle = insert_list(free_element,idle,flag);

	//释放空间
	free(used_element);

	return 1;
}










//和分区指针链表相关的操作，用来合并空闲分区链表和占用分区链表，保存链表元素的指针
struct AreaPointer_list * search_position(int s,struct AreaPointer_list * head)
{
	struct AreaPointer_list * p = NULL;
	while(head != NULL){
		if(s < (head->data)->start)
			break;
		p = head;
		head = head->next;
	}
	return p;
}
struct AreaPointer_list * emerge(struct area * idle_temp,struct area * used_temp)
{
	struct AreaPointer_list * previcious;
	struct AreaPointer_list * temp;
	
	if(used_temp != NULL)
	{
		whole = (struct AreaPointer_list *)malloc(LEN_POINTER_LIST);
		whole->data = used_temp;
		whole->next = NULL;
		previcious = whole;
		used_temp = used_temp->next;
		while(used_temp != NULL){
			temp = (struct AreaPointer_list *)malloc(LEN_POINTER_LIST);
			temp->data = used_temp;
			temp->next = NULL;
			previcious->next = temp;
			previcious = temp;

			used_temp = used_temp->next;
		}
	}

	while(idle_temp != NULL){
		struct area * idle_next = idle_temp->next;
		struct AreaPointer_list * pos = search_position(idle_temp->start,whole);
		
		if(pos == NULL)
		{
			temp = (struct AreaPointer_list *)malloc(LEN_POINTER_LIST);
			temp->data = idle_temp;
			temp->next = whole;

			whole = temp;
		}
		else
		{
			temp = (struct AreaPointer_list *)malloc(LEN_POINTER_LIST);
			temp->data = idle_temp;
			temp->next = pos->next;

			pos->next = temp;
		}
		
		idle_temp = idle_next;
	}

	return whole;
}
void printall(struct AreaPointer_list * head)
{
	struct area * data_temp;
	if(head == NULL)
		printf("Area pointer list is null...\n");
	else{
		while(head != NULL)
		{
			data_temp = head->data;
			if(data_temp->job == 0)
				printf("begin:%dK\tlength:%dK\t空闲\t\t|\n",data_temp->start,data_temp->length);
			else
				printf("begin:%dK\tlength:%dK\tuse:Job%d\t|\n",data_temp->start,data_temp->length,data_temp->job);
			
			head = head->next;
		}
	}
}
void file_printall(struct AreaPointer_list * head,FILE * file)
{
	struct area * data_temp;
	if(head == NULL)
		fprintf(file,"Area pointer list is null...\n");
	else{
		while(head != NULL)
		{
			data_temp = head->data;
			if(data_temp->job == 0)
				fprintf(file,"begin:%dK\tlength:%dK\t空闲\t\t|\n",data_temp->start,data_temp->length);
			else
				fprintf(file,"begin:%dK\tlength:%dK\tuse:Job%d\t|\n",data_temp->start,data_temp->length,data_temp->job);
			
			head = head->next;
		}
	}
}
void free_PointerList(struct AreaPointer_list * head)
{
	struct AreaPointer_list * temp;
	while(head != NULL){
		temp = head;
		head = head->next;
		free(temp);
	}
}
//和分区指针链表相关的操作，用来合并空闲分区链表和占用分区链表，保存链表元素的指针










void input_by_hand()
{
	int job;
	int is_alloc;//1 申请分区 0 释放分区
	int length;
	int flag;

	printf("请选择分区分配算法：输入0---最先适配 输入1---最优适配\n");
	scanf("%d",&flag);
	while(flag != 0 && flag != 1)
	{
		printf("数据输入错误，请参照提示重新输入\n");
		scanf("%d",&flag);
	}
	if(flag == 0)
		printf("选择最先适配算法--->请输入请求队列数据：(输入 0 0 0 结束)\n");
	if(flag == 1)
		printf("选择最优适配算法--->请输入请求队列数据：(输入 0 0 0 结束)\n");
	
	printf("输入数据格式：作业号(int>0) [输入1--申请|输入0--释放] 分区长度(int>0)\n");
	printf("例如输入 5 1 130 表示 作业5申请130K\n");
	printf("例如输入 3 0 200 表示 作业3释放200K\n");

	while(1)//输入 0 0 0 结束
	{
		scanf("%d%d%d",&job,&is_alloc,&length);
		if(job == 0 && is_alloc == 0 && length == 0)
			break;
		while(job<=0 || (is_alloc != 0 && is_alloc != 1) || length<=0)
		{
			printf("数据输入错误，请参照提示重新输入\n");
			scanf("%d%d%d",&job,&is_alloc,&length);
			if(job == 0 && is_alloc == 0 && length == 0)
				return;
		}
		

		if(is_alloc == 1)
		{
			int r = memory_alloc(length,job,flag);
			if(!r)
			{
				printf("\n");
				printf("没有符合条件的空闲分区可供分配，请等待释放...\n");
				printf("\n");
				continue;
			}
			if(r == 2)
			{
				printf("\n");
				printf("输入作业号已存在于占用分区链表，请重新输入...\n");
				printf("\n");
				continue;
			}
		}
		if(is_alloc == 0)
		{
			int r = memory_free(job,flag);
			if(!r)
			{
				printf("\n");
				printf("没有与指定作业号符合的占用分区，请重新输入...\n");
				printf("\n");
				continue;
			}
		}

		emerge(idle,used);
		
		printf("\n");
		printf("-------------------------------------------------\n");
		printf("空闲分区链表：\t\t\t\t\t|\n");
		print(idle);
		printf("\t\t\t\t\t\t|\n");

		printf("占用分区链表：\t\t\t\t\t|\n");
		print(used);
		printf("\t\t\t\t\t\t|\n");

		printf("整个内存情况：\t\t\t\t\t|\n");
		printf("低地址\t\t\t\t\t\t|\n");
		printall(whole);
		printf("高地址\t\t\t\t\t\t|\n");
		printf("-------------------------------------------------\n");
		printf("\n");
		free_PointerList(whole);
		whole = NULL;
	}

	//释放空间
	free_AreaList(idle);
	free_AreaList(used);
	idle = NULL;
	used = NULL;
}

void input_by_file(int flag)
{
	int job;
	int is_alloc;//1 申请分区 0 释放分区
	int length;
	char* result;
	int r;

	FILE * file1;
	FILE * file2;

	if(flag == 0)
		result = "result_data_1.txt";
	else
		result = "result_data_2.txt";

	if((file1 = fopen("source_data.txt","r")) == NULL)
	{
		printf("不能打开source_data.txt文件...\n");
		exit(0);
	}
	if((file2 = fopen(result,"w")) == NULL)
	{
		printf("不能打开source_data.txt文件...\n");
		exit(0);
	}

	if(flag == 0)
	{
		printf("按照最先分配算法得出的结果：\n\n");
		fprintf(file2,"按照最先分配算法得出的结果：\n\n");
	}
	else
	{
		printf("按照最优分配算法得出的结果：\n\n");
		fprintf(file2,"按照最优分配算法得出的结果：\n\n");
	}


	while(!feof(file1)){
		fscanf(file1,"%d%d%d",&job,&is_alloc,&length);

		if(job<=0 || (is_alloc != 0 && is_alloc != 1) || length<=0)
		{
			printf("文件中数据%d %d %d输入的格式错误，不于处理\n\n",job,is_alloc,length);
			fprintf(file2,"文件中数据%d %d %d输入的格式错误，不于处理\n\n",job,is_alloc,length);
			continue;
		}

		if(is_alloc == 1)
		{
			printf("JOB %d申请%dK\n\n",job,length);
			fprintf(file2,"JOB %d申请%dK\n\n",job,length);

			r = memory_alloc(length,job,flag);
			if(!r)
			{
				printf("没有符合条件的空闲分区可供分配，不于处理\n\n");
				fprintf(file2,"没有符合条件的空闲分区可供分配，不于处理\n\n");
				continue;
			}
			if(r == 2)
			{
				printf("输入作业号已存在于占用分区链表，不于处理\n\n");
				fprintf(file2,"输入作业号已存在于占用分区链表，不于处理\n\n");
				continue;
			}
		}
		else
		{
			printf("JOB %d释放%dK\n\n",job,length);
			fprintf(file2,"JOB %d释放%dK\n\n",job,length);

			r = memory_free(job,flag);
			if(!r)
			{
				printf("没有与指定作业号符合的占用分区，不于处理\n\n");
				fprintf(file2,"没有与指定作业号符合的占用分区，不于处理\n\n");
				continue;
			}
		}

		emerge(idle,used);
		
		printf("-------------------------------------------------\n");
		fprintf(file2,"-------------------------------------------------\n");
		printf("空闲分区链表：\t\t\t\t\t|\n");
		fprintf(file2,"空闲分区链表：\t\t\t\t\t|\n");
		print(idle);
		file_print(idle,file2);
		printf("\t\t\t\t\t\t|\n");
		fprintf(file2,"\t\t\t\t\t\t|\n");

		printf("占用分区链表：\t\t\t\t\t|\n");
		fprintf(file2,"占用分区链表：\t\t\t\t\t|\n");
		print(used);
		file_print(used,file2);
		printf("\t\t\t\t\t\t|\n");
		fprintf(file2,"\t\t\t\t\t\t|\n");

		printf("整个内存情况：\t\t\t\t\t|\n");
		fprintf(file2,"整个内存情况：\t\t\t\t\t|\n");
		printf("低地址\t\t\t\t\t\t|\n");
		fprintf(file2,"低地址\t\t\t\t\t\t|\n");
		printall(whole);
		file_printall(whole,file2);
		printf("高地址\t\t\t\t\t\t|\n");
		fprintf(file2,"高地址\t\t\t\t\t\t|\n");
		printf("-------------------------------------------------\n");
		fprintf(file2,"-------------------------------------------------\n");
		printf("\n");
		fprintf(file2,"\n");
		free_PointerList(whole);
		whole = NULL;
	}

	printf("========================================\n\n");
	fprintf(file2,"========================================\n\n");

	//释放空间
	free_AreaList(idle);
	free_AreaList(used);
	idle = NULL;
	used = NULL;

	fclose(file1);
	fclose(file2);
}

int main(){
	
	int method;

	idle = insert(0,640,0,NULL,NULL);
	used = NULL;

	printf("请选择测试方式：输入0---手工输入源数据 输入1---文件输入源数据\n");
	printf("手工输入：手工在控制台console逐条输入请求数据，结果显示在控制台console\n");
	printf("文件输入：从文件source_data.txt中取出请求数据，结果放在result_data.txt\n");
	scanf("%d",&method);
	while(method != 0 && method != 1)
	{
		printf("数据输入错误，请参照提示重新输入\n");
		scanf("%d",&method);
	}

	if( method == 0)
		input_by_hand();
	else
	{
		input_by_file(0);

		idle = insert(0,640,0,NULL,NULL);
		used = NULL;

		input_by_file(1);
	}
}
