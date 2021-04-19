#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include "Stack.hpp"

#include <cstdio>

class BinaryTree {
public:
    struct Node {
        char *data;
        Node *left;
        Node *right;
        Node *parent;
    } *root;

    size_t size;

    BinaryTree();

    BinaryTree(const BinaryTree &) = delete;
    BinaryTree(BinaryTree &&) = delete;

    BinaryTree &operator=(const BinaryTree &) = delete;
    BinaryTree &operator=(BinaryTree &&) = delete;

    ~BinaryTree();

    void dumpToText();
    void dumpToTextLeaf(std::FILE *dumpTextFile, const Node *node, int depth) const;
    void dumpToTextRecursively(std::FILE *dumpTextFile, const Node *node, int depth);
    void dumpToDot();
    void dumpToDotRoot(std::FILE *dumpDotFile, size_t *index);
    void dumpToDotNode(std::FILE *dumpDotFile, size_t *index, const Node *node);
    static void dumpToDotNodeData(std::FILE *dumpDotFile, size_t index, const char *data);
    void dumpToDotRecursively(std::FILE *dumpDotFile, const Node *node);

    void readFromText();

    void traceNode(const Node *node, Stack<const Node *> *nodeTrace);

    void insertRight(Node *parent, const char *data);
    void insertLeft(Node *parent, const char *data);

    const Node *findNodeRecursively(const Node *node, const char *data);

    static bool isLeaf(const Node *node);
private:
    const char *dumpTextFileName = "BinaryTreeDump.txt";

    void deleteRecursively(Node *node);

    Node *readTreeRecursively(char *text);
};

#endif /* BINARYTREE_HPP */
