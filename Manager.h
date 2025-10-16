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

    void addPipe();
    void addCS();

    void viewAllObjects();
    void deletePipe();
    void deleteCS();

    void findPipes();
    void findCSs();
    
    void batchEditPipes();

    void saveData();
    void loadData();

private:
    // Вспомогательные методы для поиска
    std::vector<int> findPipeIndicesByName(const std::string& name);
    std::vector<int> findPipeIndicesByRepairStatus(bool status);
};

#endif // MANAGER_H