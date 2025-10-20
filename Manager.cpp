// Реализация класса Manager

#include "Manager.h"
#include "Utils.h" // Ради getValidInput
#include <fstream>
#include <algorithm> // для max и sort
#include <sstream>

// Теперь меню - это приватный метод класса
void Manager::printMenu() {
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
    std::string name = getValidInput<std::string>("Введите название трубы: ");
    log(name);
    for (const auto& pair : pipes) {
        if (pair.second.getName() == name) {
            std::cout << "Ошибка: Труба с таким названием уже существует.\n";
            return;
        }
    }

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

    pipes.emplace(next_pipe_id, Pipe::createPipe(next_pipe_id, name, length, diameter));
    next_pipe_id++;
    std::cout << "Труба успешно добавлена!\n";
}

void Manager::addCS() {
    std::string name = getValidInput<std::string>("Введите название КС: ");
    log(name);
    for (const auto& pair : stations) {
        if (pair.second.getName() == name) {
            std::cout << "Ошибка: КС с таким названием уже существует.\n";
            return;
        }
    }

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

    stations.emplace(next_cs_id, CS::createCS(next_cs_id, name, workshops_total, workshops_in_operation));
    next_cs_id++;
    std::cout << "КС успешно добавлена!\n";
}

void Manager::viewAllObjects() {
    std::cout << "\n========== ВСЕ ТРУБЫ ==========\n";
    if (pipes.empty()) {
        std::cout << "Трубы отсутствуют.\n";
    }
    else {
        for (const auto& pair : pipes) {
            std::cout << pair.second << std::endl;
        }
    }
    std::cout << "\n========== ВСЕ КС ==========\n";
    if (stations.empty()) {
        std::cout << "КС отсутствуют.\n";
    }
    else {
        for (const auto& pair : stations) {
            std::cout << pair.second << std::endl;
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

        std::vector<int> found_ids;
        if (filter_choice == 1) {
            std::string name = getValidInput<std::string>("Введите часть названия для поиска: ");
            log(name);
            found_ids = findPipeIdsByName(name);
        } else if (filter_choice == 2) {
            int status_in = getValidInput<int>("Искать в ремонте (1) или в эксплуатации (0)? ");
            log(std::to_string(status_in));
            found_ids = findPipeIdsByRepairStatus(status_in == 1);
        } else {
            std::cout << "Неверный выбор.\n";
            return;
        }
        processPipesPackage(found_ids);

    } else if (choice == 2) {
        std::cout << "Критерии поиска КС:\n1. По названию\n2. По проценту незадействованных цехов\n";
        int filter_choice = getValidInput<int>("Выберите критерий: ");
        log(std::to_string(filter_choice));

        std::vector<int> found_ids;
        if (filter_choice == 1) {
            std::string name = getValidInput<std::string>("Введите часть названия для поиска: ");
            log(name);
            found_ids = findCSIdsByName(name);
        } else if (filter_choice == 2) {
            double percent = getValidInput<double>("Введите минимальный процент незадействованных цехов: ");
            log(std::to_string(percent));
            found_ids = findCSIdsByUnusedPercent(percent);
        } else {
            std::cout << "Неверный выбор.\n";
            return;
        }
        processCsPackage(found_ids);
    }
}

// --- Обработка пакета НАЙДЕННЫХ ТРУБ ---
void Manager::processPipesPackage(const std::vector<int>& ids) {
    if (ids.empty()) {
        std::cout << "Трубы по вашему запросу не найдены.\n";
        return;
    }

    std::cout << "\n--- Найденные трубы ---\n";
    for (int id : ids) {
        std::cout << pipes.at(id) << std::endl;
    }
    
    std::cout << "\nЧто сделать с найденными трубами?\n"
              << "1. Редактировать статус\n"
              << "2. Удалить\n"
              << "0. Отмена\n";
    int action = getValidInput<int>("Ваш выбор: ");
    log(std::to_string(action));

    if (action == 1) { // Редактирование
        std::vector<int> to_edit_ids = getIdsFromUser(ids);
        if (to_edit_ids.empty()) return;

        int new_status_in = getValidInput<int>("Установить статус 'в ремонте' (1) или 'в эксплуатации' (0)? ");
        log(std::to_string(new_status_in));
        for (int id : to_edit_ids) {
            pipes.at(id).setRepairStatus(new_status_in == 1);
        }
        std::cout << "Статус выбранных труб успешно изменен.\n";

    } else if (action == 2) { // Удаление
        std::vector<int> to_delete_ids = getIdsFromUser(ids);
        if (to_delete_ids.empty()) return;

        for (int id : to_delete_ids) {
            pipes.erase(id);
        }
        std::cout << "Выбранные трубы успешно удалены.\n";
    }
}

// --- Обработка пакета НАЙДЕННЫХ КС ---
void Manager::processCsPackage(const std::vector<int>& ids) {
    if (ids.empty()) {
        std::cout << "КС по вашему запросу не найдены.\n";
        return;
    }

    std::cout << "\n--- Найденные КС ---\n";
    for (int id : ids) {
        std::cout << stations.at(id) << std::endl;
    }

    std::cout << "\nЧто сделать с найденными КС?\n"
              << "1. Редактировать\n"
              << "2. Удалить\n"
              << "0. Отмена\n";
    int action = getValidInput<int>("Ваш выбор: ");
    log(std::to_string(action));

    if (action == 1) {
        std::vector<int> to_edit_ids = getIdsFromUser(ids);
        for (int id : to_edit_ids) {
            editCS(id);
        }
    } else if (action == 2) { // Удаление
        std::vector<int> to_delete_ids = getIdsFromUser(ids);
        if (to_delete_ids.empty()) return;

        for (int id : to_delete_ids) {
            stations.erase(id);
        }
        std::cout << "Выбранные КС успешно удалены.\n";
    }
}

// Новый приватный метод для выбора ID из списка
std::vector<int> Manager::getIdsFromUser(const std::vector<int>& available_ids) {
    std::cout << "\nВведите ID объектов для операции (через пробел, например '1 3') или 'all' для всех: ";
    std::string selection_str = getValidInput<std::string>("");
    log(selection_str);

    std::vector<int> selected_ids;
    if (selection_str == "all") {
        return available_ids;
    } else {
        std::stringstream ss(selection_str);
        int id;
        while (ss >> id) {
            bool found = false;
            for (int available_id : available_ids) {
                if (id == available_id) {
                    selected_ids.push_back(id);
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Предупреждение: ID " << id << " не найден в списке доступных и будет проигнорирован.\n";
            }
        }
    }
    return selected_ids;
}

// --- Редактирование ОДНОЙ КС ---
void Manager::editCS(int id) {
    try {
        CS& cs = stations.at(id);
        std::cout << "\nРедактирование КС: " << cs.getName() << " (ID: " << cs.getId() << ")\n";
        std::cout << "Текущее кол-во цехов в работе: " << cs.getWorkshopsInOperation() << " из " << cs.getWorkshopsTotal() << "\n";
        
        while (true) {
            int new_count = getValidInput<int>("Введите новое количество цехов в работе: ");
            log(std::to_string(new_count));
            if (new_count >= 0 && new_count <= cs.getWorkshopsTotal()) {
                cs.setWorkshopsInOperation(new_count);
                std::cout << "Данные обновлены.\n";
                break;
            }
            std::cout << "Ошибка: число работающих цехов не может быть отрицательным или больше общего числа (" << cs.getWorkshopsTotal() << ").\n";
        }
    } catch (const std::out_of_range& oor) {
        std::cout << "Ошибка: КС с ID " << id << " не найдена.\n";
    }
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
    for (const auto& pair : pipes) {
        const Pipe& p = pair.second;
        fout << p.getId() << std::endl;
        fout << p.getName() << std::endl;
        fout << p.getLength() << std::endl;
        fout << p.getDiameter() << std::endl;
        fout << p.isInRepair() << std::endl;
        fout << p.isUsed() << std::endl; // Сохраняем новое поле
    }

    fout << stations.size() << std::endl;
    for (const auto& pair : stations) {
        const CS& cs = pair.second;
        fout << cs.getId() << std::endl;
        fout << cs.getName() << std::endl;
        fout << cs.getWorkshopsTotal() << std::endl;
        fout << cs.getWorkshopsInOperation() << std::endl;
        // Сохраняем соединения
        const auto& connections = cs.getOutgoingConnections();
        fout << connections.size() << std::endl;
        for (const auto& conn_pair : connections) {
            fout << conn_pair.first << " " << conn_pair.second << std::endl;
        }
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
        bool is_used = false; // Загружаем новое поле

        if (std::getline(fin, line)) try { id = std::stoi(line); } catch(...) {}
        std::getline(fin, name);
        if (std::getline(fin, line)) try { length = std::stod(line); } catch(...) {}
        if (std::getline(fin, line)) try { diameter = std::stoi(line); } catch(...) {}
        if (std::getline(fin, line)) try { in_repair = (std::stoi(line) == 1); } catch(...) {}
        if (std::getline(fin, line)) try { is_used = (std::stoi(line) == 1); } catch(...) {}
        
        if (id > 0) {
            pipes.emplace(id, Pipe(id, name, length, diameter, in_repair, is_used));
            next_pipe_id = std::max(next_pipe_id, id + 1);
        }
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
        
        if (id > 0) {
            CS new_cs(id, name, total, in_op);
            
            int conn_count = 0;
            if (std::getline(fin, line)) try { conn_count = std::stoi(line); } catch(...) {}

            for (int j = 0; j < conn_count; ++j) {
                if (std::getline(fin, line)) {
                    std::stringstream ss(line);
                    int dest_id, pipe_id;
                    if (ss >> dest_id >> pipe_id) {
                        new_cs.addConnection(dest_id, pipe_id);
                    }
                }
            }
            stations.emplace(id, new_cs);
            next_cs_id = std::max(next_cs_id, id + 1);
        }
    }

    fin.close();
    std::cout << "Данные успешно загружены из " << filename << std::endl;
}

// --- Реализации вспомогательных методов ---
std::vector<int> Manager::findPipeIdsByName(const std::string& name) {
    std::vector<int> ids;
    for (const auto& pair : pipes) {
        if (pair.second.getName().find(name) != std::string::npos) {
            ids.push_back(pair.first);
        }
    }
    return ids;
}

std::vector<int> Manager::findPipeIdsByRepairStatus(bool status) {
    std::vector<int> ids;
    for (const auto& pair : pipes) {
        if (pair.second.isInRepair() == status) {
            ids.push_back(pair.first);
        }
    }
    return ids;
}

std::vector<int> Manager::findCSIdsByName(const std::string& name) {
    std::vector<int> ids;
    for (const auto& pair : stations) {
        if (pair.second.getName().find(name) != std::string::npos) {
            ids.push_back(pair.first);
        }
    }
    return ids;
}

std::vector<int> Manager::findCSIdsByUnusedPercent(double percent) {
    std::vector<int> ids;
    for (const auto& pair : stations) {
        if (pair.second.getUnusedPercent() >= percent) {
            ids.push_back(pair.first);
        }
    }
    return ids;
}