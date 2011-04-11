//FileName : getenv_bug.idc
//Demo for find bug.

#include <idc.idc>

static main()
{

	auto addr,faddr1,faddr2,faddr3,faddr4,end,i,j,tmp;
	auto malloc_num,N_LEN,F_LEN,ENV_LEN;
	N_LEN = 50;
	F_LEN = 30;

	malloc_num=0;

	addr=BeginEA();
	//addr=ChooseFunction("main");
	j=2;
	if(addr == BADADDR)
		return;
	MarkPosition(addr,addr,0,100,1,"main");
	
	end=SegEnd(addr);


	if(end == BADADDR)
		return;
	i=addr;
	while(i<end)
	{
		faddr1=FindText(i,1,0,100,"getenv");
		if(faddr1 != BADADDR && faddr1 < end)
		{
			faddr2=FindText(faddr1,1,0,100,"strcpy");
			if(faddr2 !=BADADDR && faddr2 < end && faddr2 - faddr1 < N_LEN *4 )
			{
				faddr3=FindText(faddr1,1,0,100,"strlen");
				tmp = faddr3 - faddr1;
				if(tmp <0)
					tmp = 0-tmp;

				if(faddr3 == BADADDR || tmp > F_LEN * 4 ) 
				{
					i = faddr2;
				
				}
				
				
			}
			else 
			{
				faddr2=FindText(faddr1,1,0,100,"sprintf");
				if(faddr2 !=BADADDR && faddr2 < end && faddr2 - faddr1 < N_LEN *4 )
				{
					faddr3=FindText(faddr1 - F_LEN *4 ,1,0,100,"strlen");
					tmp = faddr3 - faddr1;
					if(faddr3 == BADADDR || tmp > F_LEN * 4 ) 
					{
						i = faddr2;
				
					}
				}
			}
			
			if( i == faddr2 ) //显示找到的信息并标识该位置 
			{
				Message("---0x%x : getenv sprintf/strcpy\n",faddr1);
				if(j<20)
					MarkPosition(faddr1,faddr1,0,100,j,"genv str..");
				malloc_num++;
				j=j+1;
			}
			if(i != faddr2)
				i=faddr1+4;
	
		}
		else
			break; //没有找到getenv
		
		
	}
	Message("--END--\n");

}