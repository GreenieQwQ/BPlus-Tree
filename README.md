# B+Tree

对磁盘的读取：  
每次访问到一个结点时时认为对这个结点所在的储存块进行一次磁盘访问  
每次申请新空间时认为相当于在磁盘中新找寻一个储存块即也相当于对一个储存块进行一次磁盘访问  
特别说明：  
void split( Node* n, const key_type& cur_newKey, Node* cur_newBranch, size_t pos, key_type& newKey, Node*& newBranch);  
在调用split函数时需要对新申请的空间和传入结点n的的磁盘访问，但考虑到在调用split函数的上一级已经对n进行访问，认为此时n的内容已读入内存，因此在split中不再对n进行磁盘访问。  

可以这么理解：
Node* n;  
当我们不需要用到“n->”即不需要访问指针里面的值时，不计访存次数  
若我们用到了“n->”，则增加一次访存次数，且之后在未进行很多次新的访存的时候我们再使用“n->”时不计入访存次数  

Linux下测试请在头文件前输入：  
#define UNIX  
Windows下测试请在头文件前输入：  
#define WINDOWS  
（二者请不要同时定义）  

测试文件在test.cpp 编译运行即可  
test.cpp里main函数的初始化的4个数据可以随意改动  
如果想自己编写测试函数 建议阅读test.cpp里测试的方法以后再自行测试  
本项目的github网址：https://github.com/GreenieQwQ/BPlus-Tree  欢迎大家一起讨论。
