#include <iostream>
#include <fstream>
#include <string> // для getline()
// #include <windows.h> // Добавлено для работы с консолью Windows

using namespace std;

struct Pipe
{
    string name;
    double length;
    int diameter;
    bool in_repair;
};

struct CS
{
    string name;
    int workshops_total;
    int workshops_in_operation;
    string efficiency_class;
};

void SaveToFile(const string& filename, const Pipe& pipe, const CS& cs, bool pipe_exists, bool cs_exists) 
{
    ofstream fout(filename); // переменная записи в файл
    if (fout.is_open())
    {
        fout << pipe_exists << endl;
        fout << cs_exists << endl;
        
        if (pipe_exists) 
        {
            fout << pipe.name << endl;
            fout << pipe.length << endl;
            fout << pipe.diameter << endl;
            fout << pipe.in_repair << endl;
        }
        
        if (cs_exists) 
        {
            fout << cs.name << endl;
            fout << cs.workshops_total << endl;
            fout << cs.workshops_in_operation << endl;
            fout << cs.efficiency_class << endl;
        }
        
        cout << "Данные успешно сохранены в файл " << filename << endl;
        fout.close();
    } 
    else
    {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }
}

bool LoadFromFile(const string& filename, Pipe& pipe, CS& cs, bool& pipe_exists, bool& cs_exists) 
{
    ifstream fin(filename);
    if (fin.is_open())
    {
        string line;
        
        // Читаем флаги существования
        getline(fin, line);
        pipe_exists = (line == "1");
        getline(fin, line);
        cs_exists = (line == "1");
        
        if (pipe_exists)
        {
            // Читаем данные трубы
            getline(fin, pipe.name);
            if (pipe.name.empty())
            {
                cout << "Ошибка: некорректное имя трубы в файле" << endl;
                return false;
            }
            
            getline(fin, line);
            try
            {
                pipe.length = stod(line);
                if (pipe.length <= 0)
                {
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
            try
            {
                pipe.diameter = stoi(line);
                if (pipe.diameter <= 0)
                {
                    cout << "Ошибка: некорректный диаметр трубы в файле" << endl;
                    return false;
                }
            }
            catch (...)
            {
                cout << "Ошибка при чтении диаметра трубы из файла" << endl;
                return false;
            }
            
            getline(fin, line);
            pipe.in_repair = (line == "1");
        }
        
        if (cs_exists)
        {
            // Читаем данные КС
            getline(fin, cs.name);
            if (cs.name.empty()) 
            {
                cout << "Ошибка: некорректное имя КС в файле" << endl;
                return false;
            }
            
            getline(fin, line);
            try
            {
                cs.workshops_total = stoi(line);
                if (cs.workshops_total <= 0) 
                {
                    cout << "Ошибка: некорректное общее количество цехов в файле" << endl;
                    return false;
                }
            } 
            catch (...) 
            {
                cout << "Ошибка при чтении общего количества цехов из файла" << endl;
                return false;
            }
            
            getline(fin, line);
            try 
            {
                cs.workshops_in_operation = stoi(line);
                if (cs.workshops_in_operation < 0 || cs.workshops_in_operation > cs.workshops_total) 
                {
                    cout << "Ошибка: некорректное количество рабочих цехов в файле" << endl;
                    return false;
                }
            } 
            catch (...) 
            {
                cout << "Ошибка при чтении количества рабочих цехов из файла" << endl;
                return false;
            }
            
            getline(fin, cs.efficiency_class);
            if (cs.efficiency_class.empty()) 
            {
                cout << "Ошибка: некорректный класс КС в файле" << endl;
                return false;
            }
        }
        
        cout << "Данные успешно загружены из файла " << filename << endl;
        fin.close();
        return true;
    }
    else 
    {
        cout << "Ошибка при открытии файла для чтения!" << endl;
        return false;
    }
}

int main()
{
    // Команды для корректного отображения кириллицы в консоли Windows (utf-8)
    //SetConsoleCP(65001);
    //SetConsoleOutputCP(65001);
    
    Pipe the_pipe;
    CS the_cs;
    // флаги, которые показывают созданы ли труба и кс 
    bool pipe_exists = false;
    bool cs_exists = false;
    
    while (true) 
    {
        cout << "\n========= МЕНЮ =========\n";
        cout << "1. Добавить трубу\n";
        cout << "2. Добавить КС\n";
        cout << "3. Просмотр объектов\n";
        cout << "4. Редактировать трубу (изменить статус работы)\n";
        cout << "5. Редактировать КС (включить/выключить КС)\n";
        cout << "6. Сохранить данные в фaйл\n";
        cout << "7. Загрузить данные из файла\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";

        int choice;
        cin >> choice;

        if (cin.fail()) // fail() возвращает false если ввод соответствует ожидаемому типу
        {
            cout << "Ошибка: Введите номер пункта меню.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        
        cin.ignore(10000, '\n'); // очистка буфера

        switch (choice) 
        {
            case 1: // Добавить трубу
            { 
                if (pipe_exists) 
                {
                    cout << "Ошибка: Труба уже существует. Для изменения используйте пункт 4.\n";
                } 
                else 
                {
                    string name_input;
                    cout << "Введите название трубы: ";
                    while (true) 
                    {
                        getline(cin, name_input);
                        if (!name_input.empty()) 
                        {
                            the_pipe.name = name_input;
                            break;
                        }
                        cout << "Ошибка: Название не может быть пустым.\n";
                        cout << "Введите название трубы: ";
                    }
                    
                    string length_input;
                    cout << "Введите длину (км): ";
                    while (true) 
                    {
                        getline(cin, length_input);
                        // Заменяем запятые на точки для корректного преобразования
                        for (char& c : length_input) 
                        {
                            if (c == ',') c = '.';
                        }
                        // Проверяем, что строка содержит только цифры и одну точку
                        bool valid = true;
                        int dot_count = 0;
                        for (char c : length_input) 
                        {
                            if (!isdigit(c) && c != '.') 
                            {
                                valid = false;
                                break;
                            }
                            if (c == '.') dot_count++;
                        }
                        if (valid && dot_count <= 1 && !length_input.empty()) 
                        {
                            try 
                            {
                                if (stod(length_input) > 0)
                                {
                                    the_pipe.length = stod(length_input); // преобразует строку (string) в число с плавающей точкой типа double
                                    break;
                                }
                                else
                                {
                                    valid = false;
                                }
                            }
                            catch (...) // ловит всё ошибки
                            {
                                valid = false;
                            }
                        }
                        cout << "Ошибка: Введите корректное число (например, 15.5).\n";
                        cout << "Введите длину (км): ";
                    }

                    string diameter_input;
                    cout << "Введите диаметр (мм): ";
                    while (true) 
                    {
                        getline(cin, diameter_input);
                        // Проверяем, что строка содержит только цифры
                        bool valid = true;
                        for (char c : diameter_input) 
                        {
                            if (!isdigit(c)) 
                            {
                                valid = false;
                                break;
                            }
                        }
                        if (valid && !diameter_input.empty())
                         {
                            try 
                            {
                                if (stoi(diameter_input) > 0)
                                {
                                    the_pipe.diameter = stoi(diameter_input); // преобразует строку (string) в целое число int
                                    break;
                                }
                                else
                                {
                                    valid = false;
                                }
                            }
                            catch (...) 
                            {
                                valid = false;
                            }
                        }
                        cout << "Ошибка: Введите корректное целое число (например, 1400).\n";
                        cout << "Введите диаметр (мм): ";
                    }
                    
                    the_pipe.in_repair = false;
                    pipe_exists = true;
                    cout << "Труба успешно добавлена!\n";
                }
                break;
            }

            case 2: // Добавить КС
            {
                if (cs_exists) 
                {
                    cout << "Ошибка: КС уже существует. Для изменения используйте пункт 5.\n";
                } 
                else 
                {
                    string name_input;
                    cout << "Введите название КС: ";
                    while (true) 
                    {
                        getline(cin, name_input);
                        if (!name_input.empty()) 
                        {
                            the_cs.name = name_input;
                            break;
                        }
                        cout << "Ошибка: Название не может быть пустым.\n";
                        cout << "Введите название КС: ";
                    }

                    string total_input;
                    cout << "Введите общее количество цехов: ";
                    while (true) 
                    {
                        getline(cin, total_input);
                        // Проверяем, что строка содержит только цифры
                        bool valid = true;
                        for (char c : total_input) 
                        {
                            if (!isdigit(c)) 
                            {
                                valid = false;
                                break;
                            }
                        }
                        if (valid && !total_input.empty()) 
                        {
                            try
                            {
                                the_cs.workshops_total = stoi(total_input);
                                if (the_cs.workshops_total <= 0) 
                                {
                                    valid = false;
                                } 
                                else 
                                {
                                    break;
                                }
                            }
                            catch (...) 
                            {
                                valid = false;
                            }
                        }
                        cout << "Ошибка: Введите корректное целое положительное число.\n";
                        cout << "Введите общее количество цехов: ";
                    }

                    string working_input;
                    cout << "Введите количество цехов в работе: ";
                    while (true) 
                    {
                        getline(cin, working_input);
                        // Проверяем, что строка содержит только цифры
                        bool valid = true;
                        for (char c : working_input) 
                        {
                            if (!isdigit(c)) 
                            {
                                valid = false;
                                break;
                            }
                        }
                        if (valid && !working_input.empty()) 
                        {
                            try 
                            {
                                int working = stoi(working_input);
                                if (working < 0) 
                                {
                                    valid = false;
                                    break;
                                } 
                                else if (working > the_cs.workshops_total) 
                                {
                                    cout << "Ошибка: Рабочих цехов не может быть больше общего количества (" << the_cs.workshops_total << ").\n";
                                } 
                                else 
                                {
                                    the_cs.workshops_in_operation = working;
                                    break;
                                }
                            }
                            catch (...) 
                            {
                                valid = false;
                            }
                        }
                        cout << "Ошибка: Введите корректное целое положительное число.\n";
                        cout << "Введите количество цехов в работе: ";
                    }

                    string class_input;
                    cout << "Введите класс станции: ";
                    while (true)
                    {
                        getline(cin, class_input);
                        if (!class_input.empty())
                        {
                            the_cs.efficiency_class = class_input;
                            break;
                        }
                        cout << "Ошибка: Класс станции не может быть пустым.\n";
                        cout << "Введите класс станции: ";
                    }
                    
                    cs_exists = true;
                    cout << "КС успешно добавлена!\n";
                }
                break;
            }
            case 3: // Просмотр всех объектов
            { 
                cout << "\n--- Труба ---\n";
                if (pipe_exists) 
                {
                    cout << "Название: " << the_pipe.name;
                    cout << "\nДлина: " << the_pipe.length;
                    cout << " км\nДиаметр: " << the_pipe.diameter;
                    cout << " мм\nСтатус: " << (the_pipe.in_repair ? "В ремонте\n" : "В эксплуатации\n");
                } 
                else 
                {
                    cout << "Труба еще не создана.\n";
                }

                cout << "\n--- Компрессорная станция ---\n";
                if (cs_exists) 
                {
                    cout << "Название: " << the_cs.name;
                    cout << "\nЦеха: " << the_cs.workshops_in_operation << "/" << the_cs.workshops_total;
                    cout << "\nКласс: " << the_cs.efficiency_class << "\n";
                } 
                else 
                {
                    cout << "КС еще не создана.\n";
                }
                break;
            }
            case 4: // Редактировать трубу
            { 
                if (!pipe_exists)
                {
                    cout << "Сначала нужно создать трубу (пункт 1).\n";
                } 
                else 
                {
                    cout << "\nТекущие параметры трубы:\n";
                    cout << "Название: " << the_pipe.name << endl;
                    cout << "Статус: " << (the_pipe.in_repair ? "В ремонте" : "В эксплуатации") << endl;

                    cout << "\n1. Изменить название\n";
                    cout << "2. Изменить статус ремонта\n";
                    cout << "0. Отмена\n";
                    cout << "Выберите действие: ";

                    string choice_str;
                    getline(cin, choice_str);
                    
                    // Проверка корректности ввода
                    bool valid = true;
                    for (char c : choice_str) 
                    {
                        if (!isdigit(c)) 
                        {
                            valid = false;
                            break;
                        }
                    }
                    
                    if (!valid || choice_str.empty()) 
                    {
                        cout << "Ошибка: введите корректный номер пункта меню.\n";
                        break;
                    }

                    int edit_choice = stoi(choice_str);

                    switch (edit_choice) 
                    {
                        case 1: 
                        {
                            string new_name;
                            cout << "Введите новое название трубы: ";
                            while (true) 
                            {
                                getline(cin, new_name);
                                if (!new_name.empty()) 
                                {
                                    the_pipe.name = new_name;
                                    cout << "Название трубы успешно изменено.\n";
                                    break;
                                }
                                cout << "Ошибка: Название не может быть пустым.\n";
                                cout << "Введите новое название трубы: ";
                            }
                            break;
                        }
                        case 2: 
                        {
                            the_pipe.in_repair = !the_pipe.in_repair;
                            cout << "Статус ремонта трубы изменен. Новый статус: " 
                                << (the_pipe.in_repair ? "В ремонте" : "В эксплуатации") << endl;
                            break;
                        }
                        case 0:
                            cout << "Отмена редактирования.\n";
                            break;
                        default:
                            cout << "Ошибка: неверный пункт меню.\n";
                            break;
                    }
                }
                break;
            }

            case 5: // Редактировать КС
            { 
                if (!cs_exists) 
                {
                    cout << "Сначала нужно создать КС (пункт 2).\n";
                } 
                else 
                {
                    cout << "\nТекущие параметры КС:\n";
                    cout << "Название: " << the_cs.name << endl;
                    cout << "Цеха: " << the_cs.workshops_in_operation << "/" << the_cs.workshops_total << endl;
                    cout << "Класс: " << the_cs.efficiency_class << endl;
                
                    cout << "\n1. Изменить название\n";
                    cout << "2. Изменить класс станции\n";
                    cout << "3. Изменить количество работающих цехов\n";
                    cout << "0. Отмена\n";
                    cout << "Выберите действие: ";
                
                    string choice_str;
                    getline(cin, choice_str);

                    // Проверка корректности ввода
                    bool valid = true;
                    for (char c : choice_str) 
                    {
                        if (!isdigit(c)) 
                        {
                            valid = false;
                            break;
                        }
                    }

                    if (!valid || choice_str.empty()) 
                    {
                        cout << "Ошибка: введите корректный номер пункта меню.\n";
                        break;
                    }
                
                    int edit_choice = stoi(choice_str);
                
                    switch (edit_choice) 
                    {
                        case 1: 
                        {
                            string new_name;
                            cout << "Введите новое название КС: ";
                            while (true) 
                            {
                                getline(cin, new_name);
                                if (!new_name.empty()) 
                                {
                                    the_cs.name = new_name;
                                    cout << "Название КС успешно изменено.\n";
                                    break;
                                }
                                cout << "Ошибка: Название не может быть пустым.\n";
                                cout << "Введите новое название КС: ";
                            }
                            break;
                        }
                        case 2: 
                        {
                            string new_class;
                            cout << "Введите новый класс станции: ";
                            while (true) 
                            {
                                getline(cin, new_class);
                                if (!new_class.empty()) 
                                {
                                    the_cs.efficiency_class = new_class;
                                    cout << "Класс станции успешно изменен.\n";
                                    break;
                                }
                                cout << "Ошибка: Класс станции не может быть пустым.\n";
                                cout << "Введите новый класс станции: ";
                            }
                            break;
                        }
                        case 3: 
                        {
                            cout << "Введите новое количество работающих цехов (доступно цехов: " 
                                 << the_cs.workshops_total << "): ";

                            string working_input;
                            while (true) 
                            {
                                getline(cin, working_input);
                                // Проверяем, что строка содержит только цифры
                                bool valid = true;
                                for (char c : working_input) 
                                {
                                    if (!isdigit(c)) 
                                    {
                                        valid = false;
                                        break;
                                    }
                                }
                                if (valid && !working_input.empty()) 
                                {
                                    try 
                                    {
                                        int new_working = stoi(working_input);
                                        if (new_working < 0) 
                                        {
                                            cout << "Ошибка: количество не может быть отрицательным.\n";
                                        } 
                                        else if (new_working > the_cs.workshops_total) 
                                        {
                                            cout << "Ошибка: рабочих цехов не может быть больше общего количества (" 
                                                 << the_cs.workshops_total << ").\n";
                                        } 
                                        else 
                                        {
                                            the_cs.workshops_in_operation = new_working;
                                            cout << "Количество работающих цехов успешно изменено.\n";
                                            break;
                                        }
                                    }
                                    catch (...) 
                                    {
                                        valid = false;
                                    }
                                }
                                cout << "Введите новое количество работающих цехов: ";
                            }
                            break;
                        }
                        case 0:
                            cout << "Отмена редактирования.\n";
                            break;
                        default:
                            cout << "Ошибка: неверный пункт меню.\n";
                            break;
                    }
                }
                break;
            }
            case 6: // Сохранить 
            {
                string filename;
                cout << "Введите имя файла для сохранения: ";
                getline(cin, filename);
                SaveToFile(filename, the_pipe, the_cs, pipe_exists, cs_exists);
                break;
            }              

            case 7: // Загрузить 
            {
                string filename;
                cout << "Введите имя файла для загрузки: ";
                getline(cin, filename);
                LoadFromFile(filename, the_pipe, the_cs, pipe_exists, cs_exists);
                break;
            }
            case 0: // Выход
            { 
                cout << "Выход из программы.\n";
                return 0;
            }
            default: 
            {
                cout << "Ошибка: Неверный пункт меню. Попробуйте снова.\n";
                break;
            }
        }
    }

    return 0;
}