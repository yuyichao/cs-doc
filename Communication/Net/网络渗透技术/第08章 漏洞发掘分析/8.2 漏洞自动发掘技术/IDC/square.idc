/**
文件名： square.idc
用途  ： 通过分析程序上下文来追踪数据的来历.
说明  ： 针对HP-UX PA指令。
编写  ： watercloud
日期  ： 2003-2-27
**/


//#include <idc.idc>

/**
函数： success createFunction(long)
说明： 如果指定地址处没有定义函数则试图在该处定义一个函数。错误返回0
**/
static createFunction(_addr)
{
	auto addr,funcBegin,funcEnd;
	addr=_addr;

	if(GetFunctionName(addr)=="" )
	{
		while(SegName(addr)=="$CODE$" && GetFunctionName(addr)=="" && !(GetMnem(addr) == "stw" && GetOpnd(addr,0)=="%rp") )
		{
			addr=addr-4;
		}
		//Message("funcBegin :%x Name:%s\n",addr,GetFunctionName(addr));
		if(SegName(addr) == "$CODE$" && GetFunctionName(addr)=="")
		{
			//Message("funcBegin :%x\n",addr);
			funcBegin = addr;
			funcEnd=FindFuncEnd(funcBegin);
			if(funcEnd !=BADADDR)
			{
				return MakeFunction(funcBegin,funcEnd);
			}
		}
	}
	return 0;
}
/**
函数： long getFuncBeginEA(long)
返回： 指定地址所在函数的函数起始地址，出错返回 BADADDR 。
**/
static getFuncBeginEA(_addr)
{
	auto addr_begin,name_and_off,point_offset,sOff,xOff;
	name_and_off = GetFuncOffset(_addr);
	if(name_and_off == "")
		return BADADDR;
	point_offset = strstr(name_and_off,"+");
	if(point_offset == -1)
		return BADADDR;
	sOff = substr(name_and_off,point_offset+1,-1);
	xOff = xtol(sOff);
	return _addr - xOff;
	
}

/**
函数： string getRegister(string opn)
返回： 寄存器名称，出错返回 ""
说明： 从如下：im+var_im(%s,%r) /im+var_im(%r) / im(%sr,%r) / im(%r) /%r 格式字符串中取得寄存器%r。
**/
static getRegister(opn)
{
	auto str,index1,index2;

	index1 = strstr(opn,"%");
	if(index1 == -1)
		return "";
	str = substr(opn,index1+1,-1);
	index2 = strstr(str,"%");
	if(index2 != -1)
		return substr(str,index2,strstr(str,")"));
	else
		return substr(opn,index1,strstr(opn,")"));
}

/**
宏：getRegDP() :  取得程序初始化时寄存器%dp的值,错误返回 BADADDR
**/
#define getRegDP()   SegEnd(getSegAddr("$GLOBAL$"))

/**
函数：long getSegAddr(string _name)
说明：返回指定名称段的起始地址,错误返回 BADADDR
**/
static getSegAddr(_name)
{
	auto tmpAddr;
	tmpAddr   = FirstSeg();
	while( tmpAddr != BADADDR)
	{
		if( SegName(tmpAddr) == _name )
			return SegStart(tmpAddr);
		tmpAddr = NextSeg(tmpAddr);
	}
	return BADADDR;
}

/**
函数：string getString(long _addr)
用途：从指定地址处试图得到一个字符串，如果需要在该地址创建一个String。
**/
static getString(_addr)
{
     auto  tmpStr, chr,addr,flag;
     tmpStr= "";
	 addr = _addr;

     chr = Byte(addr);
     while( chr != 0 && chr != 0xFF )
     {
          tmpStr = form("%s%c", tmpStr, chr);
          addr ++;
          chr = Byte(addr);
     }

	 if( addr - _addr >1 )
	 {
		 if( hasName(GetFlags(_addr) ) == 0 && SegName(_addr) != "$CODE$" )
		 {
			 MakeStr(_addr,addr);
		 }
	 }

     return(tmpStr);
}

/**
函数：long getValue(long _addr,long _n)
说明：取得指定地址处指令的第_n个操作数的值
返回：情形：ldo 0x44,%r 时返回0x44 / copy name,%r时返回名字name对应的真实值 /  -0x140 + var_40(%sr0,%sp)返回 -0x100
      错误返回"ERR"。
**/
static getValue(_addr,_n)
{
	auto value,tmp,type,subType,flag;

	flag=GetFlags(_addr);
	type=GetOpType(_addr,_n);
	tmp=GetOpnd(_addr,_n);	

	if(type == 5) //立即数 
	{
		value = LocByName(tmp);
		if(value == BADADDR)
			return xtol(tmp);
		else return value;
	}
	else if(type == 7) //操作数为名称
	{
		tmp = LocByName(tmp);
		if(tmp == BADADDR)
			return "ERR";
		else
			return tmp;
	}
	else if(type == 4) //基址+段寻址方式   
	{
		if(_n == 0)
			subType = isStkvar0(flag); //取第一个操作数类型
		else if(_n == 1)
			subType = isStkvar1(flag);//取第二个操作数类型
		else
			return "ERR";   //系统只提供了处理两个操作数的能力，当处理更多的操作数出错。

		if(subType == 0) //0x50C(%sr0,%r1)
		{
			return xtol(tmp);
		}
		else if(subType == 1)  //arg_18(%sr0,%sp) 或-0x180+var_20(%sr0,%sp)形势
		{
			OpHex(_addr,_n); //将其转换为：0x999(%r)形势 
			tmp = GetOpnd(_addr,_n); 
			OpStkvar(_addr,_n);
			return xtol(tmp);
		}

	}

	return "ERR";
}

/*
宏定义，用于指定函数getDataResEx的中止状态。
ERR: 出错，不能处理; STA: 遇到栈变量; RET: 遇到函数返回值；IMP: 遇到还未实现的指令。
ARG: 依赖函数被调用时传入的参数。 NOF: 没有找到需要的寄存器。
D_MSG宏用于处理打印调试信息。
*/
#define F_ERR(X,V)  return form("ERR %x %x",X,V)
#define F_STA(X,V)  return form("STA %x %x",X,V)
#define F_RET(X,V)  return form("RET %x %x",X,V)
#define F_IMP(X,V)  return form("IMP %x %x",X,V)
#define F_ARG(R,V)  return form("ARG %s %x",R,V)
#define F_NOF(R,V)  return form("NOF %s %x",R,V)
#define D_MSG(F,X,O,R) if(F>=5) Message("  %x : %s -> %s\n",X,O,R)

#define getDRStr0(S)  substr(S,0,3)
#define getDRType(S)  substr(S,0,3)
#define getDRStr1(S)  substr(S,4,4+strstr(substr(S,4,-1)," ") )
#define getDRReg(S)  substr(S,4,4+strstr(substr(S,4,-1)," ") )
#define getDRStr0_1(S) substr(S,0,4+strstr(substr(S,4,-1)," "))

static  isDRValue(_string)
{
	auto  f0;
	f0=getDRStr0(_string);
	return !(f0=="ERR" || f0=="STA" || f0=="IMP" || f0=="RET" || f0=="NOF" || f0=="ARG");
}


#define  PREVENT_UN_LOCK_CLICK 1000

/**
函数： string getDataResEx(long,string,long)
说明： 取得数据来源 :函数参数、数据区、BSS区、立即数
**/
static getDataResEx(_addr,_reg,_debugFlag)
{
	auto addr,type,subType,reg,tmp,tmpReg,tmpStr,tmpValue,tmpAddr,opcode;
	auto REG,VALUE,DEBUG_FLAG,REGDP,FUNC_BEGIN,FUNC_END,CLICK,FUNC_HEAD_REPEAT,FUNC_HEAD_REPEAT_FLAG;
	
	FUNC_HEAD_REPEAT=30;    //如果出现地址跳转错误，并试图在函数开始处找到该地址。
	DEBUG_FLAG=_debugFlag;
	addr = _addr+4;
	REG =  _reg;
	VALUE = 0;
	
	REGDP = getRegDP();
	if(REGDP == BADADDR)
	{
		if(DEBUG_FLAG >=3)
			Message("ERROR: getDataResEx -> getRegDP() FALSE!\n");
		F_ERR(0,addr);
	}


	tmpStr = GetFunctionName(_addr);
	if(tmpStr == "") //如果该处还没有函数，则试图定义一个函数
	{
		if(createFunction(_addr) == 0)
		{
			if(DEBUG_FLAG >=3)
				Message("ERROR: getDataResEx -> createFunction(%x) == FALSE\n",_addr);
			F_ERR(1,addr);
		}
		if(DEBUG_FLAG >=10)
			Message("Message : getDataResEx -> GetFunctionName(%x) = \"%s\"\n",_addr,tmpStr);
	}

	FUNC_BEGIN = getFuncBeginEA(_addr);
	FUNC_END   = FindFuncEnd(FUNC_BEGIN);
	if(FUNC_BEGIN == BADADDR || FUNC_END == BADADDR || FUNC_END <= FUNC_BEGIN)
	{
		if(DEBUG_FLAG >=3)
			Message("ERROR: getDataResEx -> getFuncBeginEA() || FindFuncEnd FALSE!\n");
		F_ERR(2,-1);
	}


	if(DEBUG_FLAG >= 10)
		Message("Trace into getDataResEx : %x %s\n",_addr,_reg);

	CLICK = 0; //防止死循环
	FUNC_HEAD_REPEAT_FLAG = 0;
	while(addr > FUNC_BEGIN && addr < FUNC_END && CLICK < PREVENT_UN_LOCK_CLICK )
	{
	//	addr=RfirstB(addr);
		addr=addr-4;
		CLICK ++; //防止死循环

		opcode = GetMnem(addr);

		//如果最终无法找到且查找对象为常见数据段使用的寄存器那么从函数头部一定范围内再给一次搜索机会。
		if(FUNC_HEAD_REPEAT_FLAG==0 && (addr <= FUNC_BEGIN  || addr >=FUNC_END || opcode == "" ) )
		{
			if(REG=="%r1"||REG=="%r3"||REG=="%r4"||REG=="%r5"||REG=="%r6"||REG=="%r7"||REG=="%r8"||REG=="%r9")
			{
				addr = FUNC_BEGIN + FUNC_HEAD_REPEAT * 4;
				FUNC_HEAD_REPEAT_FLAG = 1;
				continue;
			}
		}

		if(opcode == "")
			F_ERR(addr,VALUE);	


		if(DEBUG_FLAG >=10)
			Message("%x: %s %s %s %s\n",addr,opcode,GetOpnd(addr,0),GetOpnd(addr,1),GetOpnd(addr,2) );

		if(opcode == "addil" && GetOpnd(addr,2) == REG ) //addil  0x4567,%r,%r
		{
			D_MSG(DEBUG_FLAG,addr,"addil",REG);
			tmp = GetOpnd(addr,1);
			if(tmp == "%dp")
				return VALUE + getValue(addr,0) + REGDP;
			if(tmp == "%r0")
				return VALUE + getValue(addr,0);
			if(tmp == "%sp")
				F_STA(addr,VALUE);
			if(tmp == "%r28")
				F_RET(addr,VALUE);
			
			REG = GetOpnd(addr,1);
			VALUE = VALUE + getValue(addr,0);
		}
		else if(opcode == "copy" && GetOpnd(addr,1) == REG ) // copy 0,%r  /   copy %r,%r
		{
			D_MSG(DEBUG_FLAG,addr,"copy",REG);

			type=GetOpType(addr,0);

			if(type == 1)
			{
				tmp=GetOpnd(addr,0);

				if(tmp == "%dp")
					return VALUE + REGDP;
				if(tmp == "%r0")
					return VALUE ;
				if(tmp == "%sp")
					F_STA(addr,VALUE);
				if(tmp == "%r28")
					F_RET(addr,VALUE);

				REG = GetOpnd(addr,0);
			}
			else if(type == 5 || type == 7)
			{
				return VALUE + getValue(addr,0);
			}
			else 
				F_ERR(addr,VALUE);

		}
		else if(opcode == "ldil" && GetOpnd(addr,1) == REG ) //ldil 0x55,%r
		{
			D_MSG(DEBUG_FLAG,addr,"ldil",REG);
			return VALUE+getValue(addr,0);
		}
		else if(opcode == "ldi" && GetOpnd(addr,1) == REG ) //ldil 0x55,%r
		{
			D_MSG(DEBUG_FLAG,addr,"ldi",REG);
			return VALUE+getValue(addr,0);
		}
		else if(opcode == "ldo"  && GetOpnd(addr,1) == REG) //ldo 0x456(%r),%r  / ldo  -0xc0 +var_23(%sp),%r
		{
			D_MSG(DEBUG_FLAG,addr,"ldo",REG);
			if(GetOpType(addr,0) !=4) //非相对寻址，出错
				F_ERR(addr,VALUE);

			tmpStr = GetOpnd(addr,0);
			REG = getRegister(tmpStr);
			if(REG == "")
				F_ERR(addr,VALUE);

			if(REG == "%dp")
				return VALUE + REGDP;
			if(REG == "%r0")
				return VALUE + getValue(addr,0);
			if(REG == "%sp")
				F_STA(addr,VALUE);
			if(REG == "%r28")
				F_RET(addr,VALUE);

			VALUE = VALUE + getValue(addr,0);
		}
		else if(opcode == "or"  && GetOpnd(addr,2) == REG) //目前只能处理or的几种特殊情形
		{
			auto type1,type2;
			auto str1,str2;
			D_MSG(DEBUG_FLAG,addr,"or",REG);

			type1=GetOpType(addr,0);
			type2=GetOpType(addr,1);

			str1 = GetOpnd(addr,0);
			str2 = GetOpnd(addr,1);

			if( (type1==5 || type1==7) && (type2==5 || type2==7) ) //or 0x45,0x23 ,%r
			{
				VALUE = VALUE + (getValue(addr,0) | getValue(addr,1));
			}
			else if(type1 == 1 && (str2=="0" || str2=="%r0") ) // or %r,0,%r  / or %r,%r0,%r
			{
				if(str1 == "%dp")
					return VALUE + REGDP;
				if(str1 == "%r0")
					return VALUE ;
				if(str1 == "%sp")
					F_STA(addr,VALUE);
				if(str1 == "%r28")
					F_RET(addr,VALUE);

				REG = str1;  //如果引用的是其他积存器则继续回溯
			}
			else
				F_ERR(addr,VALUE);  //其他情形出错


		}
		else if(opcode == "ldw"  && GetOpnd(addr,1) == REG) //ldw %r15(%r23),%r3 / ldw 4(%r1),%r2 / ldw -0x44+var_8(%sp,%r3)
		{
			D_MSG(DEBUG_FLAG,addr,"ldw",REG);
			type = GetOpType(addr,0);
			if(type != 4 ) //目前只能处理基址寻址
				F_ERR(addr,VALUE);

			tmpReg = getRegister(GetOpnd(addr,0));
			if(tmpReg == "")
				F_ERR(addr,VALUE);

			tmp = getValue(addr,0);

			if(tmpReg == "%dp") //ldw -0x4882(%dp),%r26
			{
				if(tmp == "ERR")
					F_ERR(addr,VALUE);
				tmpAddr = tmp + REGDP;
				return VALUE + Dword(tmpAddr);
			}
			if(tmpReg != "%sp" && tmp != "ERR" )//如果不是栈内变量试图在前10条指令内找到 ldil 0x77,%dp,%r，如果没有找到则出错
			{
				//addil           -0x800, %dp, %r1
				// . . . . 
				//ldw             -0x150(%sr0,%r1), %r21 # ._write
				//ldw             -0x14C(%sr0,%r1), %r19 # dword_40024E74
				//该处理方式不是非常严谨，基本基于常见如上形势，出现，如果没有找到就将整个作为REG，试图找到对应的stw
				auto n ;
				n=0;
				tmpAddr = addr;
				while(tmpAddr > FUNC_BEGIN &&  tmpAddr < FUNC_END && n < 30 && CLICK <PREVENT_UN_LOCK_CLICK )
				{
					tmpAddr =tmpAddr-4;//=RfirstB(tmpAddr);
					n++;
					CLICK++; //防止死循环

					if(GetMnem(tmpAddr) == "addil" && GetOpnd(tmpAddr,1) == "%dp" && GetOpnd(tmpAddr,2) == tmpReg)
					{
						tmpValue = getValue(tmpAddr,0);
						if(tmpValue == "ERR")
							F_ERR(tmpAddr,VALUE);
						return VALUE + Dword( tmp + tmpValue+REGDP );
					}
					else if(GetMnem(tmpAddr)=="ldil" || GetMnem(tmpAddr)=="ldi" && GetOpnd(tmpAddr,1) == tmpReg)
					{
						tmpValue =getValue(tmpAddr,0);
						if(tmpValue == "ERR")
							F_ERR(tmpAddr,VALUE);
						return VALUE + Dword( tmp + tmpValue);
					}
					else if(GetMnem(tmpAddr) == "copy" && GetOpnd(tmpAddr,1) == tmpReg)
					{
						tmpReg = GetOpnd(tmpAddr,0);
						if(GetOpType(tmpAddr,0) == 1)
						{
							if(tmpReg == "%r0")
								return VALUE + Dword(tmp);
							if(tmpReg == "%dp")
								return VALUE + Dword(tmp + REGDP);
							if(tmpReg == "%sp")
								break;
						}
						else
						{
							return VALUE + Dword(tmp + getValue(tmpAddr,0));
						}
					}
				}// END while  (指定范围内寻找能迅速处理的指令)
			}

			//将整个字符串 -0x140+var_48(%sr0,%sp) 作为寻找对象，该目标只能由stw处理。
			REG = GetOpnd(addr,0);  //继续回溯
		}
		else if(opcode == "stw" && GetOpnd(addr,1) == REG) //stw %r,sw  / stw 0x88,sw
		{
			D_MSG(DEBUG_FLAG,addr,"stw",REG);
			type = GetOpType(addr,0);
			if(type == 1) //寄存器
			{
				tmpReg = GetOpnd(addr,0);
				if(tmpReg == "%dp")
					return VALUE + REGDP;
				if(tmpReg == "%r0")
					return VALUE;
				if(tmpReg == "%sp")
					F_STA(addr,VALUE);
				if(tmpReg == "%r28")
					F_RET(addr,VALUE);
				REG = tmpReg;  //继续回溯
			}	
			else if(type == 5 || type == 7)
				return VALUE + getValue(addr,0);
			else
				F_ERR(addr,VALUE);

		}
		else if(GetOpnd(addr,1)==REG)  //未实现相应处理的指令
		{
			if( opcode=="ldb" || opcode=="ldh" || opcode=="addib" || opcode=="movib")
				F_IMP(addr,VALUE);
		}
		else if(GetOpnd(addr,2)==REG)
		{
			if(opcode=="sub"||opcode=="and"||opcode=="add"||opcode=="subi")
				F_IMP(addr,VALUE);
			else if(opcode=="shrd"||opcode=="xor"||opcode=="uxor")
				F_IMP(addr,VALUE);
		}
		else if(GetOpnd(addr,3)==REG)
		{
			if(opcode=="depw" || opcode=="shladd" || opcode=="extrw")
				F_IMP(addr,VALUE);
		}


	} //END while(addr > FUNC_BEGIN)

	//现在已经找离开了函数区域
	if( CLICK == PREVENT_UN_LOCK_CLICK )
	{
		if(DEBUG_FLAG >=3)
			Message("ERROR: getDataResEx() 出现死循环 : 0x%x  0x%x!\n",_addr,addr);

		if(DEBUG_FLAG == 10) //如果调试等级为10则退出程序，退回到IDA
		{
			//没有找到合适的功能函数 :(
		}

		F_ERR(3,0);
	}
	
	if(REG=="%r26")
		F_ARG(REG,VALUE);
	if(REG=="%r25")
		F_ARG(REG,VALUE);
	if(REG=="%r24")
		F_ARG(REG,VALUE);
	if(REG=="%r23")
		F_ARG(REG,VALUE);

	//仍然没有判断出来，出错
	F_NOF(REG,VALUE);
}

/**
函数： long getDataRes(long,string,long)
说明： 取得数据来源 ，该函数是对getDataResEx()的包装。
**/
static getDataRes(_addr,_reg,_debugFlag)
{
	auto tmp,tmpValue;

	tmpValue = getDataResEx(_addr,"%r26",_debugFlag);
	tmp = substr(tmpValue,0,3);

	if( !isDRValue(tmpValue) )
	{
		if(_debugFlag >=1)
			Message("Get DataResource Failed : %s\n",tmpValue);
		return "ERR";
	}
	return tmpValue;
}
/**
函数：string getDRFunName(string _drString)
说明：返回"RET xxxx xxxx"中第一个xxxx指定处的回溯路径上小段范围内的最近的一个函数调用名称。出错返回""。
**/
static getDRFunName(_drString)
{
	auto type,addr,opcode,opnd1,opnd0,tmp;
	auto n,MAX_NUMBER,FUNC_BEGIN,FUNC_END;

	MAX_NUMBER = 40; //在回溯路径上40个指令内寻找函数调用

	type=getDRType(_drString) ;
	if(type!= "RET" )
		return "";
	tmp = getDRStr1(_drString);
	addr = xtol(tmp);

	if(addr == 0)
		return "";
	if( GetFunctionName(addr) == "")
		return "";
	if( SegName(addr) != "$CODE$")
		return "";

	FUNC_BEGIN = getFuncBeginEA(addr);
	FUNC_END   = FindFuncEnd(FUNC_BEGIN);
	if(FUNC_BEGIN == BADADDR || FUNC_END == BADADDR || FUNC_END <= FUNC_BEGIN)
		return "";

	addr=RfirstB(addr);
	n = 0;
	while(addr > FUNC_BEGIN && addr < FUNC_END &&  n < MAX_NUMBER)
	{
		addr=RfirstB(addr);
		n++;

		opcode = GetMnem(addr);
		opnd0  = GetOpnd(addr,0);
		opnd1  = GetOpnd(addr,1);
		if(opcode == "call" )
			return opnd0;
		if(opcode == "b" && opnd1 == "%r31")
			return opnd0;
	}

	return "";
}


/**
函数：string getDRStrFunName(string _drString)
说明：返回"STA xxxx xxxx"中第一个xxxx指定处的回溯路径上小段范围内的字符串操作函数调用名称。出错返回""。
**/
static getDRStrFunName(_drString)
{
	auto reg,flag,type,addr,opcode,opnd1,opnd0,tmp;
	auto n,MAX_NUMBER,FUNC_BEGIN,FUNC_END;

	MAX_NUMBER = 400; //在回溯路径上指定范围内寻找函数调用

	type=getDRType(_drString) ;
	if(type!= "STA" )
		return "";
	tmp = getDRStr1(_drString);
	addr = xtol(tmp);

	if(addr == 0)
		return "";
	if( GetFunctionName(addr) == "")
		return "";
	if( SegName(addr) != "$CODE$")
		return "";

	if(GetMnem(addr) != "ldo")
		return "";
	flag=GetFlags(addr);
	if(! isStkvar0(flag) )
		return "";
	reg=GetOpnd(addr,0);

	FUNC_BEGIN = getFuncBeginEA(addr);
	FUNC_END   = FindFuncEnd(FUNC_BEGIN);
	if(FUNC_BEGIN == BADADDR || FUNC_END == BADADDR || FUNC_END <= FUNC_BEGIN)
		return "";

	addr=RfirstB(addr);
	n = 0;
	while(addr > FUNC_BEGIN && addr < FUNC_END &&  n < MAX_NUMBER)
	{
		addr=RfirstB(addr);
		n++;

		opcode = GetMnem(addr);
		opnd0  = GetOpnd(addr,0);
		opnd1  = GetOpnd(addr,1);

		if(opcode == "ldo" && opnd0 == reg && opnd1 == "%r26") //目前仅处理给%r26赋值紧跟在call之后的情况
		{
			addr=RfirstB(addr);
			n++;
			opcode = GetMnem(addr);
			opnd0  = GetOpnd(addr,0);
			opnd1  = GetOpnd(addr,1);
			if(opcode == "call"  || (opcode == "b" && opnd1 == "%r31") )
			{
				if(opnd0=="strcpy"||opnd0=="strncpy"||opnd0=="strcat"||opnd0=="strncat")
					return opnd0;
				if(opnd0=="memcpy"||opnd0=="sprintf"||opnd0=="snprintf")
					return opnd0;
			}
		}//end opcode == "ldo" . . .
	}

	return "";
}


