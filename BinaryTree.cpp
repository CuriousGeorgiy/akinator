#include "BinaryTree.hpp"

#include "File.hpp"
#include "String.hpp"

BinaryTree::BinaryTree()
: root(nullptr), size(0)
{
}

void BinaryTree::insertRight(BinaryTree::Node *parent, const char *data)
{
    parent->right = (BinaryTree::Node *) std::calloc(1, sizeof(BinaryTree::Node));
    parent->right->parent = parent;
    parent->right->data = new char[strlen(data) + 1];
    std::strcpy(parent->right->data, data);
    ++size;
}

void BinaryTree::insertLeft(BinaryTree::Node *parent, const char *data)
{
    parent->left = (BinaryTree::Node *) std::calloc(1, sizeof(BinaryTree::Node));
    parent->left->parent = parent;
    parent->left->data = new char[strlen(data) + 1];
    std::strcpy(parent->left->data, data);
    ++size;
}

BinaryTree::~BinaryTree()
{
    deleteRecursively(root);
}

void BinaryTree::deleteRecursively(BinaryTree::Node *node)
{
    if (node->left != nullptr) {
        deleteRecursively(node->left);
    }

    if (node->right != nullptr) {
        deleteRecursively(node->right);
    }

    delete[] node->data;
    delete node;
}

void BinaryTree::dumpToText()
{
    auto dumpTextFile = std::fopen(dumpTextFileName, "w");

    std::fprintf(dumpTextFile, "{}\n"
                               "{1.0.0.0}\n"
                               "{ENG}\n");

    dumpToTextRecursively(dumpTextFile, root, 0);

    std::fclose(dumpTextFile);
}

void BinaryTree::dumpToTextRecursively(std::FILE *dumpTextFile, const BinaryTree::Node *node, int depth)
{
    if (node == nullptr) return;

    if (isLeaf(node)) {
        dumpToTextLeaf(dumpTextFile, node, depth);
        return;
    }

    if (node == root) {
        std::fprintf(dumpTextFile, "[\n"
                                   "?%s?\n", node->data);
        dumpToTextRecursively(dumpTextFile, node->right, depth + 4);
        dumpToTextRecursively(dumpTextFile, node->left, depth + 4);
        std::fprintf(dumpTextFile, "]\n");
        return;
    }

    std::fprintf(dumpTextFile, "%*c[\n"
                               "%*c?%s?\n", depth, ' ', depth, ' ', node->data);
    dumpToTextRecursively(dumpTextFile, node->right, depth + 4);
    dumpToTextRecursively(dumpTextFile, node->left, depth + 4);
    std::fprintf(dumpTextFile, "%*c]\n", depth, ' ');
}

void BinaryTree::readFromText()
{
    auto dumpTextFile = std::fopen(dumpTextFileName, "r");
    char *buffer = readFileToBuf(dumpTextFile);
    std::fclose(dumpTextFile);

    root = readTreeRecursively(strchr(buffer, '['));

    delete[] buffer;
}

BinaryTree::BinaryTree::Node *BinaryTree::readTreeRecursively(char *text)
{
    if (*text == '[') {
        ++size;
        ++text;
        auto node = (BinaryTree::Node *) std::calloc(1, sizeof(BinaryTree::Node));
        auto data = std::strtok(skipWhitespaces(text), "\n");
        data[strlen(data) - 1] = '\0';
        node->data = new char[strlen(data)];
        std::strcpy(node->data, data + 1);
        if (*data == '`') {
            return node;
        }

        node->right = readTreeRecursively(std::strchr(std::strtok(nullptr, ""), '['));
        node->right->parent = node;
        node->left = readTreeRecursively(std::strchr(std::strtok(nullptr, ""), '['));
        node->left->parent = node;

        return node;
    }

    return nullptr;
}

void BinaryTree::dumpToDot()
{
    std::FILE *dumpDotFile = std::fopen("BinaryTreeDump.dot", "w");
    std::fprintf(dumpDotFile, "digraph G {\n"
                              "\trankdir=HR\n");

    dumpToDotRecursively(dumpDotFile, root);

    std::fprintf(dumpDotFile, "}\n");
    std::fclose(dumpDotFile);

    std::system("dot -Tpng BinaryTreeDump.dot > BinaryTreeDump.png");
    std::system("start BinaryTreeDump.png");
    std::system("pause");
}

void BinaryTree::dumpToDotRecursively(std::FILE *dumpDotFile, const BinaryTree::Node *node)
{
    static size_t index = 0;

    if (node == nullptr) return;

    if (node == root) {
        dumpToDotRoot(dumpDotFile, &index);
        return;
    }

    dumpToDotNode(dumpDotFile, &index, node);
}

bool BinaryTree::isLeaf(const BinaryTree::Node *node)
{
    return (node->left == nullptr) && (node->right == nullptr);
}

void BinaryTree::dumpToTextLeaf(std::FILE *dumpTextFile, const BinaryTree::Node *node, int depth) const
{
    if (node == root) {
        std::fprintf(dumpTextFile, "[\n"
                                   "`%s`\n"
                                   "]\n", node->data);
    } else {
        std::fprintf(dumpTextFile, "%*c[\n"
                                   "%*c`%s`\n"
                                   "%*c]\n", depth, ' ', depth, ' ', node->data, depth, ' ');
    }
}

void BinaryTree::dumpToDotNodeData(std::FILE *dumpDotFile, size_t index, const char *data)
{
    std::fprintf(dumpDotFile, "\t%zu[fillcolor=\"#add8e6\", color=\"#00008b\", fontcolor=\"#00008b\", label=\""
                              "<left> left | <data> data\\n %s | <right> right\", style=filled, shape=record];\n", index, data);
}

void BinaryTree::dumpToDotRoot(std::FILE *dumpDotFile, size_t *index)
{
    dumpToDotNodeData(dumpDotFile, *index, root->data);
    if (isLeaf(root)) return;

    size_t rootI = *index;

    ++*index;
    if (root->right != nullptr) {
        dumpToDotNodeData(dumpDotFile, *index, root->right->data);
        std::fprintf(dumpDotFile, "\t%zu:<right>->%zu;\n", rootI, *index);
        dumpToDotRecursively(dumpDotFile, root->right);
    }

    ++*index;
    if (root->left != nullptr) {
        dumpToDotNodeData(dumpDotFile, *index, root->left->data);
        std::fprintf(dumpDotFile, "\t%zu:<left>->%zu;\n", rootI, *index);
        dumpToDotRecursively(dumpDotFile, root->left);
    }
}

void BinaryTree::dumpToDotNode(std::FILE *dumpDotFile, size_t *index, const BinaryTree::Node *node)
{
    if (isLeaf(node)) return;

    size_t nodeI = *index;

    ++*index;
    if (node->right != nullptr) {
        dumpToDotNodeData(dumpDotFile, *index, node->right->data);
        std::fprintf(dumpDotFile, "\t%zu:<right>->%zu;\n", nodeI, *index);
        dumpToDotRecursively(dumpDotFile, node->right);
    }

    ++*index;
    if (node->left != nullptr) {
        dumpToDotNodeData(dumpDotFile, *index, node->left->data);
        std::fprintf(dumpDotFile, "\t%zu:<left>->%zu;\n", nodeI, *index);
        dumpToDotRecursively(dumpDotFile, node->left);
    }
}

void BinaryTree::traceNode(const BinaryTree::BinaryTree::Node *node, Stack<const Node *> *nodeTrace)
{
    if (node != root) {
        nodeTrace->push(node);
        traceNode(node->parent, nodeTrace);
    }
}

const BinaryTree::Node *BinaryTree::findNodeRecursively(const BinaryTree::BinaryTree::Node *node, const char *data)
{
    if (node == nullptr) return nullptr;

    if (std::strcmp(node->data, data) == 0) return node;

    auto foundNode = findNodeRecursively(node->left, data);
    if (foundNode == nullptr) return findNodeRecursively(node->right, data);
    return foundNode;
}
