#include <iostream>
#include <string>
#include "bptree.hpp"

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
	bpt3.display_tree();
}