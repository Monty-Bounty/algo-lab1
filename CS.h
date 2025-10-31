// Заголовочный файл для класса Компрессорная Станция

#ifndef CS_H
#define CS_H

#include <iostream>
#include <string>
#include <map>

class CS {
private:
    int id;
    std::string name;
    int workshops_total;
    int workshops_in_operation;
    std::map<int, int> outgoing_connections; // key: destination CS id, value: pipe id

public:
    CS();
    // КОНСТРУКТОР: для удобной загрузки данных из файла
    CS(int id, const std::string& name, int workshops_total, int workshops_in_operation);

    int getId() const;
    const std::string& getName() const;
    int getWorkshopsTotal() const;      // геттер
    int getWorkshopsInOperation() const;// геттер
    double getUnusedPercent() const;
    const std::map<int, int>& getOutgoingConnections() const; // Геттер для соединений

    void setWorkshopsInOperation(int count); // Сеттер для редактирования
    void addConnection(int dest_cs_id, int pipe_id); // Метод для добавления соединения
    void removeConnection(int dest_cs_id);

    friend std::ostream& operator<<(std::ostream& os, const CS& cs);
    
    static CS createCS(int new_id, const std::string& name, int workshops_total, int workshops_in_operation);
};

#endif // CS_H