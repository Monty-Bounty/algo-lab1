#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// #include <windows.h> // Добавлено для работы с консолью Windows
using namespace std;

// Вспомогательные функции для проверки ввода
string getValidString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) return input;
        cout << "Ошибка: строка не может быть пустой\n";
    }
}

double getValidDouble(const string& prompt) {
    string input;
    double value;
    while (true) {
        cout << prompt;
        getline(cin, input);
        try {
            value = stod(input);
            if (value > 0) return value;
            cout << "Ошибка: число должно быть положительным\n";
        }
        catch (...) {
            cout << "Ошибка: введите корректное число\n";
        }
    }
}

int getValidInt(const string& prompt, int min = 0, int max = 1000000) {
    string input;
    int value;
    while (true) {
        cout << prompt;
        getline(cin, input);
        try {
            value = stoi(input);
            if (value >= min && value <= max) return value;
            cout << "Ошибка: число должно быть от " << min << " до " << max << "\n";
        }
        catch (...) {
            cout << "Ошибка: введите корректное целое число\n";
        }
    }
}

class Pipe {
private:
    int id;
    string name;
    double length;
    int diameter;
    bool in_repair;

public:
    Pipe() : id(0), length(0), diameter(0), in_repair(false) {}

    void setData(int newId, string newName, double newLength, int newDiameter) {
        id = newId;
        name = newName;
        length = newLength;
        diameter = newDiameter;
    }
    void setRepair(bool repair) { in_repair = repair; }
    
    int getId() { return id; }
    string getName() { return name; }
    bool getRepair() { return in_repair; }

    void display() {
        cout << "\nТруба #" << id << "\n"
             << "Название: " << name << "\n"
             << "Длина: " << length << " км\n"
             << "Диаметр: " << diameter << " мм\n"
             << "Статус: " << (in_repair ? "В ремонте" : "В эксплуатации") << "\n";
    }
};

class CS {
private:
    int id;
    string name;
    int workshops_total;
    int workshops_in_operation;
    string efficiency_class;

public:
    CS() : id(0), workshops_total(0), workshops_in_operation(0) {}

    void setData(int newId, string newName, int total, int working, string eff_class) {
        id = newId;
        name = newName;
        workshops_total = total;
        workshops_in_operation = working;
        efficiency_class = eff_class;
    }

    int getId() { return id; }
    string getName() { return name; }
    int getUnusedWorkshopsPercent() {
        return ((workshops_total - workshops_in_operation) * 100) / workshops_total;
    }

    void display() {
        cout << "\nКС #" << id << "\n"
             << "Название: " << name << "\n"
             << "Цеха: " << workshops_in_operation << "/" << workshops_total << "\n"
             << "Класс: " << efficiency_class << "\n";
    }
};

class Manager {
private:
    vector<Pipe> pipes;
    vector<CS> cs_list;
    int next_pipe_id;
    int next_cs_id;

public:
    Manager() : next_pipe_id(0), next_cs_id(0) {}

    void addPipe() {
        string name = getValidString("Введите название трубы: ");
        double length = getValidDouble("Введите длину (км): ");
        int diameter = getValidInt("Введите диаметр (мм): ");

        Pipe pipe;
        pipe.setData(++next_pipe_id, name, length, diameter);
        pipes.push_back(pipe);
        cout << "Труба #" << next_pipe_id << " добавлена\n";
    }

    void addCS() {
        string name = getValidString("Введите название КС: ");
        int total = getValidInt("Введите общее количество цехов: ", 1);
        int working = getValidInt("Введите количество работающих цехов: ", 0, total);
        string eff_class = getValidString("Введите класс КС: ");

        CS cs;
        cs.setData(++next_cs_id, name, total, working, eff_class);
        cs_list.push_back(cs);
        cout << "КС #" << next_cs_id << " добавлена\n";
    }

    void displayAll() {
        cout << "\n=== Трубы ===\n";
        if (pipes.empty()) cout << "Трубы отсутствуют\n";
        else for (Pipe& p : pipes) p.display();

        cout << "\n=== КС ===\n";
        if (cs_list.empty()) cout << "КС отсутствуют\n";
        else for (CS& cs : cs_list) cs.display();
    }

    void findPipes() {
        cout << "\n1. Поиск по названию\n2. Поиск по признаку 'в ремонте'\n";
        int choice = getValidInt("Выбор: ", 1, 2);

        if (choice == 1) {
            string name = getValidString("Введите название: ");
            for (Pipe& p : pipes) {
                if (p.getName() == name) p.display();
            }
        }
        else {
            cout << "Искать трубы в ремонте (1) или в работе (0)?\n";
            bool repair = getValidInt("Выбор: ", 0, 1);
            for (Pipe& p : pipes) {
                if (p.getRepair() == repair) p.display();
            }
        }
    }

    void findCS() {
        cout << "\n1. Поиск по названию\n2. Поиск по проценту незадействованных цехов\n";
        int choice = getValidInt("Выбор: ", 1, 2);

        if (choice == 1) {
            string name = getValidString("Введите название: ");
            for (CS& cs : cs_list) {
                if (cs.getName() == name) cs.display();
            }
        }
        else {
            int percent = getValidInt("Введите процент незадействованных цехов: ", 0, 100);
            for (CS& cs : cs_list) {
                if (cs.getUnusedWorkshopsPercent() == percent) cs.display();
            }
        }
    }
};

int main() {
    Manager manager;
    
    while (true) {
        cout << "\n=== МЕНЮ ===\n"
             << "1. Добавить трубу\n"
             << "2. Добавить КС\n"
             << "3. Просмотр всех объектов\n"
             << "4. Поиск труб\n"
             << "5. Поиск КС\n"
             << "0. Выход\n";

        int choice = getValidInt("Выбор: ", 0, 5);

        switch (choice) {
            case 1: manager.addPipe(); break;
            case 2: manager.addCS(); break;
            case 3: manager.displayAll(); break;
            case 4: manager.findPipes(); break;
            case 5: manager.findCS(); break;
            case 0: return 0;
        }
    }
}