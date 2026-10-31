#include <gtest/gtest.h>
#include <sstream>
#include "Forecast.h"

const std::time_t DAY = 86400;
const std::time_t JAN1 = 1704067200;
const std::time_t FEB1 = JAN1 + 31 * DAY;

DailyForecast make(std::time_t date, double t, Phenomenon p = SUNNY, double precip = 0) {
    return DailyForecast(date, t, t, t, p, precip);
}

TEST(DailyForecast, DefaultConstructor) {
    DailyForecast d;
    EXPECT_EQ(d.getDate(), 0);
    EXPECT_DOUBLE_EQ(d.getDayTemp(), 0);
    EXPECT_EQ(d.getPhenomenon(), SUNNY);
    EXPECT_DOUBLE_EQ(d.getPrecipitation(), 0);
}

TEST(DailyForecast, FullConstructorAndGetters) {
    DailyForecast d(JAN1, 1, 2, 3, RAIN, 4.5);
    EXPECT_EQ(d.getDate(), JAN1);
    EXPECT_DOUBLE_EQ(d.getMorningTemp(), 1);
    EXPECT_DOUBLE_EQ(d.getDayTemp(), 2);
    EXPECT_DOUBLE_EQ(d.getEveningTemp(), 3);
    EXPECT_EQ(d.getPhenomenon(), RAIN);
    EXPECT_DOUBLE_EQ(d.getPrecipitation(), 4.5);
}

TEST(DailyForecast, ConstructorValidation) {
    EXPECT_THROW(DailyForecast(0, -274, 0, 0, SUNNY, 0), std::invalid_argument);
    EXPECT_THROW(DailyForecast(0, 0, -274, 0, SUNNY, 0), std::invalid_argument);
    EXPECT_THROW(DailyForecast(0, 0, 0, -274, SUNNY, 0), std::invalid_argument);
    EXPECT_THROW(DailyForecast(0, 0, 0, 0, SUNNY, -1), std::invalid_argument);
    EXPECT_THROW(DailyForecast(-1, 0, 0, 0, SUNNY, 0), std::invalid_argument);
    EXPECT_NO_THROW(DailyForecast(0, -273, 0, 0, SUNNY, 0));
}

TEST(DailyForecast, AutoPhenomenon) {
    EXPECT_EQ(DailyForecast(0, 5, 5, 5, 0.0).getPhenomenon(), SUNNY);
    EXPECT_EQ(DailyForecast(0, -5, -5, -5, 2.0).getPhenomenon(), SNOW);
    EXPECT_EQ(DailyForecast(0, 10, 10, 10, 2.0).getPhenomenon(), RAIN);
    EXPECT_THROW(DailyForecast(0, -300, 0, 0, 1.0), std::invalid_argument);
}

TEST(DailyForecast, Setters) {
    DailyForecast d;
    d.setDate(JAN1);
    d.setMorningTemp(-1);
    d.setDayTemp(2);
    d.setEveningTemp(3);
    d.setPhenomenon(CLOUDY);
    d.setPrecipitation(7);
    EXPECT_EQ(d.getDate(), JAN1);
    EXPECT_DOUBLE_EQ(d.getMorningTemp(), -1);
    EXPECT_DOUBLE_EQ(d.getDayTemp(), 2);
    EXPECT_DOUBLE_EQ(d.getEveningTemp(), 3);
    EXPECT_EQ(d.getPhenomenon(), CLOUDY);
    EXPECT_DOUBLE_EQ(d.getPrecipitation(), 7);
    EXPECT_THROW(d.setDate(-5), std::invalid_argument);
    EXPECT_THROW(d.setMorningTemp(-300), std::invalid_argument);
    EXPECT_THROW(d.setDayTemp(-300), std::invalid_argument);
    EXPECT_THROW(d.setEveningTemp(-300), std::invalid_argument);
    EXPECT_THROW(d.setPrecipitation(-1), std::invalid_argument);
    EXPECT_DOUBLE_EQ(d.getMorningTemp(), -1);
}

TEST(DailyForecast, AverageTemperature) {
    DailyForecast d(0, 0, 3, 6, SUNNY, 0);
    EXPECT_DOUBLE_EQ(d.averageTemperature(), 3);
}

TEST(DailyForecast, IsErroneous) {
    EXPECT_FALSE(make(0, 10).isErroneous());
    EXPECT_TRUE(DailyForecast(0, 10, 10, 10, SUNNY, 1).isErroneous());
    EXPECT_TRUE(DailyForecast(0, 10, 10, 10, CLOUDY, 1).isErroneous());
    EXPECT_FALSE(DailyForecast(0, 10, 10, 10, RAIN, 1).isErroneous());
    EXPECT_TRUE(DailyForecast(0, -5, 1, -5, SNOW, 1).isErroneous());
    EXPECT_FALSE(DailyForecast(0, -5, 0, -5, SNOW, 1).isErroneous());
    EXPECT_TRUE(make(0, 61).isErroneous());
    EXPECT_TRUE(make(0, -101).isErroneous());
    EXPECT_TRUE(DailyForecast(0, 1, 1, 1, RAIN, 1501).isErroneous());
}

TEST(DailyForecast, PlusEqual) {
    DailyForecast a(JAN1, 0, 10, 20, CLOUDY, 0);
    DailyForecast b(JAN1 + 100, 10, 20, 30, RAIN, 4);
    a += b;
    EXPECT_DOUBLE_EQ(a.getMorningTemp(), 5);
    EXPECT_DOUBLE_EQ(a.getDayTemp(), 15);
    EXPECT_DOUBLE_EQ(a.getEveningTemp(), 25);
    EXPECT_DOUBLE_EQ(a.getPrecipitation(), 2);
    EXPECT_EQ(a.getPhenomenon(), RAIN);
    EXPECT_EQ(a.getDate(), JAN1);
    EXPECT_THROW(a += make(JAN1 + DAY, 0), std::invalid_argument);
}

TEST(DailyForecast, Comparison) {
    DailyForecast a = make(JAN1, 0);
    DailyForecast b = make(JAN1 + DAY, 0);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a == make(JAN1, 5));
    EXPECT_TRUE(a != b);
}

TEST(DailyForecast, DateParts) {
    DailyForecast d = make(FEB1 + 3600, 0);
    EXPECT_EQ(d.getYear(), 2024);
    EXPECT_EQ(d.getMonth(), 2);
    EXPECT_TRUE(d.sameDay(make(FEB1 + 80000, 0)));
    EXPECT_FALSE(d.sameDay(make(FEB1 + DAY, 0)));
}

TEST(DailyForecast, Streams) {
    DailyForecast d(JAN1, -1.5, 2, 3, RAIN, 4);
    std::stringstream ss;
    ss << d;
    DailyForecast r;
    ss >> r;
    EXPECT_FALSE(ss.fail());
    EXPECT_EQ(r.getDate(), JAN1);
    EXPECT_DOUBLE_EQ(r.getMorningTemp(), -1.5);
    EXPECT_EQ(r.getPhenomenon(), RAIN);
    EXPECT_DOUBLE_EQ(r.getPrecipitation(), 4);
}

TEST(DailyForecast, BadStreamSetsFailbitAndKeepsObject) {
    DailyForecast d = make(JAN1, 7);
    std::istringstream bad1("1 2 3 4 9 5");
    bad1 >> d;
    EXPECT_TRUE(bad1.fail());
    std::istringstream bad2("1 -500 3 4 2 5");
    bad2 >> d;
    EXPECT_TRUE(bad2.fail());
    std::istringstream bad3("abc");
    bad3 >> d;
    EXPECT_TRUE(bad3.fail());
    EXPECT_EQ(d.getDate(), JAN1);
    EXPECT_DOUBLE_EQ(d.getDayTemp(), 7);
}

TEST(WeatherForecast, Constructors) {
    WeatherForecast empty;
    EXPECT_EQ(empty.size(), 0u);
    DailyForecast arr[] = {make(JAN1, 0), make(JAN1 + DAY, 1), make(JAN1 + 2 * DAY, 2)};
    WeatherForecast two(arr, 2);
    EXPECT_EQ(two.size(), 2u);
    EXPECT_EQ(WeatherForecast(nullptr, 0).size(), 0u);
    EXPECT_THROW(WeatherForecast(nullptr, 1), std::invalid_argument);
    WeatherForecast one{make(JAN1, 0)};
    EXPECT_EQ(one.size(), 1u);
}

TEST(WeatherForecast, CopyAndMove) {
    WeatherForecast a{make(JAN1, 1)};
    WeatherForecast b = a;
    b += make(JAN1 + DAY, 2);
    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(b.size(), 2u);
    WeatherForecast c = std::move(b);
    EXPECT_EQ(c.size(), 2u);
}

TEST(WeatherForecast, AddAndIndex) {
    WeatherForecast w;
    w += make(JAN1, 1);
    w += make(JAN1 + DAY, 2);
    EXPECT_EQ(w.size(), 2u);
    EXPECT_DOUBLE_EQ(w[1].getDayTemp(), 2);
    w[1].setDayTemp(9);
    EXPECT_DOUBLE_EQ(w[1].getDayTemp(), 9);
    const WeatherForecast& cw = w;
    EXPECT_DOUBLE_EQ(cw[0].getDayTemp(), 1);
    EXPECT_THROW(w[2], std::out_of_range);
    EXPECT_THROW(cw[2], std::out_of_range);
}

TEST(WeatherForecast, RemoveAt) {
    WeatherForecast w;
    w += make(JAN1, 1);
    w += make(JAN1 + DAY, 2);
    w.removeAt(0);
    EXPECT_EQ(w.size(), 1u);
    EXPECT_DOUBLE_EQ(w[0].getDayTemp(), 2);
    EXPECT_THROW(w.removeAt(5), std::out_of_range);
}

TEST(WeatherForecast, FindColdest) {
    WeatherForecast w;
    w += make(JAN1, 5);
    w += make(JAN1 + DAY, -10);
    w += make(JAN1 + 2 * DAY, -20);
    EXPECT_DOUBLE_EQ(w.findColdest(JAN1, JAN1 + DAY).getDayTemp(), -10);
    EXPECT_DOUBLE_EQ(w.findColdest(JAN1, JAN1 + 2 * DAY).getDayTemp(), -20);
    EXPECT_THROW(w.findColdest(JAN1 + DAY, JAN1), std::invalid_argument);
    EXPECT_THROW(w.findColdest(JAN1 + 10 * DAY, JAN1 + 11 * DAY), std::runtime_error);
}

TEST(WeatherForecast, FindNearestSunny) {
    WeatherForecast w;
    EXPECT_THROW(w.findNearestSunny(JAN1), std::runtime_error);
    w += make(JAN1, 0, CLOUDY);
    EXPECT_THROW(w.findNearestSunny(JAN1), std::runtime_error);
    w += make(JAN1 + DAY, 1);
    w += make(JAN1 + 10 * DAY, 2);
    EXPECT_DOUBLE_EQ(w.findNearestSunny(JAN1).getDayTemp(), 1);
    EXPECT_DOUBLE_EQ(w.findNearestSunny(JAN1 + 9 * DAY).getDayTemp(), 2);
}

TEST(WeatherForecast, RemoveErroneous) {
    WeatherForecast w;
    w += make(JAN1, 10);
    w += make(JAN1 + DAY, 100);
    w += DailyForecast(JAN1 + 2 * DAY, 1, 1, 1, SUNNY, 5);
    EXPECT_EQ(w.removeErroneous(), 2u);
    EXPECT_EQ(w.size(), 1u);
    EXPECT_EQ(w.removeErroneous(), 0u);
}

TEST(WeatherForecast, MergeDuplicates) {
    WeatherForecast w;
    w += DailyForecast(JAN1 + 100, 0, 0, 0, SUNNY, 0);
    w += make(JAN1 + DAY, 5);
    w += DailyForecast(JAN1 + 200, 6, 6, 6, RAIN, 3);
    w += DailyForecast(JAN1 + 300, 12, 12, 12, CLOUDY, 0);
    w.mergeDuplicates();
    ASSERT_EQ(w.size(), 2u);
    EXPECT_DOUBLE_EQ(w[0].getDayTemp(), 6);
    EXPECT_DOUBLE_EQ(w[0].getPrecipitation(), 1);
    EXPECT_EQ(w[0].getPhenomenon(), RAIN);
    EXPECT_DOUBLE_EQ(w[1].getDayTemp(), 5);
    WeatherForecast empty;
    empty.mergeDuplicates();
    EXPECT_EQ(empty.size(), 0u);
}

TEST(WeatherForecast, SortByDate) {
    WeatherForecast w;
    w += make(JAN1 + 2 * DAY, 3);
    w += make(JAN1, 1);
    w += make(JAN1 + DAY, 2);
    w.sortByDate();
    EXPECT_DOUBLE_EQ(w[0].getDayTemp(), 1);
    EXPECT_DOUBLE_EQ(w[1].getDayTemp(), 2);
    EXPECT_DOUBLE_EQ(w[2].getDayTemp(), 3);
}

TEST(WeatherForecast, GetMonth) {
    WeatherForecast w;
    w += make(FEB1 + DAY, 3);
    w += make(JAN1 + DAY, 2);
    w += make(JAN1, 1);
    WeatherForecast jan = w.getMonth(2024, 1);
    ASSERT_EQ(jan.size(), 2u);
    EXPECT_DOUBLE_EQ(jan[0].getDayTemp(), 1);
    EXPECT_DOUBLE_EQ(jan[1].getDayTemp(), 2);
    EXPECT_EQ(w.getMonth(2024, 3).size(), 0u);
    EXPECT_EQ(w.getMonth(2023, 1).size(), 0u);
    EXPECT_THROW(w.getMonth(2024, 13), std::invalid_argument);
    EXPECT_THROW(w.getMonth(2024, 0), std::invalid_argument);
}

TEST(WeatherForecast, Streams) {
    WeatherForecast w;
    w += make(JAN1, 1);
    w += DailyForecast(JAN1 + DAY, -3, -2, -1, SNOW, 2);
    std::stringstream ss;
    ss << w;
    WeatherForecast r;
    ss >> r;
    ASSERT_EQ(r.size(), 2u);
    EXPECT_EQ(r[1].getPhenomenon(), SNOW);
    EXPECT_DOUBLE_EQ(r[1].getMorningTemp(), -3);
}

TEST(WeatherForecast, BadStreamKeepsObject) {
    WeatherForecast w{make(JAN1, 1)};
    std::istringstream bad("2\n1 1 1 1 0 0\n");
    bad >> w;
    EXPECT_TRUE(bad.fail());
    EXPECT_EQ(w.size(), 1u);
    std::istringstream bad2("abc");
    bad2 >> w;
    EXPECT_TRUE(bad2.fail());
}
