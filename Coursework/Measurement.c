#include "Measurement.h"
#include "Common.h"
#include <stdio.h>
clock_t Res[measurements_number];

float MeasurementProcessing()
{
    long int Sum;
    float AverageValue;
    //-----------------------------------------------------------
    // �������� ����������� ���������� �����:
    // ���� 1. ³��������� rejected_number ������ ����� � ��������� �� 0 �� rejected_number-1.
    // ���� 2. ����� ����� �������� ��������� min_max_number ����� � ���������� ����������
    // � min_max_number ����� � ������������� ����������, � ����� �������� ��.
    // ���� 3. ��������� ������ �������� �����, �� ����������.
    //-----------------------------------------------------------

    // ���� 1. ��� ��������� ����� 1 ������ �������� �������� � ������� rejected_number.

    // ���� 2.
    // ��� ����������� min_max_number ��������� � ������������ ������� �����
    // �������� min_max_number �������� ��������� ����� ��������� ���������� ����������
    // � ������� ������� �� rejected_number �� measurements_number-1.
    // � ���������, min_max_number ��������� ������� ����� ������ ����������� �� ��������
    // � ������� ������� �� rejected_number �� rejected_number+min_max_number-1,
    // � min_max_number ������������ ������� ����� - �� �������� � ������� �������
    // �� measurements_number-min_max_number �� measurements_number-1.

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
    
    // ���� 3.
    // ��������� ������ �������� ����� ���� ��������� rejected_number ������ �����
    // �� min_max_number ����� � ���������� ���������� �
    // min_max_number ����� � ������������� ����������, ����� ������ �������� �����
    // � ������� ������� �� rejected_number+min_max_number �� measurements_number-min_max_number-1.
    Sum = 0;
    for (int i = rejected_number + min_max_number; i < measurements_number - min_max_number; i++)
        Sum = Sum + Res[i];

    // ʳ������ �����, �� ���������� ��� ���������� ���������� ��������, �������
    // measurements_number-2*min_max_number-rejected_number
    AverageValue = (float)Sum / (float)(measurements_number - 2 * min_max_number - rejected_number);
    /////////////////////////////////////////////////////////////
    // � ��� ������ � ����� �� �������� �������� �����
    // �� �������� � ������� ��ǲ.
    // ����� �� �������� ���� ���������� �� ������
    printf("");
    /////////////////////////////////////////////////////////////

    return AverageValue;

}
void Sorting_Measurements(int*** Arr3D, char mod, clock_t(*Sorting_Function)(int***)){
    //�������� mod ������� ���������� ������ 3 ��������
    //R- random ���������� ������ ���������� ����������
    //S- sorted ������������ �����
    //B- backordered ��������� ������������� �����
    //� ��������� �� ����� ���������� ���
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