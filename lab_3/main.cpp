#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <fstream>
//#include "time.h"

using namespace std;

int **create_matrix(int, int);

string generateRandomString(int len);

//void print_matrix(int **matrix, int rows, int cols);

int damerauLevenshteinAlgoritm(string, string, int, int);

int levenshteinAlgoritmRec(string, string, int, int, int **);

int levenshteinAlgoritm(string, string, int, int);

int levenshteinAlgoritmRecCache(string, string, int, int);

int minimum(string, string, int, int, int **matrix);

void firstMode();

void secondMode();

int main() {
    int choice;
    cout << "Выберите режим работы программы:" << endl;
    cout << "1) Поиск расстояния Левенштейна 4-мя разными алгоритмами" << endl;
    cout << "2) Замер процессорного времени при поиску расстояния Левенштейна 4-мя разными алгоритмами" << endl;
    cin >> choice;

    switch (choice) {
        case 1:
            firstMode();
            break;
        case 2:
            secondMode();
            break;
        default:
            cout << "Ошибка при выборе режима!" << endl;
    }
    return 0;
}

int levenshteinAlgoritm(string firstString, string secondString, int len2, int len1) {
    if (len2 == 0)
        return len1;
    if (len1 == 0)
        return len2;
    int cols = len1 + 1, rows = len2 + 1;
    int **matrix = create_matrix(rows, cols);
    for (int i = 0; i < rows; i++)
        matrix[i][0] = i;
    for (int j = 0; j < cols; j++)
        matrix[0][j] = j;
    for (int i = 1; i < rows; i++)
        for (int j = 1; j < cols; j++) {
            int diff = (firstString[j - 1] == secondString[i - 1]) ? 0 : 1;
            matrix[i][j] = min({matrix[i][j - 1] + 1, matrix[i - 1][j] + 1, matrix[i - 1][j - 1] + diff});
        }
    int res = matrix[len2][len1];
    delete[] matrix;
    return res;
}

int levenshteinAlgoritmRec(string firstString, string secondString, int len2, int len1, int **matrix) {
    if (matrix == NULL || matrix[len2][len1] == -1) {
        if (len2 == 0)
            return len1;
        if (len1 == 0)
            return len2;
        if (matrix != NULL) {
            matrix[len2][len1] = minimum(firstString, secondString, len2, len1, matrix);
            return matrix[len2][len1];
        } else
            return minimum(firstString, secondString, len2, len1, matrix);
    } else
        return matrix[len2][len1];
}

int levenshteinAlgoritmRecCache(string firstString, string secondString, int len2, int len1) {
    int **matrix = create_matrix(len2 + 1, len1 + 1);
    if (len2 == 0)
        return len1;
    if (len1 == 0)
        return len2;
    return minimum(firstString, secondString, len2, len1, matrix);
}

int minimum(string firstString, string secondString, int len2, int len1, int **matrix) {
    int diff = (firstString[len1 - 1] == secondString[len2 - 1]) ? 0 : 1;
    return min({levenshteinAlgoritmRec(firstString, secondString, len1 - 1, len2, matrix) + 1,
                levenshteinAlgoritmRec(firstString, secondString, len1, len2 - 1, matrix) + 1,
                levenshteinAlgoritmRec(firstString, secondString, len1 - 1, len2 - 1, matrix) + diff});
}

int damerauLevenshteinAlgoritm(string firstString, string secondString, int len2, int len1) {
    if (len2 == 0)
        return len1;
    if (len1 == 0)
        return len2;
    int cols = len1 + 1, rows = len2 + 1;
    int **matrix = create_matrix(rows, cols);
    for (int i = 0; i < rows; i++)
        matrix[i][0] = i;
    for (int j = 0; j < cols; j++)
        matrix[0][j] = j;
    for (int i = 1; i < rows; i++)
        for (int j = 1; j < cols; j++) {
            int diff = (firstString[j - 1] == secondString[i - 1]) ? 0 : 1;
            matrix[i][j] = min({matrix[i][j - 1] + 1, matrix[i - 1][j] + 1, matrix[i - 1][j - 1] + diff});
            if (i > 1 && j > 1 && firstString[i - 1] == secondString[j - 2] &&
                firstString[i - 2] == secondString[j - 1])
                matrix[i][j] = min(matrix[i][j], matrix[i - 2][j - 2] + 1);
        }
    int res = matrix[len2][len1];
    delete[] matrix;
    return res;
}

int **create_matrix(int rows, int cols) {
    int **matrix = new int *[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; j++)
            matrix[i][j] = -1;
    }
    return matrix;
}

void firstMode() {
    string firstString, secondString;
    cout << "Введите первое слово: ";
    cin >> firstString;
    cout << "Введите второе слово: ";
    cin >> secondString;

    int len1 = firstString.length();
    int len2 = secondString.length();

    int levenshteinResult = levenshteinAlgoritm(firstString, secondString, len2, len1);
    cout << "Результат поиска расстояния Левенщтейна через цикл: " << levenshteinResult << endl;
    int recResult = levenshteinAlgoritmRec(firstString, secondString, len2, len1, NULL);
    cout << "Результат поиска расстояния Левенщтейна через рекурсию: " << recResult << endl;
    int recCacheResult = levenshteinAlgoritmRecCache(firstString, secondString, len2, len1);
    cout << "Результат поиска расстояния Левенщтейна через рекурсию с кэшом: " << recCacheResult << endl;
    int damerauLevenshteinResult = damerauLevenshteinAlgoritm(firstString, secondString, len2, len1);
    cout << "Результат поиска расстояния методом Дамерау-Левенштейна: " << damerauLevenshteinResult << endl;
}

void secondMode() {
    clock_t t;
    int cls = 10;
    int n = 5;
    const char *word[n * 2];
    int len = 2;


    ofstream levenshtein;
    levenshtein.open("levenshtein.txt");
    if (!levenshtein.is_open()) {
        perror("Ошибка открытия файла levenshtein.txt!");
        exit(1);
    }

    ofstream levenshteinRec;
    levenshteinRec.open("levenshtein_rec.txt");
    if (!levenshteinRec.is_open()) {
        perror("Ошибка открытия файла levenshtein_rec.txt!");
        exit(1);
    }

    ofstream levenshteinRecCache;
    levenshteinRecCache.open("levenshtein_rec_cache.txt");
    if (!levenshteinRecCache.is_open()) {
        perror("Ошибка открытия файла levenshtein_rec_cache.txt!");
        exit(1);
    }

    ofstream damerauLevenshtein;
    damerauLevenshtein.open("damerau_levenshtein.txt");
    if (!damerauLevenshtein.is_open()) {
        perror("Ошибка открытия файла damerau_levenshtein.txt!");
        exit(1);
    }

    for (int i = 0; i < n; ++i) {
        double lev_time = 0.0, rec_time = 0.0, rec_cache_time = 0.0, damerau_time = 0.0;
        for (int j = 0; j < cls; j++) {
            string str1 = generateRandomString(len);
            string str2 = generateRandomString(len);

            word[i * 2] = str1.c_str();
            word[i * 2 + 1] = str2.c_str();

            cout << i + 1 << ' ' << str1 << ' ' << str2 << endl;

            t = clock();
            levenshteinAlgoritm(word[2 * i], word[2 * i + 1], 2 * (i + 1), 2 * (i + 1));
            t = clock() - t;
            lev_time += (double) t;

            t = clock();
            levenshteinAlgoritmRec(word[2 * i], word[2 * i + 1], 2 * (i + 1), 2 * (i + 1), NULL);
            t = clock() - t;
            rec_time += (double) t;

            t = clock();
            levenshteinAlgoritmRecCache(word[2 * i], word[2 * i + 1], 2 * (i + 1), 2 * (i + 1));
            t = clock() - t;
            rec_cache_time += (double) t;

            t = clock();
            damerauLevenshteinAlgoritm(word[2 * i], word[2 * i + 1], 2 * (i + 1), 2 * (i + 1));
            t = clock() - t;
            damerau_time += (double) t;
        }
        if (levenshtein.is_open())
            levenshtein << lev_time / CLOCKS_PER_SEC / cls * 1000 << endl;
        if (levenshteinRec.is_open())
            levenshteinRec << rec_time / CLOCKS_PER_SEC / cls * 1000 << endl;
        if (levenshteinRecCache.is_open())
            levenshteinRecCache << rec_cache_time / CLOCKS_PER_SEC / cls * 1000 << endl;
        if (damerauLevenshtein.is_open())
            damerauLevenshtein << damerau_time / CLOCKS_PER_SEC / cls * 1000 << endl;
        len += 2;
    }
    levenshtein.close();
    levenshteinRec.close();
    levenshteinRecCache.close();
    damerauLevenshtein.close();
    cout << "Файл levenshtein.txt успешно записан!" << endl;
    cout << "Файл levenshtein_rec.txt успешно записан!" << endl;
    cout << "Файл levenshtein_rec_cache.txt успешно записан!" << endl;
    cout << "Файл damerau_levenshtein.txt успешно записан!" << endl;
}

string generateRandomString(int len) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(33, 126);

    string randomString;
    for (int i = 0; i < len; i++) {
        char c = static_cast<char>(dis(gen));
        randomString += c;
    }
    return randomString;
}

//void print_matrix(int **matrix, int rows, int cols) {
//    for (int i = 0; i < rows + 1; i++) {
//        for (int j = 0; j < cols + 1; j++)
//            cout << matrix[i][j] << " ";
//        cout << endl;
//    }
//}
