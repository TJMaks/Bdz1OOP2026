#include "Forecast.h"
#include <algorithm>
#include <stdexcept>

const std::time_t SECONDS_IN_DAY = 86400;

void DailyForecast::checkTemp(double t) {
    if (!(t >= -273)) {
        throw std::invalid_argument("Температура не может быть меньше -273");
    }
}

void DailyForecast::checkPrecip(double p) {
    if (!(p >= 0)) {
        throw std::invalid_argument("Осадки не могут быть отрицательными");
    }
}

void DailyForecast::check() const {
    if (date_ < 0) {
        throw std::invalid_argument("Дата не может быть отрицательной");
    }
    checkTemp(morning_);
    checkTemp(day_);
    checkTemp(evening_);
    checkPrecip(precipitation_);
}

Phenomenon DailyForecast::choosePhenomenon(double morning, double day, double evening,
                                           double precipitation) {
    if (precipitation <= 0) {
        return SUNNY;
    }
    double average = (morning + day + evening) / 3;
    if (average <= 0) {
        return SNOW;
    }
    return RAIN;
}

DailyForecast::DailyForecast()
    : date_(0), morning_(0), day_(0), evening_(0), phenomenon_(SUNNY), precipitation_(0) {}

DailyForecast::DailyForecast(std::time_t date, double morning, double day, double evening,
                             Phenomenon phenomenon, double precipitation)
    : date_(date), morning_(morning), day_(day), evening_(evening),
      phenomenon_(phenomenon), precipitation_(precipitation) {
    check();
}

DailyForecast::DailyForecast(std::time_t date, double morning, double day, double evening,
                             double precipitation)
    : date_(date), morning_(morning), day_(day), evening_(evening),
      phenomenon_(choosePhenomenon(morning, day, evening, precipitation)),
      precipitation_(precipitation) {
    check();
}

std::time_t DailyForecast::getDate() const { return date_; }
double DailyForecast::getMorningTemp() const { return morning_; }
double DailyForecast::getDayTemp() const { return day_; }
double DailyForecast::getEveningTemp() const { return evening_; }
Phenomenon DailyForecast::getPhenomenon() const { return phenomenon_; }
double DailyForecast::getPrecipitation() const { return precipitation_; }

void DailyForecast::setDate(std::time_t date) {
    if (date < 0) throw std::invalid_argument("Дата не может быть отрицательной");
    date_ = date;
}

void DailyForecast::setMorningTemp(double value) {
    checkTemp(value);
    morning_ = value;
}

void DailyForecast::setDayTemp(double value) {
    checkTemp(value);
    day_ = value;
}

void DailyForecast::setEveningTemp(double value) {
    checkTemp(value);
    evening_ = value;
}

void DailyForecast::setPhenomenon(Phenomenon value) { phenomenon_ = value; }

void DailyForecast::setPrecipitation(double value) {
    checkPrecip(value);
    precipitation_ = value;
}

DailyForecast& DailyForecast::operator+=(const DailyForecast& other) {
    if (!sameDay(other)) {
        throw std::invalid_argument("Нельзя объединять прогнозы за разные сутки");
    }
    morning_ = (morning_ + other.morning_) / 2;
    day_ = (day_ + other.day_) / 2;
    evening_ = (evening_ + other.evening_) / 2;
    precipitation_ = (precipitation_ + other.precipitation_) / 2;
    phenomenon_ = std::max(phenomenon_, other.phenomenon_);
    return *this;
}

double DailyForecast::averageTemperature() const {
    return (morning_ + day_ + evening_) / 3;
}

bool DailyForecast::isErroneous() const {
    if ((phenomenon_ == SUNNY || phenomenon_ == CLOUDY) && precipitation_ > 0) return true;
    if (phenomenon_ == SNOW && (morning_ > 0 || day_ > 0 || evening_ > 0)) return true;
    if (morning_ < -100 || morning_ > 60 || day_ < -100 || day_ > 60 || evening_ < -100 || evening_ > 60) return true;
    if (precipitation_ > 1500) return true;
    return false;
}

bool DailyForecast::operator<(const DailyForecast& other) const {
    return date_ < other.date_;
}

bool DailyForecast::operator==(const DailyForecast& other) const {
    return date_ == other.date_;
}

bool DailyForecast::operator!=(const DailyForecast& other) const {
    return !(*this == other);
}

bool DailyForecast::operator>(const DailyForecast& other) const {
    return other < *this;
}

bool DailyForecast::operator<=(const DailyForecast& other) const {
    return !(other < *this);
}

bool DailyForecast::operator>=(const DailyForecast& other) const {
    return !(*this < other);
}

bool DailyForecast::sameDay(const DailyForecast& other) const {
    return date_ / SECONDS_IN_DAY == other.date_ / SECONDS_IN_DAY;
}

int DailyForecast::getYear() const {
    std::time_t t = date_;
    std::tm* parts = std::gmtime(&t);
    if (parts == nullptr) throw std::runtime_error("Не удалось разобрать дату");
    return parts->tm_year + 1900;
}

int DailyForecast::getMonth() const {
    std::time_t t = date_;
    std::tm* parts = std::gmtime(&t);
    if (parts == nullptr) throw std::runtime_error("Не удалось разобрать дату");
    return parts->tm_mon + 1;
}

void DailyForecast::print(std::ostream& out) const {
    out << date_ << ' ' << morning_ << ' ' << day_ << ' ' << evening_ << ' ' << phenomenon_ << ' ' << precipitation_;
}

void DailyForecast::read(std::istream& in) {
    std::time_t date = 0;
    double morning = 0, day = 0, evening = 0, precipitation = 0;
    int phenomenon = 0;
    in >> date >> morning >> day >> evening >> phenomenon >> precipitation;
    if (!in) return;
    if (phenomenon < 0 || phenomenon > 3) {
        in.setstate(std::ios::failbit);
        return;
    }
    try {
        DailyForecast temp(date, morning, day, evening, static_cast<Phenomenon>(phenomenon), precipitation);
        *this = temp;
    } catch (const std::invalid_argument&) {
        in.setstate(std::ios::failbit);
    }
}

std::ostream& operator<<(std::ostream& out, const DailyForecast& f) {
    f.print(out);
    return out;
}

std::istream& operator>>(std::istream& in, DailyForecast& f) {
    f.read(in);
    return in;
}
