#include <iostream>
#include <string> // для getline()
#include <windows.h> // Добавлено для работы с консолью Windows

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

int main()
{
    // Команды для корректного отображения кириллицы в консоли Windows (utf-8)
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
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
        cout << "4. Редактировать трубу\n";
        cout << "5. Редактировать КС\n";
        // пока не реализованы файлы нет соответствующих пунктов в меню
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
                                the_pipe.length = stod(length_input); // преобразует строку (string) в число с плавающей точкой типа double
                                break;
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
                                the_pipe.diameter = stoi(diameter_input); // преобразует строку (string) в целое число int
                                break;
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
                    the_pipe.in_repair = !the_pipe.in_repair;
                    cout << "Статус ремонта трубы изменен. Новый статус: " << (the_pipe.in_repair ? "В ремонте\n" : "В эксплуатации\n");
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
                    cout << "Текущее состояние цехов: " << the_cs.workshops_in_operation << "/" << the_cs.workshops_total << "\n";
                    cout << "1. Запустить один цех\n";
                    cout << "2. Остановить один цех\n";
                    cout << "Выберите действие: ";
                    int cs_choice;
                    cin >> cs_choice;

                    if (cs_choice == 1)
                    {
                        if (the_cs.workshops_in_operation < the_cs.workshops_total) 
                        {
                            the_cs.workshops_in_operation++;
                            cout << "Цех запущен. Цехов в работе: " << the_cs.workshops_in_operation << "\n";
                        } 
                        else 
                        {
                            cout << "Ошибка: Все цеха уже работают!\n";
                        }
                    } 
                    else if (cs_choice == 2) 
                    {
                        if (the_cs.workshops_in_operation > 0) 
                        {
                            the_cs.workshops_in_operation--;
                            cout << "Цех остановлен. Цехов в работе: " << the_cs.workshops_in_operation << "\n";
                        } 
                        else 
                        {
                            cout << "Ошибка: Нет работающих цехов!\n";
                        }
                    } 
                    else 
                    {
                        cout << "Неверный выбор.\n";
                    }
                }
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