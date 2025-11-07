// #include <windows.h> // Для кодировки в консоли Windows
#include "Manager.h"

int main() {
    // Установка кодировки для корректного отображения кириллицы в Windows
    // SetConsoleOutputCP(65001);
    // SetConsoleCP(65001);

    Manager manager;
    manager.run(); // Запускаем главный цикл, который теперь внутри менеджера

    return 0;
}