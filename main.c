//Alunas: Arina Sanches - 392476 e Fernanda Bezerra Nascimento - 388834
#include <stdio.h>
#include <stdlib.h>

const int Tree_nodecapacity = 9;
//Definição das stucts da árvore
typedef struct btree_node btree_node;

struct btree_node {
    struct btree_node **Vpointer_children;
    struct btree_node *Pointer_father;
    struct btree_node *Pointer_next; //Para nós folhas
    struct btree_node *Pointer_previous; //Para nós folhas
    int *Vkeys;
    int used;
    int capacity;
};

//Função para alocar espaço do vetor de ponteiros para os filhos do nó
btree_node** Alocate_vpointer_children(){
    btree_node **Vpointer_children = malloc(sizeof(btree_node *)*Tree_nodecapacity);
    for(int i=0; i < Tree_nodecapacity; i++){
        Vpointer_children[i] = malloc(sizeof(btree_node));
    }
    return Vpointer_children;

}

void Btree_insert(btree_node *node, int value){
    if (node->used == Tree_nodecapacity){

    }
}

btree_node* Btree_searchEquality(btree_node *node, int key)
{
    int Root_capacity = node->used;

    if (node->Vpointer_children == NULL){
        for (int j = 0; j < Root_capacity; j++){
            if (node->Vkeys[j] == key){
                return node;
            }
        }
    }
    else {
        for (int i = 0; i < Root_capacity; i++) {
            if (i == Root_capacity - 1) {
                if (node->Vkeys[i] < key) {
                    return Btree_searchEquality(node->Vpointer_children[i + 1], key);

                }
            }
            else {
                if (key < node->Vkeys[i]) {
                    return Btree_searchEquality(node->Vpointer_children[i], key);

                } else if (node->Vkeys[i] <= key && node->Vkeys[i + 1] > key) {
                    return Btree_searchEquality(node->Vpointer_children[i + 1], key);

                }
            }
        }
    }
    return NULL;
}

btree_node* Allocate_btree_node(btree_node *Pointer_father,
    btree_node *Pointer_next, btree_node *Pointer_previous, int capacity){
    btree_node *node = malloc(sizeof(btree_node));
    node->Vpointer_children = malloc(sizeof(btree_node*) * (capacity + 1));
    for (int i = 0; i < (capacity + 1); ++i)
    {
        node->Vpointer_children = NULL;
    }
    node->Pointer_father = Pointer_father;
    node->Pointer_next = Pointer_next;
    node->Pointer_previous = Pointer_previous;
    node->Vkeys = malloc(sizeof(int) * capacity);
    node->used = 0;
    node->capacity = capacity;
    return node;
}

btree_node* NewTree(int size)
{
    return Allocate_btree_node(NULL,NULL,NULL, size);
}

btree_node *Btree_instert(btree_node *root, int value)
{
    // TODO! :D

    return NULL;
}

//Função Principal
int main() {
    int test1_size = 4;
    int test1_entries = 16;
    int test1_elements[test1_entries] = {22, 35, 41, 12, 17, 43, 8, 27, 64, 50, 5, 32, 12, 61, 25, 52};

    btree_node *tree = NewTree(test1_size);

    for (int i = 0; i < test1_entries; ++i)
    {
        tree = Btree_instert(tree, test1_elements[i]);
    }
    system("pause");
    return 0;
}