#include <stdio.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include <stdlib.h>

using namespace std;

struct Params
{
    int candidate;
    int n;
    int *candidates;
    pthread_mutex_t *mutexes;
};

void *countVotes2(void *params)
{
    int candidate = ((Params *)params)->candidate;
    int counter = 0;
    int bairro = ((Params *)params)->n;
    ifstream file;
    string filename = to_string(bairro) + ".txt";
    file.open(filename, ios::in);

    if (!file.is_open())
    {
        // cout << "file open error" << endl;
    }
    else
    {
        int index;
        while (file >> index)
        {
            pthread_mutex_lock(&((Params *)params)->mutexes[index]);
            ((Params *)params)->candidates[index] += 1;
            pthread_mutex_unlock(&((Params *)params)->mutexes[index]);
        }
    }
    file.close();
    // string exit = "counter " + to_string(candidate) + " = " + to_string(counter) + "\n";
    // cout << exit << endl;
    return NULL;
}

int main()
{
    cout << "Candidatos: ";
    int numCandidates;
    cin >> numCandidates;
    cout << endl;
    cout << "Bairros: ";
    int numBairros;
    cin >> numBairros;
    cout << endl;

    void *candidates = calloc(numCandidates + 1, sizeof(int));
    void *candidates_mutexes = calloc(numCandidates + 1, sizeof(pthread_mutex_t));
    int candidate[10];

    pthread_t threads[numBairros + 1];
    Params *thread_ids[numBairros + 1];
    int rc;
    int t;

    for (t = 0; t < numBairros + 1; t++)
    {
        thread_ids[t] = (Params *)malloc(sizeof(Params));
        (*thread_ids[t]).candidate = t;
        (*thread_ids[t]).n = t;
        (*thread_ids[t]).candidates = (int *)candidates;
        (*thread_ids[t]).mutexes = (pthread_mutex_t *)candidates_mutexes;
        // printf("thread_ids[%d] para o candidato %d\n", t, t);
        rc = pthread_create(&threads[t], NULL, countVotes2, (void *)thread_ids[t]);
    }

    for (t = 0; t < numCandidates + 1; t++)
    {
        pthread_join(threads[t], NULL);
    }

    cout << "Votos nulos: " << ((int *)candidates)[0] << endl;

    for (t = 1; t < numCandidates + 1; t++)
    {
        cout << "Votos no candidato " << t << ": " << ((int *)candidates)[t] << endl;
    }

    pthread_exit(NULL);

    return 0;
}