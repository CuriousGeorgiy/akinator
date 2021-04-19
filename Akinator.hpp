#ifndef AKINATOR_HPP
#define AKINATOR_HPP

#include "BinaryTree.hpp"
#include "Stack.hpp"

#include <cstdio>

class Akinator {
public:
    Akinator();

    Akinator(const Akinator &) = delete;
    Akinator(Akinator &&) = delete;

    Akinator &operator=(const Akinator &) = delete;
    Akinator &operator=(Akinator &&) = delete;

    ~Akinator();

    void createDb();
    void saveDb();
    void loadDb();
    void guess();
    void define();
    void compare();
    void showTree();

private:
    BinaryTree binaryTree;

    char *answer;
    static const size_t maxAnswerLength = 1024;

    void newObject(BinaryTree::Node *parentObject);
    void compareObjectTraces(const BinaryTree::Node *object1, const BinaryTree::Node *object2);
    static bool findCommonFeatures(const BinaryTree::Node *object1, const BinaryTree::Node *object2,
                                   Stack<const BinaryTree::Node *> *objectTrace1, Stack<const BinaryTree::Node *> *objectTrace2);
    static void findDifferentFeatures(const BinaryTree::Node *object, Stack<const BinaryTree::Node *> *objectTrace);

    void defineRecursively(const BinaryTree::Node *node, bool firstCall);
};

#endif /* AKINATOR_HPP */
