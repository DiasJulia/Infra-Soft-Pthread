#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Params
{
    int i;
};

bool garfos[5]; // array de garfos, 1 = sendo usado, 0 = livre

pthread_mutex_t mutex;
pthread_mutex_t mutex_filosofos[5];
pthread_mutex_t mutex_garfos[5];
pthread_t thread[5];

void think(void *param)
{
    Params *p = (Params *)param;
    printf("O filósofo %d está pensando\n", p->i);
}

void get_forks(void *param)
{
    Params *p = (Params *)param;

    pthread_mutex_lock(&mutex_garfos[p->i]);
    pthread_mutex_lock(&mutex_garfos[p->i + 1]);
    if (garfos[p->i] == 0 && garfos[p->i + 1] == 0) // Confere se ambos os recursos estão disponíveis antes de alocá-los para evitar deadlock
    {
        garfos[p->i] = 1;
        garfos[p->i + 1] = 1;
    }
    pthread_mutex_unlock(&mutex_garfos[p->i]);
    pthread_mutex_unlock(&mutex_garfos[p->i + 1]);

    printf("O filósofo %d está com os garfos\n", p->i);
}

void eat(void *param)
{
    Params *p = (Params *)param;
    printf("O filósofo %d está comendo\n", p->i);
}

void put_forks(void *param)
{
    Params *p = (Params *)param;
    pthread_mutex_lock(&mutex_garfos[p->i]);
    pthread_mutex_lock(&mutex_garfos[p->i + 1]);
    garfos[p->i] = 0;
    garfos[p->i + 1] = 0;
    pthread_mutex_unlock(&mutex_garfos[p->i + 1]);
    pthread_mutex_unlock(&mutex_garfos[p->i]);
    printf("O filósofo %d liberou os garfos\n", p->i);
}

void *jantar(void *param)
{
    Params *p = (Params *)param;
    printf("Filósofo %d chegou\n", p->i);

    while (true)
    {
        think(param);
        get_forks(param);
        eat(param);
        put_forks(param);
    }

    return NULL;
}

int main()
{
    Params filosofos_params[5]; // array de filosofos, 1 = comendo, 0 = pensando
    pthread_t filosofos[5];

    for (int i = 0; i < 5; i++)
    {
        Params *p = (Params *)malloc(sizeof(Params));
        p->i = i;
        filosofos_params[i] = *p;
        pthread_create(&filosofos[i], NULL, jantar, (void *)&filosofos_params[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(filosofos[i], NULL);
    }

    pthread_exit(NULL);
}