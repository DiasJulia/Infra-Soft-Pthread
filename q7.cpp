#include <stdio.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include <stdlib.h>

using namespace std;

int matriz[4][4] = {{0, 1, 2, 3}, {1, 2, 3, 0}, {2, 3, 0, 1}, {2, 0, 1, 2}};
bool checker = true;

struct Params
{
    int n;
};

void *vertical(int n)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            if (checker == false)
            {
                pthread_exit(NULL);
                return NULL;
            }
            else
            {

                printf("comparando %d e %d da coluna %d\n", i, j, n);

                if (matriz[j][n] == matriz[i][n])
                {
                    checker = false;
                    printf("Achei iguais, %d = %d, em %d e %d da coluna %d\n", matriz[j][n], matriz[i][n], j, i, n);
                    pthread_exit(NULL);
                }
            }
        }
    }
    return NULL;
}

void *horizontal(int n)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            if (checker == false)
            {
                pthread_exit(NULL);
                return NULL;
            }
            else
            {
                printf("comparando %d e %d da linha %d\n", i, j, n);
                if (matriz[n][j] == matriz[n][i])
                {
                    checker = false;
                    printf("Achei iguais, %d = %d, em %d e %d da linha %d\n", matriz[n][j], matriz[n][i], j, i, n);
                    pthread_exit(NULL);
                }
            }
        }
    }
    return NULL;
}

void *check(void *params)
{
    Params *p = (Params *)params;
    horizontal(p->n);
    vertical(p->n);
    return NULL;
}

int main()
{
    int n;
    int N = 4;
    pthread_t thread[N];
    Params params[N];

    for (int i = 0; i < N; i++)
    {
        Params *p = (Params *)malloc(sizeof(Params));
        p->n = i;
        params[i] = *p;
        pthread_create(&thread[i], NULL, check, (void *)&params[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(thread[i], NULL);
    }

    if (checker == true)
    {
        printf("É um quadrado latino!\n");
    }
    else
    {
        printf("Não é um quadrado latino\n");
    }
    // quadrado latino
    // procura 2 items iguais em cada linha + procura 2 itens iguais em cada coluna
}
// criar 4 threads