// Реализация вспомогательных утилит

#include "Utils.h"
#include <chrono>
#include <iomanip>

Logger::Logger(const std::string& filename) {
    // Открываем файл в режиме дозаписи (ios::app)
    logfile.open(filename, std::ios::app);
    if (!logfile.is_open()) {
        std::cerr << "Критическая ошибка: Не удалось открыть файл лога " << filename << std::endl;
    }
}

void Logger::log(const std::string& input) {
    if (logfile.is_open()) {
        // Просто пишем в файл то, что получили
        logfile << input << std::endl;
    }
}

// Реализация специализации для string
template<>
std::string getValidInput<std::string>(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (!value.empty()) {
            return value;
        }
        std::cout << "Ошибка: Ввод не может быть пустым.\n";
    }
}