#include "B+.h"
#include <iostream>
using namespace std;

struct Record
{
    int data;
    int key;
    Record( int a, int b ) : data(a), key(b) {} //第一个参数为数据，第二个参数为键值
    Record( int a ) : data(a), key(a) {} //键值=数据
    Record() {}
    Record( const Record& o )
    {
        data = o.data;
        key = o.key;
    }
    bool operator==( const Record& o) const
    {
        return key == o.key;
    }
    bool operator< ( const Record& o) const
    {
        return key < o.key;
    }
};

ostream& operator <<( ostream& out, const Record& r )
{
    out << r.data;
    return out;
} 

struct getKey
{
    int operator() (const Record& r) const
    {
        return r.key;
    }
};


int main()
{
    //5阶B+树、L=4 
    //认为树初始状态根不为叶子——因此至少需要（L/2）*2 = 4个数据来初始化B+树
    Record ini[ 2 * 2 ]; //初始化树的数据 顺序随意
    ini[0] = Record(15);
    ini[1] = Record(23);
    ini[2] = Record(89);
    ini[3] = Record(50);

    B_Tree<Record,int,getKey> tree(ini);
    cout << "==== Initial:\n==== M = 5\n==== L = 4\n" 
    << endl << "==== Origin:\n";
    tree.display();
    
    for( int i = 0; i < 100; ++i )
    {
        cout << endl << "Insert " << i << ":" << endl;
        if(tree.insert( Record(i) ))
            tree.display();
        else
            cout << "Duplicated." << endl;
    }

    for( int i = 10; i < 60; ++i )
    {
        cout << endl << "Erase " << i << ":" << endl;
        if(tree.erase( Record(i) ))
            tree.display();
        else
            cout << "Not exist." << endl;
    }

    cout << "\n==== Validating: containing \"Obs\" means there's some situation.\n";
    for( int i = 0; i < 10; ++i )
    {
        cout << endl << "Validating " << i << ":" << endl;
        if( !tree.contains(i) )
            cout << "Obs" << endl;
        else   
            cout << i << " is in B_Tree\n";
        tree.displayAccessTime();
    }
    for( int i = 10; i < 60; ++i )
    {
        cout << endl << "Validating " << i << ":" << endl;
        if( tree.contains(i) )
            cout << "Obs" << endl;
        else   
            cout << i << " is not in B_Tree\n";
        tree.displayAccessTime();
    }
    for( int i = 60; i < 100; ++i )
    {
        cout << endl << "Validating " << i << ":" << endl;
        if( !tree.contains(i) )
            cout << "Obs" << endl;
        else   
            cout << i << " is in B_Tree\n";
        tree.displayAccessTime();
    }
    for( int i = 100; i < 150; ++i )
    {
        cout << endl << "Validating " << i << ":" << endl;
        if( tree.contains(i) )
            cout << "Obs" << endl;
        else   
            cout << i << " is not in B_Tree\n";
        tree.displayAccessTime();
    }
    cout << "\nFinal Tree and AccessTime:\n";
    tree.display();

    cout << "\nTest for initial function:" << endl;
    cout << "==== M = 5\n==== L = 4\n";
    cout << "==== Origin:" << endl;
    tree.init(ini);
    tree.display();
    /**
     *若确认使用32位的编译器可定义宏COMPILER32来显示结点和树叶的大小
     *——在32位机器下指针大小才为4
     *可用dev-cpp使用32位编译器编译
     */
    // #define COMPILER32
    #ifdef COMPILER32 
    tree.displaySize();
    #endif
}
