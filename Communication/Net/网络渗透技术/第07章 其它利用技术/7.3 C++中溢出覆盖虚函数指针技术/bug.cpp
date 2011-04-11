/* bug.cpp
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  模拟真实情况的溢出试验
*/

#include<iostream.h> 
#include<fstream.h> 
#include<unistd.h> 

class ClassBase 
{ 
public: 
  char buff[128]; 

  void getBuff() 
  { 
     ifstream myin; 
     myin.open("bug.conf"); 
     cout << "Get buff from file : bug.conf" << endl; 
     myin >> buff;    // 看，这种用法的人不是少数吧 ! 
  }; 
  virtual void printBuffer(void){}; 
}; 

class  ClassA :public ClassBase 
{ 
public: 
  void printBuffer(void) 
  { 
     cout << "Name :" << buff << endl; 
  }; 
}; 


int main(void) 
{ 
  ClassA a; 
  ClassBase * pa = &a; 


  cout << &a << endl; 

  a.getBuff();   // ----这个里边没有边界检查 ! 
  pa->printBuffer(); 

  return 0; 
} 
