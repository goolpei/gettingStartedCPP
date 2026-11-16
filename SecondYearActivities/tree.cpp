// Implementation of Tree Preorder, In-order, 
// Post-order, and Level-order traversal
// In a Binary Search Tree

#include <iostream>
#include <queue>
using namespace std;

class Node {
public:
    int val;
    Node* left;
    Node* right;
    int frequency;

    Node(int n) : val(n), left(nullptr), right(nullptr), frequency(1) {};
};

Node* insert(Node* root, int val)
{
    if (root == nullptr)
    {
        return new Node(val);
    }

    int node_val = root->val;
    if (val > node_val)
    {
        root->right = insert(root->right, val);
    }
    else if (val < node_val)
    {
        root->left = insert(root->left, val);
    }
    else if (val == node_val)
    {
        root->frequency += 1;
    }

    return root;
}

void printPreOrder(Node* root)
{
    if (root == nullptr) return;

    cout << root->val << " ";
    printPreOrder(root->left);
    printPreOrder(root->right);

}

void printPostOrder(Node* root)
{
    if (root == nullptr) return;

    printPostOrder(root->left);
    printPostOrder(root->right);
    cout << root->val << " ";

}

void printInOrder(Node* root)
{
    if (root == nullptr) return;

    printInOrder(root->left);
    cout << root->val << " ";
    printInOrder(root->right);

}

void printLevelOrder(Node* root)
{
    if (root == nullptr) return;

    queue<Node*> q;
    Node* n;

    q.push(root);

    while (!q.empty())
    {
        n = q.front();
        q.pop();
        cout << n->val << " ";
        if (n->left != nullptr) q.push(n->left);
        if (n->right != nullptr) q.push(n->right);
    }
    return;
}

void destroyTree(Node* root)
{
    if (root == nullptr) return;

    destroyTree(root->left);
    destroyTree(root->right);
    delete root;

    return;
}

int main(void)
{

    Node* root = nullptr;
    int nums[] = {6,4,3,5,7,11,9,1,12,8};

    for (int num: nums)
    {
        root = insert(root, num);
    }
    
    cout << "\nPre-order Traversal: ";
    printPreOrder(root);
    cout << "\nPost-order Traversal: ";
    printPostOrder(root);
    cout << "\nIn-order Traversal: ";
    printInOrder(root);
    cout << "\nLevel-order Traversal: ";
    printLevelOrder(root);
    cout << "\n\n";
    
    destroyTree(root);
    root = nullptr;
    return 0;
}
