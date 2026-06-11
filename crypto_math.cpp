#include "crypto_math.h"
#include <iostream>

using namespace std;

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Реализация по теореме Ферма: a^(p-1) == 1 (mod p)
int powerFermat(int base, int exp, int mod) {
    if (!isPrime(mod)) {
        cout << "[Ферма] Ошибка: модуль " << mod << " не является простым числом!" << endl;
        return -1;
    }
    if (base % mod == 0) return 0;

    // Применяем свойства сравнений и оптимизируем степень по теореме Ферма
    int reducedExp = exp % (mod - 1);
    cout << "[Ферма] Проверка пройдена. Оптимизируем степень: " << exp << " mod (" << mod << "-1) = " << reducedExp << endl;

    // Далее считаем обычным циклическим перемножением
    int res = 1;
    base = base % mod;
    for (int i = 0; i < reducedExp; i++) {
        res = (res * base) % mod;
    }
    return res;
}

// Реализация через двоичное разложение степени (быстрое возведение)
int powerLog(int base, int exp, int mod) {
    int res = 1;
    base = base % mod;
    cout << "[Лог] Пошаговое возведение в степень (Binary Exponentiation):" << endl;
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = (res * base) % mod;
            cout << "   Степень нечетная: умножаем результат на base, res = " << res << endl;
        }
        base = (base * base) % mod;
        cout << "   Квадрируем base: новое значение base = " << base << endl;
        exp /= 2;
    }
    return res;
}

// Расширенный алгоритм Евклида в табличной логике
int extendedgcd(int a, int b, int &u, int &v) {
    int x1 = 1, x2 = 0, x3 = a;
    int y1 = 0, y2 = 1, y3 = b;

    cout << "[Евклид] Таблица промежуточных вычислений:" << endl;
    cout << "   X: (" << x3 << ", " << x1 << ", " << x2 << ")" << endl;
    cout << "   Y: (" << y3 << ", " << y1 << ", " << y2 << ")" << endl;

    while (y3 != 0) {
        int q = x3 / y3;
        int t3 = x3 - q * y3;
        int t1 = x1 - q * y1;
        int t2 = x2 - q * y2;

        x1 = y1; x2 = y2; x3 = y3;
        y1 = t1; y2 = t2; y3 = t3;

        cout << "   q = " << q << " -> X: (" << x3 << ", " << x1 << ", " << x2 << ")" << endl;
    }
    u = x1;
    v = x2;
    return x3; // Возвращаем НОД
}

// Поиск c^(-1) mod m
int modInverse(int c, int m) {
    int u, v;
    int gcd = extendedgcd(m, c, u, v); // Передаем m как 'a' и c как 'b'
    if (gcd != 1) {
        cout << "[Ошибка] Взаимообратного числа не существует, так как НОД(" << c << "," << m << ") != 1" << endl;
        return -1;
    }
    // Значение коэффициента при 'b' (то есть при c) — это v.
    // Если оно отрицательное, прибавляем модуль.
    if (v < 0) {
        cout << "   Корректируем отрицательное значение: " << v << " + " << m << " = " << v + m << endl;
        v = v + m;
    }
    return v;
}
