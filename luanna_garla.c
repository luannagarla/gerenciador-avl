/******************************************************************************/
/*              Programa de gerenciamento de árvores AVL                      */
/*                  Aluna: Luanna Garla | TURMA 2000.                         */
/******************************************************************************/

#include <stdio.h>
#include <malloc.h>
#define true 1
#define false 0

typedef int bool;
typedef int TIPOCHAVE;

typedef struct aux
{
    TIPOCHAVE chave;
    struct aux *esq;
    struct aux *dir;
    int bal;
} NO, *PONT;

#pragma region Já existiam

/* cria um novo (aloca memoria e preenche valores) no com chave=ch e retorna
       seu endereco */
PONT criarNovoNo(TIPOCHAVE ch)
{
    PONT novoNo = (PONT)malloc(sizeof(NO));
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->chave = ch;
    novoNo->bal = 0;
    return novoNo;
}

// Retorna o maior valor entre dois inteiros
int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

// Retorna a altura de uma (sub-)arvore
int altura(PONT p)
{
    if (!p)
        return -1;
    else
        return 1 + max(altura(p->esq), altura(p->dir));
}

/* Exibe arvore Em Ordem         */
void exibirArvoreEmOrdem(PONT raiz)
{
    if (raiz == NULL)
        return;
    exibirArvoreEmOrdem(raiz->esq);
    printf("%i ", raiz->chave);
    exibirArvoreEmOrdem(raiz->dir);
}

/* Exibe arvore Pre Ordem         */
void exibirArvorePreOrdem(PONT raiz)
{
    if (raiz == NULL)
        return;
    printf("%i ", raiz->chave);
    exibirArvorePreOrdem(raiz->esq);
    exibirArvorePreOrdem(raiz->dir);
}

/* Exibe arvore Pos Ordem         */
void exibirArvorePosOrdem(PONT raiz)
{
    if (raiz == NULL)
        return;
    exibirArvorePreOrdem(raiz->esq);
    exibirArvorePreOrdem(raiz->dir);
    printf("%i ", raiz->chave);
}

/* Exibe arvore Em Ordem (com parenteses para os filhos)    */
void exibirArvore(PONT raiz)
{
    if (raiz == NULL)
        return;
    printf("%i[%i]", raiz->chave, raiz->bal);
    printf("(");
    exibirArvore(raiz->esq);
    exibirArvore(raiz->dir);
    printf(")");
}

/* Rotações à direita (LL e LR)
   Retornará o endereço do nó que será a nova raiz da subárvore originalmente
   iniciada por p */
PONT rotacaoL(PONT p)
{
    printf("Rotacao a esquerda, problema no no: %i\n", p->chave);
    PONT u, v;
    u = p->esq;
    if (u->bal == -1)
    { // LL
        p->esq = u->dir;
        u->dir = p;
        p->bal = 0;
        u->bal = 0;
        return u;
    }
    else if (u->bal == 1)
    { // LR
        v = u->dir;
        u->dir = v->esq;
        v->esq = u;
        p->esq = v->dir;
        v->dir = p;
        if (v->bal == -1)
            p->bal = 1;
        else
            p->bal = 0;
        if (v->bal == 1)
            u->bal = -1;
        else
            u->bal = 0;
        v->bal = 0;
        return v;
    }
    else
    { // caso necessario apenas para a exclusao (u->bal == 0)
        p->esq = u->dir;
        u->dir = p;
        // p->bal = -1;    desnecessario pois o balanceamento de p nao chegou a ser mudado para -2
        u->bal = 1;
        return u;
    }
}

/* Rotações à esquerda (RR e RL)
   Retornará o endereço do nó que será a nova raiz da subárvore originalmente
   iniciada por p */
PONT rotacaoR(PONT p)
{
    printf("Rotacao a direita, problema no no: %i\n", p->chave);
    PONT u, v;
    u = p->dir;
    if (u->bal == 1)
    { // RR
        p->dir = u->esq;
        u->esq = p;
        p->bal = 0;
        u->bal = 0;
        return u;
    }
    else if (u->bal == -1)
    { // RL
        v = u->esq;
        u->esq = v->dir;
        v->dir = u;
        p->dir = v->esq;
        v->esq = p;
        if (v->bal == 1)
            p->bal = -1;
        else
            p->bal = 0;
        if (v->bal == -1)
            u->bal = 1;
        else
            u->bal = 0;
        v->bal = 0;
        return v;
    }
    else
    { // caso necessario apenas para a exclusao (u->bal == 0)
        p->dir = u->esq;
        u->esq = p;
        u->bal = -1;
        // p->bal = 1;    desnecessario pois o balanceamento de p nao chegou a ser mudado para 2
        return u;
    }
}

/*
   Inserção AVL:
   - O parâmetro pp é um ponteiro para o nó raiz (ou para o ponteiro do nó atual).
   - O parâmetro ch é a chave a ser inserida.
   - O parâmetro alterou indica se a altura (ou fator de balanceamento) da subárvore foi alterada
     pela inserção (inicialmente, deve ser false).
*/
void inserirAVL(PONT *pp, TIPOCHAVE ch, bool *alterou)
{
    // p aponta para o nó atual da subárvore
    PONT p = *pp;

    // Se o nó atual é NULL, então chegamos à posição de inserção
    if (!p)
    {
        // Cria um novo nó com a chave ch e o coloca na posição de *pp
        *pp = criarNovoNo(ch);
        // Indica que houve alteração (a altura aumentou)
        *alterou = true;
    }
    else
    {
        // Se a chave já existe no nó atual, não há alteração (não permite duplicatas)
        if (ch == p->chave)
            *alterou = false;
        // Se a chave a inserir é menor ou igual à chave do nó atual, vamos para a subárvore esquerda
        else if (ch <= p->chave)
        {
            // Chamada recursiva para inserir na subárvore esquerda
            inserirAVL(&(p->esq), ch, alterou);
            // Se a subárvore esquerda foi alterada, precisamos atualizar o fator de balanceamento
            if (*alterou)
                switch (p->bal)
                {
                // Caso 1: p->bal era 1 (a subárvore direita era mais alta)
                // Agora, com a inserção na esquerda, os dois lados ficam equilibrados
                case 1:
                    p->bal = 0;       // O nó fica balanceado
                    *alterou = false; // Não há mais alteração para propagar
                    break;
                // Caso 2: p->bal era 0 (a árvore estava balanceada)
                // A inserção na esquerda faz com que o lado esquerdo fique mais alto
                case 0:
                    p->bal = -1; // Aumenta a altura do lado esquerdo
                    break;
                // Caso 3: p->bal era -1 (a subárvore esquerda já era mais alta)
                // Com a nova inserção, o fator de balanceamento fica em -2 e é necessário rebalancear
                case -1:
                    *pp = rotacaoL(p); // Realiza a rotação à esquerda para rebalancear
                    *alterou = false;  // O rebalanceamento corrige a altura, não há mais alteração a propagar
                    break;
                }
        }
        else
        { // Caso a chave seja maior que a chave do nó atual: inserir na subárvore direita
            // Chamada recursiva para inserir na subárvore direita
            inserirAVL(&(p->dir), ch, alterou);
            // Se a subárvore direita foi alterada, atualizar o fator de balanceamento
            if (*alterou)
                switch (p->bal)
                {
                // Caso 1: p->bal era -1 (a subárvore esquerda era mais alta)
                // Agora, com a inserção na direita, ambos os lados ficam equilibrados
                case -1:
                    p->bal = 0;       // O nó fica balanceado
                    *alterou = false; // Não há mais alteração para propagar
                    break;
                // Caso 2: p->bal era 0 (a árvore estava balanceada)
                // A inserção na direita faz com que o lado direito fique mais alto
                case 0:
                    p->bal = 1; // Aumenta a altura do lado direito
                    break;
                // Caso 3: p->bal era 1 (a subárvore direita já era mais alta)
                // Com a nova inserção, o fator de balanceamento fica em +2 e é necessário rebalancear
                case 1:
                    *pp = rotacaoR(p); // Realiza a rotação à direita para rebalancear
                    *alterou = false;  // O rebalanceamento corrige a altura, não há mais alteração a propagar
                    break;
                }
        }
    }
}

/* retorna o endereco do NO que contem chave=ch ou NULL caso a chave nao seja
       encontrada. Utiliza busca binaria recursiva                                */
PONT buscaBinaria(TIPOCHAVE ch, PONT raiz)
{
    if (raiz == NULL)
        return NULL;
    if (raiz->chave == ch)
        return raiz;
    if (raiz->chave < ch)
        return buscaBinaria(ch, raiz->dir);
    return buscaBinaria(ch, raiz->esq);
}

// Busca binária não recursiva devolvendo o nó pai
PONT buscaNo(PONT raiz, TIPOCHAVE ch, PONT *pai)
{
    PONT atual = raiz;
    *pai = NULL;
    while (atual)
    {
        if (atual->chave == ch)
            return (atual);
        *pai = atual;
        if (ch < atual->chave)
            atual = atual->esq;
        else
            atual = atual->dir;
    }
    return (NULL);
}

/* Auxilir da funcao excluirChave, procura a maior chave menor que a chave que
   serah excluida            */
PONT maiorAEsquerda(PONT p, PONT *ant)
{
    *ant = p;
    p = p->esq;
    while (p->dir)
    {
        *ant = p;
        p = p->dir;
    }
    return (p);
}

/*
   exclui a chave com valor igual a ch
   Parâmetros:
     - raiz: ponteiro para o ponteiro do nó raiz (ou nó corrente) da subárvore.
     - ch: a chave a ser excluída.
     - alterou: variável booleana que indica se a altura da subárvore foi alterada após a exclusão.
   Retorna:
     - true se a exclusão foi realizada; false caso contrário.
*/
bool excluirAVL(PONT *raiz, TIPOCHAVE ch, bool *alterou)
{
    // Pega o nó atual apontado por *raiz
    PONT atual = *raiz;

    // Se o nó atual for NULL, a chave não foi encontrada; não há alteração
    if (!atual)
    {
        *alterou = false;
        return false;
    }

    // Se a chave do nó atual é a que desejamos excluir...
    if (atual->chave == ch)
    {
        PONT substituto, pai_substituto;
        // Se o nó atual tem zero ou um filho
        if (!atual->esq || !atual->dir)
        {
            // Escolhe o filho existente (ou NULL, se nenhum existir)
            if (atual->esq)
                substituto = atual->esq;
            else
                substituto = atual->dir;
            // Atualiza o ponteiro para o nó atual, removendo-o da árvore
            *raiz = substituto;
            // Libera a memória do nó excluído
            free(atual);
            // Indica que a altura da subárvore foi alterada
            *alterou = true;
            return true;
        }
        else
        {
            // Se o nó tem dois filhos, encontra o maior nó da subárvore esquerda
            substituto = maiorAEsquerda(atual, &pai_substituto);
            // Copia a chave do substituto para o nó atual
            atual->chave = substituto->chave;
            // Atualiza ch para continuar a exclusão do nó que continha a chave duplicada
            ch = substituto->chave; // continua o código excluindo o substituto
        }
    }

    bool res;
    // Se a chave a ser excluída é maior que a chave do nó atual,
    // a exclusão deve ocorrer na subárvore direita
    if (ch > atual->chave)
    {
        res = excluirAVL(&(atual->dir), ch, alterou);
        // Imprime mensagem para depuração (opcional) informando a exclusão na direita
        printf("Excluir recursivo a direita: %i(%i)\n", atual->chave, atual->bal);
        // Se houve alteração na altura da subárvore direita, atualiza o fator de balanceamento do nó atual
        if (*alterou)
        {
            switch (atual->bal)
            {
            // Caso 1: se o fator era 1 (direita mais alta), a exclusão equilibrou o nó
            case 1:
                atual->bal = 0;
                return true;
            // Caso 2: se o fator era 0, a exclusão faz com que o lado esquerdo fique mais alto (-1)
            case 0:
                atual->bal = -1;
                *alterou = false; // a altura do nó atual não diminuiu, não há propagação da alteração
                return true;
            // Caso 3: se o fator era -1, a exclusão gera desbalanceamento (-2) e é necessária uma rotação à esquerda
            case -1:
                *raiz = rotacaoL(atual);
                // Se, após a rotação, o fator de balanceamento não zerar, a alteração não se propaga
                if (atual->bal != 0)
                    *alterou = false;
                return true;
            }
        }
    }
    else
    {
        // Se a chave a ser excluída é menor ou igual à chave do nó atual,
        // a exclusão deve ocorrer na subárvore esquerda
        res = excluirAVL(&(atual->esq), ch, alterou);
        // Imprime mensagem para depuração (opcional) informando a exclusão na esquerda
        printf("Excluir recursivo a esquerda: %i(%i)\n", atual->chave, atual->bal);
        // Se houve alteração na altura da subárvore esquerda, atualiza o fator de balanceamento
        if (*alterou)
        {
            switch (atual->bal)
            {
            // Caso 1: se o fator era -1 (esquerda mais alta), a exclusão equilibrou o nó
            case -1:
                atual->bal = 0;
                return true;
            // Caso 2: se o fator era 0, a exclusão faz com que o lado direito fique mais alto (1)
            case 0:
                atual->bal = 1;
                *alterou = false; // a altura não diminuiu, não há propagação da alteração
                return true;
            // Caso 3: se o fator era 1, a exclusão gera desbalanceamento (fator 2) e é necessária uma rotação à direita
            case 1:
                *raiz = rotacaoR(atual);
                if (atual->bal != 0)
                    *alterou = false;
                return true;
            }
        }
    }
    // Retorna o resultado da operação (a variável res contém o valor retornado pela chamada recursiva)
    return res;
}

/* funcao auxiliar na destruicao (liberacao da memoria) de uma arvore */
void destruirAux(PONT subRaiz)
{
    if (subRaiz)
    {
        destruirAux(subRaiz->esq);
        destruirAux(subRaiz->dir);
        free(subRaiz);
    }
}

/* libera toda memoria de uma arvore e coloca NULL no valor da raiz    */
void destruirArvore(PONT *raiz)
{
    destruirAux(*raiz);
    *raiz = NULL;
}

/* inicializa arvore: raiz=NULL */
void inicializar(PONT *raiz)
{
    *raiz = NULL;
}

#pragma endregion Já existiam

// Verifica se é AVL
bool verificaAVL(PONT p)
{
    int e, d;
    bool isAVL = true;
    if (p)
    {
        isAVL = verificaAVL(p->esq); // verifica balanceamento à esquerda
        if (isAVL)
            isAVL = verificaAVL(p->dir); // verifica balanceamento à direita
        if (isAVL)
        {
            e = altura(p->esq);
            d = altura(p->dir);
            if (e - d > 1 || e - d < -1)
                isAVL = false;
            else
                isAVL = true;
        }
    }

    return (isAVL);
}

// Contagem dos nós
int contagemNos(PONT p)
{
    if (p == NULL) // árvore vazia
        return 0;
    return 1 + contagemNos(p->esq) + contagemNos(p->dir); // primeiro conta todos pela esquerda, depois pela direita, soma recursiva
}

// Exibição por nível
void exibirArvorePorNivel(PONT raiz)
{
    if (raiz == NULL)
        return;

    PONT fila[100]; // fila auxiliar
    int nivel[100];
    int inicio = 0;
    int fim = 0;

    fila[fim] = raiz;
    nivel[fim++] = 0;

    int nivelAtual = 0;
    printf("\nNível %d: ", nivelAtual);

    while (inicio < fim)
    {
        PONT atual = fila[inicio];
        int nivelNo = nivel[inicio++];

        if (nivelNo != nivelAtual)
        {
            nivelAtual = nivelNo; // altera o nível após descer
            printf("\nNível %d: ", nivelAtual);
        }

        printf("%d ", atual->chave);

        if (atual->esq != NULL)
        {
            fila[fim] = atual->esq;
            nivel[fim++] = nivelNo + 1;
        }

        if (atual->dir != NULL)
        {
            fila[fim] = atual->dir;
            nivel[fim++] = nivelNo + 1;
        }
    }

    printf("\n");
}

// Exibição de um nível, dado um nó
int getNivelPorNo(PONT raiz, int chave)
{
    // utilizei a mesma estrutura do anterior
    // mas ao invés de printar, quando chega no valor desejado, retorna nível

    if (raiz == NULL)
        return -1;

    PONT fila[100];
    int nivel[100];
    int inicio = 0;
    int fim = 0;

    fila[fim] = raiz;
    nivel[fim++] = 0;

    int nivelAtual = 0;

    while (inicio < fim)
    {
        PONT atual = fila[inicio];
        int nivelNo = nivel[inicio++];

        if (nivelNo != nivelAtual)
        {
            nivelAtual = nivelNo;
        }

        if (atual->chave == chave)
            return nivelAtual;

        if (atual->esq != NULL)
        {
            fila[fim] = atual->esq;
            nivel[fim++] = nivelNo + 1;
        }

        if (atual->dir != NULL)
        {
            fila[fim] = atual->dir;
            nivel[fim++] = nivelNo + 1;
        }
    }

    return -1;
}

// Exibição GRÁFICA
void exibirArvoreGrafica(PONT raiz)
{
    if (raiz == NULL)
        return;

    PONT fila[100];
    int nivel[100];
    int inicio = 0;
    int fim = 0;

    fila[fim] = raiz;
    nivel[fim++] = 0;

    int nivelAtual = 0;
    int espacos = 20;

    while (inicio < fim)
    {
        int elementosNesteNivel = 0;
        int i = inicio;
        while (i < fim && nivel[i] == nivelAtual)
        {
            elementosNesteNivel++;
            i++;
        }

        for (int i = 0; i < espacos; i++)
            printf(" ");

        for (int j = 0; j < elementosNesteNivel; j++)
        {
            PONT atual = fila[inicio];
            int nivelNo = nivel[inicio++];

            printf("%d", atual->chave);

            for (int i = 0; i < espacos * 2 - 2; i++)
                printf(" ");

            if (atual->esq != NULL)
            {
                fila[fim] = atual->esq;
                nivel[fim++] = nivelNo + 1;
            }

            if (atual->dir != NULL)
            {
                fila[fim] = atual->dir;
                nivel[fim++] = nivelNo + 1;
            }
        }

        printf("\n");
        espacos /= 2;
        nivelAtual++;
    }

    printf("\n");
}

// Contagem das folhas
int contarFolhas(PONT raiz)
{
    if (raiz == NULL)
        return 0;

    if (raiz->esq == NULL && raiz->dir == NULL)
        return 1;

    return contarFolhas(raiz->esq) + contarFolhas(raiz->dir);
}

// Soma de valores
int somaValores(PONT raiz)
{
    if (raiz == NULL)
        return 0;

    return raiz->chave + somaValores(raiz->esq) + somaValores(raiz->dir);
}

// Achar o menor e maior valor
int encontrarMinimo(PONT raiz)
{
    if (raiz == NULL)
        return -1;

    while (raiz->esq != NULL)
        raiz = raiz->esq;

    return raiz->chave;
}

int encontrarMaximo(PONT raiz)
{
    if (raiz == NULL)
        return -1;

    while (raiz->dir != NULL)
        raiz = raiz->dir;

    return raiz->chave;
}

// Listar em um intervalo [a,b]
void listarIntervalo(PONT raiz, int a, int b)
{
    if (raiz == NULL)
        return;

    // Se a chave pode estar na subárvore esquerda
    if (raiz->chave > a)
        listarIntervalo(raiz->esq, a, b);

    // Se a chave está dentro do intervalo, imprime
    if (raiz->chave >= a && raiz->chave <= b)
        printf("%d ", raiz->chave);

    // Se a chave pode estar na subárvore direita
    if (raiz->chave < b)
        listarIntervalo(raiz->dir, a, b);
}

// Buscar k-ésimo menor
int buscarKesimoMenor(PONT raiz, int contador, int k)
{
    if (raiz == NULL)
        return -1;

    // Percorre à esquerda
    int resultado = buscarKesimoMenor(raiz->esq, contador, k);
    if (resultado != -1)
        return resultado;

    // Visita o nó atual
    contador++;
    if (contador == k)
        return raiz->chave;

    // Percorre à direita
    return buscarKesimoMenor(raiz->dir, contador, k);
}

// Verifica se dois valores estão no mesmo nível
int mesmoNivel(PONT raiz, int x, int y)
{
    // utilizei a mesma estrutura do getNivelPorNO
    // registrando os valores do níveis afim de comparar

    if (raiz == NULL)
        return -1;

    PONT fila[100];
    int nivel[100];
    int inicio = 0;
    int fim = 0;

    fila[fim] = raiz;
    nivel[fim++] = 0;

    int nivelAtual = 0, nivelX = 0, nivelY = 0;

    while (inicio < fim)
    {
        PONT atual = fila[inicio];
        int nivelNo = nivel[inicio++];

        if (nivelNo != nivelAtual)
            nivelAtual = nivelNo;

        if (atual->chave == x)
            nivelX = nivelAtual;

        if (atual->chave == y)
            nivelY = nivelAtual;

        if (nivelY != 0 && nivelX != 0)
            return nivelY == nivelX ? nivelAtual : -1;

        if (atual->esq != NULL)
        {
            fila[fim] = atual->esq;
            nivel[fim++] = nivelNo + 1;
        }

        if (atual->dir != NULL)
        {
            fila[fim] = atual->dir;
            nivel[fim++] = nivelNo + 1;
        }
    }

    return nivelY == nivelX ? nivelAtual : -1;
}

void FuncoesObrigatorias(PONT *raiz)
{
    printf("=============== INÍCIO DOS TESTES ===============\n");
    bool alterou;

    // Inserção
    printf("\nInserindo os valores 29, 56, 21, 8, 3, 19, 7, 1, 10, 13, 45:\n\n");
    inserirAVL(raiz, 29, &alterou);
    inserirAVL(raiz, 56, &alterou);
    inserirAVL(raiz, 21, &alterou);
    inserirAVL(raiz, 8, &alterou);
    inserirAVL(raiz, 3, &alterou);
    inserirAVL(raiz, 19, &alterou);
    inserirAVL(raiz, 7, &alterou);
    inserirAVL(raiz, 1, &alterou);
    inserirAVL(raiz, 70, &alterou);
    inserirAVL(raiz, 13, &alterou);
    inserirAVL(raiz, 45, &alterou);

    // Exclusão
    printf("\nExlcuindo os valores 21, 56:\n\n");
    excluirAVL(raiz, 21, &alterou);
    excluirAVL(raiz, 56, &alterou);

    printf("\n=============== funções obrigatórias  ===============\n");

    // Busca
    PONT pai;
    PONT resultado = buscaNo(*raiz, 29, &pai);

    printf("\nTeste de busca:");
    if (resultado)
    {
        if (pai)
            printf("\nNó %d encontrado (pai: %d).\n", resultado->chave, pai->chave);
        else
            printf("\nNó %d encontrado (sem pai).\n", resultado->chave);
    }
    else
    {
        printf("\nNó 29 não encontrado.\n");
    }

    // Exibir em ordem
    printf("\nÁrvore em ordem: ");
    exibirArvoreEmOrdem(*raiz);

    // Exibir normal
    printf("\nÁrvore (filhos): ");
    exibirArvore(*raiz);

    // Exibir em nível
    printf("\n\nÁrvore por nível: ");
    exibirArvorePorNivel(*raiz);

    // Impressão gráfica
    printf("\nImpressão gráfica: \n");
    exibirArvoreGrafica(*raiz);
    printf("\n");

    // Cálculo da altura
    int alt = altura(*raiz);
    printf("\nAltura da árvore igual a %d.\n", alt);

    // Contagem dos nós
    int qtdNos = contagemNos(*raiz);
    printf("\nNúmeros de nós igual a %d.\n", qtdNos);

    // Verificação AVL
    if (verificaAVL(*raiz))
        printf("A árvore é uma AVL.\n");
    else
        printf("A árvore não é uma AVL.!\n");

    // Impressão do nível de um nó
    int valor = 29;
    int nivel = getNivelPorNo(*raiz, valor);
    if (nivel != -1)
        printf("Nível do nó %d é igual a %d.", valor, nivel);
    else
        printf("Nó %d não encontrado.", valor);
}

void FuncoesAdicionais(PONT *raiz)
{
    printf("\n\n\n=============== funções adicionais  ===============\n");

    // listagem de elementos no intervalo
    printf("\nListar em um intervalo (10-29):\n");
    listarIntervalo(*raiz, 10, 29);
    printf("\n");

    // contar folhas
    int qtdF = contarFolhas(*raiz);
    printf("\n- O número de folhas é igual a %d.", qtdF);

    // encontrar menor e maior valor
    int menor = encontrarMinimo(*raiz);
    printf("\n- O menor valor é igual a %d.", menor);

    int maior = encontrarMaximo(*raiz);
    printf("\n- O maior valor é igual a %d.", maior);

    // buscar K-esimo menor valor
    int k = 2; //segundo menor
    int kesimo = buscarKesimoMenor(*raiz, 0, k);
    if (kesimo != -1)
        printf("\n- O %dº menor valor da árvore é: %d", k, kesimo);
    else
        printf("\n- Não foi possível encontrar o %dº menor valor.", k);

    // verifica se dois valores estão no mesmo nível
    int valor1 = 29;
    int valor2 = 13;
    int nivelEncontrado = mesmoNivel(*raiz, valor1, valor2);

    if (nivelEncontrado == -1)
        printf("\n- O valor %d e %d NÃO estão no mesmo nível.", valor1, valor2);
    else
        printf("\n- O valor %d e %d estão no mesmo nível (Nível %d).", valor1, valor2, nivelEncontrado);

    // soma valores da árvore
    int soma = somaValores(*raiz);
    printf("\n- A soma dos valores é igual a %d.", soma);
}

int main()
{
    PONT raiz;
    inicializar(&raiz);
    FuncoesObrigatorias(&raiz);
    FuncoesAdicionais(&raiz);

    return 0;
}