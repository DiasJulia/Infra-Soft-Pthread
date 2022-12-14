#include <stdio.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include <stdlib.h>

using namespace std;

int matriz[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
bool checker = true;

struct Params
{
    int n;
    pthread_t *thread;
};

void *vertical(int n, pthread_t *pthread)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            printf("comparando %d e %d da coluna %d\n", i, j, n);
            if (matriz[j][n] == matriz[i][n])
            {
                checker == false;
                printf("Achei iguais, %d = %d, em %d e %d da coluna %d\n", matriz[j][n], matriz[i][n], j, i, n);
                pthread_cancel(*pthread);
            }
        }
    }
    return NULL;
}

void *horizontal(int n, pthread_t *pthread)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            printf("comparando %d e %d da linha %d\n", i, j, n);
            if (matriz[n][j] == matriz[n][i])
            {
                checker == false;
                printf("Achei iguais, %d = %d, em %d e %d da linha %d\n", matriz[n][j], matriz[n][i], j, i, n);
                pthread_cancel(*pthread);
            }
        }
    }
    return NULL;
}

void *check(void *params)
{
    Params *p = (Params *)params;
    horizontal(p->n, p->thread);
    vertical(p->n, p->thread);
    return NULL;
}

void *mainThread(void *param)
{
    printf("mainThread\n");
    int n;
    int N = ((Params *)param)->n;
    pthread_t thread[N];
    Params params[N];

    for (int i = 0; i < N; i++)
    {
        Params *p = (Params *)malloc(sizeof(Params));
        p->n = i;
        p->thread = ((Params *)param)->thread;
        params[i] = *p;
        pthread_create(&thread[i], NULL, check, (void *)&params[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(thread[i], NULL);
    }

    return NULL;
}

int main()
{
    pthread_t thread;

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    Params *p = (Params *)malloc(sizeof(Params));

    p->n = 4;
    p->thread = &thread;

    pthread_create(&thread, NULL, mainThread, (void *)p);

    pthread_join(thread, NULL);

    if (checker)
    {
        printf("?? um quadrado latino!\n");
    }
    else
    {
        printf("N??o ?? um quadrado latino");
    }
    // quadrado latino
    // procura 2 items iguais em cada linha + procura 2 itens iguais em cada coluna
}
// criar 4 threads