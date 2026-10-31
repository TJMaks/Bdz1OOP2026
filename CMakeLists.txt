cmake_minimum_required(VERSION 3.16)
project(WeatherForecast CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(forecast_app main.cpp DailyForecast.cpp WeatherForecast.cpp)
