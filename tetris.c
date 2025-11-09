#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5    //define o tamanho maximo da fila
#define TAM_PILHA 3   //define o tamanho maximo da pilha

//estrutura que representa uma peça
typedef struct {
    char tipo;    //tipo da peça (I o T L)
    int id;        //identificador unico da peça
} Peca;

//estrutura que representa uma fila circular
typedef struct {
    Peca pecas[TAM_FILA];  //array que armazena as peças da fila
    int inicio;           //indice da primeira peça na fila
    int fim;              //indice logo após a última peça inserida
    int tamanho;          //quantidade atual de peças na fila
    int proximoId;        //proximo id a ser atribuido a uma nova peça
} Fila;

//estrutura que representa uma pilha
typedef struct {
    Peca pecas[TAM_PILHA];  //array que armazena as peças da pilha
    int topo;              //indice do topo da pilha. -1 se vazia
} Pilha;

//array que define os tipos de peças no jogo
char tipos[] = { 'I', 'O', 'T', 'L' };

// FUNCOES DE INICIALIZARCOES

//inicializa uma pilha
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1; // Define o topo como -1 para indicar que a pilha está vazia
}

//inicializa uma fila
void inicializarFila(Fila *fila) {
    fila->inicio = 0;  //indice do primeiro elemento da fila
    fila->fim = 0;      //indice do proximo local disponivel para inserir uma peça
    fila->tamanho = 0;  //contador de quantos elementos a fila possui atualmente
    fila->proximoId = 0; //ID que sera atribuído para próxima peça inserida
}

// FUNCOES AUXILIARES
//verifica se tem pelo menos 3 peças na pilha
int tem_tres_pecas(Pilha *pilha) {
    if (pilha->topo < 2) {          //como o indice do topo começa em 0, topo < 2 significa menos de 3 peças
        printf("\nNão tem peças suficientes na pilha\n");
        return 0;   // Retorna 0 indicando que não há peças suficientes
    }
    return 1; // Retorna 1 se tem pelo menos 3 peças
}

//VERIFICA SE A FILA ESTA CHEIA
int filaCheia(Fila *fila) {
    return fila->tamanho == TAM_FILA;
}

//VERIFICA SE A PILHA ESTA CHEIA
int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;    
}

//VERIFICA SE A PILHA ESTA VAZIA
int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

//GERACAO DE PECAS ALEATORIAS   
Peca gerarPeca(int id) {
    Peca p;
    //escolhe um tipo aleatório de peça entre char tipos
    p.tipo = tipos[rand() % 4];
    p.id = id;
    return p;
}

//ADICIONAR PECA (FILA) peça no final da fila
int enfileirar_fila(Fila *fila) {
    //verifica se a fila ta cheia antes de inserir
    if (filaCheia(fila)) {
        return 0;
    }
    //gera um novo id para a peça e incrementa o contador de id
    int id = fila->proximoId++;
    //cria uma nova peça com tipo aleatorio e o id
    Peca p = gerarPeca(id);
    //insere a peça na posição no final
    fila->pecas[fila->fim] = p;
    //atualiza o índice fim com comportamento circular
    fila->fim = (fila->fim + 1) % TAM_FILA;
    //incrementa o contador de tamanho da fila
    fila->tamanho++;

    return 1;
}

//PREENCHER FILA AUTOMATICAMENTE SEMPRE PARA MANTE-LA CHEIA
void preencherFila(Fila *fila) {
    while (!filaCheia(fila)) {
        enfileirar_fila(fila);
    }
}

//PREENCHER PILHA com peças aleatórias
void preencherPilha(Pilha *pilha) {
    //percorre todas as posicoes da pilha
    for (int i = 0; i < TAM_PILHA; i++) {
        //gera uma nova peça com ID igual ao indice atual
        pilha->pecas[i] = gerarPeca(i);
        //o topo da pilha aponta para o ultimo elemento inserido
        pilha->topo++;
    }
}

//REMOVER PECA (FILA)
int jogarPeca(Fila *fila, Peca *p) {
    *p = fila->pecas[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAM_FILA;
    fila->tamanho--;    //diminui o contador de peças da fila

    return 1;  //sucesso na remoção
}

//REMOVER PECA (PILHA)
int jogar_reserva(Pilha *pilha, Peca *p) {
    if (pilhaVazia(pilha)) {
        printf("\nPilha vazia! Nenhuma peça na reserva.\n");
        return 0;  //falhou
    }
    *p = pilha->pecas[pilha->topo--];
    return 1;      //suxesso
}

//move uma peça da fila para a pilha de reserva
void reservarPeca(Pilha *pilha, Fila *fila) {
    // Verifica se a pilha tá cheia
    if (pilhaCheia(pilha)) {
        printf("\nPilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    Peca p; //variavel temporaria para armazenar a peça retirada da fila

    //remover uma peça da fila para reservar simulando a jogada
    if (jogarPeca(fila, &p)) {
        //inncrementa o topo e adiciona a peça na pilha
        pilha->pecas[++pilha->topo] = p;
        // Exibe mensagem de sucesso 
        printf("Peça [%c %d] reservada com sucesso!\n", p.tipo, p.id);
    }
}

//troca as 3 peças do topo da pilha com as 3 primeiras peças da fila
void trocarTresPecas(Pilha *pilha, Fila *fila) {
    int i;
    // Verifica se a pilha tem pelo menos 3 peças
    if (!tem_tres_pecas(pilha))
        return;

    Peca tempPilha[3]; //armazena temporariamente as 3 peças da pilha
    Peca tempFila[3];  //armazena temporariamente as 3 peças da fila

    // Copia as 3 do topo da pilha
    for (i = 0; i < 3; i++)
        tempPilha[i] = pilha->pecas[pilha->topo - (2 - i)];

    // Copia as 3 primeiras da fila
    for (i = 0; i < 3; i++) {
        int pos = (fila->inicio + i) % TAM_FILA;
        tempFila[i] = fila->pecas[pos];
    }

    //substitui as 3 do topo da pilha pelas da fila
    for (i = 0; i < 3; i++)
        pilha->pecas[pilha->topo - (2 - i)] = tempFila[i];

    //substitui as 3 primeiras da fila pelas da pilha
    for (i = 0; i < 3; i++) {
        int pos = (fila->inicio + i) % TAM_FILA;
        fila->pecas[pos] = tempPilha[i];
    }
}

//trocar uma peca da fila com uma da pilha
void trocar_uma_peca(Pilha *pilha, Fila *fila) {
    //verifica se há peças para trocar
    if (pilhaVazia(pilha)) {
        printf("\nErro: Pilha vazia\n");
        return;
    }
    // variavel auxiliar
    Peca temp;
    //copia a peça do topo da pilha
    temp = pilha->pecas[pilha->topo];
    //substitui o topo da pilha pela primeira da fila
    pilha->pecas[pilha->topo] = fila->pecas[fila->inicio];
    //substitui a primeira da fila pela peça que estava no topo da pilha
    fila->pecas[fila->inicio] = temp;
}    


//mostrar todas as peças armazenada na fila
void exibirFila(Fila *fila) {
    printf("\nFila atual: ");
    for (int i = 0; i < fila->tamanho; i++) {
        int idx = (fila->inicio + i) % TAM_FILA;
        //Mostra o tipo e o ID da peça
        printf("[%c %d] ", fila->pecas[idx].tipo, fila->pecas[idx].id);
    }
    printf("\n");
}

//mostrar todas as peças armazenada na pilha
void exibir_Pilha(Pilha *pilha) {
    printf("\nPilha atual (topo): ");
    //se a pilha estiver vazia
    if (pilhaVazia(pilha)) {
        printf("[vazia]\n");
        return;
    }
    //percorre do topo até a base da pilha
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%c, %d] ", pilha->pecas[i].tipo, pilha->pecas[i].id);
    }
}


int main() {
    //inicializa o gerador de numeros aleatorios
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    
    //inicializacoes
    inicializarPilha(&pilha);
    inicializarFila(&fila);
    
    //preenchendo as filas e pilha
    preencherPilha(&pilha);
    preencherFila(&fila);
    //exibe estado inicial da fila e da pilha
    exibirFila(&fila);
    exibir_Pilha(&pilha);
  

    int opcao;
    do {
        preencherFila(&fila); // GARANTE QUE SEMPRE VAI ESTA CHEIA
        // Menu de opções
        printf("\n\n--- Menu ---\n");
        printf("\n1. Jogar peça\n");
        printf("2. Enviar peça da fila para a pilha de reserva(pilha)\n");
        printf("3. Usar uma peça da reserva(pilha)\n");
        printf("4. Trocar peça da frente da fila com o topo da pilha\n");
        printf("5. Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("6. Sair\n");
        printf("\nEscolha uma opção: ");
        //leitura da opção do usuário
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            //jogar a peça da frente da fila
            case 1: {
                Peca p;
                if (jogarPeca(&fila, &p)) {
                    printf("\nVocê jogou a peça [%c %d]\n", p.tipo, p.id);
                    preencherFila(&fila); // preenche automaticamente
                }
                exibirFila(&fila);
                exibir_Pilha(&pilha);
                break;
            }    
            
            //reservar peça da fila para a pilha
            case 2: {
                reservarPeca(&pilha, &fila);
                preencherFila(&fila); // preenche automaticamente
                exibirFila(&fila);
                exibir_Pilha(&pilha);
                break;
            }

            //usar peça da pilha de reserva
            case 3: {
                Peca usada;
                if (jogar_reserva(&pilha, &usada)) {
                    printf("\nVocê jogou a peça [%c %d]\n", usada.tipo, usada.id);
                } else {
                    printf("Não há peça na reserva.\n");
                }
                exibirFila(&fila);
                exibir_Pilha(&pilha);
                break;
            }
            //trocar a primeira peça da fila com o topo da pilha
            case 4: {
                trocar_uma_peca(&pilha, &fila);
                exibirFila(&fila);
                exibir_Pilha(&pilha);
                break;
            }

            //trocar os 3 primeiros da fila com as 3 peças do topo da pilha
            case 5: {
                trocarTresPecas(&pilha, &fila);
                exibirFila(&fila);
                exibir_Pilha(&pilha);
                break;
            }
            // Sair do programa
            case 6:
                printf("Saindo...\n");
                break;

            // Opção inválida
            default:
                printf("Opção inválida!\n");
        }

    } while(opcao != 6);

    return 0;
}
