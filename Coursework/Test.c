#include "Test.h"
#include <stdio.h>
#include <stdlib.h>

void OutArr3D(int*** Arr3d)
{
 
    for (int k = 0; k < P; k++) {
        printf("\n");
        for (int i = 0; i < M; i++) {
            printf("\n\t");
            for (int j = 0; j < N; j++) {
                printf(" %d", Arr3D[k][i][j]);
            }
        }
    }

}
void Out_Vector(int* A)
{
    printf("\n");
    for (int i = 0; i < M * N;i++)
    {
        printf(" %d ", taskVector[i]);
    }
    printf("\n");
}
void Testsorting()
{
    ///////////////////////////////////////////////////////////////////////////////
    //ÏÎ×ÀÒÎÊ ÒÅÑÒÓÂÀÍÍß ÏÅÐØÎÃÎ ÀËÃÎÐÈÒÌÓ(ÑÏÎÑÎÁÓ ÎÁÕÎÄÓ)
    ///////////////////////////////////////////////////////////////////////////////
    system("cls");
    printf("Make sure to use small size of array for testing so u dont overflow terminal with symbols!!!\nCurrent size is P= %d M= %d N= %d\nPress any key to confirm that you checked if size are reasonable\n",P,M,N);
    system("pause");
    printf("Example: display of sorted by columns array:\n");
    fill_Arr(Arr3D,'S');
    OutArr3D(Arr3D);
    printf("\nTest of alorithm Select3Exchange(way1) Sorted to Sorted: \n");
    fill_Arr(Arr3D, 'S');
    OutArr3D(Arr3D);
    printf("\nAfter sorting :\n");
    Select3Exchange1(Arr3D);
    OutArr3D(Arr3D);
    printf("\nTest of alorithmSelect3Exchange(way1) Random to Sorted: \n");
    fill_Arr(Arr3D, 'R');
    OutArr3D(Arr3D);
    printf("\nAfter sorting :\n");
    Select3Exchange1(Arr3D);
    OutArr3D(Arr3D);
    printf("\nTest of alorithm Select3Exchange(way1) Reverse ordered to Sorted: \n");
    fill_Arr(Arr3D, 'B');
    OutArr3D(Arr3D);
    printf("\nAfter sorting :\n");
    Select3Exchange1(Arr3D);
    OutArr3D(Arr3D);
    printf("\n\t\tEnd of test for Select3Exchange(way1)\n");
    printf("To start testing for next alorithm press any key\n");
    system("pause");
    system("cls");
    ///////////////////////////////////////////////////////////////////////////////
    //Ê²ÍÅÖÜ ÒÅÑÒÓÂÀÍÍß ÏÅÐØÎÃÎ ÀËÃÎÐÈÒÌÓ(ÑÏÎÑÎÁÓ ÎÁÕÎÄÓ)
    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    //ÏÎ×ÀÒÎÊ ÒÅÑÒÓÂÀÍÍß ÄÐÓÃÎÃÎ ÀËÃÎÐÈÒÌÓ(ÑÏÎÑÎÁÓ ÎÁÕÎÄÓ)
    ///////////////////////////////////////////////////////////////////////////////
    printf("Example: display of sorted by columns array:\n");
    fill_Arr(Arr3D, 'S');
    OutArr3D(Arr3D);
    printf("\nTest of alorithm Select3Exchange(way2) Sorted to Sorted: \n");
    fill_Arr(Arr3D, 'S');
    OutArr3D(Arr3D);
    printf("\nAfter sorting :\n");
    Select3Exchange2(Arr3D);
    OutArr3D(Arr3D);
    printf("\nTest of alorithmSelect3Exchange(way2) Random to Sorted: \n");
    fill_Arr(Arr3D, 'R');
    OutArr3D(Arr3D);
    printf("\nAfter sorting :\n");
    Select3Exchange2(Arr3D);
    OutArr3D(Arr3D);
    printf("\nTest of alorithm Select3Exchange(way2) Reverse ordered to Sorted: \n");
    fill_Arr(Arr3D, 'B');
    OutArr3D(Arr3D);
    printf("\nAfter sorting :\n");
    Select3Exchange2(Arr3D);
    OutArr3D(Arr3D);
    printf("\n\t\tEnd of test for Select3Exchange(way2)\n");
    printf("To start testing for next alorithm press any key\n");
    system("pause");
    system("cls");
    ///////////////////////////////////////////////////////////////////////////////
    //Ê²ÍÅÖÜ ÒÅÑÒÓÂÀÍÍß ÄÐÓÃÎÃÎ ÀËÃÎÐÈÒÌÓ(ÑÏÎÑÎÁÓ ÎÁÕÎÄÓ)
    ///////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////////////////////
    //ÏÎ×ÀÒÎÊ ÒÅÑÒÓÂÀÍÍß ÒÐÅÒÜÎÃÎ ÀËÃÎÐÈÒÌÓ(ÑÏÎÑÎÁÓ ÎÁÕÎÄÓ)
    ///////////////////////////////////////////////////////////////////////////////
    printf("Example: display of sorted by columns array:\n");
    fill_Arr(Arr3D, 'S');
    OutArr3D(Arr3D);
    printf("\nTest of alorithm Select3Exchange(way3) Sorted to Sorted: \n");
    fill_Arr(Arr3D, 'S');
    OutArr3D(Arr3D);
    printf("\nAfter sorting :\n");
    Select3Exchange3(Arr3D);
    OutArr3D(Arr3D);
    printf("\nTest of alorithmSelect3Exchange(way3) Random to Sorted: \n");
    fill_Arr(Arr3D, 'R');
    OutArr3D(Arr3D);
    printf("\nAfter sortingg :\n");
    Select3Exchange3(Arr3D);
    OutArr3D(Arr3D);
    printf("\nTest of alorithm Select3Exchange(way3) Reverse ordered to Sorted: \n");
    fill_Arr(Arr3D, 'B');
    OutArr3D(Arr3D);
    printf("\nAfter sorting :\n");
    Select3Exchange3(Arr3D);
    OutArr3D(Arr3D);
    printf("\n\t\tEnd of test for Select3Exchange(way3)\n");
    printf("To start testing for next alorithm press any key\n");
    system("pause");
    system("cls");
    ///////////////////////////////////////////////////////////////////////////////
    //Ê²ÍÅÖÜ ÒÅÑÒÓÂÀÍÍß ÒÐÅÒÜÎÃÎ ÀËÃÎÐÈÒÌÓ(ÑÏÎÑÎÁÓ ÎÁÕÎÄÓ)
    ///////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////////////////////
    //ÏÎ×ÀÒÎÊ ÒÅÑÒÓÂÀÍÍß Âåêòîðó çàäàíèì àëãîðèòìîì
    ///////////////////////////////////////////////////////////////////////////////
    printf("Example: display of sorted by columns array:\n");
    fillVector(taskVector, 'S');
    Out_Vector(taskVector);
    printf("\nTest of alorithm Select3Exchange Vector Sorted to Sorted: \n");
    fillVector(taskVector, 'S');
    Out_Vector(taskVector);
    printf("\nAfter sorting :\n");
    Select3ExchangeV(taskVector);
    Out_Vector(taskVector);
    printf("\nTest of alorithmSelect3Exchange Vector Random to Sorted: \n");
    fillVector(taskVector, 'R');
    Out_Vector(taskVector);
    printf("\nAfter sorting :\n");
    Select3ExchangeV(taskVector);
    Out_Vector(taskVector);
    printf("\nTest of alorithm Select3Exchange Vector Reverse ordered to Sorted: \n");
    fillVector(taskVector, 'B');
    Out_Vector(taskVector);
    printf("\nAfter sorting :\n");
    Select3ExchangeV(taskVector);
    Out_Vector(taskVector);
    printf("\n\t\tEnd of test for Select3Exchange Vector\n");
    printf("To start testing for next alorithm press any key\n");
    system("pause");
    system("cls");
    ///////////////////////////////////////////////////////////////////////////////
    //Ê²ÍÅÖÜ ÒÅÑÒÓÂÀÍÍß Âåêòîðó
    ///////////////////////////////////////////////////////////////////////////////
}