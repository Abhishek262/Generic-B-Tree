#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<climits>
#include<typeinfo>
#include<cmath>
#include<vector>
#include<bits/stdc++.h>

using namespace std;
int def_max = 3; //size of each node


//find, copy , upper bound, lower bound,replace STL functions : iterator
//make data stored generic and its pointers
template<typename k, typename v>
class BPTree;

template <typename k_type, typename val_type>
class Node
{
	bool IS_LEAF;
	k_type *key;
	val_type *record;
	int size;
	int MAX;
	Node<k_type,val_type>** ptr;
	friend class BPTree<k_type,val_type>;
	friend class BPTree<k_type,val_type>::iterator;

public:
	Node();
	Node(int MAX);
	Node(const Node& n);
	Node<k_type,val_type>& operator= (const Node& N);
};

template <typename k_type = int, typename val_type = int>	
class BPTree
{
	Node<k_type,val_type> *root;
	int MAX;
	void insertInternal(k_type,Node<k_type,val_type>*,Node<k_type,val_type>*);
	void removeInternal(k_type,Node<k_type,val_type>*,Node<k_type,val_type>*);
	Node<k_type,val_type>* findParent(Node<k_type,val_type>*,Node<k_type,val_type>*);

public:

	BPTree();
	BPTree(int MAX);
	BPTree(const BPTree& T);
	BPTree& operator= (const BPTree& T){
		if(this != T){
			cleanup(this.root);
			if(T.root == NULL){
				return NULL;
			}
			else{
				root = copy_recursive(T.root);
			}
		}
		return *this;
	}
	//move
	bool search(k_type);
	void insert(k_type,val_type);
	void remove(k_type);
	void display(Node<k_type,val_type>*);
	void display_tree();
	Node<k_type,val_type> *copy_recursive(Node<k_type,val_type> *r);
	val_type& operator[] (k_type key);
	Node<k_type,val_type>* getRoot();
	void cleanUp(Node<k_type,val_type>*);
	~BPTree(); 


	class iterator{
		public:
			//typedef typename BPTree::k_type k_type;
			//typedef typename BPTree::value_type value_type;
			typedef k_type& reference;
			typedef k_type* pointer;
			typedef forward_iterator_tag iterator_category ;
			typedef ptrdiff_t difference_type;
			typedef val_type value_type;
			typedef k_type key_type;

		private:
			int curr_slot;
			Node<k_type,val_type>* curr_leaf;
			//maybe friend bptree
		public:
			//default constructor
			iterator() : curr_leaf(nullptr), curr_slot(0)
			{}
			// constructor with 2 args
			iterator(Node<k_type,val_type> *leafNode,int s) : curr_leaf(leafNode),curr_slot(s)
			{}
			//copy constructor
			iterator(const iterator& N) : curr_slot(N.curr_slot)
			{
				curr_leaf = N.curr_leaf;
			}
			//return key
			const k_type& key() const {
				return curr_leaf->key[curr_slot];
			}

			const val_type& value() const {
				return curr_leaf->record[curr_slot];
			}

			val_type* valuep() const {
				return &curr_leaf->record[curr_slot];
			}

        	iterator& operator++(){
				if (curr_slot + 1 < curr_leaf->size) {
					++curr_slot;
				}
				else if (curr_leaf->ptr[curr_leaf->size] != NULL) {
					curr_leaf = curr_leaf->ptr[curr_leaf->size];
					curr_slot = 0;
				}
				else {
					curr_slot = curr_leaf->size;
				}

				return *this;
			};

			// postfix ++
        	iterator& operator++(int){
				iterator tmp = new iterator;
				tmp = *this;   // copy ourselves

				if (curr_slot + 1 < curr_leaf->size) {
					++curr_slot;
				}
				else if (curr_leaf->ptr[curr_leaf->MAX+1] != NULL) {
					curr_leaf = curr_leaf->ptr[curr_leaf->MAX+1];
					curr_slot = 0;
				}
				else {
					// this is end()
					curr_slot = curr_leaf->size;
				}

				return tmp;
			};

			reference operator* () const {
				return curr_leaf->key[curr_slot];
			}	

			pointer operator-> () const {
				return &curr_leaf->key[curr_slot];
			}


			bool operator== (const iterator& x) const {
				return (x.curr_leaf == curr_leaf) && (x.curr_slot == curr_slot);
			}

			//! Inequality of iterators.
			bool operator!= (const iterator& x) const {
				return (x.curr_leaf != curr_leaf) || (x.curr_slot != curr_slot);
			}
	};


	Node<k_type,val_type>* getFirstNode(Node<k_type,val_type> *root){
		if(root==NULL){
			return NULL;
		}
		if(root->IS_LEAF){
			return root;
		}
		
		return getFirstNode(root->ptr[0]);
	}

	Node<k_type,val_type>* getLastNode(Node<k_type,val_type> *root){
		if(root==NULL){
			return NULL;
		}
		if(root->IS_LEAF){
			return root;
		}
		
		return getLastNode(root->ptr[root->size]);
	}

	iterator begin() {
        return iterator(getFirstNode(root), 0);
    }

	iterator end() {
		int slot;
		Node<k_type,val_type> *last = getLastNode(root);
		if(last==NULL){
			slot = 0;
		}
		else{
			slot = last->size;
		}
        return iterator(last, slot);
    }
};

template <typename k_type, typename val_type>	
Node<k_type,val_type>::Node()
{
	//dynamic memory allocation
	MAX = def_max;
	key = new k_type[MAX];
	record = new val_type[MAX];
	ptr = new Node<k_type,val_type>*[MAX+1];
	IS_LEAF = false;
}

template <typename k_type, typename val_type>	
Node<k_type,val_type>::Node(int MAX)
{
	//dynamic memory allocation
	MAX = MAX;
	key = new k_type[MAX];
	record = new val_type[MAX];
	ptr = new Node<k_type,val_type>*[MAX+1];
	IS_LEAF = false;
}

template <typename k_type, typename val_type>	
Node<k_type,val_type>::Node(const Node& n)
{
	MAX = n.MAX;
	IS_LEAF = n.IS_LEAF;
	size = n.size;
	key = new k_type[MAX];
	record = new val_type[MAX];
	ptr = new Node<k_type,val_type>*[MAX+1];
	for(int i = 0; i < size; i++){
		key[i] = n.key[i];
	}
	for(int i = 0; i < size; i++){
		record[i] = n.record[i];
	}
}

template <typename k_type, typename val_type>	
Node<k_type,val_type>& Node<k_type,val_type>::operator=(const Node& N){
	if( this != &N)
	{
		delete[] key;	
        delete[] record;
	}
	MAX = N.MAX;
	key = new k_type[MAX];
	record = new val_type[MAX];

	for(int i = 0; i < size; i++){
		key[i] = N.key[i];
	}

	for(int i = 0; i < size; i++){
		record[i] = N.record[i];
	}
	int  i;
	for(i = 0; i < size+1; i++){
		ptr[i] = N.ptr[i];
	}
	if(i<MAX+1){
		ptr[i] = NULL;
	}
}
//default constructor
template <typename k_type, typename val_type>	
BPTree<k_type,val_type>::BPTree()
{
	root = NULL;
	MAX = def_max;

}

//constructor
template <typename k_type, typename val_type>	
BPTree<k_type,val_type>::BPTree(int n)
{
	root = NULL;
	MAX = n;

}

template <typename k_type, typename val_type>	
BPTree<k_type,val_type>::BPTree(const BPTree& T){
	if(T.root == NULL){
		root = NULL;
	}
	else{
		MAX = T.MAX;
		root = copy_recursive(T.root);
	}
}

template <typename k_type, typename val_type>	
Node<k_type,val_type>* BPTree<k_type,val_type>::copy_recursive(Node<k_type,val_type> *r){
	if( r == NULL){
		return NULL;
	}

	Node<k_type,val_type> *NewNode = new Node<k_type,val_type>(*r);

	for( int i = 0; i <= r->size; i++){
		NewNode->ptr[i] = copy_recursive(r->ptr[i]);
	}
	return NewNode;
}

template <typename k_type, typename val_type>	
val_type& BPTree<k_type,val_type>::operator[] (k_type key){

	iterator it_b;
	it_b =find(begin(),end(),key);
	if (it_b != end()){
		val_type *x = it_b.valuep();
		return *x;
	}
	else{
		throw "out_of_range";
	}
}


template <typename k_type, typename val_type>	
bool BPTree<k_type,val_type>::search(k_type x)
{
	//search logic
	if(root==NULL)
	{
		//empty
		cout<<"Tree empty\n";
	}
	else
	{
		Node<k_type,val_type>* cursor = root;
		//in the following while loop, cursor will travel to the leaf node possibly consisting the key
		while(cursor->IS_LEAF == false)
		{
			for(int i = 0; i < cursor->size; i++)
			{
				if(x < cursor->key[i])
				{
					cursor = cursor->ptr[i];
					break;
				}
				if(i == cursor->size - 1)
				{
					cursor = cursor->ptr[i+1];
					break;
				}
			}
		}
		//in the following for loop, we search for the key if it exists
		for(int i = 0; i < cursor->size; i++)
		{
			if(cursor->key[i] == x)
			{
				cout<<"Found\n";
				return true;
			}
		}
		cout<<"Not found\n";
		return false;
	}
}


template <typename k_type, typename val_type>	
void BPTree<k_type,val_type>::insert(k_type x,val_type y)
{
	//insert logic
	if(root==NULL)
	{
		root = new Node<k_type,val_type>;
		root->key[0] = x;
        root->record[0] = y;
		root->IS_LEAF = true;
		root->size = 1;
		//cout<<"Created root\nInserted "<<x<<","<<y<<" successfully\n";
	}
	else
	{
		Node<k_type,val_type>* cursor = root;
		Node<k_type,val_type>* parent;
		//in the following while loop, cursor will travel to OverfloOverflow in leaf node!w in leaf node!the leaf node possibly consisting the key
		while(cursor->IS_LEAF == false)
		{
			parent = cursor;
			for(int i = 0; i < cursor->size; i++)
			{
				if(x < cursor->key[i])
				{
					cursor = cursor->ptr[i];
					break;
				}
				if(i == cursor->size - 1)
				{
					cursor = cursor->ptr[i+1];
					break;
				}
			}
		}
		//now cursor is the leaf node in which we'll insert the new key
		if(cursor->size < MAX)
		{
			//if cursor is not full
			//find the correct position for new key
			int i = 0;
			while(x > cursor->key[i] && i < cursor->size) i++;
			//make space for new key
			for(int j = cursor->size;j > i; j--)
			{
				cursor->key[j] = cursor->key[j-1];
				cursor->record[j] = cursor->record[j-1];

			}
			cursor->key[i] = x;
			cursor->record[i] = y;

			cursor->size++;
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1];
			cursor->ptr[cursor->size-1] = NULL;
			//cout<<"Inserted "<<x<<","<<y<<" successfully\n";
		}
		else
		{
			//cout<<"Inserted "<<x<<","<<y<<" successfully\n";
			//cout<<"Overflow in leaf node!\nSplitting leaf node\n";
			//overflow condition
			//create new leaf node
			Node<k_type,val_type>* newLeaf = new Node<k_type,val_type>;
			k_type virtualNode[MAX+1];
			val_type virtualValNode[MAX+1];
			for(int i = 0; i < MAX; i++)
			{
				virtualNode[i] = cursor->key[i];
				virtualValNode[i] = cursor->record[i];
			}
			int i = 0, j;
			while(x > virtualNode[i] && i < MAX) i++;
			//make space for new key
			for(int j = MAX;j > i; j--)
			{
				virtualNode[j] = virtualNode[j-1];
				virtualValNode[j] = virtualValNode[j-1];
			}

			virtualNode[i] = x; 	
			virtualValNode[i] = y; 	

			newLeaf->IS_LEAF = true;
			//split the cursor into two leaf nodes
			cursor->size = (MAX+1)/2;
			newLeaf->size = MAX+1-(MAX+1)/2;
			//make cursor point to new leaf node
			cursor->ptr[cursor->size] = newLeaf;
			//make new leaf node point to the next leaf node
			newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
			cursor->ptr[MAX] = NULL;
			//now give elements to new leaf nodes
			for(i = 0; i < cursor->size; i++)
			{
				cursor->key[i] = virtualNode[i];
				cursor->record[i] = virtualValNode[i];
                
			}
			for(i = 0, j = cursor->size; i <= newLeaf->size; i++, j++)
			{
				newLeaf->key[i] = virtualNode[j];
				newLeaf->record[i] = virtualValNode[j];


			}
			//modify the parent
			if(cursor == root)
			{
				//if cursor is a root node, we create a new root
				Node<k_type,val_type>* newRoot = new Node<k_type,val_type>;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->record[0] = newLeaf->record[0];
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->IS_LEAF = false;
				newRoot->size = 1;
				root = newRoot;
				//cout<<"Created new root\n";
			}
			else
			{
				//insert new key in parent node
				insertInternal(newLeaf->key[0],parent,newLeaf);
			}
		}
	}
}

template<typename k_type, typename val_type>
void BPTree<k_type,val_type>::insertInternal(k_type x, Node<k_type,val_type>* cursor, Node<k_type,val_type>* child)
{
	if(cursor->size < MAX)
	{
		//if cursor is not full
		//find the correct position for new key
		int i = 0;
		while(x > cursor->key[i] && i < cursor->size) i++;
		//make space for new key
		for(int j = cursor->size;j > i; j--)
		{
			cursor->key[j] = cursor->key[j-1];

		}//make space for new pointer
		for(int j = cursor->size+1; j > i+1; j--)
		{
			cursor->ptr[j] = cursor->ptr[j-1];
		}
		cursor->key[i] = x;

		cursor->size++;
		cursor->ptr[i+1] = child;
		//cout<<"Inserted key in an Internal node successfully\n";
	}
	else
	{
		//cout<<"Inserted key in an Internal node successfully\n";
		//cout<<"Overflow in internal node!\nSplitting internal node\n";
		//if overflow in internal node
		//create new internal node
		Node<k_type,val_type>* newInternal = new Node<k_type,val_type>;
		//create virtual Internal Node;
		k_type virtualKey[MAX+1];

		Node<k_type,val_type>* virtualPtr[MAX+2];
		for(int i = 0; i < MAX; i++)
		{
			virtualKey[i] = cursor->key[i];
            
		}
		for(int i = 0; i < MAX+1; i++)
		{
			virtualPtr[i] = cursor->ptr[i];
		}
		int i = 0, j;
		while(x > virtualKey[i] && i < MAX) i++;
		//make space for new key
		for(int j = MAX;j > i; j--)
		{
			virtualKey[j] = virtualKey[j-1];
    
		}
		virtualKey[i] = x;
		//make space for new ptr
		for(int j = MAX+1;j > i+1; j--)
		{
			virtualPtr[j] = virtualPtr[j-1];
		}
		virtualPtr[i+1] = child; 
		newInternal->IS_LEAF = false;
		//split cursor into two nodes
		cursor->size = ceil((MAX+1)/2) - 1;
		newInternal->size = MAX-cursor->size;
		//give elements and pointers to the new node		
		for(i = 0, j = cursor->size+1; i < newInternal->size; i++, j++)
		{
			newInternal->key[i] = virtualKey[j];
		}
		for(i = 0, j = cursor->size+1; i < newInternal->size+1; i++, j++)
		{
			newInternal->ptr[i] = virtualPtr[j];
		}
		newInternal->ptr[i] = NULL;
		for(i = 0; i < cursor->size; i++, j++)
		{
			cursor->key[i] = virtualKey[i];

		}
		for(i = 0; i < MAX; i++, j++)
		{
			cursor->key[i] = virtualKey[i];
		}
		for(i = 0; i < MAX+1; i++, j++)
		{
			cursor->ptr[i] = virtualPtr[i];
		}
		// m = cursor->key[cursor->size]
		if(cursor == root)
		{
			//if cursor is a root node, we create a new root
			Node<k_type,val_type>* newRoot = new Node<k_type,val_type>;
			newRoot->key[0] = cursor->key[cursor->size];
			newRoot->ptr[0] = cursor;
			newRoot->ptr[1] = newInternal;
			newRoot->IS_LEAF = false;
			newRoot->size = 1;
			root = newRoot;
			//cout<<"Created new root\n";
		}
		else
		{
			//recursion
			//find depth first search to find parent of cursor
			insertInternal(cursor->key[cursor->size] , findParent(root,cursor) ,newInternal);
		}
	}
}

template<typename k_type, typename val_type>
void BPTree<k_type,val_type>::remove(k_type x)
{
	//delete logic
	if(root==NULL)
	{
		cout<<"Tree empty\n";
	}
	else
	{
		Node<k_type,val_type>* cursor = root;
		Node<k_type,val_type>* parent;
		int leftSibling, rightSibling;
		//in the following while loop, cursor will will travel to the leaf node possibly consisting the key
		while(cursor->IS_LEAF == false)
		{
			for(int i = 0; i < cursor->size; i++)
			{
				parent = cursor;
				leftSibling = i-1; //leftSibling is the index of left sibling in the parent node
				rightSibling =  i+1; //rightSibling is the index of right sibling in the parent node
				if(x < cursor->key[i])
				{	
					cursor = cursor->ptr[i];
					break;
				}
				if(i == cursor->size - 1)
				{
					leftSibling = i;
					rightSibling = i+2;
					cursor = cursor->ptr[i+1];
					break;
				}
			}
		}
		//in the following for loop, we search for the key if it exists
		bool found = false;
		int pos;
		for(pos = 0; pos < cursor->size; pos++)
		{
			if(cursor->key[pos] == x)
			{
				found = true;
				break;
			}
		}
		if(!found)//if key does not exist in that leaf node
		{
			//cout<<"Not found\n";
			return;
		}
		//deleting the key
		for(int i = pos; i < cursor->size; i++)
		{
			if(i<cursor->MAX-1){

				cursor->key[i] = cursor->key[i+1];
				cursor->record[i] = cursor->record[i+1];
			}

		}
		cursor->size--;
		if(cursor == root)//if it is root node, then make all pointers NULL
		{
			//cout<<"Deleted "<<x<<" from leaf node successfully\n";
			for(int i = 0; i < MAX+1; i++)
			{
				cursor->ptr[i] = NULL;
			}
			if(cursor->size == 0)//if all keys are deleted
			{
				//cout<<"Tree died\n";
				delete[] cursor->key;
				delete[] cursor->record;
				delete[] cursor->ptr;
				delete cursor;
				root = NULL;
			}
			return;
		}
		cursor->ptr[cursor->size] = cursor->ptr[cursor->size+1];
		cursor->ptr[cursor->size+1] = NULL;
		//cout<<"Deleted "<<x<<" from leaf node successfully\n";
		if(cursor->size >= (MAX+1)/2)//no underflow
		{
			return;
		}
		//cout<<"Underflow in leaf node!\n";
		//underflow condition
		//first we try to transfer a key from sibling node
		//check if left sibling exists
		if(leftSibling >= 0)
		{
			Node<k_type,val_type> *leftNode = parent->ptr[leftSibling];
			//check if it is possible to transfer
			if(leftNode->size >= (MAX+1)/2+1)
			{
				//make space for transfer
				for(int i = cursor->size; i > 0; i--)
				{
					cursor->key[i] = cursor->key[i-1];
					cursor->record[i] = cursor->record[i-1];

				}
				//shift pointer to next leaf
				cursor->size++;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1];
				cursor->ptr[cursor->size-1] = NULL;
				//transfer
				cursor->key[0] = leftNode->key[leftNode->size-1];
				cursor->record[0] = leftNode->key[leftNode->size-1];

				//shift pointer of leftsibling
				leftNode->size--;
				leftNode->ptr[leftNode->size] = cursor;
				leftNode->ptr[leftNode->size+1] = NULL;
				//update parent
				parent->key[leftSibling] = cursor->key[0];
				parent->record[leftSibling] = cursor->record[0];

				// cout<<"Transferred "<<cursor->key[0]<<" from left sibling of leaf node\n";
				return;
			}
		}
		if(rightSibling <= parent->size)//check if right sibling exist
		{
			Node<k_type,val_type> *rightNode = parent->ptr[rightSibling];
			//check if it is possible to transfer
			if(rightNode->size >= (MAX+1)/2+1)
			{
				//shift pointer to next leaf
				cursor->size++;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1];
				cursor->ptr[cursor->size-1] = NULL;
				//transfer
				cursor->key[cursor->size-1] = rightNode->key[0];
				//shift pointer of rightsibling
				rightNode->size--;
				rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size+1];
				rightNode->ptr[rightNode->size+1] = NULL;
				//shift conent of right sibling
				for(int i = 0; i < rightNode->size; i++)
				{
					rightNode->key[i] = rightNode->key[i+1];
					rightNode->record[i] = rightNode->record[i+1];

				}
				//update parent
				parent->key[rightSibling-1] = rightNode->key[0];
				parent->record[rightSibling-1] = rightNode->record[0];

				// cout<<"Transferred "<<cursor->key[cursor->size-1]<<" from right sibling of leaf node\n";
				return;
			}
		}
		//must merge and delete a node
		if(leftSibling >= 0)//if left sibling exist
		{
			Node<k_type,val_type>* leftNode = parent->ptr[leftSibling];
			// transfer all keys to leftsibling and then transfer pointer to next leaf node
			for(int i = leftNode->size, j = 0; j < cursor->size; i++, j++)
			{
				leftNode->key[i] = cursor->key[j];
				leftNode->record[i] = cursor->record[j];

			}
			leftNode->ptr[leftNode->size] = NULL;
			leftNode->size += cursor->size;
			leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];
			//cout<<"Merging two leaf nodes\n";
			removeInternal(parent->key[leftSibling],parent,cursor);// delete parent node key
			delete[] cursor->key;
			delete[] cursor->record;
			delete[] cursor->ptr;
			delete cursor;
		}
		else if(rightSibling <= parent->size)//if right sibling exist
		{
			Node<k_type,val_type>* rightNode = parent->ptr[rightSibling];
			// transfer all keys to cursor and then transfer pointer to next leaf node
			for(int i = cursor->size, j = 0; j < rightNode->size; i++, j++)
			{
				cursor->key[i] = rightNode->key[j];
				cursor->record[i] = rightNode->record[j];
            
			}
			cursor->ptr[cursor->size] = NULL;
			cursor->size += rightNode->size;
			cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
			//cout<<"Merging two leaf nodes\n";
			removeInternal(parent->key[rightSibling-1],parent,rightNode);// delete parent node key
			delete[] rightNode->key;
			delete[] rightNode->record;
			delete[] rightNode->ptr;
			delete rightNode;
		}
	}
}

template<typename k_type, typename val_type>
void BPTree<k_type,val_type>::removeInternal(k_type x, Node<k_type,val_type>* cursor, Node<k_type,val_type>* child)
{
	//deleting the key x first
	//checking if key from root is to be deleted
	if(cursor == root)
	{
		if(cursor->size == 1)//if only one key is left, change root
		{
			if(cursor->ptr[1] == child)
			{
				delete[] child->key;
				delete[] child->record;
				delete[] child->ptr;
				delete child;
				root = cursor->ptr[0];
				delete[] cursor->key;
				delete[] cursor->record;
				delete[] cursor->ptr;
				delete cursor;
				//cout<<"Changed root node\n";
				return;
			}
			else if(cursor->ptr[0] == child)
			{
				delete[] child->key;
				delete[] child->record;
				delete[] child->ptr;
				delete child;
				root = cursor->ptr[1];
				delete[] cursor->key;
				delete[] cursor->record;
				delete[] cursor->ptr;
				delete cursor;
				//cout<<"Changed root node\n";
				return;
			}
		}
	}
	int pos;
	for(pos = 0; pos < cursor->size; pos++)
	{
		if(cursor->key[pos] == x)
		{
			break;
		}
	}
	for(int i = pos; i < cursor->size; i++)
	{
		cursor->key[i] = cursor->key[i+1];
		cursor->record[i] = cursor->record[i+1];

	}
	//now deleting the pointer child
	for(pos = 0; pos < cursor->size+1; pos++)
	{
		if(cursor->ptr[pos] == child)
		{
			break;
		}
	}
	for(int i = pos; i < cursor->size+1; i++)
	{
		cursor->ptr[i] = cursor->ptr[i+1];
	}
	cursor->size--;
	if(cursor->size >= (MAX+1)/2-1)//no underflow
	{
		//cout<<"Deleted "<<x<<" from internal node successfully\n";
		return;
	}
	//cout<<"Underflow in internal node!\n";
	//underflow, try to transfer first
	if(cursor==root)return;
	Node<k_type,val_type>* parent = findParent(root, cursor);
	int leftSibling, rightSibling;
	//finding left n right sibling of cursor
	for(pos = 0; pos < parent->size+1; pos++)
	{
		if(parent->ptr[pos] == cursor)
		{
			leftSibling = pos - 1;
			rightSibling = pos + 1;
			break;
		}
	}
	//try to transfer
	if(leftSibling >= 0)//if left sibling exists
	{
		Node<k_type,val_type> *leftNode = parent->ptr[leftSibling];
		//check if it is possible to transfer
		if(leftNode->size >= (MAX+1)/2)
		{
			//make space for transfer of key
			for(int i = cursor->size; i > 0; i--)
			{
				cursor->key[i] = cursor->key[i-1];
				cursor->record[i] = cursor->record[i-1];

			}
			//transfer key from left sibling through parent
			cursor->key[0] = parent->key[leftSibling];
			cursor->record[0] = parent->record[leftSibling];

			parent->key[leftSibling] = leftNode->key[leftNode->size-1];
			parent->record[leftSibling] = leftNode->record[leftNode->size-1];

			//transfer last pointer from leftnode to cursor
			//make space for transfer of ptr
			for (int i = cursor->size+1; i > 0; i--)
			{
				cursor->ptr[i] = cursor->ptr[i-1];
			}
			//transfer ptr
			cursor->ptr[0] = leftNode->ptr[leftNode->size];
			cursor->size++;
			leftNode->size--;
			//cout<<"Transferred "<<cursor->key[0]<<" from left sibling of internal node\n";
			return;
		}
	}
	if(rightSibling <= parent->size)//check if right sibling exist
	{
		Node<k_type,val_type> *rightNode = parent->ptr[rightSibling];
		//check if it is possible to transfer
		if(rightNode->size >= (MAX+1)/2)
		{
			//transfer key from right sibling through parent
			cursor->key[cursor->size] = parent->key[pos];
			cursor->record[cursor->size] = parent->record[pos];


			parent->key[pos] = rightNode->key[0];
			parent->record[pos] = rightNode->record[0];

			for (int i = 0; i < rightNode->size -1; i++)
			{
				rightNode->key[i] = rightNode->key[i+1];
				rightNode->record[i] = rightNode->record[i+1];
            
			}
			//transfer first pointer from rightnode to cursor
			//transfer ptr
			cursor->ptr[cursor->size+1] = rightNode->ptr[0];
			for (int i = 0; i < rightNode->size; ++i)
			{
				rightNode->ptr[i] = rightNode->ptr[i+1];
			}
			cursor->size++;
			rightNode->size--;
			//cout<<"Transferred "<<cursor->key[0]<<" from right sibling of internal node\n";
			return;
		}
	}
	//transfer wasnt posssible hence do merging
	if(leftSibling >= 0)
	{
		//leftnode + parent key + cursor
		Node<k_type,val_type> *leftNode = parent->ptr[leftSibling];
		leftNode->key[leftNode->size] = parent->key[leftSibling];
		leftNode->record[leftNode->size] = parent->record[leftSibling];
    
		for(int i = leftNode->size+1, j = 0; j < cursor->size; j++)
		{
			leftNode->key[i] = cursor->key[j];
			leftNode->record[i] = cursor->record[j];

		}
		for(int i = leftNode->size+1, j = 0; j < cursor->size+1; j++)
		{
			leftNode->ptr[i] = cursor->ptr[j];
			cursor->ptr[j] = NULL;
		}
		leftNode->size += cursor->size+1;
		cursor->size = 0;
		//delete cursor
		removeInternal(parent->key[leftSibling], parent, cursor);
		//cout<<"Merged with left sibling\n";

	}
	else if(rightSibling <= parent->size)
	{
		//cursor + parent key + rightnode
		Node<k_type,val_type> *rightNode = parent->ptr[rightSibling];
		cursor->key[cursor->size] = parent->key[rightSibling-1];
		for(int i = cursor->size+1, j = 0; j < rightNode->size; j++)
		{
			cursor->key[i] = rightNode->key[j];
			cursor->record[i] = rightNode->record[j];

		}
		for(int i = cursor->size+1, j = 0; j < rightNode->size+1; j++)
		{
			cursor->ptr[i] = rightNode->ptr[j];
			rightNode->ptr[j] = NULL;
		}
		cursor->size += rightNode->size+1;
		rightNode->size = 0;
		//delete cursor
		removeInternal(parent->key[rightSibling-1], parent, rightNode);
		//cout<<"Merged with right sibling\n";
	}
}


template<typename k_type, typename val_type>
Node<k_type,val_type>* BPTree<k_type,val_type>::findParent(Node<k_type,val_type>* cursor, Node<k_type,val_type>* child)
{
	//finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
	//also ignores second last level because we will never find parent of a leaf node during insertion using this function
	Node<k_type,val_type>* parent;
	if(cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF)
	{
		return NULL;
	}
	for(int i = 0; i < cursor->size+1; i++)
	{
		if(cursor->ptr[i] == child)
		{
			parent = cursor;
			return parent;
		}
		else
		{
			parent = findParent(cursor->ptr[i],child);
			if(parent!=NULL)return parent;
		}
	}
	return parent;
}

template<typename k_type, typename val_type>
void BPTree<k_type,val_type>::display(Node<k_type,val_type>* cursor)
{
	//depth first display
	if(cursor!=NULL)
	{
		if(cursor->IS_LEAF == true)
		{
			cout << "L  ";
		}
		for(int i = 0; i < cursor->size; i++)
		{
			if(cursor->IS_LEAF == true)
			{
				cout<<cursor->key[i]<<","<<cursor->record[i]<<"  ";
			}
			else{
				cout<<cursor->key[i] << " ";
			}
		}
		cout<<"\n";
		if(cursor->IS_LEAF != true)
		{
			for(int i = 0; i < cursor->size+1; i++)
			{
				display(cursor->ptr[i]);
			}
		}
	}

}

template<typename k_type, typename val_type>
void BPTree<k_type,val_type>::display_tree(){
	cout << "______________________________________\n";
	display(getRoot());
	cout << "______________________________________\n";

}


template<typename k_type, typename val_type>
Node<k_type,val_type>* BPTree<k_type,val_type>::getRoot()
{
	return root;
}

template<typename k_type, typename val_type>
void BPTree<k_type,val_type>::cleanUp(Node<k_type,val_type>* cursor)
{
	//clean up logic
	if(cursor!=NULL)
	{
		if(cursor->IS_LEAF != true)
		{
			for(int i = 0; i < cursor->size+1; i++)
			{
				cleanUp(cursor->ptr[i]);
			}
		}
		for(int i = 0; i < cursor->size; i++)
		{
			//cout<<"Deleted key from memory: "<<cursor->key[i]<<"\n";
		}
		delete[] cursor->key;
        delete[] cursor->record;
		delete[] cursor->ptr;
		delete cursor;
	}
}

template<typename k_type, typename val_type>
BPTree<k_type,val_type>::~BPTree()
{
	//calling cleanUp routine
	cleanUp(root);
}

int main(){
	BPTree<string,string> bpt(3);
	BPTree<string,string> bpt3;

	//B+ tree object that carries out all the operations
    bpt.insert("1","amk");
    bpt.insert("2","b");
    bpt.insert("3","c");
    bpt.insert("4a","d");
    bpt.insert("5","e");
    bpt.insert("6","f");
	bpt.insert("7","ggg");
	bpt.insert("8","a");
	bpt.insert("9","hi");
	bpt.insert("10","hello");
	bpt.insert("11","hi");
	bpt.insert("12","C++");
    // bpt.remove("5");
	
	bpt["5"] = "sg";
	//cout << bpt["5"]<<endl;

    bpt.display_tree();	

	bpt.remove("11");
	bpt.remove("12");

    bpt.display_tree();

	BPTree<string,string>::iterator it_b;

	it_b =find(bpt.begin(),bpt.end(),"10");

	if (it_b != bpt.end()){
		cout << *it_b<< endl;
	}

	replace(bpt.begin(),bpt.end(),string("9"),string("9_new"));
    bpt.display_tree();

	it_b = upper_bound(bpt.begin(),bpt.end(),"7");
	cout << *it_b<<endl;

	it_b = lower_bound(bpt.begin(),bpt.end(),"6");
	cout << *it_b<<endl;

	// bpt2 = bpt;
	// bpt2.display_tree();
	// BPTreebpt2 = bpt;
	BPTree<string,string> bpt2 = bpt;
	bpt2.display_tree();

	bpt3 = bpt;
}