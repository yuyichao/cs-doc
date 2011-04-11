#include <idc.idc>
#include "square.idc"
#include "tiger.idc"

static main()
{
	auto deepLen,debugFlag;
	deepLen = 3;
	debugFlag = 1;

	func1arg("getenv","%r26",deepLen,debugFlag,"");
	func1arg("_getenv","%r26",deepLen,debugFlag,"");
}

