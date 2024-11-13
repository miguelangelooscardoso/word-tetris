#include <stdio.h>
#include <stdlib.h> // to use memory allocation (malloc)
#include <string.h>
#include <locale.h> // tentativa falhada de usar acentos
#include <ctype.h>
#include <conio.h> // para usar _kbhit() e _getch() no Windows
#include <time.h> // clock_t data type; 'clock' function

#define LINHAS 12
#define COLUNAS 20

typedef struct
{
    char quadro[LINHAS][COLUNAS]; // Matriz representando o quadro de jogo
    char chave[50];               // Palavra-chave que o jogador deve formar
    char nome[50];                 // Nome do jogador
    int pontos;                   // Pontua��o do jogador
    float tempo;                    // Tempo at� ao programa terminar
    int linhas;                   // N�mero de linhas completadas
} Jogo;

typedef struct
{
    char letra; // A letra que est� caindo
    int posX;   // Posi��o X no quadro
    int posY;   // Posi��o Y no quadro
} Letra;

void menu();
void guardar_registos(Jogo *jogo);
void mostrar_registos();
void iniciar_jogo();

// quadro
void inicializar_quadro(Jogo *jogo);
void atualizar_quadro(Jogo *jogo, Letra *letras, int num_letras);
void imprimir_quadro(Jogo *jogo);

// palavra
void escolher_palavra(Jogo *jogo);
void inicializar_letras(Jogo *jogo, Letra *letras, int num_letras);
void mover_letras(Jogo *jogo, Letra *letras, int *letra_atual, int num_letras);
void ler_palavra(Jogo *jogo, Letra *letras, int *letra_atual, int num_letras);


clock_t begin;
float tempo;

int main()
{
    setlocale(LC_ALL, "Portuguese");
    menu();

    return 0;
}
void iniciar_jogo()
{
    int num_letras = 240;
    int letra_atual = 0;
    int sleep = 1000;
//    int palavra;
    Jogo jogo; // estou a criar uma vari�vel jogo do tipo Jogo (struct)
//    Letra letra;
    Letra *letras = (Letra *)malloc(num_letras * sizeof(Letra)); // cast (Letra*)

//    printf("tamanho do ponteiro: %lu\n", sizeof(letras)); // long unsigned instead of int
//    printf("tamanho da estrutura: %lu\n", sizeof(Letra));
//
//    printf("endere�o do ponteiro: %p\n", &letras);
//    printf("endere�o para onde o ponteiro aponta: %p\n", letras);
//    // printf("valor para o qual o ponteiro aponta: %c\n", *letras); // devo inicializar estrutura primeiro
//    printf("&jogo: %p\n", &jogo); // 0x7ff7b3c62528

    printf("Insira o nome: ");
//    scanf("%49[^\n]", jogo.nome);
    scanf("%s", jogo.nome);
    getchar();

    escolher_palavra(&jogo);
    begin = clock();
    inicializar_letras(&jogo, letras, num_letras);
    inicializar_quadro(&jogo);

//    clock_t begin = clock();
    while (1)
    {
        mover_letras(&jogo, letras, &letra_atual, num_letras);  // passa o endere�o de letra_atual

        atualizar_quadro(&jogo, letras, num_letras);

        imprimir_quadro(&jogo);

        Sleep(sleep); // 1 s
    }
//    clock_t end = clock();
//    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    // posso ir diminuindo o delay
    // printf("\a") // som
    free(letras); // libertar mem�ria alocada
}
void inicializar_quadro(Jogo *jogo)
{

    printf("jogo: %p\n", jogo);   // 0x7ff7b3c62528
    printf("&jogo: %p\n", &jogo); // 0x7ff7b3c624f8

    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            jogo->quadro[i][j] = ' '; // (*jogo).quadro[i][j]
        }
    }

//    for (int i = 0; i < LINHAS; i++)
//    {
//        printf("Linha %d: \n", i);
//        for (int j = 0; j < COLUNAS; j++)
//        {
//            printf("%c",jogo->quadro[i][j]); // (*jogo).quadro[i][j]
//        }
//    }
}
void atualizar_quadro(Jogo *jogo, Letra *letras, int num_letras)
{
    // n�o posso chamar a fun��o inicializar_quadro(&jogo) aqui dentro
    // eu apenas quero atualizar o quadro com base nas posi��es atuais das letras
    // se eu chamar inicializar_quadro(&jogo) eu apago as letras que j� l� est�o
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {

            jogo->quadro[i][j] = ' '; // (*jogo).quadro[i][j]

        }
    }

    for(int i = 0; i < num_letras; i++)
    {
        if (letras[i].posY < LINHAS)
        {
//            if (jogo->quadro[letras[i].posY][letras[i].posX] == ' ') // estava-me a gerar um '2' na primeira linha
//            {
            jogo->quadro[letras[i].posY][letras[i].posX] = letras[i].letra;
//            }
        }
    }
}
void imprimir_quadro(Jogo *jogo)
{
    system("cls"); // provoca aquela interfer�ncia que se v�

    printf("====================================\n");
    printf("         *** Tetris de Letras ***    \n");
    printf("====================================\n");

    // parte de cima do quadro
    printf("\t");
    for (int j = 0; j < COLUNAS; j++)
    {
        printf("_");
    }
    printf("\n"); // importante !!!

    // dentro do quadro deve ser poss�vel inserir um caracter
    for (int i = 0; i < LINHAS; i++)
    {
        printf("\t|"); // coluna � esquerda
        for (int j = 0; j < COLUNAS; j++)
        {
            printf("%c", jogo->quadro[i][j]); // quadro inicializado com ' ', jogo->quadro[i][j] or (*jogo).quadro[i][j]
        }
        printf("|\n"); // coluna � direita
    }

    // parte de baixo do quadro
    printf("\t");
    for (int j = 0; j < COLUNAS; j++)
    {
        printf("_");
    }

    printf("\n");
    printf("====================================\n");
    printf("         *** Scoreboard ***         \n");
    printf("====================================\n");
    printf(" Jogador      : %-25s \n", jogo->nome);
    printf(" Palavra Chave: %-25s \n", jogo->chave);
    printf(" Pontua��o    : %-25d \n", jogo->pontos);
    printf("====================================\n");


}
void escolher_palavra(Jogo *jogo)
{
    int palavra_valida = 0; // palavra inv�lida
    do
    {
        printf("Insira uma palavra com mais de quatro caracteres.\n");
        scanf("%s", jogo->chave);

        if (strlen(jogo->chave) < 4)
        {
            printf("A palavra deve ter menos de 4 caracteres.\n"); // palavra_valida = 0 i.e. inv�lida
        }
        else if (strlen(jogo->chave) > 20)
        {

            printf("A palavra deve ter menos de 20 caracteres.\n"); // palavra_valida = 0 i.e. inv�lida
        }
        else
        {
            palavra_valida = 1; // palavra v�lida at� prova contr�ria i.e. v�lida
            for (int k = 0; k < strlen(jogo->chave); k++)
            {
                if (!isalpha(jogo->chave[k]) || !islower(jogo->chave[k]))
                {
                    printf("A palavra deve ser formada apenas por letras min�sculas.\n");
                    palavra_valida = 0; // palavra inv�lida
                    break;              // sai do loop for
                }
            }
        }
    }
    while (!palavra_valida);
}
void inicializar_letras(Jogo *jogo, Letra *letras, int num_letras)
{
    // a estrutura Letra � passada como um array de letras (Letra *letras)

//    for (int i = 0; i < num_letras; i++)
//    {
    // inicializar apenas uma (a primeira) letra aleat�ria
    letras[0].letra = jogo->chave[rand() % strlen(jogo->chave)]; // letra aleat�ria da palavra chave
    letras[0].posX = rand() % COLUNAS;                                // coluna aleat�ria
    letras[0].posY = 0;                                          // linha index 0
//    }

    printf("Letra inicial: %c, Posi��o: (%d, %d)\n", letras[0].letra, letras[0].posY, letras[0].posX); // posY - linha; posX - coluna
}
void mover_letras(Jogo *jogo, Letra *letras, int *letra_atual, int num_letras)
{

    if (letras[*letra_atual].posY < LINHAS) // apaga a letra da posi��o anterior no quadro
    {
        jogo->quadro[letras[*letra_atual].posY][letras[*letra_atual].posX] = ' '; // apagar rasto deixado pela letra
    }

    if (letras[*letra_atual].posY < LINHAS - 1) // verifica se a letra pode cair ou se j� est� na �ltima linha
    {
        if (_kbhit()) // verifica se tecla foi pressionada
        {
            int input = _getch();  // l� caracter da tecla pressionada

            if (input == '4' && letras[*letra_atual].posX > 0)
            {
                if (jogo->quadro[letras[*letra_atual].posY][letras[*letra_atual].posX - 1] == ' ') // verifica se o espa�o � esquerda est� livre
                {
                    letras[*letra_atual].posX--; // move a letra para a esquerda
                }
                else
                {
                    while (letras[*letra_atual].posY < LINHAS - 1 && jogo->quadro[letras[*letra_atual].posY + 1][letras[*letra_atual].posX] == ' ') // caso n�o possa mover lateralmente cai para o fundo
                    {
                        letras[*letra_atual].posY++;  // move para baixo at� o final da coluna (�ltima linha poss�vel)
                    }
                }
            }

            else if (input == '6' && letras[*letra_atual].posX < COLUNAS - 1)
            {

                if (jogo->quadro[letras[*letra_atual].posY][letras[*letra_atual].posX + 1] == ' ') // verifica se existe expa�o livre � direita
                {
                    letras[*letra_atual].posX++; // move a letra para a direita
                }
                else
                {
                    while (letras[*letra_atual].posY < LINHAS - 1 && jogo->quadro[letras[*letra_atual].posY + 1][letras[*letra_atual].posX] == ' ') // caso n�o se possa mover lateralmente cai at� ao fundo
                    {
                        letras[*letra_atual].posY++;  // move para baixo at� o final da coluna (�ltima linha poss�vel)
                    }
                }
            }

            else if (input == ' ') // solta a letra
            {

                while (letras[*letra_atual].posY < LINHAS - 1 && jogo->quadro[letras[*letra_atual].posY + 1][letras[*letra_atual].posX] == ' ') // desloca a letra para o fundo desde que haja espa�o
                {
                    letras[*letra_atual].posY++;  // a letra vai para a linha abaixo desde que esta esteja dispon�vel
                }
            }
            else if (input == 'T')
            {
                printf("Voc� terminou o jogo!\n");

                jogo->tempo = (float)(clock() - begin) / CLOCKS_PER_SEC; // tempo de jogo
                printf("Tempo de jogo: %.2f segundos\n", jogo->tempo);

                guardar_registos(jogo);
//                exit(0);
                menu(); // volta ao menu em vez de sair do programa
            }
        }

        if (jogo->quadro[letras[*letra_atual].posY + 1][letras[*letra_atual].posX] == ' ') // verifica se o espa�o abaixo da letra est� dispon�vel
        {
            letras[*letra_atual].posY++; // move letra para a linha seguinte
        }
        else
        {
            jogo->quadro[letras[*letra_atual].posY][letras[*letra_atual].posX] = letras[*letra_atual].letra; // fixa a letra no fundo


            if (letras[*letra_atual].posY == 1)
            {
                printf("Game Over!\n");

                jogo->tempo = (float)(clock() - begin) / CLOCKS_PER_SEC;
                printf("Tempo total de execu��o: %.2f segundos\n", jogo->tempo);

                guardar_registos(jogo);
                exit(0);  // termina o jogo e sai do programa
            }

            jogo->pontos += 1;
            ler_palavra(jogo, letras, letra_atual, num_letras);

            *letra_atual = (*letra_atual + 1) % num_letras; // atualiza o �ndice da nova letra

            // gera a pr�xima letra e coloca-a no topo, sem sobrescrever as letras existentes.
            do
            {

                letras[*letra_atual].letra = jogo->chave[rand() % strlen(jogo->chave)]; // gera uma letra aleat�ria que faz parte da palavra
                letras[*letra_atual].posX = rand() % COLUNAS; // gera uma posi��o de queda aleat�ria
                letras[*letra_atual].posY = 0;
            }
            while (jogo->quadro[letras[*letra_atual].posY][letras[*letra_atual].posX] != ' '); // verifica se o espa�o abiaxo da letra est� dispon�vel
        }
    }
    else
    {
        jogo->quadro[letras[*letra_atual].posY][letras[*letra_atual].posX] = letras[*letra_atual].letra; // quando a letra atinge a �ltima linha, fixa-a

        if (letras[*letra_atual].posY == 1)
        {
            printf("Game Over!\n");

            jogo->tempo = (float)(clock() - begin) / CLOCKS_PER_SEC;
            printf("Tempo de jogo: %.2f segundos\n", jogo->tempo);

            guardar_registos(jogo);
            exit(0);  // termina o jogo
        }

        jogo->pontos += 1;
        ler_palavra(jogo, letras, letra_atual, num_letras);

        *letra_atual = (*letra_atual + 1) % num_letras;       // atualiza o �ndice da letra seguinte

        // gera a pr�xima letra no topo, sem sobrescrever as letras existentes.
        do
        {
            letras[*letra_atual].letra = jogo->chave[rand() % strlen(jogo->chave)]; // gera uma letra aleat�ria que faz parte da palavra
            letras[*letra_atual].posX = rand() % COLUNAS;             // deixa cair a letra a partir de uma coluna aleat�ria
            letras[*letra_atual].posY = 0;  // verifica se o espa�o abaixo est� livre
        }
        while (jogo->quadro[letras[*letra_atual].posY][letras[*letra_atual].posX] != ' ');
    }

}
void ler_palavra(Jogo *jogo, Letra *letras, int *letra_atual, int num_letras)
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j <= COLUNAS - strlen(jogo->chave); j++)
        {
            if (jogo->quadro[i][j]==jogo->chave[0])
            {
                for(int k = 1; k < strlen(jogo->chave); k++)
                {
                    if( jogo->quadro[i][j + k] != jogo->chave[k]) // se n�o encontrar palavra sai do loop
                    {
                        break;
                    }
                    else if ( k == strlen(jogo->chave) - 1)
                    {
                        // preciso de ir buscar todos os indices das letras daquela linha e desaparecer com elas
                        jogo->pontos += 100;
                        for( int w = 0; w < *letra_atual + 1; w++)
                        {
                            if(letras[w].posY == i)
                            {
                                letras[w].letra = ' ';
                            }
                            else
                            {
                                letras[w].posY++;
                            }
                        }
//                        jogo->pontos += 100;
                        jogo->linhas++;
                    }
                }

            }
        }
    }
}
void menu()
{
    int opcao;
    int ch;

    while(1)
    {
        system("cls");

        printf("====================================\n");
        printf("       *** Menu Principal ***\n");
        printf("====================================\n");
        printf(" 1. Novo Jogo\n");
        printf(" 2. Ver Registos\n"); // sistema de pontos e temporiza��o (nome do jogador opcional)
        printf(" 3. Ajuda\n");
        printf(" 4. Sair\n");
        printf("====================================\n");
        printf(" Escolha uma op��o (1-3): ");

        scanf("%d", &opcao);

        system("cls");
        switch (opcao)
        {
        case 1:
            iniciar_jogo();
            break;
        case 2:
            mostrar_registos();
            break;
        case 3:

            printf("====================================\n");
            printf("       *** Instru��es do jogo ***   \n");
            printf("====================================\n");
            printf(" Pressione '4' para mover � esquerda.\n");
            printf(" Pressione '6' para mover � direita.\n");
            printf(" Pressione 'Espa�o' para soltar.\n");
            printf(" Pressione 'T' para sair.\n");
            printf("====================================\n");
            break;
        case 4:
            printf("4. Sair"); // sistema de pontos e temporiza��o (nome do jogador opcional)
            exit(0);
        default:
            printf("Escolha outra op��o.");
        }
        ch = getchar();
        ch = getchar();
    }
}
void guardar_registos(Jogo *jogo)
{

    FILE *f = fopen("registos.txt", "a");

    if( f == NULL)
    {
        printf("Erro ao abrir o ficheiro");
        return;
    }

    fprintf(f, "%s, %s, %d, %f\n", jogo->nome, jogo->chave, jogo->pontos, jogo->tempo);

    fclose(f);
    printf("Pontua��o guardada com sucesso!");
}
void mostrar_registos()
{

    FILE *f = fopen("registos.txt", "r");

    if( f == NULL)
    {
        printf("Erro ao abrir o ficheiro");
        return;
    }

    Jogo jogo;
    char linha[256];

    fgets(linha, sizeof(linha), f);
    printf("========================================================================\n");
    printf("%-20s %-20s %-10s %-10s\n", "Nome", "Palavra Chave", "Pontua��o", "Tempo (segundos)");
    printf("========================================================================\n");

    while(fgets(linha, sizeof(linha), f) != NULL)
    {

        if (sscanf(linha, "%[^,], %[^,], %d, %f", jogo.nome, jogo.chave, &jogo.pontos, & jogo.tempo) == 4)
        {
            printf("%-20s %-20s %-10d %-10.2f\n", jogo.nome, jogo.chave, jogo.pontos, jogo.tempo);
        }
    }

    fclose(f);
}
