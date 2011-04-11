#include <idc.idc>
#include "square.idc"
#include "tiger.idc"

static main()
{
	auto deepLen,debugFlag; 
	deepLen = 3;
	debugFlag = 2;

	func2arg("strcpy",deepLen,debugFlag,"");
	func2arg("strcat",deepLen,debugFlag,"");
	func2arg("memcpy",deepLen,debugFlag,"");

	funcMarg("sprintf",deepLen,debugFlag,"");
	funcMarg("vsprintf",deepLen,debugFlag,"");

	funcNarg("strncpy",deepLen,debugFlag,"");
	funcNarg("strncat",deepLen,debugFlag,"");

	funcSnprintf("snprintf",deepLen,debugFlag,"");

}
