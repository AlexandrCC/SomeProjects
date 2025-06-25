#include <stdlib.h>
#include <stdio.h>
#include "Common.h"
void AlockMem()
{
    Arr3D = (int***)malloc(P * sizeof(int**));
    for (int k = 0; k < P; k++)
    {
        Arr3D[k] = (int**)malloc(M * sizeof(int*));
        for (int i = 0; i < M; i++)
            Arr3D[k][i] = (int*)malloc(N * sizeof(int));
    }

}

void FreeMem(int*** Arr3D) {
    for (int k = 0; k < P; k++)
    {
        for (int i = 0; i < M; i++)
            free(Arr3D[k][i]);
        free(Arr3D[k]);
    }
}

void fillVector(int* A, char mod)
{
    switch (mod)
    {
    case 'R':
        for (int i = 0; i < M * N ; i++) A[i] = rand()%(P*M*N);
        break;

    case 'B':
        for (int i = 0; i < M * N; i++) A[i] = M*N - i;
        break;
    case 'S':
        for (int i = 0; i < M * N; i++) A[i] = i;
        break;

    }
    
}
void fill_Arr(int*** Arr3d, char mod)
{
    int number;
    switch (mod)
    {
    case 'R':
        for (int k = 0; k < P; k++) {
            for (int j = 0; j < N;j++) {
                for (int i = 0; i < M; i++) {
                    Arr3d[k][i][j] = rand() % ((P * M * N) * 2);
                }
            }
        }
        break;

    case 'B':
        number = P * M * N;
        for (int k = 0; k < P; k++) {
            for (int j = 0; j < N; j++) {
                for (int i = 0; i < M; i++) {
                    Arr3d[k][i][j] = number;
                    number--;
                }
            }
        }

        break;
    case 'S':
        number = 0;
        for (int k = 0; k < P; k++) {
            for (int j = 0; j < N; j++) {
                for (int i = 0; i < M; i++) {
                    Arr3d[k][i][j] = number;
                    number++;
                }
            }
        }

        break;

    }
}
