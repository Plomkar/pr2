#ifndef ELGAMAL_H
#define ELGAMAL_H

#include <string>

// Генерация ключей Эль-Гамаля
void elgamalKeyGen(int p, int g, int x, int &y);

// Шифрование файла по схеме Эль-Гамаля
void elgamalEncryptFile(const std::string& inputFile, const std::string& outputFile, int p, int g, int y, int k);

// Расшифрование файла по схеме Эль-Гамаля
void elgamalDecryptFile(const std::string& inputFile, const std::string& outputFile, int p, int x);

#endif