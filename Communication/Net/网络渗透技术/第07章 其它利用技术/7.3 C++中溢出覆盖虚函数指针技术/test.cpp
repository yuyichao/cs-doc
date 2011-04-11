/* test.cpp
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  VC中虚函数工作机制分析示例程序
*/

#include<iostream.h>
class ClassA
{
public:
  int num1;
  ClassA(){ num1=0xffff; };
  virtual void test1(void){};
  virtual void test2(void){};
};
ClassA objA,* pobjA;

int main(void)
{
  pobjA=&objA;
  objA.test1();
  objA.test2();
  pobjA->test1();
  pobjA->test2();
  return 0;
}
