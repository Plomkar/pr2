#include <iostream>
#include <fstream>
#include <string>
#include "crypto_math.h"
#include "elgamal.h"

using namespace std;

enum class MenuOption {
    Exit = 0,
    Task1 = 1,
    Task2And3 = 2,
    Task4 = 3,
    Task6 = 4
};

int main() {
    // Настройка локализации для корректного отображения русского языка в консоли
    setlocale(LC_ALL, "Russian");

    int choice = -1;

    while (true) {
        cout << "--------------------------------------------------" << endl;
        cout << "1. Задание 1. Возведение в степень по модулю (Ферма и Логарифм)" << endl;
        cout << "2. Задание 2 и 3. Расширенный алгоритм Евклида и инверсия по модулю" << endl;
        cout << "3. Задание 4. Криптосистема Эль-Гамаля (Шифрование файлов)" << endl;
        cout << "4. Задание 6. Цепные дроби" << endl;
        cout << "0. Выход из программы" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "Выберите пункт меню: ";
        
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(32767, '\n');
            cout << "Ошибка ввода! Пожалуйста, введите число." << endl;
            continue;
        }

        // Приведение введенного числа к типу нашего enum class
        MenuOption option = static_cast<MenuOption>(choice);

        if (option == MenuOption::Exit) {
            cout << "Выход из программы. До свидания!" << endl;
            break;
        }

        cout << endl;

        switch (option) {
            case MenuOption::Task1: {
                // ЗАДАНИЕ 1: Возведение в степень
                cout << "ЗАДАНИЕ 1. Возведение в степень по модулю (a^x mod p) <<<" << endl;
                int base, exp, mod;
                
                cout << "Введите основание (a): ";
                cin >> base;
                cout << "Введите показатель степени (x): ";
                cin >> exp;
                
                // Проверка условий теоремы Ферма (модуль должен быть простым)
                while (true) {
                    cout << "Введите простой модуль (p): ";
                    cin >> mod;
                    if (isPrime(mod)) {
                        break;
                    }
                    cout << "Ошибка! Число " << mod << " не является простым. Попробуйте снова." << endl;
                }

                cout << "\n--- Вычисление через теорему Ферма ---" << endl;
                int resFermat = powerFermat(base, exp, mod);
                cout << "Результат: " << base << "^" << exp << " mod " << mod << " = " << resFermat << endl;

                cout << "\n--- Вычисление через двоичное разложение (логарифмический метод) ---" << endl;
                int resLog = powerLog(base, exp, mod);
                cout << "Результат: " << base << "^" << exp << " mod " << mod << " = " << resLog << endl;
                break;
            }

            case MenuOption::Task2And3: {
                // ЗАДАНИЕ 2 и 3: Алгоритм Евклида
                cout << "ЗАДАНИЕ 2 и 3. Расширенный алгоритм Евклида <<<" << endl;
                int c_val, m_val;
                
                cout << "Введите число c: ";
                cin >> c_val;
                cout << "Введите модуль m (числа c и m должны быть взаимно простыми): ";
                cin >> m_val;

                cout << "\nЗапуск расширенного алгоритма Евклида для поиска c^(-1) mod m..." << endl;
                int inv = modInverse(c_val, m_val);
                
                if (inv != -1) {
                    cout << "Успех! Взаимообратное число найдено." << endl;
                    cout << "Результат: " << c_val << "^(-1) mod " << m_val << " = " << inv << endl;
                    cout << "Проверка: (" << c_val << " * " << inv << ") mod " << m_val << " = " << (c_val * inv) % m_val << endl;
                }
                break;
            }

            case MenuOption::Task4: {
                // ЗАДАНИЕ 4: Вариант 3 (Эль-Гамаль)
                cout << "ЗАДАНИЕ 4. Шифрование файлов по схеме Эль-Гамаля <<<" << endl;
                
                // Имена файлов для работы
                string sourceFile = "plaintext.txt";
                string encryptedFile = "encrypted.bin";
                string decryptedFile = "decrypted.txt";

                // Очищаем буфер ввода после cin
                cin.ignore(32767, '\n'); 

                // Шаг 4.1: Создание исходного файла на основе ввода пользователя
                cout << "Введите строку текста, которую мы запишем в файл и зашифруем:\n-> ";
                string userText;
                getline(cin, userText);

                ofstream out(sourceFile);
                out << userText;
                out.close();
                cout << "[Файлы] Текст успешно записан в исходный файл '" << sourceFile << "'" << endl << endl;

                // Шаг 4.2: Ввод параметров шифрования
                int p_el, g_el, x_el, k_el;
                
                cout << "--- Настройка криптосистемы Эль-Гамаля ---" << endl;
                while (true) {
                    cout << "Введите простое число p (модуль, рекомендуемое > 255): ";
                    cin >> p_el;
                    if (isPrime(p_el)) break;
                    cout << "Ошибка! Модуль должен быть простым числом." << endl;
                }
                
                cout << "Введите первообразный корень g (база): ";
                cin >> g_el;
                cout << "Введите ваш секретный ключ x (x < p): ";
                cin >> x_el;
                cout << "Введите случайный сессионный ключ k (взаимно простой с p-1): ";
                cin >> k_el;
                cout << endl;

                // Шаг 4.3: Генерация открытого ключа
                int y_el;
                elgamalKeyGen(p_el, g_el, x_el, y_el);
                cout << endl;

                // Шаг 4.4: Шифрование созданного файла
                cout << "Запуск шифрования файла..." << endl;
                elgamalEncryptFile(sourceFile, encryptedFile, p_el, g_el, y_el, k_el);
                cout << endl;

                // Шаг 4.5: Дешифрование файла
                cout << "Запуск дешифрования файла..." << endl;
                elgamalDecryptFile(encryptedFile, decryptedFile, p_el, x_el);
                cout << endl;

                // Шаг 4.6: Проверка и вывод содержимого восстановленного файла
                ifstream inDec(decryptedFile);
                string decryptedText;
                getline(inDec, decryptedText);
                cout << "[Результат] Считывание из '" << decryptedFile << "':" << endl;
                cout << "Полученный текст: \"" << decryptedText << "\"" << endl;
                break;
            }

            case MenuOption::Task6: {
                // ЗАДАНИЕ 6: Цепные дроби
                cout << "ЗАДАНИЕ 6. Разложение в цепную дробь и проверка уравнения <<<" << endl;
                fraction(1256, 847);
                break;
            }

            default: {
                cout << "Неверный выбор! Пожалуйста, выберите пункт от 0 до 3." << endl;
                break;
            }
        }
    }

    return 0;
}
