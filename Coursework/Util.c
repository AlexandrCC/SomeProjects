#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Util.h"
#pragma warning(disable : 4996)

void OutTable_Head()
{
    printf("\t\t\t Table# for array A[P][M][N] P= %d M= %d N= %d \n", P, M, N);
    printf("\t\t\t  Ordered \t   Random \t BackOrdered \n");
}
void OutTable_Select3Exchange1(float ordered, float random, float backordered)
{
    printf("Select3Exchange1\t %7.2f \t %7.2f \t %7.2f \n", ordered, random, backordered);
    printf("\n\n");
}
void OutTable_Select3Exchange2(float ordered, float random, float backordered)
{
    printf("Select3Exchange2\t %7.2f \t %7.2f \t %7.2f \n", ordered, random, backordered);
    printf("\n\n");
}
void OutTable_Select3Exchange3(float ordered, float random, float backordered)
{

    printf("Select3Exchange3\t %7.2f \t %7.2f \t %7.2f \n", ordered, random, backordered);
    printf("\n\n");
}
void OutVectorTime(float ordered, float random, float backordered)
{
    // Усереднений результат вимірів буде виведено на екран у портібну позицію
    printf("\t\t\t Ordered \t Random \t BackOrdered \n");
    // В даному прикладі в усіх трьох колонках таблиці вимірів друкується одне й те ж саме значення
    printf("Vector (M*N)\t %7.2f \t %7.2f \t %7.2f \n", ordered, random, backordered);
    printf("\n\n");
}

void menu()
{
    srand(time(NULL));

    float SortingTimeS_method1, SortingTimeR_method1, SortingTimeB_method1;
    float SortingTimeS_method2, SortingTimeR_method2, SortingTimeB_method2;
    float SortingTimeS_method3, SortingTimeR_method3, SortingTimeB_method3;
    float Vector_SortingTime_R, Vector_SortingTime_B, Vector_SortingTime_S;

    char mode, mode_2;
    AlockMem();
    printf("To chose mode enter: \nP- pakage mode to print whole table\n1- to print only alorithm #1\n2- to print alorith #2\n3- to printf alogithm#3\nV- to print only vector measures\nUse T- to enter testing sequence");
    printf("\n If u chose single alorithm eneter needed  mode and after space 1 'x'  to chose case you want to test\nA- for pacage\nS-for sorted\nR- for random\nB- for backordered\n");
    scanf("%c %c", &mode, &mode_2);
    switch (mode)
    {
    case 'P':




        //
        //Виміри для алгоритма(способу обходу) №1
        ///////////////////////////////////////////////////////// 

        Sorting_Measurements(Arr3D, 'R', Select3Exchange1);
        SortingTimeR_method1 = MeasurementProcessing();

        Sorting_Measurements(Arr3D, 'S', Select3Exchange1);
        SortingTimeS_method1 = MeasurementProcessing();

        Sorting_Measurements(Arr3D, 'B', Select3Exchange1);
        SortingTimeB_method1 = MeasurementProcessing();

        OutTable_Head();

        OutTable_Select3Exchange1(SortingTimeS_method1, SortingTimeR_method1, SortingTimeB_method1);

        ///////////////////////////////////////////////////////
        //Кінець вимірів для алгоритма(способу обходу) №1
        //



        //
        //Виміри для алгоритма(способу обходу) №2
        /////////////////////////////////////////////////////////

        Sorting_Measurements(Arr3D, 'R', Select3Exchange2);
        SortingTimeR_method2 = MeasurementProcessing();

        Sorting_Measurements(Arr3D, 'S', Select3Exchange2);
        SortingTimeS_method2 = MeasurementProcessing();

        Sorting_Measurements(Arr3D, 'B', Select3Exchange2);
        SortingTimeB_method2 = MeasurementProcessing();

        OutTable_Select3Exchange2(SortingTimeS_method2, SortingTimeR_method2, SortingTimeB_method2);

        /////////////////////////////////////////////////////////
        //Кінець вимірів для алгоритма(способу обходу) №2
        //



        //
        //Виміри для алгоритма(способу обходу) №3
        ////////////////////////////////////////////////////////

        Sorting_Measurements(Arr3D, 'R', Select3Exchange3);
        SortingTimeR_method3 = MeasurementProcessing();

        Sorting_Measurements(Arr3D, 'S', Select3Exchange3);
        SortingTimeS_method3 = MeasurementProcessing();

        Sorting_Measurements(Arr3D, 'B', Select3Exchange3);
        SortingTimeB_method3 = MeasurementProcessing();

        OutTable_Select3Exchange3(SortingTimeS_method3, SortingTimeR_method3, SortingTimeB_method3);

        ////////////////////////////////////////////////////////
        //Кінець вимірів для алгоритма(способу обходу) №3
        //



        //
        //Виміри вектора
        ////////////////////////////////////////////////////////
        VectorMeasurement(taskVector, 'R', Select3ExchangeV);
        Vector_SortingTime_R = MeasurementProcessing();
        //

        //
        VectorMeasurement(taskVector, 'B', Select3ExchangeV);
        Vector_SortingTime_B = MeasurementProcessing();
        //

        //
        VectorMeasurement(taskVector, 'S', Select3ExchangeV);
        Vector_SortingTime_S = MeasurementProcessing();
        //
        OutVectorTime(P * Vector_SortingTime_S, P * Vector_SortingTime_R, P * Vector_SortingTime_B);
        ////////////////////////////////////////////////////////
        //Кінець вимірів вектора
        //


        ///

        ///

        break;

    case '1':
        switch (mode_2)
        {
        case 'R':

            Sorting_Measurements(Arr3D, 'R', Select3Exchange1);
            SortingTimeR_method1 = MeasurementProcessing();

            OutTable_Head();
            OutTable_Select3Exchange1(-0, SortingTimeR_method1, -0);

            break;

        case 'S':

            Sorting_Measurements(Arr3D, 'S', Select3Exchange1);
            SortingTimeS_method1 = MeasurementProcessing();

            OutTable_Head();
            OutTable_Select3Exchange1(SortingTimeS_method1, -0, -0);

            break;

            break;
        case 'B':

            Sorting_Measurements(Arr3D, 'B', Select3Exchange1);
            SortingTimeB_method1 = MeasurementProcessing();

            OutTable_Head();
            OutTable_Select3Exchange1(-0, -0, SortingTimeB_method1);

            break;

        case 'A':

            Sorting_Measurements(Arr3D, 'R', Select3Exchange1);
            SortingTimeR_method1 = MeasurementProcessing();

            Sorting_Measurements(Arr3D, 'S', Select3Exchange1);
            SortingTimeS_method1 = MeasurementProcessing();

            Sorting_Measurements(Arr3D, 'B', Select3Exchange1);
            SortingTimeB_method1 = MeasurementProcessing();

            OutTable_Head();
            OutTable_Select3Exchange1(SortingTimeS_method1, SortingTimeR_method1, SortingTimeB_method1);

            break;
        }


        break;
    case '2':

        switch (mode_2)
        {
        case 'R':

            Sorting_Measurements(Arr3D, 'R', Select3Exchange2);
            SortingTimeR_method2 = MeasurementProcessing();

            OutTable_Head();
            OutTable_Select3Exchange2(-0, SortingTimeR_method2, -0);

            break;

        case 'S':

            Sorting_Measurements(Arr3D, 'S', Select3Exchange2);
            SortingTimeS_method2 = MeasurementProcessing();

            OutTable_Head();
            OutTable_Select3Exchange2(SortingTimeS_method2, -0, -0);

            break;

            break;
        case 'B':

            Sorting_Measurements(Arr3D, 'B', Select3Exchange2);
            SortingTimeB_method2 = MeasurementProcessing();

            OutTable_Head();
            OutTable_Select3Exchange2(-0, -0, SortingTimeB_method2);
            break;

        case 'A':
            Sorting_Measurements(Arr3D, 'R', Select3Exchange2);
            SortingTimeR_method2 = MeasurementProcessing();

            Sorting_Measurements(Arr3D, 'S', Select3Exchange2);
            SortingTimeS_method2 = MeasurementProcessing();

            Sorting_Measurements(Arr3D, 'B', Select3Exchange2);
            SortingTimeB_method2 = MeasurementProcessing();

            OutTable_Head();
            OutTable_Select3Exchange2(SortingTimeS_method2, SortingTimeR_method2, SortingTimeB_method2);
            break;
        }


        break;
    case '3':
        Sorting_Measurements(Arr3D, 'R', Select3Exchange3);
        SortingTimeR_method3 = MeasurementProcessing();

        Sorting_Measurements(Arr3D, 'S', Select3Exchange2);
        SortingTimeS_method3 = MeasurementProcessing();

        Sorting_Measurements(Arr3D, 'B', Select3Exchange2);
        SortingTimeB_method3 = MeasurementProcessing();

        OutTable_Head();
        OutTable_Select3Exchange3(SortingTimeS_method3, SortingTimeR_method3, SortingTimeB_method3);
        break;
    case 'V':

        switch (mode_2)
        {
        case 'R':

            VectorMeasurement(taskVector, 'R', Select3ExchangeV);
            Vector_SortingTime_R = MeasurementProcessing();
            OutVectorTime(P * -0, P * Vector_SortingTime_R, P * -0);
            break;

        case 'S':

            VectorMeasurement(taskVector, 'S', Select3ExchangeV);
            Vector_SortingTime_S = MeasurementProcessing();
            OutVectorTime(P * Vector_SortingTime_S, P * -0, P * -0);
            break;
        case 'B':

            VectorMeasurement(taskVector, 'B', Select3ExchangeV);
            Vector_SortingTime_B = MeasurementProcessing();
            OutVectorTime(P * -0, P * -0, P * Vector_SortingTime_B);

            break;

        case 'A':

            VectorMeasurement(taskVector, 'R', Select3ExchangeV);
            Vector_SortingTime_R = MeasurementProcessing();
            //

            //
            VectorMeasurement(taskVector, 'B', Select3ExchangeV);
            Vector_SortingTime_B = MeasurementProcessing();
            //

            //
            VectorMeasurement(taskVector, 'S', Select3ExchangeV);
            Vector_SortingTime_S = MeasurementProcessing();
            //
            OutVectorTime(P * Vector_SortingTime_S, P * Vector_SortingTime_R, P * Vector_SortingTime_B);

            break;
        }


        break;
    case 'T':
        Testsorting();
        break;
    }

    FreeMem(Arr3D);
    return 0;
}