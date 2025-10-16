// Реализация класса Pipe

#include "Pipe.h"
#include "Utils.h"

// Конструктор по умолчанию
Pipe::Pipe() : id(0), name(""), length(0.0), diameter(0), in_repair(false) {}

// конструктор со всеми параметрами
Pipe::Pipe(int id_val, const std::string& name_val, double length_val, int diameter_val, bool repair_val)
    : id(id_val), name(name_val), length(length_val), diameter(diameter_val), in_repair(repair_val) {}

int Pipe::getId() const { return id; }
const std::string& Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }      // Реализация геттера
int Pipe::getDiameter() const { return diameter; }    // Реализация геттера
bool Pipe::isInRepair() const { return in_repair; }

void Pipe::setRepairStatus(bool status) { in_repair = status; }

Pipe Pipe::createPipe(int new_id) {
    Pipe p;
    p.id = new_id;
    p.name = getValidInput<std::string>("Введите название трубы: ");
    
    while (true) {
        p.length = getValidInput<double>("Введите длину (км): ");
        if (p.length > 0) break;
        std::cout << "Ошибка: Длина должна быть положительным числом.\n";
    }
    
    while (true) {
        p.diameter = getValidInput<int>("Введите диаметр (мм): ");
        if (p.diameter > 0) break;
        std::cout << "Ошибка: Диаметр должен быть положительным целым числом.\n";
    }

    p.in_repair = false;
    return p;
}

// оператор для вывода в консоль
std::ostream& operator<<(std::ostream& os, const Pipe& p) {
    os << "--- Труба ID: " << p.id << " ---\n"
       << "Название: " << p.name << "\n"
       << "Длина: " << p.length << " км\n"
       << "Диаметр: " << p.diameter << " мм\n"
       << "Статус: " << (p.in_repair ? "В ремонте" : "В эксплуатации") << "\n";
    return os;
}