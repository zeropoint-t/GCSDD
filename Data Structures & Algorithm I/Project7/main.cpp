#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <queue>

#include "BinaryNode.h"
#include "BinaryTree.h"

using namespace std;

template<class K, class V>
void visit(K& key, V& value){
    cout << "Key: " << key << endl;
}



int main(){
    ///////////////////////////////////////
    //--------Left Unbalanced Tree--------
    ///////////////////////////////////////
    //     6
    //   4   7
    // 2   5
    //1  3
    cout << "*******************************" << endl;
    cout << "-----Left Unbalanced Tree-----" << endl;
    cout << "*******************************" << endl;
    cout << "     6" << endl;
    cout << "   4   7" << endl;
    cout << " 2   5" << endl;
    cout << "1  3" << endl;
    BinaryTree<int,int> bt;
    bt.add(6,0);
    bt.add(4,0);
    bt.add(7,0);
    bt.add(5,0);
    bt.add(2,0);
    bt.add(3,0);
    bt.add(1,0);

    cout << "Preorder Traversal: " << endl;
    bt.preorderTraversal(visit);
    cout << endl;

    cout << "Inorder Traversal: " << endl;
    bt.inorderTraversal(visit);
    cout << endl;

    cout << "Postorder Traversal: " << endl;
    bt.postorderTraversal(visit);
    cout << endl;

    cout << "BreadthFirstSearch Traversal: " << endl;
    bt.breadthFirstSearch(visit);
    cout << endl;

    cout << "DepthFirstSearch Traversal: " << endl;
    bt.depthFirstSearch(visit);
    cout << endl;
    
    bt.clear();

    cout << endl;

    ///////////////////////////////////////
    //--------Right Unbalanced Tree--------
    ///////////////////////////////////////
    //     2
    //   1   4
    //      3  6
    //        5  7
    cout << "*******************************" << endl;
    cout << "-----Right Unbalanced Tree-----" << endl;
    cout << "*******************************" << endl;
    cout << "     2" << endl;
    cout << "   1   4" << endl;
    cout << "      3  6" << endl;
    cout << "        5  7" << endl;
    bt.add(2,0);
    bt.add(1,0);
    bt.add(4,0);
    bt.add(3,0);
    bt.add(6,0);
    bt.add(5,0);
    bt.add(7,0);

    cout << "Preorder Traversal: " << endl;
    bt.preorderTraversal(visit);
    cout << endl;

    cout << "Inorder Traversal: " << endl;
    bt.inorderTraversal(visit);
    cout << endl;

    cout << "Postorder Traversal: " << endl;
    bt.postorderTraversal(visit);
    cout << endl;

    cout << "BreadthFirstSearch Traversal: " << endl;
    bt.breadthFirstSearch(visit);
    cout << endl;

    cout << "DepthFirstSearch Traversal: " << endl;
    bt.depthFirstSearch(visit);
    cout << endl;

    bt.clear();

    cout << endl;
    
    ///////////////////////////////////////
    //--------Balanced Tree--------
    ///////////////////////////////////////
    //       4
    //   2       6
    // 1   3   5   7
    cout << "************************" << endl;
    cout << "-----Balanced Tree-----" << endl;
    cout << "************************" << endl;
    cout << "       4" << endl;
    cout << "   2       6" << endl;
    cout << " 1   3   5   7" << endl;
    bt.add(4,0);
    bt.add(2,0);
    bt.add(1,0);
    bt.add(3,0);
    bt.add(6,0);
    bt.add(5,0);
    bt.add(7,0);

    cout << "Preorder Traversal: " << endl;
    bt.preorderTraversal(visit);
    cout << endl;

    cout << "Inorder Traversal: " << endl;
    bt.inorderTraversal(visit);
    cout << endl;

    cout << "Postorder Traversal: " << endl;
    bt.postorderTraversal(visit);
    cout << endl;

    cout << "BreadthFirstSearch Traversal: " << endl;
    bt.breadthFirstSearch(visit);
    cout << endl;

    cout << "DepthFirstSearch Traversal: " << endl;
    bt.depthFirstSearch(visit);
    cout << endl;

    bt.clear();

    cout << endl << endl;
    

    cout << "***********************************************************" << endl;
    cout << "Demonstration of using a binary tree to compress a message" << endl;
    cout << "***********************************************************" << endl;
    //step 1  create a string to compress
    string origMsg = "This is a demonstration of using a binary tree to compress a message using Hoffman Code!!";//"SUSIE SAYS IT IS EASY\n";

    //step 2 - create a frequency table and save the unique characters
    map<char,int> frequencyMap;
    for(auto it = origMsg.begin(); it != origMsg.end(); it++){
        auto c = frequencyMap.find(*it);
        if(c != frequencyMap.end()){
            frequencyMap[*it] += 1;
        }else{
            frequencyMap.insert(pair<char,int>(*it,1));
        }
    }

    int numOfAlphabets = frequencyMap.size();
    char alphabets[numOfAlphabets];
    int cnt = 0;
    for(auto it = frequencyMap.begin(); it != frequencyMap.end(); it++){
        alphabets[cnt++] = (*it).first;
    }

    //step 3 - create a node for each alphabet and put them into a priority queue & save the number of alphabets
    priority_queue<BinaryNode<char,int>> pq;
    for(auto pair : frequencyMap){
        // cout << pair.first << " " << pair.second << endl;
        BinaryNode<char,int> n(pair.first, pair.second);
        pq.push(n);
    }

    //step 4 - pop 2 at a time and create a offman tree root
    while(pq.size() >= 2){
        BinaryNode<char,int> bn1 = pq.top();
        pq.pop();

        BinaryNode<char,int> bn2 = pq.top();
        pq.pop();

        BinaryNode<char,int> bn3('-',bn1.getValue() + bn2.getValue());
        bn3.setLeftChildPtr(make_shared<BinaryNode<char,int>>(bn1));
        bn3.setRightChildPtr(make_shared<BinaryNode<char,int>>(bn2));

        pq.push(bn3);
    }

    //step 5 - complete Hoffman Tree
    BinaryNode<char,int> hoffmanBT = pq.top();
    shared_ptr<BinaryNode<char,int>> nptr = make_shared<BinaryNode<char,int>>(hoffmanBT);
    BinaryTree<char,int> hoffmanTree(nptr);

    //step 6 - create Code Table
    map<string,char> codeMap;//maps hoffman code to a character - used to decode
    map<char,string> charMap;//maps a character t0 a hoffman code - used to encode 
    //get hoffman code for each unique character in the original message
    cout << "-----Hoffman Code for each alphabet-----" << endl;
    for(int i = 0; i < numOfAlphabets; i++){
        string hcode = hoffmanTree.getHoffmanCode(alphabets[i]);
        cout << "Alphabet: " << alphabets[i] << " Freq.: " << frequencyMap[alphabets[i]] << " Hoffman Code: " << hcode << endl;
        codeMap.insert(pair<string,char>(hcode,alphabets[i]));
        charMap.insert(pair<char,string>(alphabets[i],hcode));
    }
    
    cout << endl;

    //step 7 - create hoffmancode encoded message using a code table
    string encodedMessage;
    for(auto it = origMsg.begin(); it != origMsg.end(); it++){
        encodedMessage += charMap[*it];
        // cout << encodedMessage << endl;
    }

    //step 8 - now that we have a encoded message, let's decode it back to the original message
    string codeTrack;//track encoded message for pattern match
    string decodedOrigMsg;//original message
    for(auto it = encodedMessage.begin(); it != encodedMessage.end(); it++){
        codeTrack.push_back(*it);
        auto c = codeMap.find(codeTrack);
        if(c != codeMap.end()){//found a pattern!
            decodedOrigMsg += codeMap[codeTrack];
            codeTrack.clear();
        }
    }

    cout << "-----Original Message-----" << endl;
    cout << decodedOrigMsg << endl << endl;

    cout << "-----Encoded Message-----" << endl;
    cout << encodedMessage << endl << endl;

    cout << "-----Decoded original Message-----" << endl;
    cout << decodedOrigMsg << endl << endl;

    double compressionRate = (double)encodedMessage.size() / (origMsg.size() * 8);
    cout << "-----Compression Rate-----" << endl;
    cout << compressionRate << endl;

    int a;
    cin >> a;
}