#ifndef BPLUS_H
#define BPLUS_H

#include <cstddef>
#include <iostream>
using namespace std;

template<typename data_type, typename key_type, typename getKey, int order = 5, int L = 4>
class B_Tree
{
public:
    B_Tree(): root(nullptr) {}

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
        Node( NodeTag t = NODE ): tag(t) {}
    };
    
    union ptr
    {
        Node* node;
        Leaf* leaf;
        ptr( Node* n ): node(n) {} 
        ptr(): node(nullptr) {}
    }; //一个可以是叶子又可以是内点的union

    // struct insertNode
    // {}; //用于判断是将数据插入内点还是叶子，无意义
    // struct insertLeaf
    // {}; //用于判断是将数据插入内点还是叶子，无意义

public:
    bool contains( const key_type& x ) const;
    bool insert( const data_type& x );
    bool erase( const key_type& x ); 
    void display() const
    {
        display(root.leaf);
    }

private:
    ptr root;
    int size;
    State insert( Node*& n, const key_type& key, const data_type& data, key_type& newKey, Node*& newBranch);
    State insert( Leaf*& l, const key_type& key, const data_type& data, key_type& newKey, Leaf*& newBranch);
    void insert_key( Node* n, const key_type& newkey, void* newBranch, size_t pos );
    void insert_data( Leaf* l, const data_type& newdata, size_t pos ); 
    size_t findPos( Node* n, const key_type& x ) const;
    size_t findPos( Leaf* l, const data_type& d ) const;
    void split( Node* n, const key_type& cur_newKey, Node* cur_newBranch, size_t pos, key_type& newKey, Node*& newBranch);
    void split( Leaf* l, const data_type& data, size_t pos, key_type& newKey, Leaf*& newBranch );
    void display( Node* n, int indent = 0 ) const;
    void display( Leaf* l, int indent = 0 ) const;
    
private:    
    inline void displayIndent( int indent ) const
    {
        for( int i = 0; i < indent; ++i )
            cout << "\t";
    }
};

template<typename data_type, typename key_type, typename getKey, int order, int L>
bool B_Tree<data_type, key_type, getKey, order, L>::insert( const data_type& data )
{
    if( root.node == nullptr ) //空树
    {
        root.leaf = new Leaf;
        insert_data( root.leaf, data, 0 );
        return true;
    }

    getKey get;
    key_type key = get( data ); //获取data的关键字
    key_type newKey;
    ptr newBranch;
    State result;
    if( size <= L )
        result = insert( root.leaf, key, data, newKey, newBranch.leaf );
    else
        result = insert( root.node, key, data, newKey, newBranch.node );
    
    if( result == overflow ) //以newKey和newBranch创建新根
    {
        Node* newRoot = new Node;
        newRoot->count = 1;
        newRoot->key[0] = newKey;
        newRoot->branch.node[0] = root;
        newRoot->branch.node[1] = newBranch;
        root = newRoot;
        result = success; 
    }
    if( result == duplicate )
        return false;
    else
        return true;
}   

template<typename data_type, typename key_type, typename getKey, int order, int L>
typename B_Tree<data_type, key_type, getKey, order, L>::State
B_Tree<data_type, key_type, getKey, order, L>::insert( Node*& n, const key_type& key, const data_type& data, key_type& newKey, Node*& newBranch)
{
    size_t pos = findPos( n, key );
    ptr next_node { n->branch.node[pos] };
    
    if( n->tag == Node::LEAF )
    {
        for( int i = 0; i < next_node->count; ++i )
            if( data == next_node->data[i] )
                return duplicate;
    }
        
    key_type cur_newKey;
    ptr cur_newBranch;
    State result;
    if( n->tag == Node::NODE )
        result = insert( next_node, key, data, cur_newKey, cur_newBranch.node );
    else if( n->tag == Node::LEAF )
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
        //通过下一级操作因overflow所产生的新键和新分支来更新newKey和newBranch给上一级使用
    }
    return result;
} 

template<typename data_type, typename key_type, typename getKey, int order, int L>
typename B_Tree<data_type, key_type, getKey, order, L>::State
B_Tree<data_type, key_type, getKey, order, L>::insert( Leaf*& l, const key_type& key, const data_type& data, key_type& newKey, Leaf*& newBranch )
{
    State result = success;
    size_t pos = findPos( l, data );
    if( l->count < L )
        insert_data( l, data, pos );
    else
    {
        split( l, data, pos, newKey, newBranch ); //通过向叶子中插入data 产生新的key和新的branch 
        result = overflow;
    }
    return result;
}

template<typename data_type, typename key_type, typename getKey, int order, int L>
void B_Tree<data_type, key_type, getKey, order, L>::insert_data( Leaf* l, const data_type& newdata, size_t pos ) 
{
    for( int i = l->count; i > pos; --i ) //包括pos在内都向前移位
        l->data[i] = l->data[i-1];
    l->data[pos] = newdata;
    l->count++; 
}

template<typename data_type, typename key_type, typename getKey, int order, int L>
void B_Tree<data_type, key_type, getKey, order, L>::insert_key( Node* n, const key_type& newkey, void* newBranch, size_t pos ) 
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

template<typename data_type, typename key_type, typename getKey, int order, int L>
size_t B_Tree<data_type, key_type, getKey, order, L>::findPos( Node* n, const key_type& x ) const
{
    for( int i = 0; i < n->count; ++i )
    {
        if( x < n->key[i] )
            return i;
    }
    return n->count;
}   

template<typename data_type, typename key_type, typename getKey, int order, int L>
size_t B_Tree<data_type, key_type, getKey, order, L>::findPos( Leaf* l, const data_type& data ) const
{
    for( int i = 0; i < l->count; ++i )
    {
        if( data < l->data[i] )
            return i;
    }
    return l->count;
}  

template<typename data_type, typename key_type, typename getKey, int order, int L>
void B_Tree<data_type, key_type, getKey, order, L>::split( Node* n, const key_type& cur_newKey, Node* cur_newBranch, size_t pos, key_type& newKey, Node*& newBranch)
{
    newBranch = new Node( n->tag ); //分出的新结点的tag肯定和n相同
    size_t mid = order / 2;
    if( pos > mid ) //使mid向后数的关键字个数总小于等于左半边的
        mid++;

    for( int i = mid; i < order-1; ++i ) //拷贝key和branch 
    {
        //注意!!下标为order-2为第order-1个元素
        newBranch->key[i-mid] = n->key[i];
        newBranch->branch[i-mid+1] = n->branch[i+1];
    } 
    n->count = mid;
    newBranch->count = order - 1 - mid;
    
    if( pos > mid ) //与mid自不自增无关 位置都是pos-mid 因为拷贝都把mid拷贝过去了 
        insert_key( newBranch, cur_newKey, cur_newBranch, pos - mid );
    else
        insert_key( n, cur_newKey, cur_newBranch, pos );
    
    newKey = n->key[n->count - 1];
    newBranch = n->branch[n->count];
    n->count--;   
}   

template<typename data_type, typename key_type, typename getKey, int order, int L>
void B_Tree<data_type, key_type, getKey, order, L>::split( Leaf* l, const data_type& data, size_t pos, key_type& newKey, Leaf*& newBranch )
{
    newBranch = new Leaf;
    size_t mid = L / 2;
    if( pos > mid ) //使mid向后数的关键字个数总小于等于左半边的
        mid++;

    for( int i = mid; i < L; ++i ) //拷贝data和branch 
        newBranch->data[i-mid] = l->data[i];
    
    l->count = mid;
    newBranch->count = L - mid;
    
    if( pos > mid ) //与mid自不自增无关 位置都是pos-mid 因为拷贝都把mid拷贝过去了 
        insert_data( newBranch, data, pos - mid );
    else
        insert_data( l, data, pos );
    
    getKey get;
    newKey = get(newBranch->data[0]); //新结点第一个data的关键字作为新键
}   

template<typename data_type, typename key_type, typename getKey, int order, int L>
void B_Tree<data_type, key_type, getKey, order, L>::display( Node* n, int indent ) const
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

template<typename data_type, typename key_type, typename getKey, int order, int L>
void B_Tree<data_type, key_type, getKey, order, L>::display( Leaf* l, int indent ) const
{
    displayIndent(indent);
    
    for( int i = 0; i < l->count; ++i )
        cout << l->data[i] << ' ';

    cout << endl;
}   



#endif