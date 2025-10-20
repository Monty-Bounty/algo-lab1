#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <sstream> // Необходимо для std::stringstream

// --- Логгер для записи действий пользователя ---
class Logger {
public:
    Logger(const std::string& filename);
    void log(const std::string& input);

private:
    std::ofstream logfile;
};

// --- Шаблонная функция для безопасного ввода данных любого типа ---
template<typename T>
T getValidInput(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line); // 1. Считываем ВСЮ строку, а не часть

        std::stringstream ss(line); // 2. Помещаем строку в потоковый анализатор
        T value;
        char remaining_char; // Переменная для проверки "хвоста" строки

        // 3. Пытаемся извлечь значение И проверяем, что после него в строке ничего не осталось
        // Это не позволит ввести "99 op" или "1.2.3"
        if ((ss >> value) && !(ss >> remaining_char)) {
            return value; // Если всё чисто, возвращаем значение
        }
        
        std::cout << "Ошибка: Неверный формат ввода. Пожалуйста, введите только число.\n";
    }
}

// Специализация шаблона для типа string (остается без изменений)
template<>
std::string getValidInput<std::string>(const std::string& prompt);

bool isValidDiameter(int diameter);

#endif // UTILS_H