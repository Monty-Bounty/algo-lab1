#include <iostream>
#include <fstream>
#include <vector>
#include <climits>  // для INT_MAX.
//  INT_MAX определяет максимальное значение для типа int в системе и используется в функции getValidInt() как значение по умолчанию для параметра max.
#include <string>
// #include <windows.h> // Добавлено для работы с консолью Windows
using namespace std;

vector<Pipe> pipes;
vector<CS> cs_list;
int next_pipe_id = 0;
int next_cs_id = 0;

struct Pipe {
    int id;
    string name;
    double length;
    int diameter;
    bool in_repair;
};

struct CS {
    int id;
    string name;
    int workshops_total;
    int workshops_in_operation;
    string efficiency_class;
};

// Вспомогательные функции для проверки ввода
string getValidString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) return input;
        cout << "Ошибка: значение не может быть пустым.\n";
    }
}

double getValidDouble(const string& prompt) {
    while (true) {
        string input = getValidString(prompt);
        for (char& c : input) if (c == ',') c = '.';
        
        bool valid = true;
        int dot_count = 0;
        for (char c : input) {
            if (!isdigit(c) && c != '.') {
                valid = false;
                break;
            }
            if (c == '.') dot_count++;
        }
        
        if (valid && dot_count <= 1) {
            try {
                double value = stod(input);
                if (value > 0) return value;
            }
            catch (...) {}
        }
        cout << "Ошибка: введите корректное положительное число.\n";
    }
}

int getValidInt(const string& prompt, int min = 0, int max = INT_MAX) {
    while (true) {
        string input = getValidString(prompt);
        bool valid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }
        
        if (valid) {
            try {
                int value = stoi(input);
                if (value >= min && value <= max) return value;
                cout << "Ошибка: число должно быть между " << min << " и " << max << ".\n";
            }
            catch (...) {}
        }
        cout << "Ошибка: введите корректное целое число.\n";
    }
}

int generatePipeId() { 
    return ++next_pipe_id; 
}

int generateCSId() { 
    return ++next_cs_id; 
}

// Функции для работы с объектами

void addPipe() {
    Pipe new_pipe;
    new_pipe.id = generatePipeId();
    new_pipe.name = getValidString("Введите название трубы: ");
    new_pipe.length = getValidDouble("Введите длину (км): ");
    new_pipe.diameter = getValidInt("Введите диаметр (мм): ");
    new_pipe.in_repair = false;
    pipes.push_back(new_pipe);
    cout << "Труба #" << new_pipe.id << " успешно добавлена!\n";
}

void addCS() {
    CS new_cs;
    new_cs.id = generateCSId();
    new_cs.name = getValidString("Введите название КС: ");
    new_cs.workshops_total = getValidInt("Введите общее количество цехов: ");
    new_cs.workshops_in_operation = getValidInt("Введите количество цехов в работе: ", 0, new_cs.workshops_total);
    new_cs.efficiency_class = getValidString("Введите класс станции: ");
    cs_list.push_back(new_cs);
    cout << "КС #" << new_cs.id << " успешно добавлена!\n";
}

void displayObjects() {
    cout << "\n=== Трубы ===\n";
    if (pipes.empty()) {
        cout << "Трубы отсутствуют.\n";
    } else {
        for (const Pipe& pipe : pipes) {
            cout << "\nТруба #" << pipe.id << "\n"
                 << "Название: " << pipe.name << "\n"
                 << "Длина: " << pipe.length << " км\n"
                 << "Диаметр: " << pipe.diameter << " мм\n"
                 << "Статус: " << (pipe.in_repair ? "В ремонте" : "В эксплуатации") << "\n";
        }
    }

    cout << "\n=== Компрессорные станции ===\n";
    if (cs_list.empty()) {
        cout << "КС отсутствуют.\n";
    } else {
        for (const CS& cs : cs_list) {
            cout << "\nКС #" << cs.id << "\n"
                 << "Название: " << cs.name << "\n"
                 << "Цеха: " << cs.workshops_in_operation << "/" << cs.workshops_total << "\n"
                 << "Класс: " << cs.efficiency_class << "\n";
        }
    }
}

void SaveToFile(const string& filename) {
    ofstream fout(filename);
    if (fout.is_open()) {
        // Сохраняем трубы
        fout << pipes.size() << endl;
        for (const Pipe& pipe : pipes) {
            fout << pipe.id << endl
                 << pipe.name << endl
                 << pipe.length << endl
                 << pipe.diameter << endl
                 << pipe.in_repair << endl;
        }

        // Сохраняем КС
        fout << cs_list.size() << endl;
        for (const CS& cs : cs_list) {
            fout << cs.id << endl
                 << cs.name << endl
                 << cs.workshops_total << endl
                 << cs.workshops_in_operation << endl
                 << cs.efficiency_class << endl;
        }

        cout << "Данные успешно сохранены в файл " << filename << endl;
        fout.close();
    } else {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }
}


// todo: обновить загрузку
bool LoadFromFile(const string& filename, Pipe& pipe, CS& cs, bool& pipe_exists, bool& cs_exists) {
    ifstream fin(filename);
    if (fin.is_open()) {
        string line;
        
        // Читаем флаги существования
        getline(fin, line);
        pipe_exists = (line == "1");
        getline(fin, line);
        cs_exists = (line == "1");
        
        if (pipe_exists) {
            // Читаем данные трубы
            getline(fin, pipe.name);
            if (pipe.name.empty()) {
                cout << "Ошибка: некорректное имя трубы в файле" << endl;
                return false;
            }
            
            getline(fin, line);
            try {
                pipe.length = stod(line);
                if (pipe.length <= 0) {
                    cout << "Ошибка: некорректная длина трубы в файле" << endl;
                    return false;
                }
            } 
            catch (...)
            {
                cout << "Ошибка при чтении длины трубы из файла" << endl;
                return false;
            }
            
            getline(fin, line);
            try {
                pipe.diameter = stoi(line);
                if (pipe.diameter <= 0)
                {
                    cout << "Ошибка: некорректный диаметр трубы в файле" << endl;
                    return false;
                }
            } catch (...) {
                cout << "Ошибка при чтении диаметра трубы из файла" << endl;
                return false;
            }
            
            getline(fin, line);
            pipe.in_repair = (line == "1");
        }
        
        if (cs_exists) {
            // Читаем данные КС
            getline(fin, cs.name);
            if (cs.name.empty()) {
                cout << "Ошибка: некорректное имя КС в файле" << endl;
                return false;
            }
            
            getline(fin, line);
            try {
                cs.workshops_total = stoi(line);
                if (cs.workshops_total <= 0) {
                    cout << "Ошибка: некорректное общее количество цехов в файле" << endl;
                    return false;
                }
            } catch (...) {
                cout << "Ошибка при чтении общего количества цехов из файла" << endl;
                return false;
            }
            
            getline(fin, line);
            try {
                cs.workshops_in_operation = stoi(line);
                if (cs.workshops_in_operation < 0 || cs.workshops_in_operation > cs.workshops_total) {
                    cout << "Ошибка: некорректное количество рабочих цехов в файле" << endl;
                    return false;
                }
            } catch (...) {
                cout << "Ошибка при чтении количества рабочих цехов из файла" << endl;
                return false;
            }
            
            getline(fin, cs.efficiency_class);
            if (cs.efficiency_class.empty()) {
                cout << "Ошибка: некорректный класс КС в файле" << endl;
                return false;
            }
        }
        
        cout << "Данные успешно загружены из файла " << filename << endl;
        fin.close();
        return true;
    } else {
        cout << "Ошибка при открытии файла для чтения!" << endl;
        return false;
    }
}

int main() {
    while (true) {
        cout << "\n========= МЕНЮ =========\n"
             << "1. Добавить трубу\n"
             << "2. Добавить КС\n"
             << "3. Просмотр всех объектов\n"
             << "4. Редактировать трубу\n"
             << "5. Редактировать КС\n"
             << "6. Сохранить данные в файл\n"
             << "7. Загрузить данные из файла\n"
             << "0. Выход\n";

        int choice = getValidInt("Выберите действие: ", 0, 7);
        
        switch (choice) {
            case 1: addPipe(); break;
            case 2: addCS(); break;
            case 3: displayObjects(); break;
            case 4: /* TODO: Обновить функцию редактирования */ break;
            case 5: /* TODO: Обновить функцию редактирования */ break;
            case 6:
                SaveToFile(getValidString("Введите имя файла для сохранения: "));
                break;
            case 7:
                /* TODO: Обновить функцию загрузки */
                break;
            case 0:
                cout << "Выход из программы.\n";
                return 0;
        }
    }
}