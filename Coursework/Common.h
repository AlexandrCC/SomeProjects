#pragma once
#ifndef __Common_H__
#define __Common_H__
#define P 3
#define M 100
#define N 100
int*** Arr3D;
int taskVector[M*N];
//Функція виділення пам'яті під динамічний масив
void AlockMem();
//Функція для заповнення масиву значеннями згідно до вибраного режиму заповнення
//Аргумент mod функції заповнення приймає 3 значення R S B 
    //R- random заповнення масиву рандомними значеннями
    //S- sorted відсортований масив
    //B- backordered зворотньо впорядкований масив
void fill_Arr(int*** Arr3d, char mod);
// Аргумент mod функції заповнення приймає 3 значення R S B
//R- random заповнення вектора рандомними значеннями
//S- sorted відсортований вектор
//B- backordered зворотньо впорядкований вектор
void fillVector(int* A, char mod);
//Функція звільнення пам'яті віделеної для динамічного масива
void FreeMem(int*** Arr3D);
#endif
