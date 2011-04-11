/**
文件名： tiger.idc
用途  ： 通过分析函数调用场景来试图找出危险的调用。
说明  ： 针对HP-UX PA指令。
编写  ： watercloud
日期  ： 2003-3-1
**/


/**
函数名：void func1arg(string,string,long,long,string)  
说明：  企图分析全部对__funcName的调用时使用_reg寄存器的调用情景。
强调：  函数为调试目的对__debugFlag>10时做了特殊处理，此时__funcName为一个地址，表示从该
        地址回溯，在回溯路径上查看_reg的使用情况,此时_debugFlag=__debugFlag-10 ,researchFlag =1。
**/
static func1arg(__funcName,_reg,_deepLen,__debugFlag,_aligStr)
{

	auto type,addr,addrX,funcAddr,_funcName,funcName,_debugFlag;
	auto n,tmp,tmpStr,tmpStr1,tmpStr2,tmpAddr,tmpReg,value;
	auto researchFlag;

	if(_deepLen <=0)
		return;
	

	if(__debugFlag > 10) //特殊调试情况,回溯分析某个寄存器使用情况。
	{
		researchFlag = 1;
		_debugFlag = __debugFlag - 10;
		tmpStr=GetFunctionName(__funcName);
		if(tmpStr== "")
			return;
		funcAddr=__funcName;
		if(GetMnem(funcAddr) == "call")
			_funcName=GetOpnd(funcAddr,0);
		else
			_funcName="";
		funcAddr=funcAddr+4;
	}
	else //通常分析全文对特定函数特定参数调用使用情况
	{
		researchFlag=0;
		_funcName=__funcName;
		_debugFlag = __debugFlag;
		funcAddr=LocByName(_funcName);
		if(funcAddr == BADADDR)
			return;
		if(_reg != "%r26" && _reg != "%r25" && _reg != "%r24" && _reg !="%r23")
			return;
	}

	n=0;
	addrX=RfirstB(funcAddr);
	while(addrX != BADADDR)
	{
		n ++;
		if(researchFlag == 0 ) //通常情况
		{
			type = XrefType(); 
			if(type != fl_CF && type !=fl_CN) //如果调用类型不是函数调用则忽略
			{
				addrX = RnextB(funcAddr,addrX);
				n--;
				continue;
			}
		}
		else //如果是特殊调试情况，则无需循环，退出
		{
			if(n == 2)
				break;
		}

		//追踪数据来源
		value = getDataResEx(addrX +4,_reg,_debugFlag);
		funcName = GetFunctionName(addrX);
		type = getDRType(value);

		tmpStr = getString(value);
		//如果不是调试模式,且串为已知则忽略
		if( tmpStr !="" && _debugFlag <=1) 
		{
			addrX = RnextB(funcAddr,addrX);
			n--;
			continue;
		}

		//处理相关显示级对函数调用再次回溯递归
		Message("%s%-2i %x\t %s:\t call %s() %s=",_aligStr,n,addrX,funcName,_funcName,_reg);

		if( isDRValue(value) )
		{
			Message("%x:\"%s\"\n",value,tmpStr);
		}
		else if(type == "ARG" )
		{
			Message("%s\n",getDRStr0_1(value));
			func1arg(funcName,getDRReg(value),_deepLen-1,_debugFlag,_aligStr+"  ");
		}
		else if(type == "RET" )
		{
			tmpStr = getDRFunName(value);
			if(tmpStr == "")
				Message("%s\n",getDRStr0_1(value));
			else
				Message("RET %s()\n",tmpStr);
		}
		else if(type == "STA" )
		{
			tmpStr = getDRStrFunName(value);
			if(tmpStr == "")
				Message("%s\n",getDRStr0_1(value));
			else
				Message("RET %s()\n",tmpStr);
		}
		else
		{
			if(_debugFlag >=1 )
				Message("%s\n",getDRStr0_1(value));
			else
				Message("没有找到\n");
		}

		addrX = RnextB(funcAddr,addrX);
	}
} //end of func1arg

/**
函数名：void func2arg(string,long,long,string)  
说明：  针对两个参数的函数分析主要是memcpy,stcat,strcpy . . .。企图分析全部对_funcName的调用时的情景。
**/
static func2arg(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeSrc,typeDst,nowFunc,valueSrc,valueDst,strSrc,strDst;
	auto desSrc,desDst;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR )
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //如果调用类型不是函数调用则忽略
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		nowFunc = GetFunctionName(addrX);

		//获取数据源
		valueSrc = getDataResEx(addrX +4,"%r25",_debugFlag);
		typeSrc = getDRType(valueSrc);
		valueDst=getDataResEx(addrX+4,"%r26",_debugFlag);
		typeDst=getDRType(valueDst);

		//处理相关数据信息
		strSrc="";
		strDst="";

		if( isDRValue(valueSrc) )
			strSrc = getString(valueSrc);
		else if(typeSrc == "RET")
			desSrc= typeSrc +" " + getDRFunName(valueSrc) + "()";
		else if(typeSrc == "ARG")
			desSrc= getDRStr0_1(valueSrc);
		else
		{
			if(_debugFlag >=1)
				desSrc = getDRStr0_1(valueSrc);
			else
				desSrc=typeSrc;
		}

		if( isDRValue(valueDst) )
			strDst = getString(valueDst);
		else if(typeDst == "RET")
			desDst =typeDst + " " + getDRFunName(valueDst) + "()";
		else if(typeDst == "ARG")
			desDst= getDRStr0_1(valueDst);
		else
		{
			if(_debugFlag >=1)
				desDst = getDRStr0_1(valueDst);
			else
				desDst=typeDst;
		}

		//如果源已知，且非调试模式则忽略
		if( isDRValue(valueSrc) && strSrc !="" && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}


		//开始打印相关信息

		Message("%s%-2i %x\t %s:\t call %s(",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueDst) )
			Message("%x:\"%s\"," ,valueDst,strDst);
		else 
			Message("%s,",desDst);

		if(isDRValue(valueSrc) )
			Message("%x:\"%s\"" ,valueSrc,strSrc);
		else 
			Message("%s",desSrc);

		Message(")\n");

		if(typeDst== "ARG")
			func1arg(nowFunc,getDRReg(valueDst),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeSrc== "ARG")
			func1arg(nowFunc,getDRReg(valueSrc),_deepLen-1,_debugFlag,_aligStr+"  ");
		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of func2arg



/**
函数名：void funcNarg(string,long,long,string)  
说明：  针对两个参数的函数分析主要是strncpy. . .。企图分析全部对_funcName的调用时的情景。
**/
static funcNarg(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeSrc,typeDst,typeN,typeR23,nowFunc,valueSrc;
	auto valueDst,argNumber,tmpStr,index,frameSize;
	auto valueN,valueR23,strSrc,strDst,strN,strR23,desSrc,desDst,desN,desR23;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR )
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //如果调用类型不是函数调用则忽略
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		valueSrc =0;  valueDst=0;  valueN=0;
		argNumber=0;
		strSrc="";    strDst="";
		strN=""; 

		nowFunc = GetFunctionName(addrX);

		//追踪3个参数的数据来源
		valueSrc = getDataResEx(addrX +4,"%r25",_debugFlag);
		typeSrc = getDRType(valueSrc);
		valueDst=getDataResEx(addrX+4,"%r26",_debugFlag);
		typeDst=getDRType(valueDst);
		valueN = getDataResEx(addrX +4,"%r24",_debugFlag);
		typeN = getDRType(valueN);

		//如果目标为栈且已知拷贝最大长度N，且N<FrameSize且非调试模式则忽略
		frameSize = GetFrameSize(addrX);
		if( ( frameSize <=0 || valueN <=frameSize ) &&  typeDst == "STA" && isDRValue(valueN)  && _debugFlag <= 1 )
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//处理源串信息
		if( isDRValue(valueSrc) )
			strSrc = getString(valueSrc);
		else if(typeSrc == "RET")
			desSrc= typeSrc +" " + getDRFunName(valueSrc) + "()";
		else if(typeSrc == "ARG")
			desSrc= getDRStr0_1(valueSrc);
		else
		{
			if(_debugFlag >=1)
				desSrc = getDRStr0_1(valueSrc);
			else
				desSrc=typeSrc;
		}

		//如果源为已知，且非调试模式则忽略
		if( isDRValue(valueSrc) && strSrc !="" && _debugFlag <= 1 )
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//处理目标信息
		if( isDRValue(valueDst) )
			strDst = getString(valueDst);
		else if(typeDst == "RET")
			desDst =typeDst + " " + getDRFunName(valueDst) + "()";
		else if(typeDst == "ARG")
			desDst= getDRStr0_1(valueDst);
		else
		{
			if(_debugFlag >=1)
				desDst = getDRStr0_1(valueDst);
			else
				desDst=typeDst;
		}


		//处理N信息
		if( isDRValue(valueN) )
			strN = getString(valueN);
		else if(typeN == "RET")
			desN =typeN + " " + getDRFunName(valueN) + "()";
		else if(typeN == "ARG")
			desN= getDRStr0_1(valueN);
		else
		{
			if(_debugFlag >=1)
				desN = getDRStr0_1(valueN);
			else
				desN=typeN;
		}


		//显示相关信息
		Message("%s%-2i %x\t %s:\t call %s(",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueDst) )
			Message("%x:\"%s\"," ,valueDst,strDst);
		else 
			Message("%s,",desDst);

		if(isDRValue(valueSrc) )
			Message("%x:\"%s\"," ,valueSrc,strSrc);
		else 
			Message("%s,",desSrc);

		if(isDRValue(valueN) )
			Message("%x" ,valueN);
		else 
			Message("%s",desN);

		if( _debugFlag >=2 && typeDst=="STA")
			Message(") 栈空间大小=%x\n",frameSize);
		else
			Message(")\n");


		if(typeDst== "ARG")
			func1arg(nowFunc,getDRReg(valueDst),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeSrc== "ARG")
			func1arg(nowFunc,getDRReg(valueSrc),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeN== "ARG")
			func1arg(nowFunc,getDRReg(valueN),_deepLen-1,_debugFlag,_aligStr+"  ");

		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of funcNarg


/**
函数名：void funcMarg(string,long,long,string)  
说明：  针对两个参数的函数分析主要是sprintf . . .。企图分析全部对_funcName的调用时的情景。
**/
static funcMarg(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeFmt,typeDst,typeR24,typeR23,nowFunc,valueFmt;
	auto valueDst,argNumber,tmpStr,index;
	auto valueR24,valueR23,strFmt,strDst,strR24,strR23,desFmt,desDst,desR24,desR23;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR ) //循环处理所有对该函数的调用
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //如果调用类型不是函数调用则忽略
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		valueFmt =0;  valueDst=0;  valueR24=0;  valueR23=0;
		argNumber=0;
		strFmt="";    strDst="";
		strR24="";    strR23="";

		nowFunc = GetFunctionName(addrX);

		//追踪目标地址、格式化串相关的数据来源
		valueFmt = getDataResEx(addrX +4,"%r25",_debugFlag);
		typeFmt = getDRType(valueFmt);
		valueDst=getDataResEx(addrX+4,"%r26",_debugFlag);
		typeDst=getDRType(valueDst);


		//处理格式串相关信息
		if( isDRValue(valueFmt) )
			strFmt = getString(valueFmt);
		else if(typeFmt == "RET")
			desFmt= typeFmt +" " + getDRFunName(valueFmt) + "()";
		else if(typeFmt == "ARG")
			desFmt= getDRStr0_1(valueFmt);
		else
		{
			if(_debugFlag >=1)
				desFmt = getDRStr0_1(valueFmt);
			else
				desFmt=typeFmt;
		}

		//处理目标地址相关信息
		if( isDRValue(valueDst) )
			strDst = getString(valueDst);
		else if(typeDst == "RET")
			desDst =typeDst + " " + getDRFunName(valueDst) + "()";
		else if(typeDst == "ARG")
			desDst= getDRStr0_1(valueDst);
		else
		{
			if(_debugFlag >=1)
				desDst = getDRStr0_1(valueDst);
			else
				desDst=typeDst;
		}


		//查看有多少个%s以判断还有多少个参数需处理
		tmpStr=strFmt;
		index=strstr(tmpStr,"%s");
		while(index >=0)
		{
			argNumber ++;
			tmpStr=substr(tmpStr,index+2,-1);
			index=strstr(tmpStr,"%s");
		}

		//如果有1个以上%s则处理第二个参数
		if(argNumber >=1)
		{
			valueR24 = getDataResEx(addrX +4,"%r24",_debugFlag);
			typeR24 = getDRType(valueR24);

			if( isDRValue(valueR24) )
				strR24 = getString(valueR24);
			else if(typeR24 == "RET")
				desR24 =typeR24 + " " + getDRFunName(valueR24) + "()";
			else if(typeR24 == "ARG")
				desR24= getDRStr0_1(valueR24);
			else
			{
				if(_debugFlag >=1)
					desR24 = getDRStr0_1(valueR24);
				else
					desR24=typeR24;
			}
		}

		//如果有两个以上%s则处理第二个参数
		if(argNumber >=2)
		{
			valueR23 = getDataResEx(addrX +4,"%r23",_debugFlag);
			typeR23 = getDRType(valueR23);

			if( isDRValue(valueR23) )
				strR23 = getString(valueR23);
			else if(typeR23 == "RET")
				desR23 =typeR23 + " " + getDRFunName(valueR23) + "()";
			else if(typeR23 == "ARG")
				desR23= getDRStr0_1(valueR23);
			else
			{
				if(_debugFlag >=1)
					desR23 = getDRStr0_1(valueR23);
				else
					desR23=typeR23;
			}
		}

		
		//如果没有参数且格式串已知则忽略
		if( (argNumber == 0 && strFmt !="" && isDRValue(valueFmt) )  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}
		//如果有一个参数，而该参数是已知串，且不是调试模式则忽略
		if( (argNumber == 1 && strR24 !="" && isDRValue(valueR24) )  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}
		//如果有2个参数，且参数都是已知串，且不是调试模式则忽略
		if((argNumber ==2 && strR24 != "" && strR23 != ""&&isDRValue(valueR24)&&isDRValue(valueR23)) && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}


		//开始打印相关信息
		if(_debugFlag < 2)
		{
		}

		Message("%s%-2i %x\t %s:\t call %s(",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueDst) )
			Message("%x:\"%s\"," ,valueDst,strDst);
		else 
			Message("%s,",desDst);

		if(isDRValue(valueFmt) )
			Message("%x:\"%s\"" ,valueFmt,strFmt);
		else 
			Message("%s",desFmt);

		if(argNumber >=1)
		{
			if(isDRValue(valueR24) )
				Message(",%x:\"%s\"," ,valueR24,strR24);
			else 
				Message(",%s,",desR24);
		}

		if(argNumber >=2)
		{
			if(isDRValue(valueR23) )
				Message("%x:\"%s\"," ,valueR23,strR23);
			else 
				Message("%s,",desR23);
		}

		if(argNumber >=3)
			Message(" . . . ");

		Message(")\n");


		if(typeDst== "ARG")
			func1arg(nowFunc,getDRReg(valueDst),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeFmt== "ARG")
			func1arg(nowFunc,getDRReg(valueFmt),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeR24== "ARG")
			func1arg(nowFunc,getDRReg(valueR24),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeR23== "ARG")
			func1arg(nowFunc,getDRReg(valueR23),_deepLen-1,_debugFlag,_aligStr+"  ");

		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of funcMarg

/**
函数名：void funcSnprintf(string,long,long,string)  
说明：  分析snprintf . . .。企图分析全部对_funcName的调用时的情景。
**/
static funcSnprintf(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeFmt,typeDst,typeN,typeR23,nowFunc,valueFmt;
	auto valueDst,argNumber,tmpStr,index,frameSize;
	auto valueN,valueR23,strFmt,strDst,strN,strR23,desFmt,desDst,desN,desR23;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR )
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //如果调用类型不是函数调用则忽略
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		valueFmt =0;  valueDst=0;  valueN=0;  valueR23=0;
		argNumber=0;
		strFmt="";    strDst="";
		strN="";    strR23="";

		nowFunc = GetFunctionName(addrX);


		//追踪目标地址、拷贝最大数据N、格式串数据来源
		valueFmt = getDataResEx(addrX +4,"%r24",_debugFlag);
		typeFmt = getDRType(valueFmt);
		valueDst=getDataResEx(addrX+4,"%r26",_debugFlag);
		typeDst=getDRType(valueDst);
		valueN = getDataResEx(addrX +4,"%r25",_debugFlag);
		typeN = getDRType(valueN);

		frameSize = GetFrameSize(addrX); //求堆栈大小


		//处理格式串相关信息
		if( isDRValue(valueFmt) )
			strFmt = getString(valueFmt);
		else if(typeFmt == "RET")
			desFmt= typeFmt +" " + getDRFunName(valueFmt) + "()";
		else if(typeFmt == "ARG")
			desFmt= getDRStr0_1(valueFmt);
		else
		{
			if(_debugFlag >=1)
				desFmt = getDRStr0_1(valueFmt);
			else
				desFmt=typeFmt;
		}

		//处理目标地址相关信息
		if( isDRValue(valueDst) )
			strDst = getString(valueDst);
		else if(typeDst == "RET")
			desDst =typeDst + " " + getDRFunName(valueDst) + "()";
		else if(typeDst == "ARG")
			desDst= getDRStr0_1(valueDst);
		else
		{
			if(_debugFlag >=1)
				desDst = getDRStr0_1(valueDst);
			else
				desDst=typeDst;
		}

		//求参数个数
		tmpStr=strFmt;
		index=strstr(tmpStr,"%s");
		while(index >=0)
		{
			argNumber ++;
			tmpStr=substr(tmpStr,index+2,-1);
			index=strstr(tmpStr,"%s");
		}


		//如果目标是栈、且N<栈帧大小,并且非调试模式,且格式串为已知则忽略
		if((frameSize <=0 || valueN <=frameSize ) && isDRValue(valueN) && typeDst=="STA" && strFmt!="" && _debugFlag <=2 )
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//处理N相关信息
		if( isDRValue(valueN) )
			strN = getString(valueN);
		else if(typeN == "RET")
			desN =typeN + " " + getDRFunName(valueN) + "()";
		else if(typeN == "ARG")
			desN= getDRStr0_1(valueN);
		else
		{
			if(_debugFlag >=1)
				desN = getDRStr0_1(valueN);
			else
				desN=typeN;
		}

		//如果有1个以上%s则处理第1个参数
		if(argNumber >=1)
		{
			valueR23 = getDataResEx(addrX +4,"%r23",_debugFlag);
			typeR23 = getDRType(valueR23);

			if( isDRValue(valueR23) )
				strR23 = getString(valueR23);
			else if(typeR23 == "RET")
				desR23 =typeR23 + " " + getDRFunName(valueR23) + "()";
			else if(typeR23 == "ARG")
				desR23= getDRStr0_1(valueR23);
			else
			{
				if(_debugFlag >=1)
					desR23 = getDRStr0_1(valueR23);

				else
					desR23=typeR23;
			}
		}


		//如果没有参数且格式串已知则忽略
		if( (argNumber == 0 && strFmt !="" && isDRValue(valueFmt) )  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}
		//如果有一个参数，而该参数是已知串，且不是调试模式则忽略
		if( (argNumber == 1 && strR23 !="" && isDRValue(valueR23) )  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//开始显示相关信息
		Message("%s%-2i %x\t %s:\t call %s(",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueDst) )
			Message("%x:\"%s\"," ,valueDst,strDst);
		else 
			Message("%s,",desDst);

		if(isDRValue(valueN) )
			Message("%x," ,valueN);
		else 
			Message("%s,",desN);

		if(isDRValue(valueFmt) )
			Message("%x:\"%s\"" ,valueFmt,strFmt);
		else 
			Message("%s",desFmt);

		if(argNumber >=1)
		{
			if(isDRValue(valueR23) )
				Message(",%x:\"%s\"" ,valueR23,strR23);
			else 
				Message(",%s",desR23);
		}

		if(argNumber >=2)
			Message(" . . . ");

		Message(")\n");


		//如果需要回溯寻找相应调用参数则继续回溯相应参数
		if(typeDst== "ARG")
			func1arg(nowFunc,getDRReg(valueDst),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeN== "ARG")
			func1arg(nowFunc,getDRReg(valueN),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeFmt== "ARG")
			func1arg(nowFunc,getDRReg(valueFmt),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeR23== "ARG")
			func1arg(nowFunc,getDRReg(valueR23),_deepLen-1,_debugFlag,_aligStr+"  ");

		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of funcSnprintf


/**
函数名：void funcFmt(string,long,long,string)  
说明：  分析xxprintf格式串问题 . . .。企图分析全部对_funcName的调用时的情景。
**/
static funcFmt(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeFmt,nowFunc,valueFmt;
	auto strFmt,desFmt,regFmt;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	if( _funcName =="vsnprintf" || _funcName == "snprintf")
		regFmt="%r24";
	else if(_funcName =="fprintf" || _funcName=="vfprintf" ||_funcName=="sprintf")
		regFmt="%r25";
	else //printf  vprintf
		regFmt="%r26";

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR )
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //如果调用类型不是函数调用则忽略
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		valueFmt =0;  
		strFmt="";
		desFmt="";

		nowFunc = GetFunctionName(addrX);

		//追踪格式串数据来源
		valueFmt = getDataResEx(addrX +4,regFmt,_debugFlag);
		typeFmt = getDRType(valueFmt);


		//处理格式串相关信息
		if( isDRValue(valueFmt) )
			strFmt = getString(valueFmt);
		else if(typeFmt == "RET")
			desFmt= typeFmt +" " + getDRFunName(valueFmt) + "()";
		else if(typeFmt == "ARG")
			desFmt= getDRStr0_1(valueFmt);
		else
		{
			if(_debugFlag >=1)
				desFmt = getDRStr0_1(valueFmt);
			else
				desFmt=typeFmt;
		}

		//如果格式串已知，且不是调试模式则忽略
		if( strFmt !="" && isDRValue(valueFmt)  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//开始显示相关信息
		Message("%s%-2i %x\t %s:\t call %s( 格式串为：",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueFmt) )
			Message("%x:\"%s\"" ,valueFmt,strFmt);
		else 
			Message("%s",desFmt);

		Message(" )\n");


		//如果需要回溯寻找相应调用参数则继续回溯相应参数
		if(typeFmt== "ARG")
			func1arg(nowFunc,getDRReg(valueFmt),_deepLen-1,_debugFlag,_aligStr+"  ");
		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of funcFmt


