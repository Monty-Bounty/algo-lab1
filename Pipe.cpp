// Реализация класса Pipe

#include "Pipe.h"
#include "Utils.h"

// Конструктор по умолчанию
Pipe::Pipe() : id(0), name(""), length(0.0), diameter(0), in_repair(false), is_used(false) {}

// конструктор со всеми параметрами
Pipe::Pipe(int id_val, const std::string& name_val, double length_val, int diameter_val, bool repair_val, bool is_used_val)
    : id(id_val), name(name_val), length(length_val), diameter(diameter_val), in_repair(repair_val), is_used(is_used_val) {}

int Pipe::getId() const { return id; }
const std::string& Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }      // Реализация геттера
int Pipe::getDiameter() const { return diameter; }    // Реализация геттера
bool Pipe::isInRepair() const { return in_repair; }
bool Pipe::isUsed() const { return is_used; }

void Pipe::setRepairStatus(bool status) { in_repair = status; }
void Pipe::setUsed(bool used) { is_used = used; }

// Теперь это просто фабрика, которая вызывает конструктор
Pipe Pipe::createPipe(int new_id, const std::string& name, double length, int diameter) {
    return Pipe(new_id, name, length, diameter, false, false); // Новая труба всегда в эксплуатации и свободна
}

// оператор для вывода в консоль
std::ostream& operator<<(std::ostream& os, const Pipe& p) {
    os << "--- Труба ID: " << p.id << " ---\n"
       << "Название: " << p.name << "\n"
       << "Длина: " << p.length << " км\n"
       << "Диаметр: " << p.diameter << " мм\n"
       << "Статус: " << (p.in_repair ? "В ремонте" : "В эксплуатации") << "\n"
       << "Состояние: " << (p.is_used ? "Используется" : "Свободна") << "\n";
    return os;
}