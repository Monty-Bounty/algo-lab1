// Главный файл программы с меню

#include <iostream>
#include <windows.h> // Для кодировки в консоли Windows
#include "Manager.h"
#include "Utils.h"

void printMenu() {
    std::cout << "\n========= МЕНЮ =========\n"
              << "1. Добавить трубу\n"
              << "2. Добавить КС\n"
              << "3. Просмотр всех объектов\n"
              << "4. Удалить трубу\n"
              << "5. Удалить КС\n"
              << "6. Поиск труб\n"
              << "7. Поиск КС\n"
              << "8. Пакетное редактирование труб\n"
              << "9. Сохранить данные\n"
              << "10. Загрузить данные\n"
              << "0. Выход\n";
}

int main() {
    // Установка кодировки для корректного отображения кириллицы в Windows
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Manager manager;
    
    while (true) {
        printMenu();
        int choice = getValidInput<int>("Выберите действие: ");

        switch (choice) {
            case 1: manager.addPipe(); break;
            case 2: manager.addCS(); break;
            case 3: manager.viewAllObjects(); break;
            case 4: manager.deletePipe(); break;
            case 5: manager.deleteCS(); break;
            case 6: manager.findPipes(); break;
            case 7: manager.findCSs(); break;
            case 8: manager.batchEditPipes(); break;
            case 9: manager.saveData(); break;
            case 10: manager.loadData(); break;
            case 0:
                std::cout << "Выход из программы.\n";
                return 0;
            default:
                std::cout << "Ошибка: Неверный пункт меню.\n";
                break;
        }
    }

    return 0;
}