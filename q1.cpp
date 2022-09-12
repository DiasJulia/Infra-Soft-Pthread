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
};

void *countVotes2(void *params)
{
    int candidate = ((Params *)params)->candidate;
    int counter = 0;
    int bairros = ((Params *)params)->n;
    ifstream file;
    for (int i = 1; i < bairros + 1; i++)
    {
        string filename = to_string(i) + ".txt";
        file.open(filename, ios::in);

        if (!file.is_open())
        {
            cout << "file open error" << endl;
        }
        else
        {
            int index;
            while (file >> index)
            {
                if (index == candidate)
                {
                    counter++;
                }
            }
        }
        file.close();
    }
    string exit = "counter " + to_string(candidate) + " = " + to_string(counter) + "\n";
    cout << exit << endl;
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
    int candidate[10];

    pthread_t threads[numCandidates + 1];
    Params *thread_ids[numCandidates + 1];
    int rc;
    int t;

    for (t = 0; t < numCandidates + 1; t++)
    {
        thread_ids[t] = (Params *)malloc(sizeof(Params));
        (*thread_ids[t]).candidate = t;
        (*thread_ids[t]).n = numBairros;
        printf("thread_ids[%d] para o candidato %d\n", t, t);
        rc = pthread_create(&threads[t], NULL, countVotes2, (void *)thread_ids[t]);
    }

    printf("Main\n");
    pthread_exit(NULL);

    return 0;
}