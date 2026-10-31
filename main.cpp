#include <ctime>
#include <iostream>
#include "Forecast.h"

bool badInput() {
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Неверный ввод\n";
        return true;
    }
    return false;
}

int main() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n1 - создать прогноз\n"
                  << "2 - вывести прогноз\n"
                  << "3 - средняя температура\n"
                  << "4 - проверка на ошибочность\n"
                  << "5 - сравнить два прогноза\n"
                  << "6 - объединить два прогноза\n"
                  << "0 - выход\n"
                  << "Выбор: ";
        std::cin >> choice;
        if (std::cin.eof()) break;
        if (badInput()) { choice = -1; continue; }

        try {
            switch (choice) {
            case 1: {
                std::cout << "Введите: дата утро день вечер явление(0-3) осадки\n";
                DailyForecast d;
                std::cin >> d;
                if (badInput()) break;
                std::cout << "Создан: " << d << '\n';
                break;
            }
            case 2: {
                std::cout << "Введите прогноз: ";
                DailyForecast d;
                std::cin >> d;
                if (badInput()) break;
                std::cout << d << '\n';
                break;
            }
            case 3: {
                std::cout << "Введите прогноз: ";
                DailyForecast d;
                std::cin >> d;
                if (badInput()) break;
                std::cout << "Средняя температура: " << d.averageTemperature() << '\n';
                break;
            }
            case 4: {
                std::cout << "Введите прогноз: ";
                DailyForecast d;
                std::cin >> d;
                if (badInput()) break;
                std::cout << "Ошибочный: " << (d.isErroneous() ? "да" : "нет") << '\n';
                break;
            }
            case 5: {
                std::cout << "Два прогноза:\n";
                DailyForecast a, b;
                std::cin >> a >> b;
                if (badInput()) break;
                std::cout << "a < b: " << ((a < b) ? "да" : "нет") << '\n';
                std::cout << "a == b: " << ((a == b) ? "да" : "нет") << '\n';
                std::cout << "a > b: " << ((a > b) ? "да" : "нет") << '\n';
                break;
            }
            case 6: {
                std::cout << "Два прогноза за одни сутки:\n";
                DailyForecast a, b;
                std::cin >> a >> b;
                if (badInput()) break;
                a += b;
                std::cout << "После объединения: " << a << '\n';
                break;
            }
            case 0: break;
            default:
                std::cout << "Нет такого пункта\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << '\n';
        }
    }
    return 0;
}
