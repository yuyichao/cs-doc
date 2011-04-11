#include <idc.idc>
static main()
{

	auto addr,tmp;
	addr = ScreenEA();
	tmp = GetFlags(addr);
	Message("GetOpType 0:%x\n",GetOpType(addr,0));
	Message("hasName:%x hasValue:%x isBin0:%x isChar0:%x\n",hasName(tmp),hasValue(tmp),isBin0(tmp),isChar0(tmp));
	Message("isCode:%x isData:%x isDec0:%x isDefArg0:%x isDefArg1:%x\n",isCode(tmp),isData(tmp),isDec0(tmp),isDefArg0(tmp), isDefArg1(tmp));
	Message("isEnum0:%x isExtra:%x isFlow:%x isFop0:%x\n",isEnum0(tmp),isExtra(tmp),isFlow(tmp),isFop0(tmp));
	Message("isHead:%x isHex0:%x isOct0:%x isOff0:%x\n",isHead(tmp),isHex0(tmp),isOct0(tmp),isOff0(tmp));
	Message("isRef:%x isSeg0:%x isStkvar0:%x isStkvar1:%x isStroff0:%x\n",isRef(tmp),isSeg0(tmp),isStkvar0(tmp),isStkvar1(tmp),isStroff0(tmp));
	Message("isTail:%x isUnknown:%x isVar:%x\n\n",isTail(tmp),isUnknown(tmp),isVar(tmp));

}

