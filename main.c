//Alunas: Arina Sanches - 392476 e Fernanda Bezerra Nascimento - 388834
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
btree_node** Alocate_vpointer_children(int Tree_nodecapacity){
    btree_node **Vpointer_children = malloc(sizeof(btree_node *)*Tree_nodecapacity);
    for(int i=0; i < Tree_nodecapacity; i++){
        Vpointer_children[i] = malloc(sizeof(btree_node));
    }
    return Vpointer_children;

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
        node->Vpointer_children[i] = NULL;
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
int compare_int(const void *a, const void *b){
    if(*(int*)a == *(int*)b){
        return 0;
    }
    else if(*(int*)a > *(int*)b){
        return 1;
    }
    else{
        return -1;
    }
}

char* Btree_visualisation(btree_node *root){
    char *str_visualization = malloc(sizeof(char)*18);
    char str_num[4];
    str_visualization[0] = '[';
    str_visualization[1] = '\0';
    for (int i = 0; i < root->capacity; ++i) {
        if(i < root->used) {
            if (root->Vkeys[i] < 100) {
                strcat(str_visualization, " ");
            }
            if (root->Vkeys[i] < 10) {
                strcat(str_visualization, " ");
            }

            sprintf(str_num, "%d", root->Vkeys[i]);
            strcat(str_visualization, str_num);
        }
        else {
            sprintf(str_num, "   ");
            strcat(str_visualization, str_num);
        }
        if(i == root->capacity - 1){
            strcat(str_visualization,"]");
        }
        else{
            strcat(str_visualization, "|");
        }
    }
    return str_visualization;
}


btree_node* Btree_insert(btree_node *node, int value, btree_node *Child_right_node){

    if (node->Vpointer_children[0] == NULL || Child_right_node != NULL){
        if (node->used == node->capacity){
            //SPLIT
            int Vkeys_aux_size = node->capacity + 1;
            int *Vkeys_aux = malloc(sizeof(int)*(Vkeys_aux_size));
            btree_node *Right_node = Allocate_btree_node(node->Pointer_father, node->Pointer_next,
                                                         node, node->capacity);
            
            btree_node **Vpointer_children_aux = Alocate_vpointer_children(node->capacity + 1);

            for (int i = 0; i < node->capacity; ++i) {
                Vkeys_aux[i] = node->Vkeys[i];
                Vpointer_children_aux[i] = node->Vpointer_children[i];
            }

            for (int i = 0; i < Vkeys_aux_size - 1; ++i) {
                if(i != Vkeys_aux_size - 2){
                    if (Vkeys_aux[i] > value){
                        for(int j = Vkeys_aux_size -1 ; j > i ; j--){
                            Vkeys_aux[j] = Vkeys_aux[j - 1];
                            Vpointer_children_aux[j + 1] = Vpointer_children_aux[j];
                        }
                        Vkeys_aux[i] = value;
                        Vpointer_children_aux[i+1] = Child_right_node;
                        break;
                    }
                }
                else{
                    Vkeys_aux[Vkeys_aux_size - 1] = value;
                    Vpointer_children_aux[Vkeys_aux_size] = Child_right_node;
                }
            }

            int Middle_position = Vkeys_aux_size/2;

            Right_node->used = node->capacity - Middle_position + 1;
            for (int j = 0; j < Right_node->used; ++j) {
                Right_node->Vkeys[j] = Vkeys_aux[j+Middle_position];
                Right_node->Vpointer_children[j] = Vpointer_children_aux[j+Middle_position];
            }
            Right_node->Vpointer_children[Right_node->used] = Vpointer_children_aux[Vkeys_aux_size];

            node->Pointer_next = Right_node;
            node->used = Middle_position;
            for (int k = 0; k < node->used ; ++k) {
                node->Vkeys[k] = Vkeys_aux[k];
                node->Vpointer_children[k] = Vpointer_children_aux[k];
            }
            node->Vpointer_children[node->used] = Vpointer_children_aux[node->used];

            free(Vkeys_aux);
            //free(Vpointer_children_aux);

            btree_node *Father_node = node->Pointer_father;

            if(Father_node != NULL){
                return Btree_insert(node->Pointer_father, Right_node->Vkeys[0], Right_node);

            }

            else{
                btree_node *Root_node = Allocate_btree_node(node->Pointer_father, node->Pointer_next,
                                                             node, node->capacity);
                node->Pointer_father = Root_node;
                Root_node->Vpointer_children[0] = node;
                Root_node->Vpointer_children[1] = Right_node;
                Root_node->Vkeys[0] = Right_node->Vkeys[0];
                Root_node->used = 1;
                return Root_node;

            }

        }
        else {
            node->Vkeys[node->used] = value;
            node->used++;
            qsort(node->Vkeys, (size_t) node->used, sizeof(int), compare_int);
            while (node->Pointer_father != NULL) {
                node = node->Pointer_father;
            }
            return node;
        }

    }
    else{
        btree_node *correct_child;
        for (int i = 0; i < node->used; i++) {
            if(value < node->Vkeys[i]){
                correct_child = node->Vpointer_children[i];
            }

            else if(i == node->used - 1){
                correct_child = node->Vpointer_children[i+1];
            }

        }
        return Btree_insert(correct_child, value, NULL);
    }

    return NULL;
}

//Função Principal
int main() {
    int test1_size = 4;
    int test1_entries = 16;
    int test1_elements[16] = {22, 35, 41, 12, 17, 43, 8, 27, 64, 50, 5, 32, 61, 25, 52, 19};

    btree_node *tree = NewTree(test1_size);

    for (int i = 0; i < test1_entries; ++i)
    {
        tree = Btree_insert(tree, test1_elements[i], NULL);
        printf("%s\n",Btree_visualisation(tree));
    }
    system("pause");
    return 0;
}