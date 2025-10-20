// Заголовочный файл для класса Компрессорная Станция

#ifndef CS_H
#define CS_H

#include <iostream>
#include <string>

class CS {
private:
    int id;
    std::string name;
    int workshops_total;
    int workshops_in_operation;

public:
    CS();
    // КОНСТРУКТОР: для удобной загрузки данных из файла
    CS(int id, const std::string& name, int workshops_total, int workshops_in_operation);

    int getId() const;
    const std::string& getName() const;
    int getWorkshopsTotal() const;      // геттер
    int getWorkshopsInOperation() const;// геттер
    double getUnusedPercent() const;

    void setWorkshopsInOperation(int count); // Сеттер для редактирования

    friend std::ostream& operator<<(std::ostream& os, const CS& cs);
    
    static CS createCS(int new_id, const std::string& name, int workshops_total, int workshops_in_operation);
};

#endif // CS_H