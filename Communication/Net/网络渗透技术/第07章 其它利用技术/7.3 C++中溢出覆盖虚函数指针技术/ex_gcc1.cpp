/* ex_gcc1.cpp
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  GCC中对象的空间组织和溢出试验示例程序
*  测试环境：FreeBSD 4.4 + gcc 2.95.3
*/

#include<iostream.h>
class ClassTest
{
public:
  long buff[1];   //大小为1
  virtual void test(void)
  {
     cout << "ClassTest test()" << endl;
  }
};

void entry(void)
{
  cout << "Why are u here ?!" << endl;
}

int main(void)
{
  ClassTest a,*p =&a;
  long addr[] = {0,0,0,(long)entry}; //构建的虚函数表
                                  //test() -> entry()

  a.buff[1] = ( long ) addr;// 溢出，操作了虚函数列表指针
  a.test();    //静态联编的，不会有事
  p->test();   //动态联编的，到我们的函数表去找地址，
               // 结果就变成了调用函数  entry()

}
