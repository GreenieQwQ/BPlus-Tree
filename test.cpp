#include "B+.h"
#include <iostream>
using namespace std;

struct Record
{
    int data;
    int key;
    Record( int a, int b ) : data(a), key(b) {}
    Record( int a ) : data(a), key(a) {}
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
    B_Tree<Record,int,getKey> tree;
    cout << endl << "origin:" << endl;
    tree.display();
    for( int i = 16; i < 25; ++i )
    {
        cout << endl << "insert " << i << ":" << endl;
        tree.insert( Record(i,i) );
        tree.display();
    }
    
    for( int i = 0; i < 100; ++i )
    {
        cout << endl << "insert " << i << ":" << endl;
        if(tree.insert( Record(i,i) ))
            tree.display();
        else
            cout << "Duplicated." << endl;
    }

    for( int i = 10; i < 60; ++i )
    {
        cout << endl << "erase " << i << ":" << endl;
        if(tree.erase( Record(i,i) ))
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