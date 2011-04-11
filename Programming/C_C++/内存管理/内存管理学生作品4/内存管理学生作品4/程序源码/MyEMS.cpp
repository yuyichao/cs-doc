// MyEMS.cpp: implementation of the MyEMS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EMSDSD.h"
#include "MyEMS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyEMS::MyEMS()
{

}

MyEMS::~MyEMS()
{
}

void MyEMS::Init(long totleEMS)
{
	num = 0;
	EMST = new struct EMSTable();
	EMST->start = 0;
	EMST->end = EMST->size = totleEMS;
	EMST->isFree = true;
	EMST->next = NULL;
	EMST->before = NULL;
	worklist = wlend = new struct WorkList();
	wlend->num = 0;
	wlend->position = NULL;
	wlend->size = 0;
	wlend->next = NULL;
	wlend->before = worklist;
}

bool MyEMS::Distribute(int arithmetic, long spaceneed)
{
	if(arithmetic == 1)
	{
		struct EMSTable * temp1 = EMST, * temp2;
		for(; temp1 != NULL; temp1 = temp1->next)
		{
			if(temp1->isFree && temp1->size >= spaceneed)
			{
				if(temp1->size != spaceneed)
				{
					temp2 = new struct EMSTable();
					temp2->start = temp1->start + spaceneed;
					temp2->end = temp1->end;
					temp2->next = temp1->next;
					temp2->before = temp1;
					temp2->size = temp1->size - spaceneed;
					temp1->next = temp2;
				}
				temp1->end = temp1->start + spaceneed - 1;
				temp1->isFree = false;
				temp1->size = spaceneed;
				num++;
				wlend->num = num;
				wlend->position = temp1;
				wlend->size = temp1->size;
				wlend->next = new struct WorkList();
				wlend->next->before=wlend;
				wlend = wlend->next;
				wlend->num = 0;
				wlend->position = NULL;
				wlend->size = 0;
				wlend->next = NULL;
				return true;
			}
		}
	}
	if(arithmetic == 2)
	{
		struct EMSTable * temp1, * temp2 = EMST;
		int n = 50000;
		for(; temp2 != NULL; temp2 = temp2->next)
		{
			if(temp2->isFree && temp2->size >= spaceneed)
			{
				if(temp2->size < n)
				{
					n = temp2->size;
					temp1 = temp2;
				}
			}
		}
		if(n < 50000)
		{
			if(temp1->size != spaceneed)
			{
				temp2 = new struct EMSTable();
				temp2->start = temp1->start + spaceneed;
				temp2->end = temp1->end;
				temp2->next = temp1->next;
				temp2->before = temp1;
				temp2->size = temp1->size - spaceneed;
				temp1->next = temp2;
			}
			temp1->end = temp1->start + spaceneed - 1;
			temp1->isFree = false;
			temp1->size = spaceneed;
			num++;
			wlend->num = num;
			wlend->position = temp1;
			wlend->size = temp1->size;
			wlend->next = new struct WorkList();
			wlend->next->before=wlend;
			wlend = wlend->next;
			wlend->num = 0;
			wlend->position = NULL;
			wlend->size = 0;
			wlend->next = NULL;
			return true;
		}
	}
	return false;
}
