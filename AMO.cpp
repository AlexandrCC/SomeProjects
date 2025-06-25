#include <iostream>
#include <math.h>
#include <iomanip>
#include <string>
#include <time.h>

using namespace std;

//Матриця  для першої слар
double A1[9][9] = { -100, 9, - 94, - 11, - 62, - 97, - 100, - 94,11,
                    - 65, 100, 96, - 38, 15, 99, - 87, - 86, 89,
                    10, - 85, 18, 44, 77, - 19, - 84, - 16, - 32,
                    - 18, - 31, - 85, 61, - 27, - 85, - 68, 35, 50,
                    - 14, 24, 72, - 7, 73, - 84, 82, - 43, 83,
                    56, - 37, - 73, - 39, - 18, 86, 6, - 8, 62,
                    - 95, - 58, 100, 50, - 65, 42, - 78, - 70, 57,
                    8, 8, - 40, - 15, 42, 83, 89, 99, 1,
                    88 ,78, 86, 90, 58, - 33, 77, 25, - 2

};

double B1[9] = { 
    -81,
 - 57,
 - 21,  
 - 64,
 63,
 - 38,
 - 12,
 - 5,
 12,


};

//Матриця  для Другої слар
double A2[10][10] = { 
    221, - 128, - 118,	26,	134, - 28, - 196,	77, - 8,	3,
- 128,	195, - 4,	23, - 42,	3,	123, - 114, - 16, - 88,
- 118, - 4,	209, - 29, - 121,	67,	69, - 65,	5,	42,
26,	23, - 29,	44,	20, - 33, - 36, - 21,	20, - 17,
134, - 42, - 121,	20,	249, - 162, - 91,	10,	35, - 124,
- 28,	3,	67, - 33, - 162,	240, - 40, - 22, - 116,	77,
- 196,	123,	69, - 36, - 91, - 40,	235, - 28,	21, - 15,
77, - 114, - 65, - 21,	10, - 22, - 28,	189,	29,	102,
- 8, - 16,	5,	20,	35, - 116,	21,	29,	243, - 22,
3, - 88,	42, - 17, - 124,	77, - 15,	102, - 22,	197
};

double B2[10] = {  
55,
 238,
 90,
 - 17,
 - 49,
 - 94,
 81,
 - 248,
 28,
 - 116,
};


//Матриця  для Третьої слар
double A3[10][10] = { 333, 26,    0,    0,    0,    0,    0,    0,    0,    0,
                        66,  396,  -127,    0,    0,    0,    0,    0,    0,    0,
                         0,   371,  -744,  -244,    0,    0,    0,    0,    0,    0,
                         0,    0,  57, -935, 357,    0,    0,    0,    0,    0,
                         0,    0,    0,  -105, 339, -44,    0,    0,    0,    0,
                         0,    0,    0,    0, 38,  -79,  -27,    0,    0,    0,
                         0,    0,    0,    0,    0,   103,   711,   -88,    0,    0,
                         0,    0,    0,    0,    0,    0,  -44, -618,  -11,    0,
                         0,    0,    0,    0,    0,    0,    0,  68,  -759, -208,
                         0,    0,    0,    0,    0,    0,    0,    0, 38, -231
};

double B3[10] = {
     -497,
 - 419,
 234,
 - 470,
 649,
 966,
 461,
 - 312,
 168,
 - 785,

};


double checkA[5][5] = { { 25, 15, -5,  0,  0 },
                         { 15, 18,  0, -4,  0 },
                         { -5,  0, 11, -2,  0 },
                         {  0, -4, -2,  8,  4 },
                         {  0,  0,  0,  4, 10 } };

double checkB[5] = { 50, 41, 12, 17, 22 };



void Printer(double** name, const int nx, const int ny, string str) {
    cout << str << endl << endl;
    //cout << fixed;
    //cout.precision(4);
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            if (abs(name[i][j]) < 0.000001f) {
                cout << setw(10) << "0";
                continue;
            }
            cout << setw(10) << name[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
}

void Printer(double* name, const int n, string str) {
    cout << str << endl << endl;
    for (int i = 0; i < n; i++) {
        cout << setw(12) << name[i];
        cout << "\n";
    }
    cout << "\n";
}
//Функції що використовувались для налагодження методів але не можуть використовуватись для виконання завдання лабораторної

double** Multiply(double** a, double** b, const int na, const int nb) {
    double** result = new double* [na];
    for (int i = 0; i < nb; i++) {
        result[i] = new double[nb];
    }
    for (int i = 0; i < na; i++) {
        for (int j = 0; j < nb; j++) {
            result[i][j] = 0;
            for (int k = 0; k < na; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}


double* Multiply(double** a, double* b, const int na, const int nb) {
    double* result = new double[na];

    for (int i = 0; i < na; i++) {
        result[i] = 0;
        for (int k = 0; k < na; k++) {
            result[i] += a[i][k] * b[k];
        }
    }
    return result;
}

void getMatrixWithoutRowAndCol(double** matrix, int size, int row, int col, double** newMatrix) {
    int offsetRow = 0;
    int offsetCol = 0;
    for (int i = 0; i < size - 1; i++) {
        if (i == row) {
            offsetRow = 1;
        }

        offsetCol = 0;
        for (int j = 0; j < size - 1; j++) {
            if (j == col) {
                offsetCol = 1;
            }

            newMatrix[i][j] = matrix[i + offsetRow][j + offsetCol];
        }
    }
}

double Determinant(double** matrix, int size) {
    double det = 0;
    int degree = 1;

    if (size == 1) {
        return matrix[0][0];
    }
    if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    double** newMatrix = new double* [size - 1];
    for (int i = 0; i < size - 1; i++) {
        newMatrix[i] = new double[size - 1];
    }
    for (int j = 0; j < size; j++) {
        getMatrixWithoutRowAndCol(matrix, size, 0, j, newMatrix);
        det = det + (degree * matrix[0][j] * Determinant(newMatrix, size - 1));
        degree = -degree;
    }
    for (int i = 0; i < size - 1; i++) {
        delete[] newMatrix[i];
    }
    delete[] newMatrix;

    return det;
}

double** Allied(double** a, const int na) {
    double** result = new double* [na];
    for (int i = 0; i < na; i++) {
        result[i] = new double[na];
    }
    double** b = new double* [na - 1];
    for (int i = 0; i < na - 1; i++) {
        b[i] = new double[na - 1];
    }
    for (int i = 0; i < na; i++) {
        for (int j = 0; j < na; j++) {
            //result[i][j] = -(2*((i+j)%2)-1) * Determinant(a, na);
            for (int p = 0; p < i; p++) {
                for (int r = 0; r < j; r++) {
                    b[p][r] = a[p][r];
                }
                for (int r = j + 1; r < na; r++) {
                    b[p][r - 1] = a[p][r];
                }
            }
            for (int p = i + 1; p < na; p++) {
                for (int r = 0; r < j; r++) {
                    b[p - 1][r] = a[p][r];
                }
                for (int r = j + 1; r < na; r++) {
                    b[p - 1][r - 1] = a[p][r];
                }
            }
            result[i][j] = -(2 * ((i + j) % 2) - 1) * Determinant(b, na - 1);
        }
    }
    return result;
}

double** Transposing(double** a, const int nx, const int ny) {
    double** result = new double* [ny];
    for (int i = 0; i < nx; i++) {
        result[i] = new double[nx];
    }
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            result[i][j] = a[j][i];
        }
    }
    return result;
}

double** Inversion(double** a, const int na) {
    double** result = new double* [na];
    for (int i = 0; i < na; i++) {
        result[i] = new double[na];
    }
    result = Allied(a, na);
    //Printer(result, na);
    result = Transposing(result, na, na);
    //Printer(result, na);
    double det = Determinant(a, na);
    //cout << det << endl;
    for (int i = 0; i < na; i++) {
        for (int j = 0; j < na; j++) {
            result[i][j] /= det;
        }
    }
    //Printer(result, na);
    return result;
}


void MethodLU_L() {
    cout << "Method LU factorisation with unar diagonal in L\n" << endl;
    

    const int n = 5;//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
    double** L = new double* [n];
    double** U = new double* [n];
    double* y = new double[n];
    double* help = new double[n];
    double** ttt = new double* [n];
    double** A_1 = new double* [n];
    clock_t start = clock();
    for (int i = 0; i < n; i++) {
        L[i] = new double[n];
        U[i] = new double[n];
        ttt[i] = new double[n];
        A_1[i] = new double[n];
        help[i] = checkB[i];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            L[i][j] = 0;
            U[i][j] = 0;
            ttt[i][j] = checkA[i][j];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
        }
        L[i][i] = 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i <= j) {
                U[i][j] = checkA[i][j];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
                for (int k = 0; k < i; k++) {
                    U[i][j] -= L[i][k] * U[k][j];
                }
            }
            if (i > j) {
                L[i][j] = checkA[i][j];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
                for (int k = 0; k < j; k++) {
                    L[i][j] -= L[i][k] * U[k][j];
                }
                L[i][j] /= U[j][j];
            }

        }
    }
    Printer(ttt, n, n, "Matrix A1:");   

    Printer(L, n, n, "Matrix L:");
    Printer(U, n, n, "Matrix U:");

    //y = Multiply(Inversion(L, n), help, n, n);Ті самі строчки що використовувались для налагодження
    double buffer;
    for (int i = 0; i < n; i++) {
        y[i] = help[i] / L[i][i];
        for (int j = 0; j < i; j++) {
            buffer = (L[i][j] * y[j]);
            y[i] -= buffer;
        }
        y[i] /= L[i][i];
    }
    Printer(y, n, "Vector Y:");
    //help = Multiply(Inversion(U, n), y, n, n);Ті самі строчки що використовувались для налагодження
    for (int i = n - 1; i >= 0; i--) {
        help[i] = y[i];
        for (int j = i + 1; j < n; j++) {
            help[i] -= U[i][j] * help[j];
        }
        help[i] /= U[i][i];
    }
    Printer(help, n, "Vector X (result):");
    /*y = Multiply(ttt, help, n, n);Ті самі строчки що використовувались для налагодження
    Printer(y, n, "A1 * X = B1");*/
    
    clock_t end = clock();
    double ms = (double)(end - start);
    cout << ms << " ms." << endl;
    int Cr = 0;
    float sum = 0;
    for (int p = n - 1; p >= 0; --p)
    {

        int j = p;
        for (int i = j; i >= 0; --i)
        {
            int Cr = 0;
            if (i == j)
            {
                Cr = 1;
            }
            float sum = 0;
            for (int k = i + 1; k < n; ++k)
            {
                sum += U[i][k] * A_1[k][j];
            }
            A_1[i][j] = (Cr - sum) / U[i][i];
        }
        int i = p;
        for (j = i - 1; j >= 0; --j)
        {
            sum = 0;
            for (int k = j + 1; k < n; ++k)
            {
                sum += A_1[i][k] * L[k][j];
            }
            A_1[i][j] = -sum;
        }
    }
    Printer(A_1, n, n, "Matrix A-1:");
}

void MethodLU_U() {
    cout << "Method LU factorisation with unar diagonal in U\n" << endl;
    clock_t start = clock();

    const int n = 5;//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
    double** L = new double* [n];
    double** U = new double* [n];
    double* y = new double [n];
    double* help = new double [n];
    double** ttt = new double* [n];
    double** A_1 = new double* [n];
    for (int i = 0; i < n; i++) {
        L[i] = new double[n];
        U[i] = new double[n];
        ttt[i] = new double[n];
        A_1[i] = new double[n];
        help[i] = checkB[i];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            L[i][j] = 0;
            U[i][j] = 0;
            ttt[i][j] = checkA[i][j];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
        }
        U[i][i] = 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i < j) {
                U[i][j] = checkA[i][j];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
                for (int k = 0; k < i; k++) {
                    U[i][j] -= L[i][k] * U[k][j];
                }
                U[i][j] /= L[i][i];
            }
            else {
                L[i][j] = checkA[i][j];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
                for (int k = 0; k < j; k++) {
                    L[i][j] -= L[i][k] * U[k][j];
                }
            }
        }
    }
    Printer(ttt, n, n, "Matrix A1:");

    Printer(L, n, n, "Matrix L:");
    Printer(U, n, n, "Matrix U:");

    //y = Multiply(Inversion(L, n), help, n, n);Ті самі строчки що використовувались для налагодження
    for (int i = 0; i < n; i++) {
        y[i] = help[i] / L[i][i];
        for (int j = i - 1; j >= 0; j--) {
            y[i] -= (L[i][j] * y[j]) / L[i][i];
        }
    }
    Printer(y, n, "Vector Y:");
    //help = Multiply(Inversion(U, n), y, n, n);Ті самі строчки що використовувались для налагодження
    for (int i = n - 1; i >= 0; i--) {
        help[i] = y[i];
        for (int j = i + 1; j < n; j++) {
            help[i] -= U[i][j] * help[j];
        }
    }
    Printer(help, n, "Vector X (result):");
    /*y = Multiply(ttt, help, n, n);Ті самі строчки що використовувались для налагодження
    Printer(y, n, "A1 * X = B1");*/


   
    clock_t end = clock();
    double ms = (double)(end - start);
    cout << ms << " ms." << endl;
    int Cr = 1;
    double sum = 0;
    int i, j;
    for (int k = n - 1; k >= 0; k--)
    {
        i = k;
        for (j = i; j >= 0; j--)
        {
            sum = 0;
            for (int t = j + 1; t < n; t++)
            {
                sum += A_1[i][t] * L[t][j];
            }
            if (i == j) Cr = 1; else Cr = 0;
            A_1[i][j] = (Cr - sum) / L[j][j];
        }

        j = k;
        for (i = j - 1; i >= 0; i--)
        {
            sum = 0;
            for (int t = i + 1; t < n; t++)
            {
                sum += U[i][t] * A_1[t][j];
            }
            A_1[i][j] = 0 - sum;
        }
    }
    Printer(A_1, n, n, "Matrix A-1:");
}


void MethodCholesky() {
    cout << "Method of Kholesky\n" << endl;
    clock_t start = clock();

    const int n = 5;//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
    double** a = new double* [n];
    double** L = new double* [n];
    double** result = new double* [n];
    double* y = new double [n];
    double* b = new double [n];
    double* tmp = new double[n + 1];
    double** A_1 = new double*[n];
    double** z = new double* [n];

    for (int i = 0; i < n; i++) {
        a[i] = new double[n];
        L[i] = new double[n];
        result[i] = new double[n + 1];
        A_1[i] = new double[n];
        z[i] = new double[n];
        b[i] = checkB[i];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = checkA[i][j];//РЕДАГУВАТИ ЦЕй РЯДОК ПІД НЕОБХІДНЕ ЗАВДАННЯ
            L[i][j] = 0;
        }
    }
    Printer(a, n, n, "Matrix A2:");

    for (int i = 0; i < n; i++) {
        L[i][i] = a[i][i];
        for (int k = 0; k < i; k++) {
            L[i][i] -= pow(L[i][k], 2);
        }
        L[i][i] = sqrt(L[i][i]);

        for (int j = i + 1; j < n; j++) {
            L[j][i] = a[j][i];
            for (int k = 0; k < i; k++) {
                L[j][i] -= L[i][k] * L[j][k];;
            }
            L[j][i] /= L[i][i];
        }
    }
    Printer(L, n, n, "Matrix L:");

    //y = Multiply(Inversion(L, n), b, n, n); Ті самі строчки що використовувались для налагодження
    for (int i = 0; i < n; i++) {
        y[i] = b[i] / L[i][i];
        for (int j = i - 1; j >= 0; j--) {
            y[i] -= (L[i][j] * y[j]) / L[i][i];
        }
    }
    
    Printer(y, n, "Matrix Y:");
    //b = Multiply(Inversion(Transposing(L, n, n), n), y, n, n);Ті самі строчки що використовувались для налагодження
    for (int i = n - 1; i >= 0; i--) {
        b[i] = y[i]/L[i][i];
        for (int j = i + 1; j < n; j++) {
            b[i] -= (L[j][i] * b[j])/L[i][i];
        }
    }
    
    Printer(b, n, "Matrix X:");


    
    clock_t end = clock();
    double ms = (double)(end - start);
    cout << ms << " ms." << endl;
    int Cr;
    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sum = 0;
            for (int k = 0; k <= (i - 1); k++)
            {
                sum += L[i][k] * z[k][j];
            }
            if (i == j) Cr = 1; else Cr = 0;
            z[i][j] = (Cr - sum) / L[i][i];
        }
    }

    for (int j = 0; j < n; j++)
    {
        for (int i = n - 1; i >= 0; i--)
        {
            sum = 0;
            for (int k = i + 1; k < n; k++)
            {
                sum += L[k][i] * A_1[k][j];
            }
            A_1[i][j] = (z[i][j] - sum) / L[i][i];
        }
    }
    Printer(A_1, n, n, "Matrix A-1:");
}

void MethodSprint() {
    cout << "Method of sprint\n" << endl;
    clock_t start = clock();

    const int n = 5;
    double** a = new double* [n];
    double* b = new double[n];
    double* delta = new double[n];
    double* liambda = new double[n];
    double* x = new double[n];
    double* t = new double[n];

    double** L = new double* [n];
    double** U = new double* [n];
    double** A_1 = new double* [n];

    for (int i = 0; i < n; i++) {
        a[i] = new double[n];
        U[i] = new double[n];
        L[i] = new double[n];
        A_1[i] = new double[n];
        b[i] = checkB[i];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = checkA[i][j];
        }
    }
    Printer(a, n, n, "Matrix A:");

    t[0] = a[0][0];
    delta[0] = -a[0][1] / t[0];
    liambda[0] = b[0] / t[0];

    for (int i = 1; i < n - 1; i++) {
        t[i] = a[i][i] + a[i][i - 1] * delta[i - 1];
        delta[i] = -a[i][i + 1] / t[i];
        liambda[i] = (b[i] - a[i][i - 1] * liambda[i - 1]) / t[i];
    }
    t[n - 1] = a[n - 1][n - 1] + a[n - 1][n - 2] * delta[n - 2];
    delta[n - 1] = 0;
    liambda[n - 1] = (b[n - 1] - a[n - 1][n - 2] * liambda[n - 2]) / t[n - 1];

    Printer(t, n, "T:");
    Printer(delta, n, "Delta:");
    Printer(liambda, n, "Liambda:");

    x[n - 1] = liambda[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        x[i] = delta[i] * x[i + 1] + liambda[i];
    }
    Printer(x, n, "Vector X:");

    double det = t[0];
    for (int i = 1; i < n; i++) {
        det *= t[i];
    }
    cout << "\nDeterminant: " << det << endl << endl;

  
    
    clock_t end = clock();
    double ms = (double)(end - start);
    cout << ms << " ms." << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j) U[i][j] = 1;
            else U[i][j] = 0;
            L[i][j] = 0;
        }
    }
    for (int i = 0; i < n; i++)
    {
        L[i][i] = t[i];
        L[i][i - 1] = a[i][i - 1];
        U[i][i + 1] = -delta[i];
    }

    Printer(L, n, n, "Matrix L:");
    Printer(U, n, n, "Matrix U:");

    L[n - 1][n - 1] = t[n - 1];
    double sum = 0;
    int i, j;
    for (int p = n - 1; p >= 0; --p)
    {
        int i = p;
        for (int j = i; j >= 0; --j)
        {
            int Cr = 0;
            if (i == j)
            {
                Cr = 1;
            }
            float sum = 0;
            for (int k = j + 1; k < n; ++k)
            {
                sum += A_1[i][k] * L[k][j];
            }
            A_1[i][j] = (Cr - sum) / L[j][j];
        }

        int j = p;
        for (i = j - 1; i >= 0; --i)
        {
            float sum = 0;
            for (int k = i + 1; k < n; ++k)
            {
                sum += U[i][k] * A_1[k][j];
            }

            A_1[i][j] = -sum;
        }
    }
    Printer(A_1, n, n, "Matrix A-1:");
}

int main()
{
   MethodLU_U();
   MethodLU_L();
   MethodCholesky();
    MethodSprint();
}
