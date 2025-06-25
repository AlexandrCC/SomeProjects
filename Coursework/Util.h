#ifndef __Sorting_H__
#define _Sorting_H__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Common.h"
#include "Measurement.h"
#include "Test.h"
#include "Sorting.h"
void OutTable_Head();
void OutTable_Select3Exchange1(float ordered, float random, float backordered);
void OutTable_Select3Exchange2(float ordered, float random, float backordered);
void OutTable_Select3Exchange3(float ordered, float random, float backordered);
void OutVectorTime(float ordered, float random, float backordered);
void menu();
#endif

