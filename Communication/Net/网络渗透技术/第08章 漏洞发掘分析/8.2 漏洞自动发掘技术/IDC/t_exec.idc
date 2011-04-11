#include <idc.idc>
#include "square.idc"
#include "tiger.idc"

static main()
{
	auto deepLen,debugFlag; 
	deepLen = 5;
	debugFlag = 1;

	func1arg("setuid","%r26",deepLen,debugFlag,"");
	func1arg("setgid","%r26",deepLen,debugFlag,"");
	func1arg("setresuid","%r26",deepLen,debugFlag,"");
	func1arg("setreguid","%r26",deepLen,debugFlag,"");

	func1arg("execl","%r26",deepLen,debugFlag,"");
	func1arg("execle","%r26",deepLen,debugFlag,"");
	func1arg("execlp","%r26",deepLen,debugFlag,"");
	func1arg("execve","%r26",deepLen,debugFlag,"");
	func1arg("execvp","%r26",deepLen,debugFlag,"");
	func1arg("system","%r26",deepLen,debugFlag,"");
}

