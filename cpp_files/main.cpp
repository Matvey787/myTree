#include <stdio.h>
#include <stdlib.h>


typedef int data_t;

struct node_t
{
    data_t data;
    node_t* left;
    node_t* right;
};

struct tree_t
{
    node_t** nodes;
    size_t numOfNodes;
};

int writeTree(tree_t* tree, const char* fileName);
void writeNodes(tree_t* tree, FILE* wFile);
void insertElem(tree_t* tree, node_t* node, int value);
int writePngFile();
void writeHtmlFile(const char* fileName, int numOfFiles);

const size_t startSizeTree = 6;

int main(){
    int numberOfFiles = 0;

    node_t node10 = {10, NULL, NULL};
    node_t node65 = {65, NULL, NULL};
    node_t node80 = {80, NULL, NULL};
    node_t node30 = {30, &node10, NULL};
    node_t node70 = {70, &node65, &node80};
    node_t node50 = {50, &node30, &node70};

    tree_t tree = {};

    tree.numOfNodes = startSizeTree;
    tree.nodes = (node_t**)calloc(tree.numOfNodes, sizeof(node_t*));

    tree.nodes[0] = &node50;
    tree.nodes[1] = &node65;
    tree.nodes[2] = &node80;
    tree.nodes[3] = &node30;
    tree.nodes[4] = &node70;
    tree.nodes[5] = &node10;

    numberOfFiles = writeTree(&tree, "tree.dot");
    insertElem(&tree, tree.nodes[0], 66);
    numberOfFiles = writeTree(&tree, "tree.dot");
    writeHtmlFile("trees.html", numberOfFiles);
    for (size_t i = 0; i <tree.numOfNodes; i++)
        if (i >= startSizeTree) 
            free(tree.nodes[i]);
    free(tree.nodes);
    return 0;
}

void insertElem(tree_t* tree, node_t* node, int value){
    if (value < node->data){
        if (!node->left){
            tree->nodes = (node_t**)realloc(tree->nodes, sizeof(node_t*)*(++tree->numOfNodes));
            tree->nodes[tree->numOfNodes - 1] = (node_t*)calloc(1, sizeof(node_t));
            tree->nodes[tree->numOfNodes - 1]->data =value;
            tree->nodes[tree->numOfNodes - 1]->left = NULL;
            tree->nodes[tree->numOfNodes - 1]->right = NULL;

            node->left = tree->nodes[tree->numOfNodes - 1];

        } else {
            insertElem(tree, node->left, value);
        }
    } else {
        if (!node->right){
            tree->nodes = (node_t**)realloc(tree->nodes, sizeof(node_t*)*(++tree->numOfNodes));
            tree->nodes[tree->numOfNodes - 1] = (node_t*)calloc(1, sizeof(node_t));
            tree->nodes[tree->numOfNodes - 1]->data =value;
            tree->nodes[tree->numOfNodes - 1]->left = NULL;
            tree->nodes[tree->numOfNodes - 1]->right = NULL;

            node->right = tree->nodes[tree->numOfNodes - 1];

        } else {
            insertElem(tree, node->right, value);
        }
    }
}

int writeTree(tree_t* tree, const char* fileName){
    FILE* wFile = fopen(fileName, "wb");

    fprintf(wFile, "digraph\n{ \nrankdir=HR;\n");
    fprintf(wFile, "\n");

    for (size_t i = 0; i < tree->numOfNodes; i++)
        fprintf(wFile, "node%p [ shape=record, rank = %lu, label= \"{ %p | data = %d | {<n%p_l> left | <n%p_r> right}} \" ];\n", 
                                                                                                    (tree->nodes)[i], i+1,
                                                                                                    (tree->nodes)[i], (tree->nodes)[i]->data,
                                                                                                    (tree->nodes)[i], (tree->nodes)[i]);
    fprintf(wFile, "\n");

    writeNodes(tree, wFile);

    fprintf(wFile, "}\n");

    fclose(wFile);
    return writePngFile();
}

int writePngFile(){
    static int numOfCall = 0;
    char command[100];
    const char* startOfName = "dot tree.dot -Tpng -o png_files/tree";
    sprintf(command, "%s%d.png", startOfName, numOfCall++);
    system(command);

    return numOfCall;
}

void writeHtmlFile(const char* fileName, int numOfFiles){
    FILE* wFile = fopen(fileName, "wb");
    fprintf(wFile, "<pre>\n");
    for (int i = 0; i < numOfFiles; i++){
        fprintf(wFile, "<hr>\n");
        fprintf(wFile, "<img src=\"png_files\\tree%d.png\">\n", i);
    }
}


void writeNodes(tree_t* tree, FILE* wFile){
    for (size_t i = 0; i < tree->numOfNodes; i++){
        if ((tree->nodes)[i]->left != NULL)
            fprintf(wFile, "node%p:<n%p_l>:s -> node%p:n\n [ color = blue; ]", (tree->nodes)[i], (tree->nodes)[i], (tree->nodes)[i]->left);
                                                                                                         
        if ((tree->nodes)[i]->right != NULL)
            fprintf(wFile, "node%p:<n%p_r>:s -> node%p:n\n [ color = red; ]", (tree->nodes)[i], (tree->nodes)[i], (tree->nodes)[i]->right);
                                                                                                          
    }
    fprintf(wFile, "\n");
}