#include "Akinator.hpp"

#include "File.hpp"
#include "String.hpp"

#define TX_USE_SPEAK
#include "TXLib.h"

#include <cstring>
#include <cstdlib>

Akinator::Akinator()
: answer(new char[maxAnswerLength])
{
}


void Akinator::saveDb()
{
    binaryTree.dumpToText();
}

void Akinator::loadDb()
{
    binaryTree.readFromText();
}

void Akinator::guess()
{
    BinaryTree::BinaryTree::Node *node = binaryTree.root;

    while (node != nullptr) {
        txSpeak("\vIs it %s?\n", node->data);
        auto questionResponse = readLineRemoveNewline(stdin, answer, maxAnswerLength);
        toLowerStr(questionResponse);

        if (std::strcmp(questionResponse, "yes") == 0) {
            delete[] questionResponse;

            if (BinaryTree::isLeaf(node)) {
                txSpeak("\vTold you!\n");
                return;
            }

            node = node->right;
            continue;
        }

        if (std::strcmp(questionResponse, "no") == 0) {
            delete[] questionResponse;

            if (BinaryTree::isLeaf(node)) {
                newObject(node);
                saveDb();
                txSpeak("\vGot it!\n");
                return;
            }

            node = node->left;
            continue;
        }

        delete[] questionResponse;
        txSpeak("\vPossible answers: yes or no.\n");
    }
}

void Akinator::newObject(BinaryTree::Node *parentObject)
{
    txSpeak("\vWho was it then?\n");
    auto objectData = readLineRemoveNewline(stdin, answer, maxAnswerLength);

    txSpeak("\vHow does %s differ from %s?\n"
                "It is...\n", objectData, parentObject->data);
    auto featureData = readLineRemoveNewline(stdin, answer, maxAnswerLength);

    auto parentObjectData = parentObject->data;
    parentObject->data = featureData;
    binaryTree.insertRight(parentObject, objectData);
    binaryTree.insertLeft(parentObject, parentObjectData);
}

void Akinator::define()
{
    txSpeak("\vTell me the name of the object:\n");
    auto object = readLineRemoveNewline(stdin, answer, maxAnswerLength);

    auto foundNode = binaryTree.findNodeRecursively(binaryTree.root, object);
    if (foundNode == nullptr) {
        txSpeak("\vThis object does not exist.\n");
        return;
    }

    txSpeak("\v%s ", object);
    defineRecursively(foundNode, true);
    txSpeak("\v\n");
}

void Akinator::defineRecursively(const BinaryTree::Node *node, bool firstCall)
{
    if (node->parent == nullptr) return;

    defineRecursively(node->parent, false);

    if (node == node->parent->right) {
        txSpeak("\vis %s", node->parent->data);
    } else {
        txSpeak("\vis not %s", node->parent->data);
    }

    if (!firstCall) {
        txSpeak("\v, ");
    } else {
        txSpeak("\v ");
    }
}

void Akinator::compare()
{
    if (binaryTree.size == 1) {
        txSpeak("\vLess than two objects to compare in database.\n");
        return;
    }

    txSpeak("\vTell me the name of the first object:\n");
    auto objectName1 = readLineRemoveNewline(stdin, answer, maxAnswerLength);
    auto object1 = binaryTree.findNodeRecursively(binaryTree.root, objectName1);
    if (object1 == nullptr) {
        txSpeak("\vThis object does not exist.\n");
        return;
    }

    txSpeak("\vTell me the name of the second object:\n");
    auto objectName2 = readLineRemoveNewline(stdin, answer, maxAnswerLength);
    auto object2 = binaryTree.findNodeRecursively(binaryTree.root, objectName2);
    if (object2 == nullptr) {
        txSpeak("\vThis object does not exist.\n");
        return;
    }

    compareObjectTraces(object1, object2);
}

void Akinator::showTree()
{
    binaryTree.dumpToDot();
}

void Akinator::createDb()
{
    binaryTree.root = (BinaryTree::Node *) std::calloc(1, sizeof(BinaryTree::Node));

    auto unknownWho = new char[strlen("Unknown who") + 1];
    std::strcpy(unknownWho, "Unknown who");
    binaryTree.root->data = unknownWho;
}

void Akinator::compareObjectTraces(const BinaryTree::Node *object1, const BinaryTree::Node *object2)
{
    Stack<const BinaryTree::Node *> objectTrace1, objectTrace2;
    binaryTree.traceNode(object1, &objectTrace1);
    binaryTree.traceNode(object2, &objectTrace2);

    bool haveAtLeastOneCommonFeature = findCommonFeatures(object1, object2, &objectTrace1, &objectTrace2);

    if ((objectTrace1.size == 0) && (objectTrace2.size == 0)) {
        txSpeak("\v\n");
        return;
    }

    if (haveAtLeastOneCommonFeature) txSpeak("\vbut ");
    findDifferentFeatures(object1, &objectTrace1);
    txSpeak("\vand ");
    findDifferentFeatures(object2, &objectTrace2);
}

bool Akinator::findCommonFeatures(const BinaryTree::Node *object1, const BinaryTree::Node *object2,
                                  Stack<const BinaryTree::Node *> *objectTrace1, Stack<const BinaryTree::Node *> *objectTrace2)
{
    bool haveAtLeastOneCommonFeature = false;
    bool error = false;

    while ((objectTrace1->size > 0) && (objectTrace2->size > 0)) {
        const BinaryTree::Node *node1 = objectTrace1->pop(&error);
        const BinaryTree::Node *node2 = objectTrace2->pop(&error);

        if (node1 != node2) {
            objectTrace1->push(node1);
            objectTrace2->push(node2);
            break;
        }

        if (!haveAtLeastOneCommonFeature) {
            txSpeak("\v%s and %s are both ", object1->data, object2->data);
            haveAtLeastOneCommonFeature = true;
        }

        if (node1 == node1->parent->right) {
            txSpeak("\v%s, ", node1->parent->data);
        } else {
            txSpeak("\vnot %s, ", node1->parent->data);
        }
    }

    return haveAtLeastOneCommonFeature;
}

void Akinator::findDifferentFeatures(const BinaryTree::Node *object, Stack<const BinaryTree::Node *> *objectTrace)
{
    bool error = false;

    if (objectTrace->size > 0) {
        txSpeak("\v%s is ", object->data);
        while (objectTrace->size > 0) {
            const BinaryTree::Node *node = objectTrace->pop(&error);

            if (node == node->parent->right) {
                txSpeak("\v%s", node->parent->data);
            } else {
                txSpeak("\vnot %s", node->parent->data);
            }

            if (objectTrace->size > 0) {
                txSpeak("\v, ");
            } else {
                txSpeak("\v ");
            }
        }
        txSpeak("\v\n");
    }
}

Akinator::~Akinator()
{
    delete[] answer;
}

