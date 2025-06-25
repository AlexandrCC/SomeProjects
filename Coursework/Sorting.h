#pragma once
#ifndef __Sorting_H__
#define _Sorting_H__
#include <time.h>
//Функція сортування 3ьох вимірного масиву за допомогою алгоритма №20 Способу обходу (1) 
//Використовуючи фізичний вектор з виділення пам'яті та перепису матриці в вектор
clock_t Select3Exchange1(int*** Arr3d);
clock_t Select3Exchange2(int*** Arr3d);
clock_t Select3Exchange3(int*** Arr3d);
clock_t Select3ExchangeV(int* A);

#endif
