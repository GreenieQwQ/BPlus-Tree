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
    ini[0] = Record(0);
    ini[1] = Record(23);
    ini[2] = Record(9);
    ini[3] = Record(28);

    B_Tree<Record,int,getKey> tree(ini);
    cout << endl << "origin:" << endl;
    tree.display();
    
    for( int i = 0; i < 100; ++i )
    {
        cout << endl << "insert " << i << ":" << endl;
        if(tree.insert( Record(i) ))
            tree.display();
        else
            cout << "Duplicated." << endl;
    }

    for( int i = 10; i < 60; ++i )
    {
        cout << endl << "erase " << i << ":" << endl;
        if(tree.erase( Record(i) ))
            tree.display();
        else
            cout << "Not exist." << endl;
    }

    cout << "Validating contains: \"Obs\" means there's some situation.\n";
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
}