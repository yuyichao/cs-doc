/* ex_vc.cpp
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  VC中对象的空间组织和溢出试验示例程序
*/

#include<iostream.h>
class ClassEx
{
public:
int buff[1];
virtual void test(void){ cout << "ClassEx::test()" << endl;};
};
void entry(void)
{
  cout << "Why a u here ?!" << endl;
};

ClassEx obj1,obj2,* pobj;

int main(void)
{

  pobj=&obj2;
  obj2.test();
  
  int vtab[1] = { (int) entry };//构造vtab，
                                //entry的入口地址
  obj1.buff[1] = (int)vtab;     //obj1.buff[1]就是 obj2的pvftable域
                                //这里修改了函数指针列表的地址到vtab
  pobj->test();
  return 0;
}
