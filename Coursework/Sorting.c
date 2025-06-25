#include "Common.h"
#include "Sorting.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
clock_t Select3Exchange1(int*** Arr3d)
{
    int Min, Max, tmp,  c;
    int L, R;
    clock_t time_start, time_stop;
    int* A;
    A = (int*)malloc(M * N * sizeof(int));
    time_start = clock();//Початок таймеру сортування установлений тут згідно до пояснень у курсі відеолекцій 
    for ( int k = 0; k < P; k++) {
        c = 0;
        for (int j = 0; j < N; j++)
        {
            for (int i = 0; i < M; i++)
            {
                A[c] = Arr3d[k][i][j];
                c++;
            }
        }

        L = 0; R = M * N - 1;
        while (L < R) {
            if (A[L] > A[R]) {
                tmp = A[L];
                A[L] = A[R];
                A[R] = tmp;
            }
            Min = A[L]; Max = A[R];
            for (int i = L + 1; i < R + 1; i++) {
                if (A[i] < Min) {
                    Min = A[i];
                    A[i] = A[L];
                    A[L] = Min;
                }
                else
                    if (A[i] > Max) {
                        Max = A[i];
                        A[i] = A[R];
                        A[R] = Max;
                    }
            }
            L = L + 1; R = R - 1;
       
        }
        c = 0;
        for (int j = 0; j < N; j++)
        {
            for (int i = 0; i < M; i++)
            {
                Arr3d[k][i][j] = A[c];
                c++;
            }
        }

    }
    time_stop = clock();
    free(A);
    return time_stop - time_start;
}
clock_t Select3Exchange2(int*** Arr3d) 
{
        int Min, Max, tmp;   
        int L, R;
        int iL, jL, iR, jR;
        int IndexI, IndexJ;
        clock_t time_start, time_stop;
        time_start = clock();//Початок таймеру сортування установлений тут згідно до пояснень у курсі відеолекцій 
        for (int k = 0; k < P; k++) {
            L = 0; R = (M * N) - 1;
            while (L < R) {
                iL = L % M;
                jL = L / M;
                iR = R % M;
                jR = R / M;
                if (Arr3d[k][iL][jL] > Arr3d[k][iR][jR]) {
                    tmp = Arr3d[k][iL][jL];
                    Arr3d[k][iL][jL] = Arr3d[k][iR][jR];
                    Arr3d[k][iR][jR] = tmp;
                }
                Min = Arr3d[k][iL][jL]; Max = Arr3d[k][iR][jR];
                for (int i = L + 1; i < R + 1; i++) {
                    IndexI = i % M;
                    IndexJ = i / M;
                    if (Arr3d[k][IndexI][IndexJ] < Min) {
                        Min = Arr3d[k][IndexI][IndexJ];
                        Arr3d[k][IndexI][IndexJ] = Arr3d[k][iL][jL];
                        Arr3d[k][iL][jL] = Min;
                    }
                    else
                        if (Arr3d[k][IndexI][IndexJ] > Max) {
                            Max = Arr3d[k][IndexI][IndexJ];
                            Arr3d[k][IndexI][IndexJ] = Arr3d[k][iR][jR];
                            Arr3d[k][iR][jR] = Max;
                        }
                }
                L = L + 1; R = R - 1;
            }
        }
        time_stop = clock();
        return time_stop - time_start;
}
clock_t Select3Exchange3(int*** Arr3d)
{
    int Min, Max, tmp;
    int L_i, R_i;
    int L_j, R_j;
    int i, j;
    int k = 0;
    clock_t time_start, time_stop;
    time_start = clock();//Початок таймеру сортування установлений тут згідно до пояснень у курсі відеолекцій
    for (k = 0; k < P; k++) {
        L_i = 0;
        R_i = M - 1;
        L_j = 0;
        R_j = N - 1;
        while (L_j <= R_j) {
            if (L_j == R_j)
            {
                int L = 0;
                int R = M - 1;
                while (L < R) {
                    if (Arr3d[k][L][R_j] > Arr3d[k][R][R_j]) {
                        tmp = Arr3d[k][L][R_j];
                        Arr3d[k][L][R_j] = Arr3d[k][R][R_j];
                        Arr3d[k][R][R_j] = tmp;
                    }
                    Min = Arr3d[k][L][R_j]; Max = Arr3d[k][R][R_j];
                    for (int i = L + 1; i < R + 1; i++) {
                        if (Arr3d[k][i][R_j] < Min) {
                            Min = Arr3d[k][i][R_j];
                            Arr3d[k][i][R_j] = Arr3d[k][L][R_j];
                            Arr3d[k][L][R_j] = Min;
                        }
                        else
                            if (Arr3d[k][i][R_j] > Max) {
                                Max = Arr3d[k][i][R_j];
                                Arr3d[k][i][R_j] = Arr3d[k][R][R_j];
                                Arr3d[k][R][R_j] = Max;
                            }
                    }
                    L = L + 1; R = R - 1;
                }
                L_j++;
                R_j--;
            }

            else
            {

                if (Arr3d[k][L_i][L_j] > Arr3d[k][R_i][R_j]) {
                    tmp = Arr3d[k][R_i][R_j];
                    Arr3d[k][R_i][R_j] = Arr3d[k][L_i][L_j];
                    Arr3d[k][L_i][L_j] = tmp;
                }
                Min = Arr3d[k][L_i][L_j];
                Max = Arr3d[k][R_i][R_j];
                for (j = L_j + 1; j <= R_j - 1; j++)
                {
                    for (i = 0; i < M; i++)
                    {
                        if (Arr3d[k][i][j] < Min) {
                            Min = Arr3d[k][i][j];
                            Arr3d[k][i][j] = Arr3d[k][L_i][L_j];
                            Arr3d[k][L_i][L_j] = Min;
                        }
                        else
                            if (Arr3d[k][i][j] > Max) {
                                Max = Arr3d[k][i][j];
                                Arr3d[k][i][j] = Arr3d[k][R_i][R_j];
                                Arr3d[k][R_i][R_j] = Max;
                            }
                    }
                }
                for (i = 0; i <= R_i; i++) {
                    if (Arr3d[k][i][R_j] < Min) {
                        Min = Arr3d[k][i][R_j];
                        Arr3d[k][i][R_j] = Arr3d[k][L_i][L_j];
                        Arr3d[k][L_i][L_j] = Min;
                    }
                    else
                        if (Arr3d[k][i][R_j] > Max) {
                            Max = Arr3d[k][i][R_j];
                            Arr3d[k][i][R_j] = Arr3d[k][R_i][R_j];
                            Arr3d[k][R_i][R_j] = Max;
                        }
                }
                for (i = M - 1; i > L_i - 1; i--) {
                    if (Arr3d[k][i][L_j] < Min) {
                        Min = Arr3d[k][L_i][L_j];
                        Arr3d[k][i][L_j] = Arr3d[k][L_i][L_j];
                        Arr3d[k][L_i][L_j] = Min;
                    }
                    else
                        if (Arr3d[k][i][L_j] > Max) {
                            Max = Arr3d[k][i][L_j];
                            Arr3d[k][i][L_j] = Arr3d[k][R_i][R_j];
                            Arr3d[k][R_i][R_j] = Max;
                        }
                }
                L_i = L_i + 1;
                R_i = R_i - 1;
                if (L_i == M) {
                    L_i = 0;
                    L_j = L_j + 1;
                    R_i = M - 1;
                    R_j = R_j - 1;
                }
            }
        }
    }
    time_stop = clock();
    return time_stop - time_start;
}
clock_t Select3ExchangeV(int* A)
{
    int Min, Max, tmp;
    int L, R;
    clock_t time_start, time_stop;
    time_start = clock();
    L = 0; R = M*N - 1;
    while (L < R) {
        if (A[L] > A[R]) {
            tmp = A[L];
            A[L] = A[R];
            A[R] = tmp;
        }
        Min = A[L]; Max = A[R];
        for (int i = L + 1; i < R + 1; i++) {
            if (A[i] < Min) {
                Min = A[i];
                A[i] = A[L];
                A[L] = Min;
            }
            else
                if (A[i] > Max) {
                    Max = A[i];
                    A[i] = A[R];
                    A[R] = Max;
                }
        }
        L = L + 1; R = R - 1;
    }
    time_stop = clock();
    return time_stop - time_start;

}