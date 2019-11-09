# B+Tree

对磁盘的读取：  
每次访问到一个结点时时认为对这个结点所在的储存块进行一次磁盘访问  
每次申请新空间时认为相当于在磁盘中新找寻一个储存块即也相当于对一个储存块进行一次磁盘访问  
特别说明：  
void split( Node* n, const key_type& cur_newKey, Node* cur_newBranch, size_t pos, key_type& newKey, Node*& newBranch);  
在调用split函数时需要对新申请的空间和传入结点n的的磁盘访问，但考虑到在调用split函数的上一级已经对n进行访问，认为此时n的内容已读入内存，因此在split中不再对n进行磁盘访问。  
