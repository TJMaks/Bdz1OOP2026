#pragma once
#include <cstddef>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <vector>

enum Phenomenon { SUNNY = 0, CLOUDY = 1, RAIN = 2, SNOW = 3 };

class DailyForecast {
public:
    DailyForecast();
    DailyForecast(std::time_t date, double morning, double day, double evening,
                  Phenomenon phenomenon, double precipitation);
    DailyForecast(std::time_t date, double morning, double day, double evening,
                  double precipitation);

    std::time_t getDate() const;
    double getMorningTemp() const;
    double getDayTemp() const;
    double getEveningTemp() const;
    Phenomenon getPhenomenon() const;
    double getPrecipitation() const;

    void setDate(std::time_t date);
    void setMorningTemp(double value);
    void setDayTemp(double value);
    void setEveningTemp(double value);
    void setPhenomenon(Phenomenon value);
    void setPrecipitation(double value);

    DailyForecast& operator+=(const DailyForecast& other);
    double averageTemperature() const;
    bool isErroneous() const;

    bool operator<(const DailyForecast& other) const;
    bool operator==(const DailyForecast& other) const;
    bool operator!=(const DailyForecast& other) const;
    bool operator>(const DailyForecast& other) const;
    bool operator<=(const DailyForecast& other) const;
    bool operator>=(const DailyForecast& other) const;

    bool sameDay(const DailyForecast& other) const;
    int getYear() const;
    int getMonth() const;
    void print(std::ostream& out) const;
    void read(std::istream& in);

private:
    static void checkTemp(double t);
    static void checkPrecip(double p);
    void check() const;
    static Phenomenon choosePhenomenon(double morning, double day, double evening,
                                       double precipitation);

    std::time_t date_;
    double morning_;
    double day_;
    double evening_;
    Phenomenon phenomenon_;
    double precipitation_;
};

std::ostream& operator<<(std::ostream& out, const DailyForecast& f);
std::istream& operator>>(std::istream& in, DailyForecast& f);

class WeatherForecast {
public:
    WeatherForecast();
    WeatherForecast(const DailyForecast* arr, std::size_t count);
    explicit WeatherForecast(const DailyForecast& day);

    std::size_t size() const;
    WeatherForecast& operator+=(const DailyForecast& day);
    DailyForecast& operator[](std::size_t index);
    const DailyForecast& operator[](std::size_t index) const;
    void removeAt(std::size_t index);

    const DailyForecast& findColdest(std::time_t from, std::time_t to) const;
    const DailyForecast& findNearestSunny(std::time_t date) const;
    std::size_t removeErroneous();
    void mergeDuplicates();
    void sortByDate();
    WeatherForecast getMonth(int year, int month) const;

    void print(std::ostream& out) const;
    void read(std::istream& in);

private:
    std::vector<DailyForecast> days_;
};

std::ostream& operator<<(std::ostream& out, const WeatherForecast& w);
std::istream& operator>>(std::istream& in, WeatherForecast& w);
