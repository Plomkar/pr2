#ifndef CRYPTO_MATH_H
#define CRYPTO_MATH_H

// Проверка числа на простоту (для проверки модуля)
bool isPrime(int n);

// Задание 1. Возведение в степень по модулю через теорему Ферма
int powerFermat(int base, int exp, int mod);

// Задание 1. Возведение в степень через двоичное разложение (логарифмический метод)
int powerLog(int base, int exp, int mod);

// Задание 2 и 3. Расширенный алгоритм Евклида
// Возвращает НОД, а через ссылки u и v передает коэффициенты Безу
int extendedgcd(int a, int b, int &u, int &v);

// Нахождение взаимообратного числа c^(-1) mod m
int modInverse(int c, int m);

#endif