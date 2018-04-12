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

int Count_nodes(btree_node *node, int current_count){
    for (int i = 0; i < node->used+1 ; ++i) {
        if (node->Vpointer_children[i] != NULL) {
            current_count += Count_nodes(node->Vpointer_children[i], 0);
        }
    }
    current_count++;
    return current_count;
}

/// imprime o valor de um nó em uma string
/// \param node - nó q vai ser impresso
/// \return versao impressa do nó passado
char* Print_btree_node(btree_node *node){
    char *str_visualization = malloc(sizeof(char)*18);
    char str_num[4];
    str_visualization[0] = '[';
    str_visualization[1] = '\0';
    for (int i = 0; i < node->capacity; ++i) {
        if(i < node->used) {
            if (node->Vkeys[i] < 100) {
                strcat(str_visualization, " ");
            }
            if (node->Vkeys[i] < 10) {
                strcat(str_visualization, " ");
            }

            sprintf(str_num, "%d", node->Vkeys[i]);
            strcat(str_visualization, str_num);
        }
        else {
            sprintf(str_num, "   ");
            strcat(str_visualization, str_num);
        }
        if(i == node->capacity - 1){
            strcat(str_visualization,"]");
        }
        else{
            strcat(str_visualization, "|");
        }

    }
    return str_visualization;
}

void Print_btree_recursive(btree_node *node, char* current_string, int *num_pointers, int **pointers_pos,
                           int indentation){

    // Imprimir ponteiros anteriores
    if((*num_pointers) > 0) {
        // Imprimir a linha com todos os pontieros
        int pos = 0;
        for (int i = 0; i < (*num_pointers); ++i) {
            for (int j = pos; j < (*pointers_pos)[i]; ++j) {
                strcat(current_string, " ");
                pos++;
            }
            strcat(current_string, "|");
            pos++;
        }
        strcat(current_string, "\n");

        // Imprimir os caracteres antes do nodo atual
        pos = 0;
        for (int i = 0; i < (*num_pointers); ++i) {
            for (int j = pos; j < (*pointers_pos)[i]; ++j) {
                strcat(current_string, " ");
                pos++;
            }
            if(i == (*num_pointers) - 1) {
                strcat(current_string, "+---");
            }
            else {
                strcat(current_string, "|");
            }
            pos++;
        }

        (*num_pointers)--;
    }

    // Imprimir o nodo atual
    strcat(current_string,Print_btree_node(node));

    // Reorganizar os ponteiros
    int pointers_count = 0;
    for (int j = 0; j < node->used + 1 ; ++j) {
        if (node->Vpointer_children[j] != NULL){
            pointers_count ++;
        }
    }

    if (pointers_count > 0) {
        if ((*num_pointers) == 0){
            (*pointers_pos) = malloc(sizeof(int)*pointers_count);
        }
        else{
            (*pointers_pos) = (int*) realloc((*pointers_pos), ((*num_pointers) + pointers_count) * sizeof(int));
        }

        int pointers_pos_start = (*num_pointers);
        for (int k = 0; k < pointers_count ; ++k) {
            (*pointers_pos)[k + pointers_pos_start] = indentation;
            indentation += 4;
        }
        (*num_pointers) += pointers_count;
    }

    for (int i = node->used; i >= 0 ; --i) {
        if(node->Vpointer_children[i] != NULL){
            strcat(current_string, "\n");
            Print_btree_recursive(node->Vpointer_children[i], current_string, num_pointers, pointers_pos, indentation);
            indentation -= 4;
        }
    }
}

///
/// \param root
/// \return
char* Print_btree(btree_node *root){
    int count_nodes = Count_nodes(root, 0);
    char *str_print = malloc(sizeof(char*)*5000);
    str_print[0]    = '\0';
    int num_pointers = 0;
    int **pointerpos = malloc(sizeof(int*));
    (*pointerpos) = NULL;
    Print_btree_recursive(root, str_print, &num_pointers, pointerpos, 0);
    return str_print;
}

/// Insere um valor em um nó da arvore
/// \param node - nó onde o valor deve ser inserido
/// \param value - valor a ser inserido
/// \param Child_right_node - ponteiro da direita do novo valor a ser inserido, caso seja NULL,
///                           o valor a ser inserido eh um valor novo de folha
/// \return retorna a raiz da arvore depois do valor ser inserido
btree_node* Btree_insert(btree_node *node, int value, btree_node *Child_right_node){

    // Checa se o noh atual eh uma folha OU
    // se essa insercao eh uma insercao recursiva em um noh pai
    if (node->Vpointer_children[0] == NULL || Child_right_node != NULL){

        // Checa se o noh atual nao tenha mais espaco para um novo elemento
        if (node->used == node->capacity){

            // Comeca o split do no atual
            // Criacao do vetor de valores auxiliar
            int Vkeys_aux_size = node->capacity + 1;
            int *Vkeys_aux = malloc(sizeof(int)*(Vkeys_aux_size));

            // Alocacao do novo noh da direita geredo no split
            btree_node *Right_node = Allocate_btree_node(node->Pointer_father, node->Pointer_next,
                                                         node, node->capacity);

            // Alocacao do vetor de ponteiros auxiliar para divisao dos ponteiros no noh
            btree_node **Vpointer_children_aux = Alocate_vpointer_children(node->capacity + 1);
            for (int i = 0; i < node->capacity+1; ++i) {
                if (i != node->capacity) {
                    Vkeys_aux[i] = node->Vkeys[i];
                }
                Vpointer_children_aux[i] = node->Vpointer_children[i];
            }

            // Insere o novo valor no vetor de valores auxiliar e tambem o ponteiro da direita no
            // vetor de ponteiros auxiliar, os valores maiores que o novo valor vao ser movidos
            // pra direita junto com os ponteiros
            for (int i = 0; i < Vkeys_aux_size; ++i) {
                if(i != Vkeys_aux_size - 1){
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

            // Ponto onde o vetor de valores auxiliar vai ser dividido
            int Middle_position = Vkeys_aux_size/2;

            // Passa todos o valores que devem ir pro noh da direita
            Right_node->used = node->capacity - Middle_position + 1;

            int adjust = 0;
            // Caso o noh nao seja uma noh folha, nao devemos passar o valor do meio na divisao. Para isso diminuimos o
            // numero de elementos do noh da direita e usamos "adjust" pra pegar os ultimos valores do vetor auxiliar
            if (Vpointer_children_aux[0] != NULL) {
                adjust = 1;
                Right_node->used--;
            }
            for (int j = 0 ; j < Right_node->used; ++j) {
                Right_node->Vkeys[j] = Vkeys_aux[j + Middle_position + adjust];
                Right_node->Vpointer_children[j] = Vpointer_children_aux[j+Middle_position + adjust];
            }
            Right_node->Vpointer_children[Right_node->used] = Vpointer_children_aux[Vkeys_aux_size];

            // Ajusta o noh atual pra ser o novo noh da esquerda, removendo os elementos q foram
            // pro outro noh
            node->Pointer_next = Right_node;
            node->used = Middle_position;
            for (int k = 0; k < node->used ; ++k) {
                node->Vkeys[k] = Vkeys_aux[k];
                node->Vpointer_children[k] = Vpointer_children_aux[k];
            }
            node->Vpointer_children[node->used] = Vpointer_children_aux[node->used];

            // Libera a memoria usada pelos vetores auxiliares


            btree_node *Father_node = node->Pointer_father;

            // Checa se o noh atual tem pai
            if(Father_node != NULL){

                // Caso o noh atual tenha pai, fazemos o noh da direita tambem apontar pra ele
                // como pai e inserimos o elemento do meio da divisao nesse noh pai junto com o
                // novo ponteiro da direita
                Right_node->Pointer_father = Father_node;
                return Btree_insert(node->Pointer_father, Vkeys_aux[Middle_position], Right_node);

            }
            else{ // Caso o noh atual nao tenha pai

                // Criamos um pai novo
                btree_node *Root_node = Allocate_btree_node(node->Pointer_father, node->Pointer_next,
                                                             node, node->capacity);

                // Fazemos o noh atual apontar pro pai novo
                node->Pointer_father = Root_node;

                // Colocamos o valor do meio no pai novo com os ponteiros da esquerda e direita
                Root_node->Vpointer_children[0] = node;
                Root_node->Vpointer_children[1] = Right_node;
                Root_node->Vkeys[0] = Vkeys_aux[Middle_position];
                Root_node->used = 1;

                // Fazemos o noh da direita apontar pro pai novo
                Right_node->Pointer_father = Root_node;

                // Retornamos o novo noh raiz
                return Root_node;

            }

        }
        else { // Caso o no atual tenha espaco pra inserir o novo valor


            // Insere o valor novo na posicao correta junto com o ponteiro da direita dele e tambem
            // move os valores maiores junto com os ponteiros deles uma casa pra direita
            for (int i = 0; i < node->used + 1; ++i) {
                if(i != node->used){
                    if (node->Vkeys[i] > value){
                        for(int j = node->used ; j >= i ; j--){
                            node->Vkeys[j] = node->Vkeys[j - 1];
                            node->Vpointer_children[j + 1] = node->Vpointer_children[j];

                        }
                        node->Vkeys[i] = value;
                        node->Vpointer_children[i+1] = Child_right_node;
                        break;
                    }
                }
                else{
                    node->Vkeys[i] = value;
                    node->Vpointer_children[i+1] = Child_right_node;
                }
            }
            node->used++;

            // Procura a raiz da arvore
            while (node->Pointer_father != NULL) {
                node = node->Pointer_father;
            }

            // Retorna a raiz da arvore
            return node;
        }

    }
    else{ // Caso nao o noh nao seja uma folha E nao seja uma insercao em pai

        // Temos q descer na arvore para achar o noh correto onde esse valor deve ser inserido

        // Procuramos o ponteiro pro noh que devemos descer para insercao
        btree_node *correct_child;
        for (int i = 0; i < node->used; i++) {
            if(value < node->Vkeys[i]){
                correct_child = node->Vpointer_children[i];
                break;
            }

            else if(i == node->used - 1){
                correct_child = node->Vpointer_children[i+1];
                break;
            }

        }

        // Insercao recursiva no noh encontrado
        return Btree_insert(correct_child, value, NULL);
    }

    return NULL;
}

// Função Principal
int main() {

    // Lista de valores aleatorios para fazer caso de teste da nossa inseracao sequencial
    int test1_size = 4;
    int test1_entries = 16;
    int test1_elements[16] = {22, 35, 41, 12, 17, 43, 8, 27, 64, 50, 5, 32, 61, 25, 52, 19};

    // Criacao da arvore fazia
    btree_node *tree = NewTree(test1_size);

    // Insercao em massa (Bulk loading)
    for (int i = 0; i < test1_entries; ++i) {
        tree = Btree_insert(tree, test1_elements[i], NULL);
        // Impressao da arvore
        printf("%d\n", test1_elements[i]);
        printf("%s\n", Print_btree(tree));
        printf("%d\n\n", Count_nodes(tree,0));
    }

    system("pause");
    return 0;
}