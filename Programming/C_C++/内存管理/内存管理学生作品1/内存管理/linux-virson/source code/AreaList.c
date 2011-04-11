#include "AreaList.h"
#include "AreaPointerList.h"


#define LEN  sizeof(struct area)


extern struct area * idle;						//全局变量，空闲分区链表头指针
extern struct area * used;						//全局变量，占用分区链表头指针
extern struct AreaPointer_list * whole;			//全局变量，分区指针链表头指针


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
