#include <stdio.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include <stdlib.h>

using namespace std;

struct Params
{
    int train;
    int *intersections;
    pthread_mutex_t *mutexes;
};

void *ctrlTrains(void *params)
{
    Params *p = (Params *)params;
    for (int i = 0; i < 5; i++)
    {
        // Só tá deixando passar 2 de cada vez, caso tenha 2 ou mais ele tenta de novo
        pthread_mutex_lock(&p->mutexes[i]);
        if (p->intersections[i] < 2)
        {
            p->intersections[i] += 1;
            pthread_mutex_unlock(&p->mutexes[i]);
            string exit = "O trem " + to_string(p->train) + " está na intersessão " + to_string(i) + "\n \n" + "Há " + to_string(p->intersections[i]) + " trem na intersessão " + to_string(i) + "\n \n";
            cout << exit;
            exit = "O trem " + to_string(p->train) + " saiu da intersessão " + to_string(i) + "\n \n";
            cout << exit;
            pthread_mutex_lock(&p->mutexes[i]);
            p->intersections[i] -= 1;
            pthread_mutex_unlock(&p->mutexes[i]);
        }
        else
        {
            pthread_mutex_unlock(&p->mutexes[i]);
            i--;
        }
    }
    return NULL;
}

int main()
{

    int numTrains = 5;
    int numIntersections = 5;

    void *intersections = calloc(numIntersections + 1, sizeof(int));
    void *intersections_mutexes = calloc(numIntersections + 1, sizeof(pthread_mutex_t));

    pthread_t threads[numTrains + 1];
    Params *thread_ids[numTrains + 1];

    int rc;
    int t;

    for (t = 0; t < numTrains + 1; t++)
    {
        thread_ids[t] = (Params *)malloc(sizeof(Params));
        (*thread_ids[t]).train = t;
        (*thread_ids[t]).intersections = (int *)intersections;
        (*thread_ids[t]).mutexes = (pthread_mutex_t *)intersections_mutexes;
        // printf("thread_ids[%d] para o candidato %d\n", t, t);
        rc = pthread_create(&threads[t], NULL, ctrlTrains, (void *)thread_ids[t]);
    }
}