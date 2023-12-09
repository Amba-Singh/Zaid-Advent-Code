#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

struct node {
    std::string name;
    node* left;
    node* right;
};

size_t findNode(std::vector<node*>& nodeSet, std::string& node) {
    for (int i = 0; i < nodeSet.size(); i++) {
        if (strcmp(nodeSet[i]->name.c_str(), node.c_str()) == 0) return i;
    }
    return -1;
}

void addNode(std::vector<node*>& nodeSet, std::string& curr, std::string& left, std::string& right) {
    size_t cNode = findNode(nodeSet, curr);
    size_t lNode = findNode(nodeSet, left);

    if (cNode == -1) {
        node *n = new node;
        n->name = curr;
        nodeSet.push_back(n);

        if (lNode == -1) {
            node* l = new node;
            l->name = left;
            n->left = l;
            nodeSet.push_back(l);
        }
        else {
            n->left = nodeSet[lNode];
        }

        size_t rNode = findNode(nodeSet, right);

        if (rNode == -1) {
            node* r = new node;
            r->name = right;
            n->right = r;
            nodeSet.push_back(r);
        }
        else {
            n->right = nodeSet[rNode];
        }


    }
    else {
        node *n = nodeSet[cNode];

        if (lNode == -1) {
            node* l = new node;
            l->name = left;
            n->left = l;
            nodeSet.push_back(l);
        }
        else {
            n->left = nodeSet[lNode];
        }

        size_t rNode = findNode(nodeSet, right);

        if (rNode == -1) {
            node* r = new node;
            r->name = right;
            n->right = r;
            nodeSet.push_back(r);
        }
        else {
            n->right = nodeSet[rNode];
        }
    }
}

void getNode(std::vector<node*>& nodeSet, std::fstream& file) {
    std::string line;
    std::string curr;
    getline(file, line);
    while (!file.eof()) {
        getline(file, line);
        std::istringstream iss{line};
        std::string curr;
        std::string eq;
        std::string left;
        std::string right;

        iss >> curr >> eq >> left >> right;
        left = left.substr(1, 3);
        right = right.substr(0, 3);

        addNode(nodeSet, curr, left, right);
    }
}

void getInstruction(std::string& instructionSet, std::vector<node*>& nodeSet, std::fstream& file) {
    std::string line;
    getline(file, line);
    instructionSet = line;
    getNode(nodeSet, file);
}

long long pathLength(std::fstream& file) {
    std::vector<node*> nodeSet;
    std::string instructionSet;
    getInstruction(instructionSet, nodeSet, file);

    long long count = 0;
    bool notFound = true;

    std::vector<node*> startingArray;

    for (int i = 0; i < nodeSet.size(); i++) {
        if (nodeSet[i]->name[2] == 'A') startingArray.push_back(nodeSet[i]);
    }

    std::vector<long long> lengths;

    for (node* starter : startingArray) {
        count = 0;
        notFound = true;
        while (notFound) {
            for (int i = 0; i < instructionSet.size(); i++) {

                char ins = instructionSet[i];

                if (starter->name[2] == 'Z') {
                    notFound = false;
                    lengths.push_back(count);
                }

                starter = (ins == 'L') ? starter->left : starter->right;

                count++;

            }
        }
    }

    for (long long num : lengths) {
        std::cout << num << '\n';
    }

    for (int i = 0; i < nodeSet.size(); i++) {
        delete nodeSet[i];
    }
    return count;
}

int main()
{
    std::fstream file;
    file.open("Input.txt", std::fstream::in);
    pathLength(file);
    file.close();
}
