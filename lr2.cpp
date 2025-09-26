#include <iostream>
#include <fstream>
#include <climits>  // для INT_MAX.
//  INT_MAX определяет максимальное значение для типа int в системе и используется в функции getValidInt() как значение по умолчанию для параметра max.
#include <string>
// #include <windows.h> // Добавлено для работы с консолью Windows
using namespace std;

struct Pipe {
    string name;
    double length;
    int diameter;
    bool in_repair;
};

struct CS {
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

// Функции для работы с объектами
void addPipe(Pipe& pipe, bool& pipe_exists) {
    if (pipe_exists) {
        cout << "Ошибка: Труба уже существует. Для изменения используйте пункт 4.\n";
        return;
    }
    
    pipe.name = getValidString("Введите название трубы: ");
    pipe.length = getValidDouble("Введите длину (км): ");
    pipe.diameter = getValidInt("Введите диаметр (мм): ");
    pipe.in_repair = false;
    pipe_exists = true;
    cout << "Труба успешно добавлена!\n";
}

void addCS(CS& cs, bool& cs_exists) {
    if (cs_exists) {
        cout << "Ошибка: КС уже существует. Для изменения используйте пункт 5.\n";
        return;
    }
    
    cs.name = getValidString("Введите название КС: ");
    cs.workshops_total = getValidInt("Введите общее количество цехов: ");
    cs.workshops_in_operation = getValidInt("Введите количество цехов в работе: ", 0, cs.workshops_total);
    cs.efficiency_class = getValidString("Введите класс станции: ");
    cs_exists = true;
    cout << "КС успешно добавлена!\n";
}

void displayObjects(const Pipe& pipe, const CS& cs, bool pipe_exists, bool cs_exists) {
    cout << "\n--- Труба ---\n";
    if (pipe_exists) {
        cout << "Название: " << pipe.name
             << "\nДлина: " << pipe.length << " км"
             << "\nДиаметр: " << pipe.diameter << " мм"
             << "\nСтатус: " << (pipe.in_repair ? "В ремонте" : "В эксплуатации") << "\n";
    } else {
        cout << "Труба еще не создана.\n";
    }

    cout << "\n--- Компрессорная станция ---\n";
    if (cs_exists) {
        cout << "Название: " << cs.name
             << "\nЦеха: " << cs.workshops_in_operation << "/" << cs.workshops_total
             << "\nКласс: " << cs.efficiency_class << "\n";
    } else {
        cout << "КС еще не создана.\n";
    }
}

void editPipe(Pipe& pipe, bool pipe_exists) {
    if (!pipe_exists) {
        cout << "Сначала нужно создать трубу (пункт 1).\n";
        return;
    }

    cout << "\nТекущие параметры трубы:\n"
         << "Название: " << pipe.name << "\n"
         << "Статус: " << (pipe.in_repair ? "В ремонте" : "В эксплуатации") << "\n"
         << "\n1. Изменить название\n2. Изменить статус ремонта\n0. Отмена\n";

    int choice = getValidInt("Выберите действие: ", 0, 2);
    switch (choice) {
        case 1:
            pipe.name = getValidString("Введите новое название трубы: ");
            cout << "Название трубы успешно изменено.\n";
            break;
        case 2:
            pipe.in_repair = !pipe.in_repair;
            cout << "Статус ремонта трубы изменен. Новый статус: " 
                 << (pipe.in_repair ? "В ремонте" : "В эксплуатации") << "\n";
            break;
        case 0:
            cout << "Отмена редактирования.\n";
            break;
    }
}

void editCS(CS& cs, bool cs_exists) {
    if (!cs_exists) {
        cout << "Сначала нужно создать КС (пункт 2).\n";
        return;
    }

    cout << "\nТекущие параметры КС:\n"
         << "Название: " << cs.name << "\n"
         << "Цеха: " << cs.workshops_in_operation << "/" << cs.workshops_total << "\n"
         << "Класс: " << cs.efficiency_class << "\n"
         << "\n1. Изменить название\n2. Изменить класс станции\n"
         << "3. Изменить количество работающих цехов\n0. Отмена\n";

    int choice = getValidInt("Выберите действие: ", 0, 3);
    switch (choice) {
        case 1:
            cs.name = getValidString("Введите новое название КС: ");
            cout << "Название КС успешно изменено.\n";
            break;
        case 2:
            cs.efficiency_class = getValidString("Введите новый класс станции: ");
            cout << "Класс станции успешно изменен.\n";
            break;
        case 3:
            cs.workshops_in_operation = getValidInt("Введите новое количество работающих цехов: ", 
                                                  0, cs.workshops_total);
            cout << "Количество работающих цехов успешно изменено.\n";
            break;
        case 0:
            cout << "Отмена редактирования.\n";
            break;
    }
}

void SaveToFile(const string& filename, const Pipe& pipe, const CS& cs, bool pipe_exists, bool cs_exists) {
    ofstream fout(filename); // переменная записи в файл
    if (fout.is_open()) {
        fout << pipe_exists << endl;
        fout << cs_exists << endl;
        
        if (pipe_exists) {
            fout << pipe.name << endl;
            fout << pipe.length << endl;
            fout << pipe.diameter << endl;
            fout << pipe.in_repair << endl;
        }
        
        if (cs_exists) {
            fout << cs.name << endl;
            fout << cs.workshops_total << endl;
            fout << cs.workshops_in_operation << endl;
            fout << cs.efficiency_class << endl;
        }
        
        cout << "Данные успешно сохранены в файл " << filename << endl;
        fout.close();
    } else {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }
}

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
    Pipe the_pipe;
    CS the_cs;
    bool pipe_exists = false;
    bool cs_exists = false;
    
    while (true) {
        cout << "\n========= МЕНЮ =========\n"
             << "1. Добавить трубу\n"
             << "2. Добавить КС\n"
             << "3. Просмотр объектов\n"
             << "4. Редактировать трубу\n"
             << "5. Редактировать КС\n"
             << "6. Сохранить данные в файл\n"
             << "7. Загрузить данные из файла\n"
             << "0. Выход\n";

        int choice = getValidInt("Выберите действие: ", 0, 7);
        
        switch (choice) {
            case 1: addPipe(the_pipe, pipe_exists); break;
            case 2: addCS(the_cs, cs_exists); break;
            case 3: displayObjects(the_pipe, the_cs, pipe_exists, cs_exists); break;
            case 4: editPipe(the_pipe, pipe_exists); break;
            case 5: editCS(the_cs, cs_exists); break;
            case 6:
                SaveToFile(getValidString("Введите имя файла для сохранения: "), 
                          the_pipe, the_cs, pipe_exists, cs_exists);
                break;
            case 7:
                LoadFromFile(getValidString("Введите имя файла для загрузки: "), 
                           the_pipe, the_cs, pipe_exists, cs_exists);
                break;
            case 0:
                cout << "Выход из программы.\n";
                return 0;
        }
    }
}