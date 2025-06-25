#include "Measurement.h"
#include "Common.h"
#include <stdio.h>
clock_t Res[measurements_number];

float MeasurementProcessing()
{
    long int Sum;
    float AverageValue;
    //-----------------------------------------------------------
    // Методика усереднення результатів виміру:
    // Крок 1. Відкидається rejected_number перших вимірів з індексами від 0 до rejected_number-1.
    // Крок 2. Серед інших елементів знаходимо min_max_number вимірів з мінімальними значеннями
    // і min_max_number вимірів з максимальними значеннями, і також відкидаємо їх.
    // Крок 3. Знаходимо середнє значення вимірів, що залишилися.
    //-----------------------------------------------------------

    // Крок 1. Для виконання Кроку 1 просто починаємо алгоритм з індексу rejected_number.

    // Крок 2.
    // Для знаходження min_max_number мінімальних і максимальних значень вимірів
    // виконуємо min_max_number ітерацій головного цикла алгоритму шейкерного сортування
    // в діапазоні індексів від rejected_number до measurements_number-1.
    // В результаті, min_max_number мінімальних значень вимірів будуть знаходитись на позиціях
    // в діапазоні індексів від rejected_number до rejected_number+min_max_number-1,
    // а min_max_number максимальних значень вимірів - на позиціях в діапазоні індексів
    // від measurements_number-min_max_number до measurements_number-1.

    clock_t buf;
    int L = rejected_number, R = measurements_number - 1;
    int k = rejected_number;
    for (int j = 0; j < min_max_number; j++) {
        for (int i = L; i < R; i++) {
            if (Res[i] > Res[i + 1]) {
                buf = Res[i];
                Res[i] = Res[i + 1];
                Res[i + 1] = buf;
                k = i;
            }
        }
        R = k;
        for (int i = R - 1; i >= L; i--) {
            if (Res[i] > Res[i + 1]) {
                buf = Res[i];
                Res[i] = Res[i + 1];
                Res[i + 1] = buf;
                k = i;
            }
        }
        L = k + 1;
    }
    
    // Крок 3.
    // Знаходимо середнє значення вимірів після відкидання rejected_number перших вимірів
    // та min_max_number вимірів з мінімальними значеннями і
    // min_max_number вимірів з максимальними значеннями, тобто середнє значення вимірів
    // в діапазоні індексів від rejected_number+min_max_number до measurements_number-min_max_number-1.
    Sum = 0;
    for (int i = rejected_number + min_max_number; i < measurements_number - min_max_number; i++)
        Sum = Sum + Res[i];

    // Кількість вимірів, що залишилась для обчислення середнього значення, дорівнює
    // measurements_number-2*min_max_number-rejected_number
    AverageValue = (float)Sum / (float)(measurements_number - 2 * min_max_number - rejected_number);
    /////////////////////////////////////////////////////////////
    // А цей пустий і нібито не потрібний оператор друку
    // НЕ ВИДАЛЯТИ В ЖОДНОМУ РАЗІ.
    // Навіщо він потрібний буду розповідати на лекції
    printf("");
    /////////////////////////////////////////////////////////////

    return AverageValue;

}
void Sorting_Measurements(int*** Arr3D, char mod, clock_t(*Sorting_Function)(int***)){
    //Аргумент mod функції заповнення приймає 3 значення
    //R- random заповнення масиву рандомними значеннями
    //S- sorted відсортований масив
    //B- backordered зворотньо впорядкований масив
    //В залежності від цього вимірюється чач
    double progress = 0;
    switch (mod)
    {
    case 'R':
        for (int i = 0; i < measurements_number; i++)
        {
            fill_Arr(Arr3D, mod);
            Res[i] = Sorting_Function(Arr3D);

           
        }
        break;

    case 'S':
        for (int i = 0; i < measurements_number; i++)
        {
            fill_Arr(Arr3D, mod);
            Res[i] = Sorting_Function(Arr3D);

            
        }
        break;

        break;
    case 'B':
        for (int i = 0; i < measurements_number; i++)
        {
            fill_Arr(Arr3D, mod);
            Res[i] = Sorting_Function(Arr3D);
            
        }
        break;

        break;

    }
}
void VectorMeasurement(int* A, char mod, clock_t(*Sorting_FunctionV)(int*))
{
    int i = 0;
    double progress = 0;
    switch (mod)
    {
    case 'R':
        for ( i = 0; i < measurements_number; i++)
        {
            fillVector(A, mod);
            Res[i] = Sorting_FunctionV(A);
            
        }
        break;

    case 'S':
        for ( i = 0; i < measurements_number; i++)
        {
            fillVector(A, mod);
            Res[i] = Sorting_FunctionV(A);

            
        }
        break;

        break;
    case 'B':
        for ( i = 0; i < measurements_number; i++)
        {
            fillVector(A, mod);
            Res[i] = Sorting_FunctionV(A);
            
        }
        printf("\n");
        break;
    }
}