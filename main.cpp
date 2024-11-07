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

//void writeNodes(node_t* node, FILE* file);
void writeTree(tree_t* tree, const char* fileName);
void insertElem(tree_t* tree, node_t* node, int value);
void writePngFile();

int main(){

    node_t node10 = {10, NULL, NULL};
    node_t node65 = {65, NULL, NULL};
    node_t node80 = {80, NULL, NULL};
    node_t node30 = {30, &node10, NULL};
    node_t node70 = {70, &node65, &node80};
    node_t node50 = {50, &node30, &node70};

    tree_t tree = {};

    tree.numOfNodes = 6;
    tree.nodes = (node_t**)calloc(tree.numOfNodes, sizeof(node_t*));
    tree.nodes[0] = &node50;
    tree.nodes[1] = &node65;
    tree.nodes[2] = &node80;
    tree.nodes[3] = &node30;
    tree.nodes[4] = &node70;
    tree.nodes[5] = &node10;

    writeTree(&tree, "tree.dot");
    insertElem(&tree, tree.nodes[0], 66);
    writeTree(&tree, "tree.dot");

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

void writeTree(tree_t* tree, const char* fileName){
    FILE* wFile = fopen(fileName, "wb");

    fprintf(wFile, "digraph\n{ \nrankdir=HR;\n");
    fprintf(wFile, "\n");

    for (size_t i = 0; i < tree->numOfNodes; i++)
        fprintf(wFile, "node%p [ shape=record, rank = %lu, label= \"{ %p | data = %d | {<n%p_l> left | <n%p_r> right}} \" ];\n", 
                                                                                                    (tree->nodes)[i], i+1,
                                                                                                    (tree->nodes)[i], (tree->nodes)[i]->data,
                                                                                                    (tree->nodes)[i], (tree->nodes)[i]);
    fprintf(wFile, "\n");

    for (size_t i = 0; i < tree->numOfNodes; i++){
        if ((tree->nodes)[i]->left != NULL)
            fprintf(wFile, "node%p:<n%p_l>:s -> node%p:n\n", (tree->nodes)[i], (tree->nodes)[i], (tree->nodes)[i]->left);
                                                                                                         
        if ((tree->nodes)[i]->right != NULL)
            fprintf(wFile, "node%p:<n%p_r>:s -> node%p:n\n", (tree->nodes)[i], (tree->nodes)[i], (tree->nodes)[i]->right);
                                                                                                          
    }
    fprintf(wFile, "\n");

    fprintf(wFile, "}\n");

    fclose(wFile);
    writePngFile();
}

void writePngFile(){
    static int numOfCall = 0;
    char command[100];
    const char* startOfName = "dot tree.dot -Tpng -o png_files/tree";
    sprintf(command, "%s%d.png", startOfName, numOfCall++);
    system(command);
}



void writeNodes(node_t* node, FILE* wFile, size_t rank){
    if (!node) return;
    fprintf(wFile, "node%p [shape=Mrecord label= \" data = %d | <n%p_l> l | <n%p_r> r \" ];\n", node, node->data, node, node);

    if (node->left) {
        fprintf(wFile, "node%p [shape=Mrecord label= \" data = %d | <n%p_l> l | <n%p_r> r \" ];\n", node->left, node->data, node->left, node->left);
        writeNodes(node, wFile, ++rank);
    }
        fprintf(wFile, "node%p [shape=Mrecord label= \" data = %d | <n%p_l> l | <n%p_r> r \" ];\n", node, node->data, node, node);
}