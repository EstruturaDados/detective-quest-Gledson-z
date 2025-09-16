#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50

typedef struct Sala
{
    char nome[MAX_NOME];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

Sala *criarSala(const char *nome)
{
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL)
    {
        printf("Erro de alocacao de memoria.\n");
        exit(1);
    }
    strncpy(novaSala->nome, nome, MAX_NOME - 1);
    novaSala->nome[MAX_NOME - 1] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

void explorarSalas(Sala *atual)
{
    if (atual == NULL)
    {
        return;
    }

    printf("\nVoce esta na sala: %s\n", atual->nome);

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
        explorarSalas(atual->esquerda);
    }
    else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL)
    {
        explorarSalas(atual->direita);
    }
    else
    {
        printf("Opcao invalida. Tente novamente.\n");
        explorarSalas(atual);
    }
}

int main()
{
    Sala *hallEntrada = criarSala("Hall de entrada");
    Sala *salaEstar = criarSala("Sala de estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *escritorio = criarSala("Escritorio");
    Sala *quarto = criarSala("Quarto");

    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = escritorio;
    cozinha->direita = quarto;

    printf("Bem vindo ao Detective Quest!\nExplore a mansao para encontrar pistas.\n");
    explorarSalas(hallEntrada);

    free(hallEntrada);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(escritorio);
    free(quarto);

    printf("Obrigado por jogar!\n");
    return 0;
}