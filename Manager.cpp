// Реализация класса Manager

#include "Manager.h"
#include "Utils.h" // Ради getValidInput
#include <fstream>
#include <algorithm> // для max и sort
#include <sstream>

// Меню теперь тоже часть менеджера
void printMenu() {
    std::cout << "\n========= МЕНЮ =========\n"
              << "1. Добавить трубу\n"
              << "2. Добавить КС\n"
              << "3. Просмотр всех объектов\n"
              << "4. Работа с объектами (поиск, редактирование, удаление)\n"
              << "5. Сохранить данные\n"
              << "6. Загрузить данные\n"
              << "0. Выход\n";
}

Manager::Manager() : next_pipe_id(1), next_cs_id(1), logger("log.txt") {}

// Публичный логгер, чтобы его можно было вызвать из main
void Manager::log(const std::string& input) {
    logger.log(input);
}

// Главный цикл программы
void Manager::run() {
    while (true) {
        printMenu();
        int choice = getValidInput<int>("Выберите действие: ");
        log(std::to_string(choice)); // Логируем выбор

        switch (choice) {
            case 1: addPipe(); break;
            case 2: addCS(); break;
            case 3: viewAllObjects(); break;
            case 4: workWithPackages(); break;
            case 5: saveData(); break;
            case 6: loadData(); break;
            case 0:
                std::cout << "Выход из программы.\n";
                return;
            default:
                std::cout << "Ошибка: Неверный пункт меню.\n";
                break;
        }
    }
}

void Manager::addPipe() {
    // Проверка на дубликаты по имени
    std::string name = getValidInput<std::string>("Введите название трубы: ");
    log(name);
    for (const auto& p : pipes) {
        if (p.getName() == name) {
            std::cout << "Ошибка: Труба с таким названием уже существует.\n";
            return;
        }
    }

    // Теперь менеджер собирает все данные
    double length;
    while (true) {
        length = getValidInput<double>("Введите длину (км): ");
        log(std::to_string(length));
        if (length > 0) break;
        std::cout << "Ошибка: Длина должна быть положительным числом.\n";
    }
    
    int diameter;
    while (true) {
        diameter = getValidInput<int>("Введите диаметр (мм): ");
        log(std::to_string(diameter));
        if (diameter > 0) break;
        std::cout << "Ошибка: Диаметр должен быть положительным целым числом.\n";
    }

    pipes.push_back(Pipe::createPipe(next_pipe_id++, name, length, diameter));
    std::cout << "Труба успешно добавлена!\n";
}

void Manager::addCS() {
    // Проверка на дубликаты по имени
    std::string name = getValidInput<std::string>("Введите название КС: ");
    log(name);
    for (const auto& cs : stations) {
        if (cs.getName() == name) {
            std::cout << "Ошибка: КС с таким названием уже существует.\n";
            return;
        }
    }

    // Теперь менеджер собирает все данные
    int workshops_total;
    while (true) {
        workshops_total = getValidInput<int>("Введите общее количество цехов: ");
        log(std::to_string(workshops_total));
        if (workshops_total > 0) break;
        std::cout << "Ошибка: общее число цехов должно быть больше нуля.\n";
    }
    
    int workshops_in_operation;
    while (true) {
        workshops_in_operation = getValidInput<int>("Введите количество цехов в работе: ");
        log(std::to_string(workshops_in_operation));
        if (workshops_in_operation >= 0 && workshops_in_operation <= workshops_total) break;
        std::cout << "Ошибка: число работающих цехов не может быть отрицательным или больше общего числа.\n";
    }

    stations.push_back(CS::createCS(next_cs_id++, name, workshops_total, workshops_in_operation));
    std::cout << "КС успешно добавлена!\n";
}

void Manager::viewAllObjects() {
    std::cout << "\n========== ВСЕ ТРУБЫ ==========\n";
    if (pipes.empty()) {
        std::cout << "Трубы отсутствуют.\n";
    }
    else {
        for (const auto& p : pipes) {
            std::cout << p << std::endl;
        }
    }
    std::cout << "\n========== ВСЕ КС ==========\n";
    if (stations.empty()) {
        std::cout << "КС отсутствуют.\n";
    }
    else {
        for (const auto& cs : stations) {
            std::cout << cs << std::endl;
        }
    }
}

// --- Новый единый воркфлоу для работы с объектами ---
void Manager::workWithPackages() {
    std::cout << "\n--- Работа с объектами ---\n"
              << "1. Работать с трубами\n"
              << "2. Работать с КС\n"
              << "0. Назад в главное меню\n";
    int choice = getValidInput<int>("Ваш выбор: ");
    log(std::to_string(choice));

    if (choice == 1) {
        std::cout << "Критерии поиска труб:\n1. По названию\n2. По статусу 'в ремонте'\n";
        int filter_choice = getValidInput<int>("Выберите критерий: ");
        log(std::to_string(filter_choice));

        std::vector<int> found_indices;
        if (filter_choice == 1) {
            std::string name = getValidInput<std::string>("Введите часть названия для поиска: ");
            log(name);
            found_indices = findPipeIndicesByName(name);
        } else if (filter_choice == 2) {
            int status_in = getValidInput<int>("Искать в ремонте (1) или в эксплуатации (0)? ");
            log(std::to_string(status_in));
            found_indices = findPipeIndicesByRepairStatus(status_in == 1);
        } else {
            std::cout << "Неверный выбор.\n";
            return;
        }
        processPipesPackage(found_indices);

    } else if (choice == 2) {
        std::cout << "Критерии поиска КС:\n1. По названию\n2. По проценту незадействованных цехов\n";
        int filter_choice = getValidInput<int>("Выберите критерий: ");
        log(std::to_string(filter_choice));

        std::vector<int> found_indices;
        if (filter_choice == 1) {
            std::string name = getValidInput<std::string>("Введите часть названия для поиска: ");
            log(name);
            found_indices = findCSIndicesByName(name);
        } else if (filter_choice == 2) {
            double percent = getValidInput<double>("Введите минимальный процент незадействованных цехов: ");
            log(std::to_string(percent));
            found_indices = findCSIndicesByUnusedPercent(percent);
        } else {
            std::cout << "Неверный выбор.\n";
            return;
        }
        processCsPackage(found_indices);
    }
}

// --- Обработка пакета НАЙДЕННЫХ ТРУБ ---
void Manager::processPipesPackage(const std::vector<int>& indices) {
    if (indices.empty()) {
        std::cout << "Трубы по вашему запросу не найдены.\n";
        return;
    }

    std::cout << "\n--- Найденные трубы ---\n";
    for (size_t i = 0; i < indices.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << pipes[indices[i]];
    }
    
    std::cout << "\nЧто сделать с найденными трубами?\n"
              << "1. Редактировать статус\n"
              << "2. Удалить\n"
              << "0. Отмена\n";
    int action = getValidInput<int>("Ваш выбор: ");
    log(std::to_string(action));

    if (action == 1) { // Редактирование
        std::vector<int> to_edit_indices = getObjectIndices(pipes, indices);
        if (to_edit_indices.empty()) return;

        int new_status_in = getValidInput<int>("Установить статус 'в ремонте' (1) или 'в эксплуатации' (0)? ");
        log(std::to_string(new_status_in));
        for (int index : to_edit_indices) {
            pipes[index].setRepairStatus(new_status_in == 1);
        }
        std::cout << "Статус выбранных труб успешно изменен.\n";

    } else if (action == 2) { // Удаление
        std::vector<int> to_delete_indices = getObjectIndices(pipes, indices);
        if (to_delete_indices.empty()) return;

        // Сортируем индексы в обратном порядке, чтобы не нарушить их при удалении
        std::sort(to_delete_indices.rbegin(), to_delete_indices.rend());

        for (int index : to_delete_indices) {
            pipes.erase(pipes.begin() + index);
        }
        std::cout << "Выбранные трубы успешно удалены.\n";
    }
}

// --- Обработка пакета НАЙДЕННЫХ КС ---
void Manager::processCsPackage(const std::vector<int>& indices) {
    if (indices.empty()) {
        std::cout << "КС по вашему запросу не найдены.\n";
        return;
    }

    std::cout << "\n--- Найденные КС ---\n";
    for (size_t i = 0; i < indices.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << stations[indices[i]];
    }

    std::cout << "\nЧто сделать с найденными КС?\n"
              << "1. Редактировать\n"
              << "2. Удалить\n"
              << "0. Отмена\n";
    int action = getValidInput<int>("Ваш выбор: ");
    log(std::to_string(action));

    if (action == 1) {
        std::vector<int> to_edit_indices = getObjectIndices(stations, indices);
        for (int index : to_edit_indices) {
            editCS(index);
        }
    } else if (action == 2) { // Удаление
        std::vector<int> to_delete_indices = getObjectIndices(stations, indices);
        if (to_delete_indices.empty()) return;

        std::sort(to_delete_indices.rbegin(), to_delete_indices.rend());
        for (int index : to_delete_indices) {
            stations.erase(stations.begin() + index);
        }
        std::cout << "Выбранные КС успешно удалены.\n";
    }
}

// --- Редактирование ОДНОЙ КС ---
void Manager::editCS(int index) {
    std::cout << "\nРедактирование КС: " << stations[index].getName() << " (ID: " << stations[index].getId() << ")\n";
    std::cout << "Текущее кол-во цехов в работе: " << stations[index].getWorkshopsInOperation() << " из " << stations[index].getWorkshopsTotal() << "\n";
    
    while (true) {
        int new_count = getValidInput<int>("Введите новое количество цехов в работе: ");
        log(std::to_string(new_count));
        if (new_count >= 0 && new_count <= stations[index].getWorkshopsTotal()) {
            stations[index].setWorkshopsInOperation(new_count);
            std::cout << "Данные обновлены.\n";
            break;
        }
        std::cout << "Ошибка: число работающих цехов не может быть отрицательным или больше общего числа (" << stations[index].getWorkshopsTotal() << ").\n";
    }
}

// --- Шаблонный метод для выбора объектов из списка ---
template<typename T>
std::vector<int> Manager::getObjectIndices(const std::vector<T>& objects, const std::vector<int>& found_indices) {
    std::cout << "\nВведите номера объектов для операции (через пробел, например '1 3') или 'all' для всех: ";
    std::string selection_str = getValidInput<std::string>("");
    log(selection_str);

    std::vector<int> selected_indices;
    if (selection_str == "all") {
        return found_indices; // Возвращаем все найденные индексы
    } else {
        std::stringstream ss(selection_str);
        int num;
        while (ss >> num) {
            if (num > 0 && num <= found_indices.size()) {
                selected_indices.push_back(found_indices[num - 1]);
            }
        }
    }
    return selected_indices;
}


void Manager::saveData() {
    std::string filename = getValidInput<std::string>("Введите имя файла для сохранения: ");
    log(filename);
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл для записи.\n";
        return;
    }
    
    fout << pipes.size() << std::endl;
    for (const auto& p : pipes) {
        fout << p.getId() << std::endl;
        fout << p.getName() << std::endl;
        fout << p.getLength() << std::endl;
        fout << p.getDiameter() << std::endl;
        fout << p.isInRepair() << std::endl;
    }

    fout << stations.size() << std::endl;
    for (const auto& cs : stations) {
        fout << cs.getId() << std::endl;
        fout << cs.getName() << std::endl;
        fout << cs.getWorkshopsTotal() << std::endl;
        fout << cs.getWorkshopsInOperation() << std::endl;
    }

    fout.close();
    std::cout << "Данные успешно сохранены в " << filename << std::endl;
}

void Manager::loadData() {
    std::string filename = getValidInput<std::string>("Введите имя файла для загрузки: ");
    log(filename);
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл для чтения.\n";
        return;
    }
    
    // Сначала чистим старые данные, как и хотели
    pipes.clear();
    stations.clear();
    std::string line;

    int pipe_count = 0;
    if (std::getline(fin, line)) {
        try { pipe_count = std::stoi(line); } catch(...) { /* ошибка формата */ }
    }

    for (int i = 0; i < pipe_count; ++i) {
        if (!fin.good()) {
             std::cout << "Ошибка: файл поврежден или закончился раньше времени при чтении труб.\n";
             break;
        }
        int id = 0, diameter = 0;
        std::string name;
        double length = 0.0;
        bool in_repair = false;

        if (std::getline(fin, line)) try { id = std::stoi(line); } catch(...) {}
        std::getline(fin, name);
        if (std::getline(fin, line)) try { length = std::stod(line); } catch(...) {}
        if (std::getline(fin, line)) try { diameter = std::stoi(line); } catch(...) {}
        if (std::getline(fin, line)) try { in_repair = (std::stoi(line) == 1); } catch(...) {}
        
        pipes.emplace_back(id, name, length, diameter, in_repair);
        next_pipe_id = std::max(next_pipe_id, id + 1);
    }
    
    int cs_count = 0;
    if (std::getline(fin, line)) {
         try { cs_count = std::stoi(line); } catch(...) { /* ошибка формата */ }
    }

    for (int i = 0; i < cs_count; ++i) {
        if (!fin.good()) {
             std::cout << "Ошибка: файл поврежден или закончился раньше времени при чтении КС.\n";
             break;
        }
        int id = 0, total = 0, in_op = 0;
        std::string name;

        if (std::getline(fin, line)) try { id = std::stoi(line); } catch(...) {}
        std::getline(fin, name);
        if (std::getline(fin, line)) try { total = std::stoi(line); } catch(...) {}
        if (std::getline(fin, line)) try { in_op = std::stoi(line); } catch(...) {}
        
        stations.emplace_back(id, name, total, in_op);
        next_cs_id = std::max(next_cs_id, id + 1);
    }

    fin.close();
    std::cout << "Данные успешно загружены из " << filename << std::endl;
}

// --- Реализации вспомогательных методов ---
// Теперь ищем по подстроке, а не по точному совпадению
std::vector<int> Manager::findPipeIndicesByName(const std::string& name) {
    std::vector<int> indices;
    for (size_t i = 0; i < pipes.size(); ++i) {
        if (pipes[i].getName().find(name) != std::string::npos) {
            indices.push_back(i);
        }
    }
    return indices;
}

std::vector<int> Manager::findPipeIndicesByRepairStatus(bool status) {
    std::vector<int> indices;
    for (size_t i = 0; i < pipes.size(); ++i) {
        if (pipes[i].isInRepair() == status) {
            indices.push_back(i);
        }
    }
    return indices;
}

// Тоже поиск по подстроке
std::vector<int> Manager::findCSIndicesByName(const std::string& name) {
    std::vector<int> indices;
    for (size_t i = 0; i < stations.size(); ++i) {
        if (stations[i].getName().find(name) != std::string::npos) {
            indices.push_back(i);
        }
    }
    return indices;
}

std::vector<int> Manager::findCSIndicesByUnusedPercent(double percent) {
    std::vector<int> indices;
    for (size_t i = 0; i < stations.size(); ++i) {
        if (stations[i].getUnusedPercent() >= percent) {
            indices.push_back(i);
        }
    }
    return indices;
}