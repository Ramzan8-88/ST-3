cmake_minimum_required(VERSION 3.10)
project(TimedDoorSystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем директории с заголовками
include_directories(include)

# Основной исполняемый файл
add_executable(timed_door src/main.cpp src/TimedDoor.cpp)

# Тесты
enable_testing()

# Ищем Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Исполняемый файл для тестов
add_executable(tests test/tests.cpp src/TimedDoor.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)

# Добавляем тест в CTest
add_test(NAME TimedDoorTests COMMAND tests)