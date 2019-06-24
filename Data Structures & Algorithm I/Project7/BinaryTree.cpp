#include "BinaryTree.h"

template<class K, class V>
BinaryTree<K,V>::BinaryTree(){}

template<class K, class V>
BinaryTree<K,V>::BinaryTree(shared_ptr<BinaryNode<K,V>> rt){
    this->root = rt;
}

template<class K, class V>
BinaryTree<K,V>::BinaryTree(const K& k, const V& v){
    this->root = make_shared<BinaryNode<K,V>>(k,v);
}

template<class K, class V>
BinaryTree<K,V>::BinaryTree(const K& k, const V& v, shared_ptr<BinaryNode<K,V>> leftPrt, shared_ptr<BinaryNode<K,V>> rightPtr){
    this->root = make_shared<BinaryNode<K,V>>(k,v);
    this->root->setLeftChildPtr(leftPrt);
    this->root->setRightChildPtr(rightPtr);
}

template<class K, class V>
BinaryTree<K,V>::~BinaryTree(){
    cout << "Tree being destroyed" << endl;
}

// template<class K, class V>
// string BinaryTree<K,V>::getHoffmanCode(K& k) const{
//     if(this->root == nullptr)
//         return "";

//     stack<char> hoffmanStack;
//     stack<shared_ptr<BinaryNode<K,V>>> nodeStack;

//     shared_ptr<BinaryNode<K,V>> curNode = this->root;
//     nodeStack.push(curNode);

//     while(curNode->getLeftChildPtr() != nullptr){
//         curNode = curNode->getLeftChildPtr();
//         // cout << curNode->getKey() << " " << curNode->getValue() << endl;
//         nodeStack.push(curNode);
//         hoffmanStack.push('0');
//     }

//     while(nodeStack.empty() == false){
//         displayHoffmanCode(hoffmanStack);
//         curNode = nodeStack.top();
//         nodeStack.pop();
        
//         //leaf node
//         if(curNode->getLeftChildPtr() == nullptr && curNode->getRightChildPtr() == nullptr){
//             //key is found?
//             if(curNode->getKey() == k){
//                 break;
//             }
//         //go to next right
//         }else if(curNode->getRightChildPtr() != nullptr){
//             curNode = curNode->getRightChildPtr();
//             nodeStack.push(curNode);
//             hoffmanStack.push('1');

//             while(curNode->getLeftChildPtr() != nullptr){
//                 curNode = curNode->getLeftChildPtr();
//                 nodeStack.push(curNode);
//                 hoffmanStack.push('0');
//             }

//             continue;
//         }
//         // cout << hoffmanStack.size() << " " << nodeStack.size() << endl;
//         // while(hoffmanStack.size() >= nodeStack.size())
//         hoffmanStack.pop();

//         displayHoffmanCode(hoffmanStack);
//     }

//     //insert hoffman code into another stack to rearrange order
//     stack<char> hoffmanFinalStack;
//     while(hoffmanStack.empty() == false){
//         hoffmanFinalStack.push(hoffmanStack.top());
//         hoffmanStack.pop();
//     }

//     string hoffmanCode;
//     while(hoffmanFinalStack.empty() == false){
//         // cout << hoffmanFinalStack.top() << endl;
//         hoffmanCode += string(1,hoffmanFinalStack.top());
//         hoffmanFinalStack.pop();
//     }

//     return hoffmanCode;
// }

template<class K, class V>
string BinaryTree<K,V>::getHoffmanCode(K& k) const{
    bool found = false;
    string hoffmanCode = "";
    hoffmanCode = getHoffmanCodeRec(k,this->root,hoffmanCode,found);
    return hoffmanCode;
}

//find Hoffman Code recursively
template<class K, class V>
string BinaryTree<K,V>::getHoffmanCodeRec(K& k, shared_ptr<BinaryNode<K,V>> n, string& hoffmanCode, bool& found) const{
    //leaf and found
    if(n->getLeftChildPtr() == nullptr && n->getRightChildPtr() == nullptr){
        if(k == n->getKey()){
            found = true;
        }
        return hoffmanCode;
    }

    if(found == false && n->getLeftChildPtr() != nullptr){
        hoffmanCode.push_back('0');
        hoffmanCode = getHoffmanCodeRec(k,n->getLeftChildPtr(),hoffmanCode,found);
    }

    if(found == false)
        hoffmanCode.pop_back();

    if(found == false && n->getRightChildPtr() != nullptr){
        hoffmanCode.push_back('1');
        hoffmanCode = getHoffmanCodeRec(k,n->getRightChildPtr(),hoffmanCode,found);
    }

    if(found == false)
        hoffmanCode.pop_back();
    
    return hoffmanCode;
    

}

template<class K, class V>
void BinaryTree<K,V>::add(const K& k, const V& v){
    if(this->root == nullptr){
        this->root = make_shared<BinaryNode<K,V>>(k,v);;
        return;
    }

    if(k < this->root->getKey()){
        if(this->root->getLeftChildPtr() == nullptr){
            auto newNode = make_shared<BinaryNode<K,V>>(k,v);
            this->root->setLeftChildPtr(newNode);
        }else{
            add(this->root->getLeftChildPtr(),k,v);
        }
    }else{
        if(this->root->getRightChildPtr() == nullptr){
            auto newNode = make_shared<BinaryNode<K,V>>(k,v);
            this->root->setRightChildPtr(newNode);
        }else{
            add(this->root->getRightChildPtr(),k,v);
        }
    }
}

template<class K, class V>
void BinaryTree<K,V>::add(shared_ptr<BinaryNode<K,V>> ptr, const K& k, const V& v){
    if(ptr == nullptr){
        ptr = make_shared<BinaryNode<K,V>>(k,v);
    }else if(k < ptr->getKey()){
        if(ptr->getLeftChildPtr() == nullptr){
            auto newNode = make_shared<BinaryNode<K,V>>(k,v);
            ptr->setLeftChildPtr(newNode);
        }else{
            add(ptr->getLeftChildPtr(),k,v);
        }
    }else{
        if(ptr->getRightChildPtr() == nullptr){
            auto newNode = make_shared<BinaryNode<K,V>>(k,v);
            ptr->setRightChildPtr(newNode);
        }else{
            add(ptr->getRightChildPtr(),k,v);
        }
    }
}

template<class K, class V>
void BinaryTree<K,V>::clear(){
    if(this->root == nullptr)
        return;
    
    clear(this->root);
    this->root = nullptr;
}

template<class K, class V>
void BinaryTree<K,V>::clear(shared_ptr<BinaryNode<K,V>> ptr){
    if(ptr == nullptr)
        return;

    clear(ptr->getLeftChildPtr());
    ptr->setLeftChildPtr(nullptr);
    clear(ptr->getRightChildPtr());
    ptr->setRightChildPtr(nullptr);
}

template<class K, class V>
void BinaryTree<K,V>::preorderTraversal(void visit(K&, V&)) const{
    preorderTraversal(this->root, visit);
}

template<class K, class V>
void BinaryTree<K,V>::preorderTraversal(shared_ptr<BinaryNode<K,V>> ptr, void visit(K&, V&)) const{
    if(ptr == nullptr)
        return;

    K k = ptr->getKey();
    V v = ptr->getValue();
    visit(k,v);

    preorderTraversal(ptr->getLeftChildPtr(),visit);

    preorderTraversal(ptr->getRightChildPtr(),visit);
}

template<class K, class V>
void BinaryTree<K,V>::inorderTraversal(void visit(K&, V&)) const{
    inorderTraversal(this->root, visit);
}

template<class K, class V>
void BinaryTree<K,V>::inorderTraversal(shared_ptr<BinaryNode<K,V>> ptr, void visit(K&, V&)) const{
    if(ptr == nullptr)
        return;

    inorderTraversal(ptr->getLeftChildPtr(),visit);

    K k = ptr->getKey();
    V v = ptr->getValue();
    visit(k,v);

    inorderTraversal(ptr->getRightChildPtr(),visit);
}

template<class K, class V>
void BinaryTree<K,V>::postorderTraversal(void visit(K&, V&)) const{
    postorderTraversal(this->root,visit);
}

template<class K, class V>
void BinaryTree<K,V>::postorderTraversal(shared_ptr<BinaryNode<K,V>> ptr, void visit(K&, V&)) const{
    if(ptr == nullptr)
        return;

    postorderTraversal(ptr->getLeftChildPtr(),visit);

    postorderTraversal(ptr->getRightChildPtr(),visit);

    K k = ptr->getKey();
    V v = ptr->getValue();
    visit(k,v);
}

template<class K, class V>
void BinaryTree<K,V>::breadthFirstSearch(void visit(K&, V&)) const{
    if(this->root == nullptr)
        return;

    queue<shared_ptr<BinaryNode<K,V>>> q;
    q.push(this->root);

    while(q.empty() == false){
        shared_ptr<BinaryNode<K,V>> ptr = q.front();
        q.pop();

        if(ptr->getLeftChildPtr() != nullptr)
            q.push(ptr->getLeftChildPtr());

        if(ptr->getRightChildPtr() != nullptr)
            q.push(ptr->getRightChildPtr());

        K k = ptr->getKey();
        V v = ptr->getValue();
        visit(k,v);
    }
}

template<class K, class V>
void BinaryTree<K,V>::depthFirstSearch(void visit(K&, V&)) const{
    if(this->root == nullptr)
        return;

    stack<shared_ptr<BinaryNode<K,V>>> s;
    s.push(this->root);

    while(s.empty() == false){
        shared_ptr<BinaryNode<K,V>> ptr = s.top();
        s.pop();

        if(ptr->getRightChildPtr() != nullptr)
            s.push(ptr->getRightChildPtr());
            
        if(ptr->getLeftChildPtr() != nullptr)
            s.push(ptr->getLeftChildPtr());

        K k = ptr->getKey();
        V v = ptr->getValue();
        visit(k,v);
    }
}