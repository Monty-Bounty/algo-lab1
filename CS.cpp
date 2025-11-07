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

const std::map<int, int>& CS::getOutgoingConnections() const {
    return outgoing_connections;
}

double CS::getUnusedPercent() const {
    if (workshops_total == 0) return 0.0;
    return (double)(workshops_total - workshops_in_operation) / workshops_total * 100.0;
}

// Просто меняю значение, вся логика проверки будет в Manager
void CS::setWorkshopsInOperation(int count) {
    workshops_in_operation = count;
}

void CS::addConnection(int dest_cs_id, int pipe_id) {
    outgoing_connections[dest_cs_id] = pipe_id;
}

void CS::removeConnection(int dest_cs_id) {
    outgoing_connections.erase(dest_cs_id);
}

// Теперь это просто cs, которая вызывает конструктор
CS CS::createCS(int new_id, const std::string& name, int workshops_total, int workshops_in_operation) {
    return CS(new_id, name, workshops_total, workshops_in_operation);
}

// оператор для вывода в консоль
std::ostream& operator<<(std::ostream& os, const CS& cs) {
    os << "--- КС ID: " << cs.id << " ---\n"
       << "Название: " << cs.name << "\n"
       << "Цеха (в работе/всего): " << cs.workshops_in_operation << "/" << cs.workshops_total << "\n"
       << "Процент незадействованных цехов: " << cs.getUnusedPercent() << "%\n";
    if (!cs.outgoing_connections.empty()) {
        os << "Соединения:\n";
        for (const auto& pair : cs.outgoing_connections) {
            os << "  -> КС ID: " << pair.first << " (через трубу ID: " << pair.second << ")\n";
        }
    }
    return os;
}