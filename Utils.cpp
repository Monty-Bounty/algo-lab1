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

void Logger::log(const std::string& message) {
    if (logfile.is_open()) {
        // Получаем текущее время
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        // Записываем время и сообщение
        logfile << std::put_time(std::localtime(&time_t_now), "%d-%m-%Y %H:%M:%S")
                << " - " << message << std::endl;
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