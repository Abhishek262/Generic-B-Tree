B+ Tree
========

Implementation of a generic B+ Tree data structure in C++

A B+ tree is an m-ary tree with a variable but often large number of children per node. A B+ tree consists of a root, internal nodes and leaves. The root may be either a leaf or a node with two or more children.

A B+ tree can be viewed as a B-tree in which each node contains only keys (not key–value pairs), and to which an additional level is added at the bottom with linked leaves. B+ Tree is an ordered and associative container where each value is associated with their key and not the position in the B+ tree/

This implementation is an in-memory representation of the B+ tree. It has methods for insertion, deletion and searching for the B+ tree.



Implementation
---------------

bptree.hpp contains the implementation of the generic B+ Tree data structure.
The container has a forward iterator which only traverses through leaf nodes and can be used with STL algorithms library.
The B+ Tree works similarly to an STL map but the value are stored only in the leaf nodes and the keys stored in non-leaf nodes are used to traverse teh tree.


Template structure
--------------------
```
template <typename k_type = int, typename val_type = int,class Compare = std::less<k_type>> 
```

How to use
-------------
Include the bptree.hpp file which  has the bptree structure and all the functions related to it.

```
#include "bptree.hpp"
Using namespace std;

g++ test.cpp -o bptree
./bptree
```


Example usage
---------------

```
   BPTree<string,string> bpt(3);  <br>
   bpt.insert("2","b"); <br>
   bpt.insert("3","c"); <br>
   bpt.remove("5"); <br>
   bpt.display_tree(); <br>
 
   it_b =find(bpt.begin(),bpt.end(),"10");
 
   if (it_b != bpt.end()){
       cout << *it_b<< endl;
   }
```

Member Types
--------------

|Member_type				|	definition                   |
|---------------------------|--------------------------------|
|K_type					    |	key type for the B+ tree     |
|Val_type				    |	record type for the B+ tree  |
|pointer |k_type* |
|reference |K_type&|
|iterator| A forward iterator which returns a pointer to node and |position of key in node|
|forward_iterator_tag |iterator_category|
|difference_type |ptrdiff_t|
|key_compare |Compare|
|value_compare |Compare|

<br>
Member Functions
----------------- 

|Member_type				|	definition                   |
|---------------------------|--------------------------------|
|Constructor BPTree() |Constructs the B+ Tree|
|Destructor ~BPTree() |Destructs the B+ Tree|
|Copy Constructor BPTree(const BPTree& T); |Copy Constructor|
|copy_recursive |Helper function used by assignment operator and copy |constructor|
|operator= |Assignment operator|
|display_tree |Displays the complete B+ Tree|
|getRoot() |Helper function used that returns the root node.|

Modifiers
-----------

|Modifier				    |	definition                   |
|---------------------------|--------------------------------|
insert(key, record)  |  Inserts a key and a record pair to the tree|
insertInternal(key,record)  |  Helper function for insert(). Implements insertion in internal nodes|
remove(key)  |  Removes specified key from the tree|
removeInternal(key)  |  Helper function for remove(). Implements removal in internal nodes|
cleanUp()  |  Removes all key-record pairs from the tree |
operator[key]  |  Returns a reference to the record value pertaining to a particular key. Use this to modify record values for a given key|

Lookup
-----------
 * Search(key) : Returns a boolean value specifying if the key is found


Iterator Access
------------------

* Begin() : Return an iterator to the starting leaf node
* End() : Return an iterator to the ending leaf node

