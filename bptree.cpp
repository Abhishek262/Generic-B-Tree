#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<climits>
using namespace std;
int MAX; //size of each node

//find, copy and replace STL functions : iterator
//make data stored generic and its pointers
template<typename k, typename v>
class BPTree;

template <typename key_type, typename val_type>
class Node
{
	bool IS_LEAF;
	key_type *key;
	int size;
	Node<key_type,val_type>** ptr;
	friend class BPTree<key_type,val_type>;
public:
	Node();
};

template <typename key_type = int, typename val_type = int>	
class BPTree
{
	Node<key_type,val_type> *root;
	void insertInternal(val_type,Node<key_type,val_type>*,Node<key_type,val_type>*);
	// void removeInternal(int,Node*,Node*);
	Node<key_type,val_type>* findParent(Node<key_type,val_type>*,Node<key_type,val_type>*);
public:
	BPTree();
	void search(key_type);
	void insert(val_type);
	// void remove(int);
	void display(Node<key_type,val_type>*);
	Node<key_type,val_type>* getRoot();
	void cleanUp(Node<key_type,val_type>*);
	~BPTree(); 
};
template <typename key_type, typename val_type>	
Node<key_type,val_type>::Node()
{
	//dynamic memory allocation
	key = new key_type[MAX];
	ptr = new Node<key_type,val_type>*[MAX+1];
}

template <typename key_type, typename val_type>	
BPTree<key_type,val_type>::BPTree()
{
	root = NULL;
}
template <typename key_type, typename val_type>	
void BPTree<key_type,val_type>::search(key_type x)
{
	//search logic
	if(root==NULL)
	{
		//empty
		cout<<"Tree empty\n";
	}
	else
	{
		Node<key_type,val_type>* cursor = root;
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
				return;
			}
		}
		cout<<"Not found\n";
	}
}

template <typename key_type, typename val_type>	
void BPTree<key_type,val_type>::insert(val_type x)
{
	//insert logic
	if(root==NULL)
	{
		root = new Node<key_type,val_type>;
		root->key[0] = x;
		root->IS_LEAF = true;
		root->size = 1;
		// cout<<"Created root\nInserted "<<x<<" successfully\n";
	}
	else
	{
		Node<key_type,val_type>* cursor = root;
		Node<key_type,val_type>* parent;
		//in the following while loop, cursor will travel to the leaf node possibly consisting the key
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
			}
			cursor->key[i] = x;
			cursor->size++;
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1];
			cursor->ptr[cursor->size-1] = NULL;
			cout<<"Inserted 148 "<<x<<" successfully\n";
		}
		else
		{
			cout<<"Inserted 152 "<<x<<" successfully\n";
			cout<<"Overflow in leaf node!\nSplitting leaf node\n";
			//overflow condition
			//create new leaf node
			Node<key_type,val_type>* newLeaf = new Node<key_type,val_type>;
			//create a virtual node and insert x into it
			key_type virtualNode[MAX+1];
			for(int i = 0; i < MAX; i++)
			{
				virtualNode[i] = cursor->key[i];
			}
			int i = 0, j;
			while(x > virtualNode[i] && i < MAX) i++;
			//make space for new key
			for(int j = MAX+1;j > i; j--)
			{
				virtualNode[j] = virtualNode[j-1];
			}
			virtualNode[i] = x; 
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
			}
			for(i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
			{
				newLeaf->key[i] = virtualNode[j];
			}
			//modify the parent
			if(cursor == root)
			{
				//if cursor is a root node, we create a new root
				Node<key_type,val_type>* newRoot = new Node<key_type,val_type>;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->IS_LEAF = false;
				newRoot->size = 1;
				root = newRoot;
				cout<<"Created new root\n";
			}
			else
			{
				//insert new key in parent node
				insertInternal(newLeaf->key[0],parent,newLeaf);
			}
		}
	}
}

template<typename key_type, typename val_type>
void BPTree<key_type,val_type>::insertInternal(val_type x, Node<key_type,val_type>* cursor, Node<key_type,val_type>* child)
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
		cout<<"Inserted key in an Internal node successfully\n";
	}
	else
	{
		cout<<"Inserted key in an Internal node successfully\n";
		cout<<"Overflow in internal node!\nSplitting internal node\n";
		//if overflow in internal node
		//create new internal node
		Node<key_type,val_type>* newInternal = new Node<key_type,val_type>;
		//create virtual Internal Node;
		key_type virtualKey[MAX+1];
		Node<key_type,val_type>* virtualPtr[MAX+2];
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
		for(int j = MAX+1;j > i; j--)
		{
			virtualKey[j] = virtualKey[j-1];
		}
		virtualKey[i] = x; 
		//make space for new ptr
		for(int j = MAX+2;j > i+1; j--)
		{
			virtualPtr[j] = virtualPtr[j-1];
		}
		virtualPtr[i+1] = child; 
		newInternal->IS_LEAF = false;
		//split cursor into two nodes
		cursor->size = (MAX+1)/2;
		newInternal->size = MAX-(MAX+1)/2;
		//give elements and pointers to the new node
		for(i = 0, j = cursor->size+1; i < newInternal->size; i++, j++)
		{
			newInternal->key[i] = virtualKey[j];
		}
		for(i = 0, j = cursor->size+1; i < newInternal->size+1; i++, j++)
		{
			newInternal->ptr[i] = virtualPtr[j];
		}
		// m = cursor->key[cursor->size]
		if(cursor == root)
		{
			//if cursor is a root node, we create a new root
			Node<key_type,val_type>* newRoot = new Node<key_type,val_type>;
			newRoot->key[0] = cursor->key[cursor->size];
			newRoot->ptr[0] = cursor;
			newRoot->ptr[1] = newInternal;
			newRoot->IS_LEAF = false;
			newRoot->size = 1;
			root = newRoot;
			cout<<"Created new root\n";
		}
		else
		{
			//recursion
			//find depth first search to find parent of cursor
			insertInternal(cursor->key[cursor->size] ,findParent(root,cursor) ,newInternal);
		}
	}
}

template<typename key_type, typename val_type>
Node<key_type,val_type>* BPTree<key_type,val_type>::findParent(Node<key_type,val_type>* cursor, Node<key_type,val_type>* child)
{
	//finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
	//also ignores second last level because we will never find parent of a leaf node during insertion using this function
	Node<key_type,val_type>* parent;
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

template<typename key_type, typename val_type>
void BPTree<key_type,val_type>::display(Node<key_type,val_type>* cursor)
{
	//depth first display
	if(cursor!=NULL)
	{
		for(int i = 0; i < cursor->size; i++)
		{
			cout<<cursor->key[i]<<" ";
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

template<typename key_type, typename val_type>
Node<key_type,val_type>* BPTree<key_type,val_type>::getRoot()
{
	return root;
}

template<typename key_type, typename val_type>
void BPTree<key_type,val_type>::cleanUp(Node<key_type,val_type>* cursor)
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
			cout<<"Deleted key from memory: "<<cursor->key[i]<<"\n";
		}
		delete[] cursor->key;
		delete[] cursor->ptr;
		delete cursor;
	}
}

template<typename key_type, typename val_type>
BPTree<key_type,val_type>::~BPTree()
{
	//calling cleanUp routine
	cleanUp(root);
}


//give command line argument to load a tree from log
//to create a fresh tree, do not give any command line argument
int main(int argc, char* argv[])
{
	BPTree<double,double> bpt;//B+ tree object that carries out all the operations
	string command;
	int x;
	bool close = false;
	string logBuffer;//used to save into log
	ifstream fin;
	ofstream fout;
	//create tree from log file from command line input
	if(argc > 1)
	{
		fin.open(argv[1]);//open file
		if(!fin.is_open())
		{
			cout<<"File not found\n";
			return 0;
		}
		int i = 1;
		getline(fin, logBuffer, '\0');//copy log from file to logBuffer for saving purpose
		fin.close();
		fin.open(argv[1]);//reopening file
		getline(fin,command);
		stringstream max(command);//first line of log contains the max degree
		max>>MAX;
		while(getline(fin,command))//iterating over every line ie command
		{
			if(!command.substr(0,6).compare("insert"))
			{
				stringstream argument(command.substr(7));
				argument>>x;
				bpt.insert(x);
			}
			// else if(!command.substr(0,6).compare("delete"))
			// {
			// 	stringstream argument(command.substr(7));
			// 	argument>>x;
			// 	bpt.remove(x);
			// }
			else
			{
				cout<<"Unknown command: "<<command<<" at line #"<<i<<"\n";
				return 0;
			}
			i++;
		}
		cout<<"Tree loaded successfully from: \""<<argv[1]<<"\"\n";
		fin.close();
	}
	else//create fresh tree
	{
		cout<<"Enter the max degree\n";
		cin>>command;
		stringstream max(command);
		max>>MAX;
		logBuffer.append(command);
		logBuffer.append("\n");
		cin.clear();
		cin.ignore(1);
	}
	//command line menu
	cout<<"Commands:\nsearch <value> to search\n";
	cout<<"insert <value> to insert\n";
	// cout<<"delete <value> to delete\n";
	cout<<"display to display\n";
	cout<<"save to save log\n";
	cout<<"exit to exit\n";
	do
	{
		cout<<"Enter command: ";
		getline(cin,command);
		if(!command.substr(0,6).compare("search"))
		{
			stringstream argument(command.substr(7));
			argument>>x;
			bpt.search(x);
		}
		else if(!command.substr(0,6).compare("insert"))
		{
			stringstream argument(command.substr(7));
			argument>>x;
			bpt.insert(x);
			logBuffer.append(command);
			logBuffer.append("\n");
		}
		// else if(!command.substr(0,6).compare("delete"))
		// {
		// 	stringstream argument(command.substr(7));
		// 	argument>>x;
		// 	bpt.remove(x);
		// 	logBuffer.append(command);
		// 	logBuffer.append("\n");
		// }
		else if(!command.compare("display"))
		{
			bpt.display(bpt.getRoot());
		}
		else if(!command.compare("save"))
		{
			cout<<"Enter file name: ";
			string filename;
			cin>>filename;
			fout.open(filename);
			fout<<logBuffer;
			fout.close();
			cout<<"Saved successfully into file: \""<<filename<<"\"\n";
			cin.clear();
			cin.ignore(1);
		}
		else if(!command.compare("exit"))
		{
			close = true;
		}
		else
		{
			cout<<"Invalid command\n";
		}
	}while(!close);
	return 0;
}