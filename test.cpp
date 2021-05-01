#include <iostream>
#include <string>
#include "bptree.hpp"

int main(){
    BPTree<string,string> bpt;//B+ tree object that carries out all the operations
    bpt.insert("1");
    bpt.insert("2");
    bpt.insert("3");
    bpt.insert("4");
    bpt.insert("5");
    bpt.insert("6");

    bpt.display_tree();

    // BPTree<string,string> bpt.begin()    




}