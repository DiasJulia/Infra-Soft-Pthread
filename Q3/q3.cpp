#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>

#include <stdlib.h>

using namespace std;

char array[10] = {'a', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; // array de caracteres a serem escritos em loop na base de dados

bool escrevendo = false;

pthread_mutex_t mutex_escrita = PTHREAD_MUTEX_INITIALIZER; // mutex de proteção da base de dados

pthread_cond_t cond_escrita = PTHREAD_COND_INITIALIZER; // condição de escrita da base de dados
pthread_cond_t cond_leitura = PTHREAD_COND_INITIALIZER; // condição de leitura da base de dados

struct Params
{
    int n;
};

void *leitora(void *params)
{
    Params *p = (Params *)params;
    int i = 0;
    while (true)
    {
        pthread_mutex_lock(&mutex_escrita); // verifica se há alguma thread escrevendo no array + impede que alguma thread comece a escrever
        while (escrevendo)
        {
            pthread_cond_wait(&cond_escrita, &mutex_escrita);
        }

        // Código de leitura

        pthread_mutex_unlock(&mutex_escrita);
    }
    return NULL;
}

void *escritora(void *params)
{
    Params *p = (Params *)params;
    int i = p->n;
    while (true)
    {
        pthread_mutex_lock(&mutex_escrita);
        while (escrevendo)
        {
            pthread_cond_wait(&cond_escrita, &mutex_escrita);
        }
        escrevendo = true;

        // Código de escrita de caracter

        escrevendo = false;
        pthread_mutex_unlock(&mutex_escrita);
    }
    return NULL;
}

int main()
{

    int N = 4;
    pthread_t threads_leitoras[N];
    Params params_leitoras[N];

    int M = 4;
    pthread_t threads_escritoras[M];
    Params params_escritoras[M];

    for (int i = 1; i < N; i++)
    {
        Params *p = (Params *)malloc(sizeof(Params));
        p->n = i;
        params_leitoras[i] = *p;
        pthread_create(&threads_leitoras[i], NULL, leitora, (void *)&params_leitoras[i]);
    }

    for (int i = 0; i < M; i++)
    {
        Params *p = (Params *)malloc(sizeof(Params));
        p->n = i;
        params_escritoras[i] = *p;
        pthread_create(&threads_escritoras[i], NULL, escritora, (void *)&params_escritoras[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads_leitoras[i], NULL);
    }

    for (int i = 0; i < M; i++)
    {
        pthread_join(threads_escritoras[i], NULL);
    }

    pthread_exit(NULL);
}