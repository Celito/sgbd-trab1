//Alunas: Arina Sanches - 392476 e Fernanda Bezerra Nascimento - 388834
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Ocupação mínima da árvore
const int minimum_occupancy = 4;

//Definição das stucts da árvore
typedef struct btree_node btree_node;

//Struct do nó
struct btree_node {
    struct btree_node **Vpointer_children;
    struct btree_node *Pointer_father;
    struct btree_node *Pointer_next; //Para nós folhas
    struct btree_node *Pointer_previous; //Para nós folhas
    int *Vkeys;
    int used;
    int capacity;
};

///Função auxiliar para o qsort
int cmpfunc (const void * a, const void * b) {
    if(*(int*)a < *(int*)b){
        return -1;
    }
    else if (*(int*)a == *(int*)b){
        return 0;
    }
    else {
        return 1;
    }
}

/// Função para alocar espaço do vetor de ponteiros para os filhos do nó
/// \param Tree_nodecapacity = É a capacidade de um nó
/// \return o vetor de ponteiros Vpointer_children
btree_node** Alocate_vpointer_children(int Tree_nodecapacity){
    btree_node **Vpointer_children = malloc(sizeof(btree_node *)*Tree_nodecapacity);
    for(int i=0; i < Tree_nodecapacity; i++){
        Vpointer_children[i] = malloc(sizeof(btree_node));
    }
    return Vpointer_children;

}

btree_node* Btree_searchEquality(btree_node *node, int key)
{
    int Root_used = node->used;

    if (node->Vpointer_children[0] == NULL){
        for (int j = 0; j < Root_used; j++){
            if (node->Vkeys[j] == key){
                return node;
            }
        }
    }
    else {
        for (int i = 0; i < Root_used + 1; i++) {
            if (i == Root_used) {
                if (node->Vkeys[i] <= key) {
                    if(node->Vpointer_children[i+1] != NULL){
                        return Btree_searchEquality(node->Vpointer_children[i + 1], key);
                    }
                }
            }
            else {
                if (key < node->Vkeys[i]) {
                    if(node->Vpointer_children[i+1] != NULL) {
                        return Btree_searchEquality(node->Vpointer_children[i], key);
                    }

                } else if (node->Vkeys[i] <= key && node->Vkeys[i + 1] > key) {
                    if(node->Vpointer_children[i+1] != NULL) {
                        return Btree_searchEquality(node->Vpointer_children[i + 1], key);
                    }
                }
            }
        }
    }
    return NULL;
}

/// Função para alocar um nó de árvore
/// \param Pointer_father   = ponteiro para o pai, se for raiz será NULO
/// \param Pointer_next     = ponteiro para o irmão da direita se for um nó não folha, sera NULO
/// \param Pointer_previous = ponteiro para o irmão da esquerda se for um nó não folha, sera NULO
/// \param capacity         = é a quantidade de chaves que cabem em um nó e a quantidades de ponteiros em um nó será
///                           capacity + 1
/// \return                 = Um novo nó com a memória já alocada
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

/// Função para criar uma nova árvore
/// \param capacity = é a quantidade de chaves que cabem em um nó e a quantidades de ponteiros em um nó será
///                   capacity + 1
/// \return o ponteiro para uma nova árvore
btree_node* NewTree(int capacity)
{
    return Allocate_btree_node(NULL,NULL,NULL, capacity);
}

/// Conta quantos nós tem em uma árvore
/// \param node          = nó de arvore
/// \param current_count = contador para o calculo da quantidade de nós
/// \return um inteiro com a quantidade de nós na árvore
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

/// Função para imprimir | ou +--- de acordo com a posição
/// \param node            = Nó que vamos imprimir
/// \param current_string  = A string atual
/// \param num_pointers    = Quantidade de ponteiros para filhos o nó possui (alteramos a variavel em todos os escopos
///                          pois passamos diretamente o endereço de memoria dessa variavel)
/// \param pointers_pos    = Posição que os ponteiros serao impressos (Caso eu tenha 2 ponteiros em um nó que so tem
///                          uma chave vamos imprimir os ponteiros nas posições 0, 4 pois cada espaço de chave na
///                          visualização possui 4 espaços)
/// \param indentation     = Parametro que é alterado em todo o escopo para saber o quao indentado devemos estar
void Print_btree_recursive(btree_node *node, char* current_string, int *num_pointers, int **pointers_pos,
                           int indentation){

    // Imprimir ponteiros anteriores
    if((*num_pointers) > 0) {
        // Imprimir a linha com todos os ponteiros
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

/// Função para imprimir a arvore na qual contaremos o numero de nós em uma chave, alocaremos o espeço para as variáveis
///    e chamamos a função para concatenar a string e depois imprimi-la na tela
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

    // Checa se o nó atual eh uma folha OU
    // se essa insercao eh uma insercao recursiva em um nó pai

    if (node->Vpointer_children[0] == NULL || Child_right_node != NULL){
        // Checa se o nó atual nao tenha mais espaco para um novo elemento
        if (node->used == node->capacity){

            // Comeca o split do nó atual
            // Criacao do vetor de valores auxiliar
            int Vkeys_aux_size = node->capacity + 1;
            int *Vkeys_aux = malloc(sizeof(int)*(Vkeys_aux_size));

            // Alocacao do novo nó da direita geredo no split
            btree_node *Right_node = Allocate_btree_node(node->Pointer_father, node->Pointer_next,
                                                         node, node->capacity);

            // Alocacao do vetor de ponteiros auxiliar para divisao dos ponteiros no nó
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
            // Caso o nó nao seja uma nó folha, nao devemos passar o valor do meio na divisao. Para isso diminuimos o
            // numero de elementos do nó da direita e usamos "adjust" pra pegar os ultimos valores do vetor auxiliar
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

/// Função para fazer a busca por intervalo em uma árvore B+
/// \param node    = nó da árvore
/// \param key_min = o menor valor do intervalo
/// \param key_max = o maior valor do intervalo
/// \return um vetor de inteiros com as chaves encontradas nesse intervalo
int* Btree_searchInterval(btree_node *node, int key_min, int key_max, int *size_of_vector){
    int Root_used = node->used;

    if (node->Vpointer_children[0] == NULL){
        int* keys = malloc(sizeof(int));
        int index = 1, control = 1;

        while(node != NULL && control == 1){

            for(int k = 0; k < node->used; k++){
                if(node->Vkeys[k] >= key_min && node->Vkeys[k] <= key_max){
                    keys = (int*) realloc(keys, index * sizeof(int));
                    keys[index - 1] = node->Vkeys[k];
                    index ++;
                }else if(node->Vkeys[k] > key_max){
                    control = 0;
                }
            }
            node = node->Pointer_next;
        }
        *size_of_vector = index;
        if(index > 0){
            return keys;
        }else{
            return NULL;
        }
    }
    else {
        for (int i = 0; i < Root_used + 1; i++) {
            if (i == Root_used) {
                if (node->Vkeys[i] <= key_min) {
                    if(node->Vpointer_children[i+1] != NULL) {
                        return Btree_searchInterval(node->Vpointer_children[i + 1], key_min, key_max, size_of_vector);
                    }
                }
            }
            else {
                if (key_min < node->Vkeys[i]) {
                    if(node->Vpointer_children[i + 1] != NULL) {
                        return Btree_searchInterval(node->Vpointer_children[i], key_min, key_max, size_of_vector);
                    }

                } else if (node->Vkeys[i] <= key_min && node->Vkeys[i + 1] > key_min) {
                    if(node->Vpointer_children[i+1] != NULL) {
                        return Btree_searchInterval(node->Vpointer_children[i + 1], key_min, key_max, size_of_vector);
                    }
                }
            }
        }
    }
    return NULL;
}

btree_node* Fix_tree(btree_node *node){
    return node;
}

btree_node* Btree_delete(btree_node *node, int value){

    btree_node *node_aux = Btree_searchEquality(node, value);

    if (node_aux != NULL){
        //Eliminando a chave do no
        for(int i = 0; i < node_aux->used; i++){
            if(node_aux->Vkeys[i] == value){
                for(int j = i; j < node_aux->used; j++){
                    node_aux->Vkeys[j] = node_aux->Vkeys[j+1];
                }
                node_aux->used --;
            }
        }

        if(node_aux->used < minimum_occupancy){

            // caso tenha um irmaoo a direita
            if(node_aux->Pointer_next != NULL && node_aux->Pointer_next->used - 1 >= minimum_occupancy){

                //adicionando o valor de chave no primeiro no(o que estava com a capacidade menor que a minima)
                node_aux->Vkeys[node_aux->used] = node_aux->Pointer_next->Vkeys[0];
                node_aux->used ++;

                printf("qtd uedv%d \n", node_aux->Pointer_next->used );
                for(int i = 0; i < node_aux->Pointer_next->used - 1 ; i++){
                    node_aux->Pointer_next->Vkeys[i] = node_aux->Pointer_next->Vkeys[i +1];
                }
                node_aux->Pointer_next->used --;

                // Ajustar o valor de chave do pai para apontar para o menor valor da segunda chave
                for(int i = 0; i < node_aux->Pointer_father->used; i++){
                    if(node_aux->Pointer_father->Vpointer_children[i] == node_aux){
                        node_aux->Pointer_father->Vkeys[i] = node_aux->Pointer_next->Vkeys[0];
                    }
                }
                return node_aux;
                // caso o no tenha um irmao a esquerda
            } else if(node_aux->Pointer_previous != NULL && node_aux->Pointer_previous->used - 1 >= minimum_occupancy){

                //empurrando todas as chaves de pesquisa uma posicao para frente
                for(int i = node_aux->used +1; i > 0 ; i--){
                    node_aux->Vkeys[i] = node_aux->Vkeys[i - 1];
                }
                //Adicionar a segunda chave o ultimo elemento da primeira
                node_aux->Vkeys[0] = node_aux->Pointer_previous->Vkeys[node_aux->Pointer_previous->used -1];
                node_aux->used ++;


                //eliminar a ultima chave do primeiro o
                node_aux->Pointer_previous->used --;

                // Ajustar o valor de chave do pai para apontar para o menor valor da segunda chave
                for(int i = 0; i < node_aux->Pointer_father->used; i++){
                    if(node_aux->Pointer_father->Vpointer_children[i] == node_aux){
                        node_aux->Pointer_father->Vkeys[i -1] = node_aux->Vkeys[0];
                    }
                }
                return node_aux;
                // caso nao seja possivel emprestar de nenhum irmao, fazer o merge a esquerda
            }else if(node_aux->Pointer_previous != NULL){
                btree_node* first_node = node_aux->Pointer_previous;

                //intercalando os nos folha, juntar com o no da direita
                for(int i = 0; i < node_aux->used; i++){
                    first_node->Vkeys[first_node->used + i] = node_aux->Vkeys[i];
                    first_node->used ++;
                }

                //ajustando os ponteiro de next e previous
                first_node->Pointer_next = node_aux->Pointer_next;
                if(node_aux->Pointer_next != NULL) {
                    node_aux->Pointer_next->Pointer_previous = first_node;
                }

                btree_node* father_node = first_node->Pointer_father;

                //eliminar o ponteiro que aponta para o primeiro(o da esquerda) no


                if(father_node->used > 1) {
                    for (int i = 0; i < father_node->used; i++) {
                        if (father_node->Vpointer_children[i] == first_node) {
                            for (int j = i; j < father_node->used - 1; j++) {
                                father_node->Vkeys[i] = father_node->Vkeys[i + 1];
                            }
                            for (int j = i; j < father_node->used; j++) {
                                father_node->Vpointer_children[i + 1] = father_node->Vpointer_children[i + 2];
                            }
                        }
                    }
                    father_node->used--;
                }

                // se o pai ficar com capacidade minima
                if(father_node->used < minimum_occupancy){
                    return Fix_tree(father_node);
                }
            }
        }
        // caso a remocao nao faz o no ter subocupacao
        return node_aux;
    }
    // caso a chave nao esteja na arvore
    return NULL;
}

/// Função para inserir os nós na árvore
/// \param tree             = árvore
/// \param btree_new_nodes  = vetor para os novos nós que criamos na função Btree_bulk_loading
/// \param btree_capacity   = capacidade da árvore
/// \param number_new_nodes = numero de novos nós criados
/// \return a raiz da arvore
btree_node* Btree_bulk_loading_insertion(btree_node* tree, btree_node **btree_new_nodes, int btree_capacity,
                                         int number_new_nodes) {
    if (number_new_nodes == 1){
        return btree_new_nodes[0];
    }
    tree->Vpointer_children[0] = btree_new_nodes[0];
    tree->Vpointer_children[1] = btree_new_nodes[1];
    btree_new_nodes[0]->Pointer_father = tree;
    btree_new_nodes[1]->Pointer_father = tree;
    tree->Vkeys[0] = btree_new_nodes[1]->Vkeys[0];
    tree->used++;
    for (int i = 2; i < number_new_nodes; ++i) {
        btree_new_nodes[i]->Pointer_father = tree;
        tree = Btree_insert(tree, btree_new_nodes[i]->Vkeys[0], btree_new_nodes[i]);
    }

    return tree;
}

/// Recebe um vetor de inteiros o ordena e o divide em nós para inseção em massa na árvore
/// \param tree           = arvore
/// \param Vkeys          = Vetores com chaves para serem ordenados
/// \param btree_capacity = capacidade da arvore
/// \param number_keys    = numero de chaves no VKEYS
/// \return a raiz da arvore
btree_node* Btree_bulk_loading(btree_node *tree, int *Vkeys, int btree_capacity, int number_keys){
    //ordenar o vetor
    //number_keys representa o numero de elementos no vetor
    qsort(Vkeys, (size_t)number_keys, sizeof(int), cmpfunc);
    int number_new_nodes = number_keys/btree_capacity;
    if(number_keys%btree_capacity != 0){
        number_new_nodes ++;
    }
    btree_node **btree_new_nodes = malloc(sizeof(btree_node *) * number_new_nodes);
    for (int k = 0; k < number_new_nodes; ++k) {
        btree_new_nodes[k] = Allocate_btree_node(NULL, NULL, NULL, btree_capacity);
    }
    //divisão das paginas
    int index_aux = 0;
    int keys_to_insert = 0;
    for (int j = 0; j < number_new_nodes; ++j) {
        if((j + 1)*btree_capacity <= number_keys){
            keys_to_insert = btree_capacity;
        }
        else{
            keys_to_insert = number_keys%btree_capacity;
        }
        for (int i = 0; i < keys_to_insert; ++i) {
            btree_new_nodes[j]->Vkeys[i] = Vkeys[index_aux];
            index_aux++;
        }
        btree_new_nodes[j]->used = keys_to_insert;
        for (int k = 0; k < number_new_nodes - 1 ; ++k) {
            if (k == 0){
                btree_new_nodes[k]->Pointer_previous = NULL;
            }
            else if (k == number_new_nodes - 1){
                btree_new_nodes[k+1]->Pointer_next = NULL;
            }
            else if(k!=0) {
                btree_new_nodes[k]->Pointer_next = btree_new_nodes[k + 1];
                btree_new_nodes[k]->Pointer_previous = btree_new_nodes[k - 1];
            }
        }
    }
    return Btree_bulk_loading_insertion(tree, btree_new_nodes, btree_capacity, number_new_nodes);
}

// Função Principal
int main() {

    // Lista de valores aleatorios para fazer caso de teste da nossa inseracao sequencial
    int option;
    int *search_by_interval;
    int value, value_min, value_max, aux, flag = 0;
    int btree_capacity = 9;

    btree_node *tree = NewTree(btree_capacity);

    while(flag == 0){
        //Menu para o usuário
        printf("|          ++++ MENU DE OPCOES ++++         |\n");
        printf("|___________________________________________|\n");
        printf("| ++++ Escolha uma opcao ++++               |\n");
        printf("|     ++++ 1 - Visualizacao da arvore ++++  |\n");
        printf("|     ++++ 2 - Insercao   na   arvore ++++  |\n");
        printf("|     ++++ 3 - Delecao    da   arvore ++++  |\n");
        printf("|     ++++ 4 - Busca   por  igualdade ++++  |\n");
        printf("|     ++++ 5 - Busca   por  intervalo ++++  |\n");
        printf("|     ++++ 6 - Bulk  loading          ++++  |\n");
        printf("|___________________________________________|\n");
        printf("|---> ");
        scanf("%d", &option);
        printf("|___________________________________________|\n");

        if (option == 1) {
            printf("| ++++    Visualizacao      ++++            |\n");
            printf("%s\n", Print_btree(tree));
        }

        if (option == 2) {
            printf("| ++++    Insercao       ++++               |\n");
            printf("|Digite um valor positivo e menor que 999:  |\n");
            printf("|---> ");
            scanf("%d", &value);
            while(Btree_searchEquality(tree, value) != NULL){
                printf("\n|Esse numero ja existe, digite outro:       |\n");
                scanf("%d", &value);
            }
            tree = Btree_insert(tree, value, NULL);
            printf("|        INSERCAO EFETUADA                  |\n");
            printf("|___________________________________________|\n");

        }

        if (option == 3) {
            printf("| ++++    Delecao        ++++               |\n");
            printf("|Digite um valor positivo e menor que 999:  |\n");
            printf("|---> ");
            scanf("%d", &value);
            Btree_delete(tree, value);

        }

        if (option == 4) {
            printf("| ++++   Busca   por  igualdade   ++++      |\n");
            printf("|Digite um valor positivo e menor que 999:  |\n");
            printf("|---> ");
            scanf("%d", &value);
            if (Btree_searchEquality(tree, value) != NULL) {
                printf("|         ENCONTRADO                       |\n");
                printf("|__________________________________________|\n");
            } else {
                printf("|        NAO ENCONTRADO                    |\n");
                printf("|__________________________________________|\n");
            }
        }

        if (option == 5) {
            printf("| ++++   Busca   por  intervalo   ++++      |\n");
            printf("|Digite o menor valor positivo              |\n");
            printf("|e menor que 999:                           |\n");
            printf("|---> ");
            scanf("%d", &value_min);
            printf("\n|Digite o menor valor positivo              |\n");
            printf("|e maior que 999:                           |\n");
            printf("|---> ");
            scanf("%d", &value_max);
            int size_search_vector = 0;
            search_by_interval = Btree_searchInterval(tree, value_min, value_max, &size_search_vector);
            for (int i = 0; i < size_search_vector - 1; ++i) {
                printf("|%d|", search_by_interval[i]);
            }
        }

        if (option == 6) {
            if(tree->used != 0){
                printf("Nao e possivel efetuar, a arvore nao esta vazia");
            }
            else {
                printf("| ++++   Bulk Loading             ++++      |\n");
                printf("|Digite quantos valores deseja inserir:     |\n");
                printf("|---> ");
                scanf("%d", &aux);
                int Vector_int[aux];
                for (int i = 0; i < aux; ++i) {
                    printf("|Digite um valor positivo e menor que 999:  |\n");
                    printf("|---> ");
                    scanf("%d", &Vector_int[i]);
                }
                tree = Btree_bulk_loading(tree, Vector_int, btree_capacity, aux);
            }
        }
        printf("|___________________________________________|\n");
        printf("|            Deseja continuar?              |\n");
        printf("|               0 - SIM                     |\n");
        printf("|               1 - NAO                     |\n");

        printf("|---> ");
        scanf("%d",&flag);
    }

    system("pause");
    return 0;
}