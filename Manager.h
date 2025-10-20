// Заголовочный файл для класса Manager, который управляет объектами

#ifndef MANAGER_H
#define MANAGER_H

#include "Pipe.h"
#include "CS.h"
#include "Utils.h"
#include <vector>

class Manager {
private:
    std::vector<Pipe> pipes;
    std::vector<CS> stations;
    int next_pipe_id;
    int next_cs_id;
    Logger logger; // Объект логгера

public:
    Manager();
    void run(); // Главный цикл программы будет здесь
    void log(const std::string& input); // Публичный метод для логирования

    void addPipe();
    void addCS();
    void viewAllObjects();
    void workWithPackages(); // Новый единый метод для работы с объектами
    void saveData();
    void loadData();

private:
    // Вспомогательные методы для поиска
    std::vector<int> findPipeIndicesByName(const std::string& name);
    std::vector<int> findPipeIndicesByRepairStatus(bool status);
    std::vector<int> findCSIndicesByName(const std::string& name);
    std::vector<int> findCSIndicesByUnusedPercent(double percent);

    // Новые методы для пакетной обработки
    void processPipesPackage(const std::vector<int>& indices);
    void processCsPackage(const std::vector<int>& indices);
    
    // Метод для редактирования КС, как и просили
    void editCS(int index);

    // Шаблонный метод для выбора конкретных объектов из найденных
    template<typename T>
    std::vector<int> getObjectIndices(const std::vector<T>& objects, const std::vector<int>& found_indices);
};

#endif // MANAGER_H