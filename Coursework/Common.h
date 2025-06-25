#pragma once
#ifndef __Common_H__
#define __Common_H__
#define P 3
#define M 100
#define N 100
int*** Arr3D;
int taskVector[M*N];
//������� �������� ���'�� �� ��������� �����
void AlockMem();
//������� ��� ���������� ������ ���������� ����� �� ��������� ������ ����������
//�������� mod ������� ���������� ������ 3 �������� R S B 
    //R- random ���������� ������ ���������� ����������
    //S- sorted ������������ �����
    //B- backordered ��������� ������������� �����
void fill_Arr(int*** Arr3d, char mod);
// �������� mod ������� ���������� ������ 3 �������� R S B
//R- random ���������� ������� ���������� ����������
//S- sorted ������������ ������
//B- backordered ��������� ������������� ������
void fillVector(int* A, char mod);
//������� ��������� ���'�� ������� ��� ���������� ������
void FreeMem(int*** Arr3D);
#endif
