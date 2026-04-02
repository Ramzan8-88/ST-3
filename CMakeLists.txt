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

# Google Test + Google Mock (tests use gmock)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests test/tests.cpp src/TimedDoor.cpp)
if(TARGET GTest::gmock)
  target_link_libraries(tests PRIVATE GTest::GTest GTest::gmock Threads::Threads)
else()
  target_link_libraries(tests PRIVATE ${GTEST_LIBRARIES} gmock Threads::Threads)
endif()

# Добавляем тест в CTest
add_test(NAME TimedDoorTests COMMAND tests)