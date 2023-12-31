#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <Windows.h>
#include <locale>
#include <regex>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <cctype>

class Advertisement {
private:
    std::string product;
    std::string startDate;
    int duration;
    std::string availableSlots;
    std::string price;

public:
    Advertisement(const std::string& product, const std::string& startDate, const std::string& duration,
        const std::string& availableSlots, const std::string& price)
        : product(product), startDate(startDate), availableSlots(availableSlots), price(price) {
 
        this->duration = std::stoi(duration);
    }

    std::string getProduct() const {
        return product;
    }

    std::string getStartDate() const {
        return startDate;
    }

    int getDuration() const {
        return duration;
    }

    std::string getAvailableSlots() const {
        return availableSlots;
    }

    std::string getPrice() const {
        return price;
    }

    bool operator<(const Advertisement& other) const {
        return product < other.product;
    }

    void display() const {
        std::cout << "Продукт: " << product << std::endl;
        std::cout << "Дата начала: " << startDate << std::endl;
        std::cout << "Продолжительность: " << duration << " дней" << std::endl;
        std::cout << "Доступные слоты: " << availableSlots << std::endl;
        std::cout << "Цена: " << price << " рублей" << std::endl;
    }

    bool containsSubstring(const std::string& substring) const {
        std::string lowerCaseData = product + startDate + std::to_string(duration) +
            availableSlots + price;

        std::string lowerCaseSubstring = substring;
        std::transform(lowerCaseData.begin(), lowerCaseData.end(), lowerCaseData.begin(),
            [](unsigned char c) -> char { return std::tolower(c, std::locale("")); });
        std::transform(lowerCaseSubstring.begin(), lowerCaseSubstring.end(), lowerCaseSubstring.begin(),
            [](unsigned char c) -> char { return std::tolower(c, std::locale("")); });

        return lowerCaseData.find(lowerCaseSubstring) != std::string::npos;
    }
};

#include <iostream>
#include <sstream>
#include <ctime>

bool isValidDate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    if (ss.fail()) {
        std::cout << "Ошибка формата даты. Пожалуйста, введите дату в формате гггг-мм-дд.\n";
        return false;
    }

    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;
    int day = tm.tm_mday;

    if (month < 1 || month > 12) {
        std::cout << "Некорректный месяц. Месяц должен быть в пределах от 1 до 12.\n";
        return false;
    }

    int daysInMonth;
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        daysInMonth = 31;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
    }
    else if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth = 29; // Високосный год
    }
    else {
        daysInMonth = 28;
    }

    if (day < 1 || day > daysInMonth) {
        std::cout << "Некорректный день для выбранного месяца.\n";
        return false;
    }

    return true;
}

int main() {
    system("chcp 1251");
    
    int sortField = 0;

    std::vector<Advertisement> advertisements;

    std::ifstream inputFile("advertisements_data.txt");
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::stringstream ss(line);
            std::string product, startDate, duration, availableSlots, price;

            std::getline(ss, product, ',');
            std::getline(ss, startDate, ',');
            std::getline(ss, duration, ',');
            std::getline(ss, availableSlots, ',');
            std::getline(ss, price, ',');

            advertisements.emplace_back(product, startDate, duration, availableSlots, price);
        }
        inputFile.close();
    }
    else {
        std::cout << "Ошибка открытия файла advertisements_data.txt" << std::endl;
        return 1;
    }

    int choice = 0;
    while (choice != 6) {
        std::cout << "1. Поиск рекламы\n";
        std::cout << "2. Сортировка рекламы\n";
        std::cout << "3. Вывод всех объявлений\n";
        std::cout << "4. Добавить рекламу\n";
        std::cout << "5. Сохранить в файл CSV\n";
        std::cout << "6. Выход\n";
        std::cout << "Введите номер пункта меню: ";
        while (!(std::cin >> choice)) {
            std::cout << "Некорректный ввод. Пожалуйста, введите число: ";
            std::cin.clear();  // очистка флагов ошибок
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');  // очистка буфера ввода
        }
        if (choice == 1) {
            std::string searchQuery;
            std::cout << "Введите запрос для поиска: ";
            std::cin >> searchQuery;

            bool found = false;
            for (const auto& advertisement : advertisements) {
                if (advertisement.containsSubstring(searchQuery)) {
                    advertisement.display();
                    found = true;
                }
            }

            if (!found) {
                std::cout << "Реклама с такими данными не найдена.\n";
            }
        }
        else if (choice == 2) {
            // Добавлен выбор поля для сортировки
            std::cout << "Выберите поле для сортировки:\n";
            std::cout << "1. Продукт\n";
            std::cout << "2. Дата начала\n";
            std::cout << "3. Продолжительность\n";
            std::cout << "4. Доступные слоты\n";
            std::cout << "5. Цена\n";
            std::cin >> sortField;

            switch (sortField) {
            case 1:
                std::sort(advertisements.begin(), advertisements.end(), [](const Advertisement& a, const Advertisement& b) {
                    return a.getProduct() < b.getProduct();
                    });
                break;
            case 2:
                std::sort(advertisements.begin(), advertisements.end(), [](const Advertisement& a, const Advertisement& b) {
                    return a.getStartDate() < b.getStartDate();
                    });
                break;
            case 3:
                std::sort(advertisements.begin(), advertisements.end(), [](const Advertisement& a, const Advertisement& b) {
                    return a.getDuration() < b.getDuration();
                    });
                break;
            case 4:
                std::sort(advertisements.begin(), advertisements.end(), [](const Advertisement& a, const Advertisement& b) {
                    return a.getAvailableSlots() < b.getAvailableSlots();
                    });
                break;
            case 5:
                std::sort(advertisements.begin(), advertisements.end(), [](const Advertisement& a, const Advertisement& b) {
                    return a.getPrice() < b.getPrice();
                    });
                break;
            default:
                std::cout << "Неверный выбор поля сортировки.\n";
                break;
            }
            std::cout << "Реклама отсортирована.\n";
        }
        else if (choice == 3) {
            for (const auto& advertisement : advertisements) {
                advertisement.display();
                std::cout << "--------------------------\n";
            }
        }
        else if (choice == 4) {
            std::string product, startDate, duration, availableSlots, price;
            std::cin.ignore();

            // Проверка корректности ввода продукта
            do {
                std::cout << "Введите продукт рекламы: ";
                std::getline(std::cin, product);
            } while (product.empty());  // Повторять ввод, пока строка не будет непустой

            // Проверка корректности ввода даты
            do {
                std::cout << "Введите дату начала (гггг-мм-дд): ";
                std::getline(std::cin, startDate);
            } while (!isValidDate(startDate));

            // Проверка корректности ввода длительности (положительное число)
            // Проверка корректности ввода длительности (положительное число)
            while (true) {
                try {
                    std::cout << "Введите продолжительность (в днях): ";
                    std::getline(std::cin, duration);
                    if (std::all_of(duration.begin(), duration.end(), ::isdigit)) {
                        int durationValue = std::stoi(duration);
                        if (durationValue > 0) {
                            break;
                        }
                        else {
                            std::cout << "Длительность должна быть положительным числом. Пожалуйста, введите корректное значение.\n";
                        }
                    }
                    else {
                        std::cout << "Некорректный ввод. Пожалуйста, введите число для длительности.\n";
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Некорректный ввод. Пожалуйста, введите число для длительности.\n";
                }
            }

            // Проверка корректности ввода доступных слотов
            do {
                std::cout << "Введите количество доступных слотов: ";
                std::getline(std::cin, availableSlots);
            } while (availableSlots.empty() || !std::all_of(availableSlots.begin(), availableSlots.end(), ::isdigit));

            // Проверка корректности ввода цены
            do {
                std::cout << "Введите цену рекламы: ";
                std::getline(std::cin, price);
            } while (price.empty() || !std::all_of(price.begin(), price.end(), ::isdigit));

            Advertisement newAdvertisement(product, startDate, duration, availableSlots, price);
            advertisements.push_back(newAdvertisement);
            std::cout << "Реклама добавлена.\n";

            // Открытие файла для добавления данных новой рекламы
            std::ofstream outputFile("advertisements_data.txt", std::ios::out | std::ios::app);
            if (outputFile.is_open()) {
                // Запись данных новой рекламы в файл
                outputFile << newAdvertisement.getProduct() << "," << newAdvertisement.getStartDate() << ","
                    << newAdvertisement.getDuration() << "," << newAdvertisement.getAvailableSlots() << ","
                    << newAdvertisement.getPrice() << "\n";
                outputFile.close();
                std::cout << "Реклама добавлена и данные обновлены в файле.\n";
            }
            else {
                std::cout << "Ошибка открытия файла для записи.\n";
            }
        }
        else if (choice == 5) {
            std::string fileName;
            std::cout << "Введите название файла : ";
            std::cin >> fileName;

            std::ofstream csvFile(fileName);
            if (csvFile.is_open()) {
                for (const auto& advertisement : advertisements) {
                    csvFile << advertisement.getProduct() << " ; "
                        << advertisement.getStartDate() << " ; "
                        << advertisement.getDuration() << " ; "
                        << advertisement.getAvailableSlots() << " ; "
                        << advertisement.getPrice() << "\n";
                }
                csvFile.close();
                std::cout << "Данные сохранены в файле " << fileName << "\n";
            }
            else {
                std::cout << "Ошибка открытия файла для сохранения CSV.\n";
            }
        }
        else if (choice == 6) {
            break;
        }
        else {
            std::cout << "Неверный выбор. Пожалуйста, попробуйте снова.\n";
        }
        system("pause");
        system("cls");  // Очистка консоли для следующего вывода меню
    }

    return 0;
}
