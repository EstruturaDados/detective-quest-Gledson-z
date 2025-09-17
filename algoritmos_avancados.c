#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_PISTA 100
#define HASH_SIZE 101

// Estrutura para representar uma sala da mansão
typedef struct Sala
{
    char nome[MAX_NOME];
    char pista[MAX_PISTA]; // pista associada à sala (string vazia se não houver)
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

// Estrutura para tabela hash: cada entrada armazena pista e suspeito associado
typedef struct HashNode
{
    char pista[MAX_PISTA];
    char suspeito[MAX_NOME];
    struct HashNode *proximo;
} HashNode;

// Tabela hash global
HashNode *tabelaHash[HASH_SIZE] = {NULL};

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

/**
 * hashFunction() – função hash simples para strings
 * @param str: string para calcular hash
 * @return índice da tabela hash
 */
unsigned int hashFunction(const char *str)
{
    unsigned int hash = 0;
    while (*str)
    {
        hash = (hash * 31) + (unsigned char)(*str);
        str++;
    }
    return hash % HASH_SIZE;
}

/**
 * inserirNaHash() – insere associação pista/suspeito na tabela hash.
 * @param pista: chave da tabela hash
 * @param suspeito: valor associado
 */
void inserirNaHash(const char *pista, const char *suspeito)
{
    unsigned int idx = hashFunction(pista);
    HashNode *novoNo = (HashNode *)malloc(sizeof(HashNode));
    if (novoNo == NULL)
    {
        printf("Erro de alocacao de memoria.\n");
        exit(1);
    }
    strncpy(novoNo->pista, pista, MAX_PISTA - 1);
    novoNo->pista[MAX_PISTA - 1] = '\0';
    strncpy(novoNo->suspeito, suspeito, MAX_NOME - 1);
    novoNo->suspeito[MAX_NOME - 1] = '\0';
    novoNo->proximo = tabelaHash[idx];
    tabelaHash[idx] = novoNo;
}

/**
 * encontrarSuspeito() – consulta o suspeito correspondente a uma pista.
 * @param pista: chave para busca
 * @return nome do suspeito ou NULL se não encontrado
 */
const char *encontrarSuspeito(const char *pista)
{
    unsigned int idx = hashFunction(pista);
    HashNode *atual = tabelaHash[idx];
    while (atual != NULL)
    {
        if (strcmp(atual->pista, pista) == 0)
        {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

/**
 * liberarHash() – libera memória da tabela hash
 */
void liberarHash()
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        HashNode *atual = tabelaHash[i];
        while (atual != NULL)
        {
            HashNode *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

/**
 * contarPistasSuspeito() – conta quantas pistas coletadas apontam para um suspeito.
 * @param raiz: raiz da árvore de pistas coletadas
 * @param suspeito: nome do suspeito a contar
 * @return número de pistas que apontam para o suspeito
 */
int contarPistasSuspeito(PistaNode *raiz, const char *suspeito)
{
    if (raiz == NULL)
        return 0;

    int count = 0;
    const char *suspeitoDaPista = encontrarSuspeito(raiz->pista);
    if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, suspeito) == 0)
    {
        count = 1;
    }

    return count + contarPistasSuspeito(raiz->esquerda, suspeito) + contarPistasSuspeito(raiz->direita, suspeito);
}

/**
 * explorarSalas() – navega pela árvore e ativa o sistema de pistas.
 * @param atual: sala atual
 * @param pistas: ponteiro para a raiz da árvore de pistas coletadas
 */
void explorarSalas(Sala *atual, PistaNode **pistas)
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
        explorarSalas(atual->esquerda, pistas);
    }
    else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL)
    {
        explorarSalas(atual->direita, pistas);
    }
    else
    {
        printf("Opcao invalida. Tente novamente.\n");
        explorarSalas(atual, pistas);
    }
}

/**
 * verificarSuspeitoFinal() – conduz à fase de julgamento final.
 * @param pistas: árvore de pistas coletadas
 */
void verificarSuspeitoFinal(PistaNode *pistas)
{

    printf("\nSuspeitos disponiveis:\n");
    printf("Sr. Verde\n");
    printf("Sra. Rosa\n");
    printf("Coronel Mostarda\n");
    printf("Professor Plum\n");
    printf("Sra. Rosa\n");
    printf("Sr. Verde\n");
    printf("Coronel Mostarda\n");
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    char suspeitoAcusado[MAX_NOME];
    if (fgets(suspeitoAcusado, sizeof(suspeitoAcusado), stdin) == NULL)
    {
        printf("Erro na leitura da entrada.\n");
        return;
    }
    // Remove newline
    size_t len = strlen(suspeitoAcusado);
    if (len > 0 && suspeitoAcusado[len - 1] == '\n')
        suspeitoAcusado[len - 1] = '\0';

    int numPistas = contarPistasSuspeito(pistas, suspeitoAcusado);

    printf("\nAcusacao: %s\n", suspeitoAcusado);
    printf("Numero de pistas que apontam para este suspeito: %d\n", numPistas);

    if (numPistas >= 2)
    {
        printf("Parabens! Ha evidencias suficientes para sustentar a acusacao.\n");
        printf("Investigacao encerrada com sucesso.\n");
    }
    else
    {
        printf("Nao ha evidencias suficientes para sustentar a acusacao.\n");
        printf("Investigacao encerrada sem conclusao.\n");
    }
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

    // Criando a tabela hash de pistas para suspeitos
    // Associações fixas (pista -> suspeito)
    inserirNaHash("Pegada estranha no tapete", "Sr. Verde");
    inserirNaHash("Vidro quebrado na janela", "Sra. Rosa");
    inserirNaHash("Faca com manchas de sangue", "Coronel Mostarda");
    inserirNaHash("Livro fora do lugar", "Professor Plum");
    inserirNaHash("Pegadas na terra molhada", "Sra. Rosa");
    inserirNaHash("Carta rasgada", "Sr. Verde");
    inserirNaHash("Relogio parado", "Coronel Mostarda");

    printf("Bem vindo ao Detective Quest!\nExplore a mansao para encontrar pistas.\n");

    PistaNode *pistasColetadas = NULL;

    // Inicia exploração a partir do hall de entrada
    explorarSalas(hallEntrada, &pistasColetadas);

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

    // Fase de acusação final
    verificarSuspeitoFinal(pistasColetadas);

    // Libera memória
    liberarSalas(hallEntrada);
    liberarPistas(pistasColetadas);
    liberarHash();

    printf("Obrigado por jogar!\n");
    return 0;
}