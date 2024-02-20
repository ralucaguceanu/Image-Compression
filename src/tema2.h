/* GUCEANU Raluca-Zinca-Ioana 312CD */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* structura pentru un pixel */
typedef struct pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

/* structura pentru imagine */
typedef struct image {
    int width;
    int height;
    int maxColorValue;
    pixel p;
} image;

/* structura pentru un nod din arborele cuaternar */
typedef struct node {
    unsigned char type;  // frunza sau nod intern
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    struct node *first;
    struct node *second;
    struct node *third;
    struct node *fourth;
} TreeNode, *Tree;

/* structura pentru un nod al cozii cu ajutorul careia se realizeaza
parcurgerea pe nivel a arborelui cuaternar */
typedef struct list {
    TreeNode *value;
    struct list *next;
} *List;

/* structura pentru coada */
typedef struct queue {
    List front, rear;
} Queue;

TreeNode *createNode(unsigned char red, unsigned char green,
    unsigned char blue, unsigned char type);
TreeNode *insertNode(pixel **grid, Tree root, int x, int y, int size,
    int factor, int *found_max_undiv);
void freeTree(Tree root);
int height(Tree root);
int countLeaves(Tree root);
Queue *initQueue();
List createList(Tree value);
int isEmptyQueue(Queue *q);
void enqueue(Queue *q, TreeNode *node);
Tree dequeue(Queue *q);
void freeQueue(Queue *q);
void printLevelOrder(Tree root, Queue *q, FILE *out);
