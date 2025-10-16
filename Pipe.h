// Заголовочный файл для класса Pipe

#ifndef PIPE_H
#define PIPE_H

#include <iostream>
#include <string>

class Pipe {
private:
    int id;
    std::string name;
    double length;
    int diameter;
    bool in_repair;

public:
    Pipe();
    // НОВЫЙ КОНСТРУКТОР: для удобной загрузки данных из файла
    Pipe(int id, const std::string& name, double length, int diameter, bool in_repair);


    // Геттеры (методы для получения значений полей)
    // Сеттеры (методы для установки значений полей)
    int getId() const;
    const std::string& getName() const;
    double getLength() const;
    int getDiameter() const;
    bool isInRepair() const;

    void setRepairStatus(bool status);
    
    // Этот оператор теперь используется ТОЛЬКО для вывода в консоль
    friend std::ostream& operator<<(std::ostream& os, const Pipe& p);

    static Pipe createPipe(int new_id);
};

#endif // PIPE_H