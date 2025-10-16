// Реализация класса CS

#include "CS.h"
#include "Utils.h"

// Конструктор по умолчанию
CS::CS() : id(0), name(""), workshops_total(0), workshops_in_operation(0) {}

// РЕАЛИЗАЦИЯ: конструктор со всеми параметрами
CS::CS(int id_val, const std::string& name_val, int total_val, int in_op_val)
    : id(id_val), name(name_val), workshops_total(total_val), workshops_in_operation(in_op_val) {}

int CS::getId() const { return id; }
const std::string& CS::getName() const { return name; }
int CS::getWorkshopsTotal() const { return workshops_total; }          // Реализация геттера
int CS::getWorkshopsInOperation() const { return workshops_in_operation; } // Реализация геттера

double CS::getUnusedPercent() const {
    if (workshops_total == 0) return 0.0;
    return (double)(workshops_total - workshops_in_operation) / workshops_total * 100.0;
}

CS CS::createCS(int new_id) {
    CS cs;
    cs.id = new_id;
    cs.name = getValidInput<std::string>("Введите название КС: ");
    
    while (true) {
        cs.workshops_total = getValidInput<int>("Введите общее количество цехов: ");
        if (cs.workshops_total > 0) break;
        std::cout << "Ошибка: общее число цехов должно быть больше нуля.\n";
    }
    
    while (true) {
        cs.workshops_in_operation = getValidInput<int>("Введите количество цехов в работе: ");
        if (cs.workshops_in_operation >= 0 && cs.workshops_in_operation <= cs.workshops_total) break;
        std::cout << "Ошибка: число работающих цехов не может быть отрицательным или больше общего числа.\n";
    }
    return cs;
}

// оператор для вывода в консоль
std::ostream& operator<<(std::ostream& os, const CS& cs) {
    os << "--- КС ID: " << cs.id << " ---\n"
       << "Название: " << cs.name << "\n"
       << "Цеха (в работе/всего): " << cs.workshops_in_operation << "/" << cs.workshops_total << "\n"
       << "Процент незадействованных цехов: " << cs.getUnusedPercent() << "%\n";
    return os;
}