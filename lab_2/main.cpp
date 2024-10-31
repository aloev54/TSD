#include <iostream>
#include "math.h"

using namespace std;

double func(double);

double leftRectangleMethod(double, double, int);

double rightRectangleMethod(double, double, int);

double trapezoidMethod(double, double, int);

double accuracyLeftRectangleMethod(double, double, int, int, double);

double accuracyRightRectangleMethod(double, double, int, int, double);

double accuracyTrapezoidMethod(double, double, int, int, double);

void firstMode(double, double, int);

void secondMode(double, double, int);

//double allowMaxQuantity(int, int);


int main() {
    double xMin, xMax;
    int quantity, mode;

    cout << "Введите xMin: ";
    cin >> xMin;
    cout << "Введите xMax: ";
    cin >> xMax;
    if (xMin > xMax) {
        perror("xMin должно быть меньше xMax");
        exit(1);
    }
    cout << "Введите кол-во разбиений: ";
    cin >> quantity;

    cout << "Введите режим работы программы:" << endl;
    cout << "1)Вычисление интегралла ф-ии 3-мя методами без заданной прогрешности" << endl;
    cout << "2)Вычисление интегралла ф-ии 3-мя методами с заданной прогрешностью" << endl;
    cin >> mode;

    switch (mode) {
        case 1:
            firstMode(xMin, xMax, quantity);
            break;
        case 2:
            secondMode(xMin, xMax, quantity);
            break;
        default:
            cout << "Режим с номером " << mode << " не существует";
    }

    return 0;
}

double func(double x) {
    return x * x - 2 * x + 3;
}

double leftRectangleMethod(double xMin, double xMax, int quantity) {
    double square = 0;
    double h = (xMax - xMin) / quantity;
    for (int i = 0; i < quantity; i++)
        square += fabs(func(xMin + h * i) * h);
    double hLast = (xMax - xMin) - (quantity - 1) * h;
    double xLast = xMin + (quantity - 1) * h;
    square += fabs(func(xLast) * hLast);
    return square;
}

double rightRectangleMethod(double xMin, double xMax, int quantity) {
    double square = 0;
    double h = (xMax - xMin) / quantity;
    for (int i = 1; i < quantity + 1; i++)
        square += fabs(func(xMin + h * i) * h);
    double hLast = (xMax - xMin) - (quantity - 1) * h;
    double xLast = xMax;
    square += fabs(func(xLast) * hLast);
    return square;
}

double trapezoidMethod(double xMin, double xMax, int quantity) {
    double square = 0;
    double h = (xMax - xMin) / quantity;
    for (int i = 0; i < quantity; i++)
        square += fabs((func(xMin + h * i) + func(xMin + h * (i + 1))) / 2 * h);
    double hLast = (xMax - xMin) - (quantity - 1) * h;
    double xLast = xMin + (quantity - 1) * h;
    square += fabs((func(xLast) + func(xMax)) / 2 * hLast);
    return square;
}

double accuracyLeftRectangleMethod(double xMin, double xMax, int quantity, int allowQuantity, double epsilon) {
    double i1, i2;
    i2 = leftRectangleMethod(xMin, xMax, quantity);
    do {
        i1 = i2;
        quantity *= 2;
        i2 = leftRectangleMethod(xMin, xMax, quantity);
        if (allowQuantity < quantity) {
            cout << "Достигнут разрешенный максимум с точностью " << fabs(i1 - i2) << endl;
            return i2;
        }
    } while (fabs(i1 - i2) > epsilon);
    return i2;
}

double accuracyRightRectangleMethod(double xMin, double xMax, int quantity, int allowQuantity, double epsilon) {
    double i1, i2;
    i2 = rightRectangleMethod(xMin, xMax, quantity);
    do {
        i1 = i2;
        quantity *= 2;
        i2 = rightRectangleMethod(xMin, xMax, quantity);
        if (allowQuantity < quantity) {
            cout << "Достигнут разрешенный максимум с точностью " << fabs(i1 - i2) << endl;
            return i2;
        }
    } while (fabs(i1 - i2) > epsilon);
    return i2;
}

double accuracyTrapezoidMethod(double xMin, double xMax, int quantity, int allowQuantity, double epsilon) {
    double i1, i2;
    i2 = trapezoidMethod(xMin, xMax, quantity);
    do {
        i1 = i2;
        quantity *= 2;
        i2 = trapezoidMethod(xMin, xMax, quantity);
        if (allowQuantity < quantity) {
            cout << "Достигнут разрешенный максимум с точностью " << fabs(i1 - i2) << endl;
            return i2;
        }
    } while (fabs(i1 - i2) > epsilon);
    return i2;
}

void firstMode(double xMin, double xMax, int quantity) {
    double leftRectangleSquare = leftRectangleMethod(xMin, xMax, quantity);
    cout << "Результат интегрирования методом левых прямоугольников: " << leftRectangleSquare << endl;
    double rightRectangleSquare = rightRectangleMethod(xMin, xMax, quantity);
    cout << "Результат интегрирования методом правых прямоугольников: " << rightRectangleSquare << endl;
    double trapezoidSquare = trapezoidMethod(xMin, xMax, quantity);
    cout << "Результат интегрирования методом трапеций: " << trapezoidSquare << endl;
}

void secondMode(double xMin, double xMax, int quantity) {
    int allowQuantity;
    double epsilon;
    cout << "Введите макс кол-во разбиений: ";
    cin >> allowQuantity;
    if (allowQuantity < quantity) {
        perror("Максимальное кол-во разбиений д.б. больше заданного");
        exit(1);
    }
    cout << "Введите погрешность: ";
    cin >> epsilon;

    double leftRectangleSquare = accuracyLeftRectangleMethod(xMin, xMax, quantity, allowQuantity, epsilon);
    cout << "Результат интегрирования методом левых прямоугольников с точностью: " << leftRectangleSquare << endl;
    double rightRectangleSquare = accuracyRightRectangleMethod(xMin, xMax, quantity, allowQuantity, epsilon);
    cout << "Результат интегрирования методом правых прямоугольников с точностью: " << rightRectangleSquare << endl;
    double trapezoidSquare = accuracyTrapezoidMethod(xMin, xMax, quantity, allowQuantity, epsilon);
    cout << "Результат интегрирования методом трапеций с точностью: " << trapezoidSquare << endl;
}

//double allowMaxQuantity(int quantity, int allowQuantity) {
//}