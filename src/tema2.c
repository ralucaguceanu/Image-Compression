/* GUCEANU Raluca-Zinca-Ioana 312CD */

#include "tema2.h"
#define leaf 1
#define internal_node 0

/* alocarea memoriei pentru un nod din arbore */
TreeNode *createNode(unsigned char red, unsigned char green,
    unsigned char blue, unsigned char type) {

	TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
    root->type = type;
    root->red = red;
    root->green = green;
    root->blue = blue;
	root->first = NULL;
	root->second = NULL;
    root->third = NULL;
    root->fourth = NULL;
	return root;
}

/* parcurgea matricei de pixeli si crearea arborelui de compresie */
TreeNode *insertNode(pixel **grid, Tree root, int x, int y, int size,
    int factor, int *found_max_undiv) {

    unsigned long long red, green, blue, mean;
    int i, j;
    red = 0, green = 0, blue = 0, mean = 0;

    if (size < 1) {  // matricea nu mai poate fi divizata
        return root;
    }

    /* calculul valorii medii pentru un bloc */
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            red += grid[i][j].red;
            green += grid[i][j].green;
            blue += grid[i][j].blue;
        }
    }

    red = red / (size * size);
    green = green / (size * size);
    blue = blue / (size * size);


    /* calculul scorului de similaritate pentru acelasi bloc */
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            mean += (red - grid[i][j].red) * (red - grid[i][j].red)
                + (green - grid[i][j].green) * (green - grid[i][j].green)
                + (blue - grid[i][j].blue) * (blue - grid[i][j].blue);
        }
    }

    mean = mean / (3 * size * size);

    if (mean <= factor) {

        /* actualizarea dimensiunii laturii patratului pentru cea mai mare
        zona din imagine care a ramas nedivizata */
        if (*found_max_undiv == 0) {
            *found_max_undiv = size;
        } else if (*found_max_undiv < size) {
            *found_max_undiv = size;
        }

        /* actualizarea valorilor culorilor din pixel pentru cazul cand
        imaginea nu se divizeaza niciodata, deci radacina este frunza */ 
        if (root != NULL) {
            root->type = leaf;
            root->red = red;
            root->green = green;
            root->blue = blue;
            return root;
        }

        /* inserarea unei frunze */
        if (root == NULL) {
            return createNode(red, green, blue, leaf);
        }
    } else {

        /* inserarea unui nod intern */
        if (root == NULL) {
            root = createNode(0, 0, 0, internal_node);
        }
    }

    root->first = insertNode(grid, root->first, x, y, size/2,
        factor, found_max_undiv);
    root->second = insertNode(grid, root->second, x, y + size/2, size/2,
        factor, found_max_undiv);
    root->third = insertNode(grid, root->third, x + size/2, y + size/2, size/2,
        factor, found_max_undiv);
    root->fourth = insertNode(grid, root->fourth, x + size/2, y, size/2,
        factor, found_max_undiv);

    return root;
}

/* calcularea numarului de niveluri din arbore */
int height(Tree root) {
    if (root == NULL) {
        return 0;
    }

    int height1 = height(root->first);
    int height2 = height(root->second);
    int height3 = height(root->third);
    int height4 = height(root->fourth);

    int max_height = height1;

    if (height2 > max_height) {
        max_height = height2;
    }

    if (height3 > max_height) {
        max_height = height3;
    }

    if (height4 > max_height) {
        max_height = height4;
    }

    return 1 + max_height;
}

/* calcularea numarului de blocuri pentru care scorul similaritatii
pixelilor este mai mic sau egal decat factorul furnizat */
int countLeaves(Tree root) {
    if (root == NULL) {
        return 0;
    }

    if (root->first == NULL && root->second == NULL && root->third == NULL
        && root->fourth == NULL) {
            return 1;
    }

    int leaves1 = countLeaves(root->first);
    int leaves2 = countLeaves(root->second);
    int leaves3 = countLeaves(root->third);
    int leaves4 = countLeaves(root->fourth);

    return leaves1 + leaves2 + leaves3 + leaves4;
}

/* eliberarea memoriei alocate pentru arbore */
void freeTree(Tree root) {
	if (root == NULL) {
		return;
	}

	freeTree(root->first);
	freeTree(root->second);
    freeTree(root->third);
    freeTree(root->fourth);
	free(root);
}

/* alocarea memoriei pentru coada cu cu ajutorul careia se realizeaza
parcurgerea pe nivel a arborelui cuaternar */
Queue *initQueue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

/* alocarea memoriei pentru elementele din coada */
List createList(Tree value) {
    List head = (List)malloc(sizeof(struct list));
    head->value = value;
    head->next = NULL;
    return head;
}

/* verificare coada goala */
int isEmptyQueue(Queue *q) {
    return q->front == NULL;
}

/* adaugare in coada */
void enqueue(Queue *q, TreeNode *node) {
    if (q->front == NULL) {
        q->front = q->rear = createList(node);
    } else {
        List list = createList(node);
        q->rear->next = list;
        q->rear = list;
    }
}

/* eliminare din coada */
Tree dequeue(Queue *q) {
    if (q->front == NULL) {
        return NULL;
    }

    Tree result = q->front->value;
    List tmp = q->front;
    q->front = q->front->next;

    if (tmp->next == NULL) {
        q->rear = NULL;
    }

    free(tmp);
    return result;
}

/* eliberarea memoriei alocate pentru coada */
void freeQueue(Queue *q) {
	while (!isEmptyQueue(q)) {
		dequeue(q);
	}

	free(q);
}

/* parcurgerea pe nivel a arborelui */
void printLevelOrder(Tree root, Queue *q, FILE *out) {
    if (root == NULL) {
        return;
    }

    Tree current;
    enqueue(q, root);

    while (!isEmptyQueue(q)) {
        current = dequeue(q);
        if (current != NULL) {
            fwrite(&current->type, sizeof(unsigned char), 1, out);
            if (current->type == leaf) {
                fwrite(&current->red, sizeof(unsigned char), 1, out);
                fwrite(&current->green, sizeof(unsigned char), 1, out);
                fwrite(&current->blue, sizeof(unsigned char), 1, out);
            }
            enqueue(q, current->first);
            enqueue(q, current->second);
            enqueue(q, current->third);
            enqueue(q, current->fourth);
        }
    }
}

int main(int argc, char const *argv[]) {
    FILE *in = fopen(argv[3], "rb");  // deschiderea fisierului de input
    char fileInfo[3];
    char white_space = '\0';

    if (!in) {
        fprintf(stderr, "Unable to open file");
        exit(1);
    }

    /* citirea informatiilor despre fisier */
    fgets(fileInfo, sizeof(fileInfo), in);

    if (fileInfo[0] != 'P' || fileInfo[1] != '6') {
         fprintf(stderr, "Invalid image format (must be 'P6')\n");
         exit(1);
    }

    /* alocarea memoriei pentru imagine */
    image *img = (image *)malloc(sizeof(image));

    /* citirea informatiilor despre imagine */
    fscanf(in, "%d", &img->width);
    fscanf(in, "%d", &img->height);
    fscanf(in, "%d", &img->maxColorValue);
    fscanf(in, "%c", &white_space);

    /* alocarea memoriei pentru matricea de pixeli */
    int i, j;
    pixel **grid = (pixel **)malloc(img->height * sizeof(pixel *));
    for (i = 0; i < img->width; ++i) {
        grid[i] = (pixel *)malloc(img->width * sizeof(pixel));
    }

    /* citirea valorilor culorilor pentru fiecare pixel si crearea
    matricei de pixeli */
    for (i = 0; i < img->height; ++i) {
        for (j = 0; j < img->width; ++j) {
            fread(&img->p.red, 1, sizeof(unsigned char), in);
            fread(&img->p.green, 1, sizeof(unsigned char), in);
            fread(&img->p.blue, 1, sizeof(unsigned char), in);
            grid[i][j] = img->p;
        }
    }

    fclose(in);

    int factor = atoi(argv[2]);  // factorul furnizat

    /* presupunem ca radacina arborelui este un nod intern */
    Tree root = createNode(0, 0, 0, internal_node);
    int found_max_undiv = 0;

    /* construirea recursiva a arborelui */
    insertNode(grid, root, 0, 0, img->width, factor, &found_max_undiv);

    /* afisarea informatiilor cerute la prima cerinta */
    if (strcmp("-c1", argv[1]) == 0) {
        FILE *out = fopen(argv[4], "wt");  // deschiderea fisierului de output
        int tree_height = height(root);
        int leaves_number = countLeaves(root);
        fprintf(out, "%d\n", tree_height);
        fprintf(out, "%d\n", leaves_number);
        fprintf(out, "%d\n", found_max_undiv);
        fclose(out);
    } else {

        /* realizarea compresiei conform formatului din a doua cerinta */
        FILE *out = fopen(argv[4], "wb");  // deschiderea fisierului de output
        Queue *q = initQueue();
        fwrite(&img->width, sizeof(unsigned int), 1, out);
        printLevelOrder(root, q, out);
        fclose(out);
        freeQueue(q);
    }

    /* eliberarea memoriei alocate */
    for (i = 0; i < img->width; i++) {
        free(grid[i]);
    }

    free(grid);
    free(img);
    freeTree(root);

    return 0;
}