// Заголовочный файл для класса Manager, который управляет объектами

#ifndef MANAGER_H
#define MANAGER_H

#include "Pipe.h"
#include "CS.h"
#include "Utils.h"
#include <vector>
#include <map>

class Manager {
private:
    std::map<int, Pipe> pipes;
    std::map<int, CS> stations;
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

    void connect_stations();
    void topological_sort();
    void calculateMaxFlow();
    void findShortestPath();

private:
    double calculateCapacity(const Pipe& p);
    void printMenu();
    bool bfs_for_max_flow(const std::map<int, std::map<int, double>>& residual_graph, int s, int t, std::map<int, int>& parent);
    // Вспомогательные методы для поиска, возвращают ID объектов
    std::vector<int> findPipeIdsByName(const std::string& name);
    std::vector<int> findPipeIdsByRepairStatus(bool status);
    std::vector<int> findCSIdsByName(const std::string& name);
    std::vector<int> findCSIdsByUnusedPercent(double percent);

    // Новые методы для пакетной обработки
    void processPipesPackage(const std::vector<int>& ids);
    void processCsPackage(const std::vector<int>& ids);
    
    // Метод для редактирования КС
    void editCS(int id);

    // Новый приватный метод для выбора ID из списка
    std::vector<int> getIdsFromUser(const std::vector<int>& available_ids);
};

#endif // MANAGER_H