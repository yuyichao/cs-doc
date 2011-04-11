#include <idc.idc>
#include "square.idc"
#include "tiger.idc"

static main()
{
	auto deepLen,debugFlag;
	deepLen=3;
	debugFlag=1;
	funcFmt("printf",deepLen,debugFlag,"");
	funcFmt("fprintf",deepLen,debugFlag,"");
	funcFmt("vfprintf",deepLen,debugFlag,"");
	funcFmt("sprintf",deepLen,debugFlag,"");
	funcFmt("vsprintf",deepLen,debugFlag,"");
	funcFmt("snprintf",deepLen,debugFlag,"");
	funcFmt("vsnprintf",deepLen,debugFlag,"");
}
