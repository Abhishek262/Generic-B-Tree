#include <iostream>
#include <string>
#include "bptree.hpp"

class newType
{
public:
    int val;
    newType(int n = 0){
        val = n;
    }
    newType(const newType &rhs){
        val = rhs.val;
    }

    bool operator==(const newType &rhs){
        return val == rhs.val;
    }

    bool operator>(newType &rhs){
        return val > rhs.val;
    }


    friend bool operator==(const newType &lhs, const newType &rhs){
        return lhs.val == rhs.val;
    }

    int operator*(){
        return val;
    }

    friend bool operator>(const newType &lhs, const newType &rhs){
        // cout << "> is called\n";
        return lhs.val > rhs.val;
    }

    friend bool operator<(const newType &lhs, const newType &rhs){
        return lhs.val < rhs.val;
    }

    friend ostream &operator<<(ostream &os, const newType &m){
        os << m.val;
        return os;
    }
};


struct compareMod{
	bool operator()(newType& x1, newType& x2){
		// return (x1%10) > (x2%10);
		return x1.val%10 >   x2.val%10;	
	}
};

int main(){
	BPTree<newType,newType,compareMod> bpt(3);
	// BPTree<string,string> bpt3;

	//B+ tree object that carries out all the operations

    bpt.insert(newType(6),newType(8));
    bpt.insert(newType(16),newType(28));
    bpt.insert(newType(12),newType(18));

    bpt.insert(newType(32),newType(19));
    bpt.insert(newType(22),newType(20));
    bpt.insert(newType(72),newType(21));
    bpt.insert(newType(34),newType(22));

    bpt.insert(newType(62),newType(23));

    // bpt.insert("2","b");

    // bpt.insert("3","c");
    // bpt.insert("4a","d");
    // bpt.insert("5","e");
    // bpt.insert("6","f");
	// bpt.insert("7","ggg");
	// bpt.insert("8","a");
	// bpt.insert("9","hi");
	// bpt.insert("10","hello");
	// bpt.insert("11","hi"); 
	// bpt.insert("12","C++");
    // // bpt.remove("5");
	
	// bpt["5"] = "sg";
	// //cout << bpt["5"]<<endl;

    bpt.display_tree();	

	// bpt.remove("11");
	// bpt.remove("12");

    // bpt.display_tree();

	// BPTree<string,string,greater<string>>::iterator it_b;

	// it_b =find(bpt.begin(),bpt.end(),"10");

	// if (it_b != bpt.end()){
	// 	cout << *it_b<< endl;
	// }

	// replace(bpt.begin(),bpt.end(),string("9"),string("9_new"));
    // bpt.display_tree();

	// it_b = upper_bound(bpt.begin(),bpt.end(),"7");
	// cout << *it_b<<endl;

	// it_b = lower_bound(bpt.begin(),bpt.end(),"6");
	// cout << *it_b<<endl;

	// bpt2 = bpt;
	// bpt2.display_tree();
	// BPTreebpt2 = bpt;

}