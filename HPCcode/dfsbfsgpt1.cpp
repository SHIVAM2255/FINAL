#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <omp.h>
using namespace std;

// TreeNode structure
struct TreeNode {
    int data;
    vector<TreeNode*> children;
    TreeNode(int val) : data(val) {}
};

// Tree class
class Tree {
    TreeNode* root;
    unordered_map<int, TreeNode*> nodeMap;

public:
    Tree(int val) {
        root = new TreeNode(val);
        nodeMap[val] = root;
    }

    void addChild(int parentData, int childData) {
        TreeNode* parent = nodeMap[parentData];
        TreeNode* newNode = new TreeNode(childData);
        parent->children.push_back(newNode);
        nodeMap[childData] = newNode;
    }

    TreeNode* getRoot() {
        return root;
    }

    // Parallel DFS
    void parallelDFS(TreeNode* node) {
        if (!node) return;
        cout << node->data << " ";
        #pragma omp parallel
        {
            #pragma omp single nowait
            {
                for (size_t i = 0; i < node->children.size(); ++i) {
                    #pragma omp task
                    parallelDFS(node->children[i]);
                }
            }
        }
    }

    // Parallel BFS
    void parallelBFS() {
        if (!root) return;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* current = q.front();
            q.pop();
            cout << current->data << " ";

            vector<TreeNode*> nextLevel;
            #pragma omp parallel for
            for (int i = 0; i < current->children.size(); ++i) {
                #pragma omp critical
                nextLevel.push_back(current->children[i]);
            }

            for (auto child : nextLevel) {
                q.push(child);
            }
        }
    }
};

int main() {
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;

    cout << "Enter root node value: ";
    int rootVal;
    cin >> rootVal;
    Tree tree(rootVal);

    cout << "Enter edges in format (parent child) for " << n - 1 << " nodes:\n";
    for (int i = 0; i < n - 1; i++) {
        int parent, child;
        cin >> parent >> child;
        tree.addChild(parent, child);
    }

    cout << "\nParallel Depth-First Search (DFS): ";
    tree.parallelDFS(tree.getRoot());

    cout << "\nParallel Breadth-First Search (BFS): ";
    tree.parallelBFS();

    return 0;
}
