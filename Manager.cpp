// Реализация класса Manager

#include "Manager.h"
#include "Utils.h" // Ради getValidInput
#include <fstream>
#include <algorithm> // для max и sort
#include <sstream>
#include <vector>
#include <queue>
#include <map>

// Теперь меню - это приватный метод класса
void Manager::printMenu() {
    std::cout << "\n========= МЕНЮ =========\n"
              << "1. Добавить трубу\n"
              << "2. Добавить КС\n"
              << "3. Просмотр всех объектов\n"
              << "4. Работа с объектами (поиск, редактирование, удаление)\n"
              << "5. Сохранить данные\n"
              << "6. Загрузить данные\n"
              << "7. Соединить КС\n"
              << "8. Топологическая сортировка\n"
              << "9. Расчет максимального потока\n"
              << "10. Поиск кратчайшего пути\n"
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
            case 7: connect_stations(); break;
            case 8: topological_sort(); break;
            case 9: calculateMaxFlow(); break;
            case 10: findShortestPath(); break;
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
        if (isValidDiameter(diameter)) break;
        std::cout << "Ошибка: Диаметр должен быть одним из следующих: 500, 700, 1000, 1400.\n";
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

void Manager::connect_stations() {
    std::cout << "\n--- Соединение компрессорных станций ---\n";
    
    int src_id = getValidInput<int>("Введите ID исходной КС: ");
    log(std::to_string(src_id));
    if (stations.find(src_id) == stations.end()) {
        std::cout << "Ошибка: КС с ID " << src_id << " не найдена.\n";
        return;
    }

    int dest_id = getValidInput<int>("Введите ID конечной КС: ");
    log(std::to_string(dest_id));
    if (stations.find(dest_id) == stations.end()) {
        std::cout << "Ошибка: КС с ID " << dest_id << " не найдена.\n";
        return;
    }

    if (src_id == dest_id) {
        std::cout << "Ошибка: Нельзя соединить КС саму с собой.\n";
        return;
    }

    if (stations.at(src_id).getOutgoingConnections().count(dest_id)) {
        std::cout << "Ошибка: Эти КС уже соединены.\n";
        return;
    }

    std::cout << "Доступные диаметры: 500, 700, 1000, 1400 мм.\n";
    int diameter;
     while (true) {
        diameter = getValidInput<int>("Введите желаемый диаметр трубы: ");
        log(std::to_string(diameter));
        if (isValidDiameter(diameter)) break;
        std::cout << "Ошибка: Диаметр должен быть одним из следующих: 500, 700, 1000, 1400.\n";
    }

    int pipe_id_to_use = -1;
    for (auto& pair : pipes) {
        if (!pair.second.isUsed() && pair.second.getDiameter() == diameter) {
            pipe_id_to_use = pair.first;
            break;
        }
    }

    if (pipe_id_to_use != -1) {
        stations.at(src_id).addConnection(dest_id, pipe_id_to_use);
        pipes.at(pipe_id_to_use).setUsed(true);
        std::cout << "Станции успешно соединены трубой с ID " << pipe_id_to_use << ".\n";
    } else {
        std::cout << "Свободной трубы с диаметром " << diameter << " мм не найдено.\n";
        std::cout << "Хотите создать новую трубу? (1 - да, 0 - нет): ";
        int choice = getValidInput<int>("");
        log(std::to_string(choice));
        if (choice == 1) {
            addPipe();
        }
    }
}

void Manager::topological_sort() {
    std::cout << "\n--- Топологическая сортировка графа КС ---\n";

    std::map<int, int> in_degree;
    for (const auto& pair : stations) {
        in_degree[pair.first] = 0;
    }

    for (const auto& pair : stations) {
        for (const auto& conn : pair.second.getOutgoingConnections()) {
            in_degree[conn.first]++;
        }
    }

    std::queue<int> q;
    for (const auto& pair : in_degree) {
        if (pair.second == 0) {
            q.push(pair.first);
        }
    }

    std::vector<int> result;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(u);

        if (stations.count(u)) {
            for (const auto& conn : stations.at(u).getOutgoingConnections()) {
                int v = conn.first;
                in_degree[v]--;
                if (in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }
    }

    if (result.size() < stations.size()) {
        std::cout << "Ошибка: В графе обнаружен цикл! Топологическая сортировка невозможна.\n";
    } else {
        std::cout << "Результат топологической сортировки:\n";
        for (size_t i = 0; i < result.size(); ++i) {
            std::cout << stations.at(result[i]).getName() << (i == result.size() - 1 ? "" : " -> ");
        }
        std::cout << std::endl;
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
            // Перед удалением трубы разрываем соединение, которое она образует
            for (auto& station_pair : stations) {
                CS& station = station_pair.second; 
                int dest_cs_id_to_remove = -1;
                for (const auto& conn : station.getOutgoingConnections()) {
                    if (conn.second == id) { // conn.second - это pipe_id
                        dest_cs_id_to_remove = conn.first;
                        break;
                    }
                }
                if (dest_cs_id_to_remove != -1) {
                    station.removeConnection(dest_cs_id_to_remove);
                    std::cout << "Разорвано соединение, использующее трубу ID " << id 
                              << " (от КС ID " << station.getId() << " к КС ID " << dest_cs_id_to_remove << ").\n";
                    break; // Труба может использоваться только в одном соединении
                }
            }
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
            bool is_connected = false;
            try {
                // Проверка на исходящие соединения
                if (!stations.at(id).getOutgoingConnections().empty()) {
                    is_connected = true;
                }
                // Проверка на входящие соединения
                if (!is_connected) {
                    for (const auto& station_pair : stations) {
                        if (station_pair.first == id) continue;
                        if (station_pair.second.getOutgoingConnections().count(id)) {
                            is_connected = true;
                            break;
                        }
                    }
                }

                if (is_connected) {
                    std::cout << "КС ID " << id << " является частью сети. Удаление приведет к разрыву соединений.\n";
                    std::cout << "Вы уверены, что хотите продолжить? (1 - да, 0 - нет): ";
                    int confirm = getValidInput<int>("");
                    log(std::to_string(confirm));
                    if (confirm != 1) {
                        std::cout << "Удаление КС ID " << id << " отменено.\n";
                        continue; // Переход к следующему ID
                    }
                }

                // 1. Освобождаем трубы, которые шли ИЗ удаляемой КС
                const auto& outgoing_conns = stations.at(id).getOutgoingConnections();
                for (const auto& conn : outgoing_conns) {
                    if (pipes.count(conn.second)) {
                        pipes.at(conn.second).setUsed(false);
                        std::cout << "Труба ID " << conn.second << " освобождена.\n";
                    }
                }

                // 2. Удаляем входящие соединения В удаляемую КС из других станций
                for (auto& station_pair : stations) {
                    if (station_pair.first == id) continue;
                    CS& other_station = station_pair.second;
                    if (other_station.getOutgoingConnections().count(id)) {
                        int pipe_id_to_free = other_station.getOutgoingConnections().at(id);
                        other_station.removeConnection(id);
                        if (pipes.count(pipe_id_to_free)) {
                            pipes.at(pipe_id_to_free).setUsed(false);
                            std::cout << "Разорвано входящее соединение с КС ID " << station_pair.first
                                      << " (труба ID " << pipe_id_to_free << " освобождена).\n";
                        }
                    }
                }

                stations.erase(id);
                std::cout << "КС ID " << id << " успешно удалена.\n";

            } catch (const std::out_of_range& oor) {
                 std::cout << "Ошибка: КС с ID " << id << " не найдена для удаления (возможно, уже удалена в этой сессии).\n";
            }
        }
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

void Manager::calculateMaxFlow() {
    std::cout << "\n--- Расчет максимального потока ---\n";
    if (stations.size() < 2) {
        std::cout << "Для расчета потока необходимо как минимум 2 компрессорные станции.\n";
        return;
    }

    int source = getValidInput<int>("Введите ID исходной КС (исток): ");
    log(std::to_string(source));
    if (stations.find(source) == stations.end()) {
        std::cout << "Ошибка: КС с ID " << source << " не найдена.\n";
        return;
    }

    int sink = getValidInput<int>("Введите ID конечной КС (сток): ");
    log(std::to_string(sink));
    if (stations.find(sink) == stations.end()) {
        std::cout << "Ошибка: КС с ID " << sink << " не найдена.\n";
        return;
    }

    if (source == sink) {
        std::cout << "Ошибка: Исток и сток не могут совпадать.\n";
        return;
    }

    // Создание остаточной сети (residual graph)
    std::map<int, std::map<int, double>> residual_graph;
    for (const auto& station_pair : stations) {
        int u = station_pair.first;
        for (const auto& connection : station_pair.second.getOutgoingConnections()) {
            int v = connection.first;
            int pipe_id = connection.second;
            if (pipes.count(pipe_id)) {
                double capacity = calculateCapacity(pipes.at(pipe_id));
                residual_graph[u][v] = capacity;
                // Инициализируем обратное ребро с 0 пропускной способностью
                if (residual_graph.find(v) == residual_graph.end() || residual_graph[v].find(u) == residual_graph[v].end()) {
                    residual_graph[v][u] = 0;
                }
            }
        }
    }

    std::map<int, int> parent;
    double max_flow = 0;

    // Пока существует увеличивающий путь от истока к стоку
    while (bfs_for_max_flow(residual_graph, source, sink, parent)) {
        double path_flow = std::numeric_limits<double>::max();
        // Находим минимальную остаточную пропускную способность на пути
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            path_flow = std::min(path_flow, residual_graph[u][v]);
        }

        // Обновляем остаточные пропускные способности ребер
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residual_graph[u][v] -= path_flow;
            residual_graph[v][u] += path_flow;
        }

        // Добавляем поток этого пути к общему максимальному потоку
        max_flow += path_flow;
    }

    std::cout << "Максимальный поток от КС " << source << " к КС " << sink << " составляет: " << max_flow << " условных единиц.\n";
}

bool Manager::bfs_for_max_flow(const std::map<int, std::map<int, double>>& residual_graph, int s, int t, std::map<int, int>& parent) {
    std::map<int, bool> visited;
    for(const auto& pair : stations) {
        visited[pair.first] = false;
    }

    std::queue<int> q;
    q.push(s);
    visited[s] = true;
    parent.clear();
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (residual_graph.count(u)) {
             for (const auto& pair : residual_graph.at(u)) {
                int v = pair.first;
                double capacity = pair.second;
                if (!visited[v] && capacity > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }
    }

    // Если мы смогли посетить сток, значит, путь найден
    return (visited[t] == true);
}


void Manager::findShortestPath() {
    std::cout << "\n--- Поиск кратчайшего пути (Алгоритм Дейкстры) ---\n";
    if (stations.size() < 2) {
        std::cout << "Для поиска пути необходимо как минимум 2 компрессорные станции.\n";
        return;
    }

    int start_node = getValidInput<int>("Введите ID начальной КС: ");
    log(std::to_string(start_node));
    if (stations.find(start_node) == stations.end()) {
        std::cout << "Ошибка: КС с ID " << start_node << " не найдена.\n";
        return;
    }

    int end_node = getValidInput<int>("Введите ID конечной КС: ");
    log(std::to_string(end_node));
    if (stations.find(end_node) == stations.end()) {
        std::cout << "Ошибка: КС с ID " << end_node << " не найдена.\n";
        return;
    }
    
    if (start_node == end_node) {
        std::cout << "Ошибка: Начальная и конечная КС не могут совпадать.\n";
        return;
    }

    std::map<int, double> dist;
    std::map<int, int> prev;
    for (const auto& pair : stations) {
        dist[pair.first] = std::numeric_limits<double>::max();
    }
    dist[start_node] = 0;

    using pii = std::pair<double, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;
    pq.push({0.0, start_node});

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) {
            continue;
        }
        if (u == end_node) break; // Оптимизация: останавливаемся, как только добрались до цели

        if (stations.count(u)) {
            for (const auto& conn : stations.at(u).getOutgoingConnections()) {
                int v = conn.first;
                int pipe_id = conn.second;

                if (pipes.count(pipe_id)) {
                    const Pipe& p = pipes.at(pipe_id);
                    double weight = p.isInRepair() ? std::numeric_limits<double>::max() : p.getLength();

                    if (dist[u] != std::numeric_limits<double>::max() && dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        prev[v] = u;
                        pq.push({dist[v], v});
                    }
                }
            }
        }
    }

    if (dist[end_node] == std::numeric_limits<double>::max()) {
        std::cout << "Путь от КС " << start_node << " до КС " << end_node << " не найден.\n";
    } else {
        std::cout << "Кратчайший путь от КС " << start_node << " до КС " << end_node << " имеет длину: " << dist[end_node] << " км.\n";
        std::cout << "Путь: ";
        std::vector<int> path;
        for (int at = end_node; at != 0; at = prev.count(at) ? prev[at] : 0) {
            path.push_back(at);
             if (at == start_node) break;
        }
        std::reverse(path.begin(), path.end());
        
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << stations.at(path[i]).getName() << (i == path.size() - 1 ? "" : " -> ");
        }
        std::cout << std::endl;
    }
}


double Manager::calculateCapacity(const Pipe& p) {
    if (p.isInRepair() || p.getLength() <= 0) {
        return 0.0;
    }
    // Формула: sqrt(d^5/l) * C
    // Переводим диаметр из мм в м
    double diameter_m = p.getDiameter() / 1000.0;
    // Переводим длину из км в м
    double length_m = p.getLength() * 1000.0;
    // Используем поправочный коэффициент, чтобы получить разумные числа
    double capacity = sqrt(pow(diameter_m, 5) / length_m) * 10000; 
    return capacity;
}
