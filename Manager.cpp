// Реализация класса Manager

#include "Manager.h"
#include <fstream>
#include <algorithm> // для max
#include <sstream>

Manager::Manager() : next_pipe_id(1), next_cs_id(1), logger("log.txt") {}

void Manager::addPipe() {
    logger.log("Пользователь выбрал 'Добавить трубу'.");
    pipes.push_back(Pipe::createPipe(next_pipe_id++));
    std::cout << "Труба успешно добавлена!\n";
    logger.log("Новая труба с ID " + std::to_string(next_pipe_id - 1) + " создана.");
}

void Manager::addCS() {
    logger.log("Пользователь выбрал 'Добавить КС'.");
    stations.push_back(CS::createCS(next_cs_id++));
    std::cout << "КС успешно добавлена!\n";
    logger.log("Новая КС с ID " + std::to_string(next_cs_id - 1) + " создана.");
}

void Manager::viewAllObjects() {
    logger.log("Пользователь выбрал 'Просмотр всех объектов'.");
    std::cout << "\n========== ВСЕ ТРУБЫ ==========\n";
    if (pipes.empty()) {
        std::cout << "Трубы отсутствуют.\n";
    } else {
        for (const auto& p : pipes) {
            std::cout << p << std::endl;
        }
    }
    std::cout << "\n========== ВСЕ КС ==========\n";
    if (stations.empty()) {
        std::cout << "КС отсутствуют.\n";
    } else {
        for (const auto& cs : stations) {
            std::cout << cs << std::endl;
        }
    }
}

void Manager::deletePipe() {
    logger.log("Пользователь выбрал 'Удалить трубу'.");
    int id_to_delete = getValidInput<int>("Введите ID трубы для удаления: ");
    for (auto it = pipes.begin(); it != pipes.end(); ++it) {
        if (it->getId() == id_to_delete) {
            pipes.erase(it);
            std::cout << "Труба с ID " << id_to_delete << " успешно удалена.\n";
            logger.log("Труба с ID " + std::to_string(id_to_delete) + " удалена.");
            return;
        }
    }
    std::cout << "Труба с ID " << id_to_delete << " не найдена.\n";
    logger.log("Попытка удаления несуществующей трубы с ID " + std::to_string(id_to_delete) + ".");
}

void Manager::deleteCS() {
    logger.log("Пользователь выбрал 'Удалить КС'.");
    int id_to_delete = getValidInput<int>("Введите ID КС для удаления: ");
    for (auto it = stations.begin(); it != stations.end(); ++it) {
        if (it->getId() == id_to_delete) {
            stations.erase(it);
            std::cout << "КС с ID " << id_to_delete << " успешно удалена.\n";
            logger.log("КС с ID " + std::to_string(id_to_delete) + " удалена.");
            return;
        }
    }
    std::cout << "КС с ID " << id_to_delete << " не найдена.\n";
    logger.log("Попытка удаления несуществующей КС с ID " + std::to_string(id_to_delete) + ".");
}


void Manager::findPipes() {
    logger.log("Пользователь выбрал 'Поиск труб'.");
    std::cout << "Критерии поиска:\n1. По названию\n2. По статусу 'в ремонте'\n";
    int choice = getValidInput<int>("Выберите критерий: ");
    
    std::vector<int> found_indices;
    if (choice == 1) {
        std::string name = getValidInput<std::string>("Введите название для поиска: ");
        logger.log("Поиск труб по названию: '" + name + "'.");
        found_indices = findPipeIndicesByName(name);
    } else if (choice == 2) {
        bool status = getValidInput<int>("Искать в ремонте (1) или в эксплуатации (0)? ");
        logger.log("Поиск труб по статусу: " + std::string(status ? "в ремонте" : "в эксплуатации") + ".");
        found_indices = findPipeIndicesByRepairStatus(status);
    } else {
        std::cout << "Неверный выбор.\n";
        return;
    }

    if (found_indices.empty()) {
        std::cout << "Трубы по вашему запросу не найдены.\n";
    } else {
        std::cout << "Найденные трубы:\n";
        for (int index : found_indices) {
            std::cout << pipes[index] << std::endl;
        }
    }
}

void Manager::findCSs() {
    logger.log("Пользователь выбрал 'Поиск КС'.");
    std::cout << "Критерии поиска:\n1. По названию\n2. По проценту незадействованных цехов\n";
    int choice = getValidInput<int>("Выберите критерий: ");

    bool found = false;
    if (choice == 1) {
        std::string name = getValidInput<std::string>("Введите название для поиска: ");
        logger.log("Поиск КС по названию: '" + name + "'.");
        for (const auto& cs : stations) {
            if (cs.getName() == name) {
                std::cout << cs << std::endl;
                found = true;
            }
        }
    } else if (choice == 2) {
        double percent = getValidInput<double>("Введите процент незадействованных цехов для поиска: ");
        logger.log("Поиск КС по проценту незадействованных цехов: " + std::to_string(percent) + "%.");
        for (const auto& cs : stations) {
            if (cs.getUnusedPercent() >= percent) {
                std::cout << cs << std::endl;
                found = true;
            }
        }
    } else {
        std::cout << "Неверный выбор.\n";
        return;
    }

    if (!found) {
        std::cout << "КС по вашему запросу не найдены.\n";
    }
}

void Manager::batchEditPipes() {
    logger.log("Пользователь выбрал 'Пакетное редактирование труб'.");
    std::cout << "Сначала нужно найти трубы для редактирования.\n";
    std::cout << "Критерии поиска:\n1. По названию\n2. По статусу 'в ремонте'\n";
    int choice = getValidInput<int>("Выберите критерий: ");

    std::vector<int> found_indices;
    if (choice == 1) {
        std::string name = getValidInput<std::string>("Введите название для поиска: ");
        found_indices = findPipeIndicesByName(name);
    } else if (choice == 2) {
        bool status = getValidInput<int>("Искать в ремонте (1) или в эксплуатации (0)? ");
        found_indices = findPipeIndicesByRepairStatus(status);
    } else {
        std::cout << "Неверный выбор.\n";
        return;
    }
    
    if (found_indices.empty()) {
        std::cout << "Трубы для редактирования не найдены.\n";
        logger.log("Пакетное редактирование отменено: трубы не найдены.");
        return;
    }

    std::cout << "Найденные трубы:\n";
    for (size_t i = 0; i < found_indices.size(); ++i) {
        std::cout << i + 1 << ". " << pipes[found_indices[i]].getName() 
                  << " (ID: " << pipes[found_indices[i]].getId() << ")\n";
    }

    std::cout << "\nВведите номера труб для редактирования (через пробел, например '1 3') или 'all' для всех: ";
    std::string selection = getValidInput<std::string>("");
    
    bool new_status = getValidInput<int>("Установить статус 'в ремонте' (1) или 'в эксплуатации' (0)? ");

    if (selection == "all") {
        logger.log("Редактирование статуса всех найденных труб.");
        for (int index : found_indices) {
            pipes[index].setRepairStatus(new_status);
        }
    } else {
        std::stringstream ss(selection);
        int num;
        std::string log_ids = "";
        while (ss >> num) {
            if (num > 0 && num <= found_indices.size()) {
                pipes[found_indices[num - 1]].setRepairStatus(new_status);
                log_ids += std::to_string(pipes[found_indices[num - 1]].getId()) + " ";
            }
        }
         logger.log("Редактирование статуса труб с ID: " + log_ids);
    }

    std::cout << "Статус выбранных труб успешно изменен.\n";
}


void Manager::saveData() {
    logger.log("Пользователь выбрал 'Сохранить'.");
    std::string filename = getValidInput<std::string>("Введите имя файла для сохранения: ");
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл для записи.\n";
        logger.log("Ошибка сохранения: не удалось открыть файл " + filename);
        return;
    }
    
    // --- Сохраняем трубы ---
    fout << pipes.size() << std::endl;
    for (const auto& p : pipes) {
        fout << p.getId() << std::endl;
        fout << p.getName() << std::endl;
        fout << p.getLength() << std::endl;
        fout << p.getDiameter() << std::endl;
        fout << p.isInRepair() << std::endl;
    }

    // --- Сохраняем КС ---
    fout << stations.size() << std::endl;
    for (const auto& cs : stations) {
        fout << cs.getId() << std::endl;
        fout << cs.getName() << std::endl;
        fout << cs.getWorkshopsTotal() << std::endl;
        fout << cs.getWorkshopsInOperation() << std::endl;
    }

    fout.close();
    std::cout << "Данные успешно сохранены в " << filename << std::endl;
    logger.log("Данные сохранены в файл: " + filename);
}

void Manager::loadData() {
    logger.log("Пользователь выбрал 'Загрузить'.");
    std::string filename = getValidInput<std::string>("Введите имя файла для загрузки: ");
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл для чтения.\n";
        logger.log("Ошибка загрузки: не удалось открыть файл " + filename);
        return;
    }
    
    pipes.clear();
    stations.clear();
    std::string line;

    // --- Загрузка труб ---
    int pipe_count = 0;
    if (std::getline(fin, line)) {
        try { pipe_count = std::stoi(line); } catch(...) { /* ошибка формата */ }
    }

    for (int i = 0; i < pipe_count; ++i) {
        if (!fin.good()) { // Проверка, что файл не закончился и не в состоянии ошибки
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
    
    // --- Загрузка КС ---
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
    logger.log("Данные загружены из файла: " + filename);
}

// --- Реализации вспомогательных методов ---
std::vector<int> Manager::findPipeIndicesByName(const std::string& name) {
    std::vector<int> indices;
    for (size_t i = 0; i < pipes.size(); ++i) {
        if (pipes[i].getName() == name) {
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