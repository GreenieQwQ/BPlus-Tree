#ifndef BPLUS_H
#define BPLUS_H

#include <cstddef>
#include <iostream>
using namespace std;

template<typename data_type, typename key_type, int order = 4, int L = 4>
class B_Tree
{
public:
    B_Tree();
    B_Tree( const B_Tree& );
private:
    enum State { success, failure, overflow, duplicate };
    
    struct Leaf
    {
        int count;
        data_type data[L];
    };
    
    struct Node
    {
        int count;
        enum NodeTag:char { LEAF, NODE };
        union Branch
        {
            Node* node[order];
            Leaf* leaf[order];
        };
        Branch branch;
        key_type key[order-1];
        NodeTag tag;
        Node(): count(0), tag(NODE) {}
    };
    
    union ptr
    {
        Node* node;
        Leaf* leaf; 
    }; //一个可以是叶子又可以是内点的union

    // struct insertNode
    // {}; //用于判断是将数据插入内点还是叶子，无意义
    // struct insertLeaf
    // {}; //用于判断是将数据插入内点还是叶子，无意义

public:
    bool contains( const key_type& x ) const;
    bool insert( const data_type& x );
    bool erase( const key_type& x ); 

private:
    Node* root;
    State insert( Node*& n, const key_type& key, const data_type& data, key_type& newKey, Node*& newBranch);
    State insert( Node*& n, const key_type& key, const data_type& data, key_type& newKey, Leaf*& newBranch);
    void insert_key( Node* n, const key_type& newkey, void* newBranch, size_t pos );
    size_t findPos( Node* n, const key_type& x ) const;
    void split( Node* n, const key_type& cur_newKey, Node* cur_newBranch, size_t pos, key_type& newKey, Node*& newBranch);
    void split( Leaf*& n, Node* pre );
    void display( Node* n, int indent = 0 ) const;
    void display( Leaf* l, int indent = 0 ) const;
    
private:    
    inline void displayIndent( int indent ) const
    {
        for( int i = 0; i < indent; ++i )
            cout << "\t";
    }
};

template<typename data_type, typename key_type, int order, int L>
bool B_Tree<data_type, key_type, order, L>::insert( const data_type& data )
{
    key_type key = getKey( data ); //获取data的关键字
    key_type newKey;
    Node* newBranch;
    State result = insert( root, key, data, newKey, newBranch );
    if( result == overflow ) //以newKey和newBranch创建新根
    {
        Node* newRoot = new Node;
        newRoot->count = 1;
        newRoot->key[0] = newKey;
        newRoot->branch[0] = root;
        newRoot->branch[1] = newBranch;
        root = newRoot;
        result = success; 
    }
    if( result == duplicate )
        return false;
    else
        return true;
}   

template<typename data_type, typename key_type, int order, int L>
typename B_Tree<data_type, key_type, order, L>::State
B_Tree<data_type, key_type, order, L>::insert( Node*& n, const key_type& key, const data_type& data, key_type& newKey, Node*& newBranch)
{
    size_t pos = findPos( n, key );
    ptr next_node { n->branch.node[pos] };
    
    if( n->NodeTag == Node::LEAF )
    {
        for( int i = 0; i < next_node->count; ++i )
            if( data == newt_node->data[i] )
                return duplicate;
    }
        
    key_type cur_newKey;
    ptr cur_newBranch;
    State result;
    if( n->NodeTag == Node::NODE )
        result = insert( next_node, key, data, cur_newKey, cur_newBranch.node );
    else if( n->NodeTag == Node::LEAF )
        result = insert( next_node, key, data, cur_newKey, cur_newBranch.leaf );
    
    if( result == overflow )
    {
        if( n->count < order - 1 )
        {
            result = success;
            insert_key( n, cur_newKey, cur_newBranch, pos );
        }
        else
            split( n, cur_newKey, cur_newBranch, pos, newKey, newBranch );
        //需要解决：含叶子与不含叶子是否有本质不同？    
        //通过现在操作所产生的新键和新分支来更新newKey和newBranch给上一级使用
    }
    return result;
} 

template<typename data_type, typename key_type, int order, int L>
typename B_Tree<data_type, key_type, order, L>::State
B_Tree<data_type, key_type, order, L>::insert( Node*& n, const key_type& key, const data_type& data, key_type& newKey, Leaf*& newBranch )
{
   
}

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::insert_key( Node* n, const key_type& newkey, void* newBranch, size_t pos ) 
{
    for( int i = n->count; i > pos; --i ) //包括pos在内都向前移位
    {
        n->key[i] = n->key[i-1];
        n->branch.node[i+1] = n->branch.node[i];
    }
    n->key[pos] = newkey;
    n->branch.node[pos+1] = (Node*)newBranch;   //branch的pos部分已经更新不需更改，只需插入新生成的分支即可
    n->count++; 
}   

template<typename data_type, typename key_type, int order, int L>
size_t B_Tree<data_type, key_type, order, L>::findPos( Node* n, const key_type& x ) const
{
    for( int i = 0; i < n->count; ++i )
    {
        if( x < n->key[i] )
            return i;
    }
    return n->count;
}   

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::split( Node* n, const key_type& cur_newKey, Node* cur_newBranch, size_t pos, key_type& newKey, Node*& newBranch)
{
    //需要解决：含叶子与不含叶子是否有本质不同？   
}   

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::split( Leaf*& n, Node* pre )
{
    //需要解决：含叶子与不含叶子是否有本质不同？   
}   

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::display( Node* n, int indent ) const
{
    if( n->NodeTag == Node::NODE )
    {
        for( int i = n->count - 1; i >= 0; --i )
        {
            display( n->branch.node[i+1], indent + 1 );
            displayIndent(indent);
            cout << n->key[i] << endl;
        }
        display( n->branch.node[0], indent + 1 );
    }
    else if( n->NodeTag == Node::LEAF )
    {
        for( int i = n->count - 1; i >= 0; --i )
        {
            display( n->branch.leaf[i+1], indent + 1 );
            displayIndent(indent);
            cout << n->key[i] << endl;
        }
        display( n->branch.leaf[0], indent + 1 );
    }       
} 

template<typename data_type, typename key_type, int order, int L>
void B_Tree<data_type, key_type, order, L>::display( Leaf* l, int indent ) const
{
    displayIndent(indent);
    
    for( int i = 0; i < l->count; ++i )
        cout << l->data[i] << ' ';

    cout << endl;
}   



#endif