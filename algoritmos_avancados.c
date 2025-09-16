#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_PISTA 100

// Estrutura para representar uma sala da mansão
typedef struct Sala
{
    char nome[MAX_NOME];
    char pista[MAX_PISTA]; // pista opcional associada à sala
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para representar um nó da árvore BST de pistas
typedef struct PistaNode
{
    char pista[MAX_PISTA];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/**
 * criarSala() – cria dinamicamente um cômodo com ou sem pista.
 * @param nome: nome da sala
 * @param pista: pista associada (string vazia se não houver)
 * @return ponteiro para a sala criada
 */
Sala *criarSala(const char *nome, const char *pista)
{
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL)
    {
        printf("Erro de alocacao de memoria.\n");
        exit(1);
    }
    strncpy(novaSala->nome, nome, MAX_NOME - 1);
    novaSala->nome[MAX_NOME - 1] = '\0';

    if (pista != NULL)
    {
        strncpy(novaSala->pista, pista, MAX_PISTA - 1);
        novaSala->pista[MAX_PISTA - 1] = '\0';
    }
    else
    {
        novaSala->pista[0] = '\0'; // sem pista
    }

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * inserirPista() – insere uma nova pista na árvore de pistas (BST).
 * @param raiz: raiz da árvore de pistas
 * @param pista: pista a ser inserida
 * @return nova raiz da árvore após inserção
 */
PistaNode *inserirPista(PistaNode *raiz, const char *pista)
{
    if (raiz == NULL)
    {
        PistaNode *novoNo = (PistaNode *)malloc(sizeof(PistaNode));
        if (novoNo == NULL)
        {
            printf("Erro de alocacao de memoria.\n");
            exit(1);
        }
        strncpy(novoNo->pista, pista, MAX_PISTA - 1);
        novoNo->pista[MAX_PISTA - 1] = '\0';
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        return novoNo;
    }

    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0)
    {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    }
    else if (cmp > 0)
    {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se a pista já existe, não insere duplicata
    return raiz;
}

/**
 * exibirPistas() – imprime a árvore de pistas em ordem alfabética (in-order traversal).
 * @param raiz: raiz da árvore de pistas
 */
void exibirPistas(PistaNode *raiz)
{
    if (raiz == NULL)
        return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/**
 * explorarSalasComPistas() – controla a navegação entre salas e coleta de pistas.
 * @param atual: sala atual
 * @param pistas: ponteiro para a raiz da árvore de pistas coletadas
 */
void explorarSalasComPistas(Sala *atual, PistaNode **pistas)
{
    if (atual == NULL)
    {
        return;
    }

    printf("\nVoce esta na sala: %s\n", atual->nome);

    // Se a sala possui pista, coleta automaticamente
    if (strlen(atual->pista) > 0)
    {
        printf("Voce encontrou uma pista: %s\n", atual->pista);
        *pistas = inserirPista(*pistas, atual->pista);
    }
    else
    {
        printf("Nenhuma pista encontrada nesta sala.\n");
    }

    // Se não há caminhos para explorar, fim da exploração
    if (atual->esquerda == NULL && atual->direita == NULL)
    {
        printf("Esta sala nao possui mais caminhos. Fim da exploracao.\n");
        return;
    }

    printf("Escolha o caminho para explorar:\n");
    if (atual->esquerda != NULL)
    {
        printf("  (e) Esquerda -> %s\n", atual->esquerda->nome);
    }
    if (atual->direita != NULL)
    {
        printf("  (d) Direita -> %s\n", atual->direita->nome);
    }
    printf("  (s) Sair da exploracao\n");
    printf("Digite sua escolha: ");

    char escolha;
    do
    {
        escolha = getchar();
    } while (escolha == '\n' || escolha == ' ');

    while (getchar() != '\n')
        ;

    if (escolha == 's' || escolha == 'S')
    {
        printf("Exploracao encerrada pelo jogador.\n");
        return;
    }
    else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL)
    {
        explorarSalasComPistas(atual->esquerda, pistas);
    }
    else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL)
    {
        explorarSalasComPistas(atual->direita, pistas);
    }
    else
    {
        printf("Opcao invalida. Tente novamente.\n");
        explorarSalasComPistas(atual, pistas);
    }
}

/**
 * liberarPistas() – libera memória da árvore de pistas
 */
void liberarPistas(PistaNode *raiz)
{
    if (raiz == NULL)
        return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

/**
 * liberarSalas() – libera memória da árvore de salas
 */
void liberarSalas(Sala *raiz)
{
    if (raiz == NULL)
        return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}

int main()
{
    // Criando o mapa da mansão com pistas associadas
    Sala *hallEntrada = criarSala("Hall de entrada", "Pegada estranha no tapete");
    Sala *salaEstar = criarSala("Sala de estar", "Vidro quebrado na janela");
    Sala *cozinha = criarSala("Cozinha", "Faca com manchas de sangue");
    Sala *biblioteca = criarSala("Biblioteca", "Livro fora do lugar");
    Sala *jardim = criarSala("Jardim", "Pegadas na terra molhada");
    Sala *escritorio = criarSala("Escritorio", "Carta rasgada");
    Sala *quarto = criarSala("Quarto", "Relogio parado");

    // Montando a árvore binária do mapa
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = escritorio;
    cozinha->direita = quarto;

    printf("Bem vindo ao Detective Quest!\nExplore a mansao para encontrar pistas.\n");

    PistaNode *pistasColetadas = NULL;

    // Inicia exploração a partir do hall de entrada
    explorarSalasComPistas(hallEntrada, &pistasColetadas);

    // Exibe todas as pistas coletadas em ordem alfabética
    printf("\nPistas coletadas durante a exploracao:\n");
    if (pistasColetadas == NULL)
    {
        printf("Nenhuma pista foi coletada.\n");
    }
    else
    {
        exibirPistas(pistasColetadas);
    }

    // Libera memória
    liberarSalas(hallEntrada);
    liberarPistas(pistasColetadas);

    printf("Obrigado por jogar!\n");
    return 0;
}