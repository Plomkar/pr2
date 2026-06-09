#include "elgamal.h"
#include "crypto_math.h"
#include <iostream>
#include <fstream>

using namespace std;

// Открытый ключ y = g^x mod p
void elgamalKeyGen(int p, int g, int x, int &y) {
    cout << "[Эль-Гамаль] Генерация открытого ключа..." << endl;
    y = powerLog(g, x, p);
    cout << "   Открытый ключ (p=" << p << ", g=" << g << ", y=" << y << ")" << endl;
    cout << "   Закрытый ключ x = " << x << endl;
}

// Шифрование файла. Каждый байт m превращается в пару (a, b), где a = g^k mod p, b = (y^k * m) mod p
void elgamalEncryptFile(const string& inputFile, const string& outputFile, int p, int g, int y, int k) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);

    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка при открытии файлов для шифрования!" << endl;
        return;
    }

    // Вычисляем общую для всех сессионную компоненту 'a'
    int a = powerLog(g, k, p);
    // Вычисляем общий множитель для сообщения: y^k mod p
    int y_k = powerLog(y, k, p);

    cout << "[Шифрование] Сессионный параметр a = " << a << ", общий множитель y^k mod p = " << y_k << endl;

    char byte;
    while (in.get(byte)) {
        int m = (unsigned char)byte;
        int b = (y_k * m) % p;

        // Записываем пару чисел (a, b) в файл как два int
        out.write((char*)&a, sizeof(a));
        out.write((char*)&b, sizeof(b));
    }

    in.close();
    out.close();
    cout << "[Шифрование] Файл успешно зашифрован и сохранен в " << outputFile << endl;
}

// Расшифрование файла. Формула: m = b * (a^x)^(-1) mod p
void elgamalDecryptFile(const string& inputFile, const string& outputFile, int p, int x) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);

    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка при открытии файлов для дешифрования!" << endl;
        return;
    }

    cout << "[Дешифрование] Чтение зашифрованных пар чисел..." << endl;

    int a, b;
    bool firstByte = true;
    int inverse_ax = 0;

    // Читаем попарно структуры int a и int b
    while (in.read((char*)&a, sizeof(a)) && in.read((char*)&b, sizeof(b))) {
        // Так как 'a' и 'x' одинаковы для всего файла, вычисляем инверсию один раз
        if (firstByte) {
            int ax = powerLog(a, x, p); // a^x mod p
            inverse_ax = modInverse(ax, p); // (a^x)^-1 mod p
            cout << "   Вычислено общее значение a^x mod p = " << ax << ", его инверсия = " << inverse_ax << endl;
            firstByte = false;
        }

        int m = (b * inverse_ax) % p;
        char decryptedByte = (char)m;
        out.put(decryptedByte);
    }

    in.close();
    out.close();
    cout << "[Дешифрование] Файл успешно расшифрован и сохранен в " << outputFile << endl;
}