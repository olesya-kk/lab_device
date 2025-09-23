# ==========================================================
# CMakeLists.txt для проекта lab_device
# ==========================================================
# @file CMakeLists.txt
# @brief Конфигурация сборки проекта C++ с классом ComplexReactor и тестами GoogleTest.
#
# Основные задачи:
# - Создать статическую библиотеку reactor из ComplexReactor.cpp
# - Подключить заголовочные файлы
# - Настроить GoogleTest через FetchContent
# - Создать исполняемый файл тестов и подключить библиотеку и gtest
# - Включить поддержку CTest для автоматического запуска тестов
# ==========================================================

cmake_minimum_required(VERSION 3.14)  # Минимальная версия CMake

project(lab_device LANGUAGES CXX)     # Название проекта и используемые языки

# ----------------------------
# Настройки стандарта C++
# ----------------------------
set(CMAKE_CXX_STANDARD 17)            # Используем C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Требовать поддержки C++17 компилятором

# ----------------------------
# Создание библиотеки reactor
# ----------------------------
add_library(reactor STATIC src/ComplexReactor.cpp)
# Указываем директорию с заголовочными файлами
target_include_directories(reactor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ----------------------------
# Подключение GoogleTest
# ----------------------------
include(FetchContent)                 # Используем FetchContent для автоматической загрузки
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# Не менять настройки компилятора/линкера родительского проекта
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)  # Загружаем и делаем доступным GTest

# ----------------------------
# Включаем поддержку тестирования
# ----------------------------
enable_testing()  # Включаем CTest

# ----------------------------
# Создание исполняемого файла с тестами
# ----------------------------
add_executable(tests
  tests/TestComplexReactor.cpp
)
# Линкуем библиотеку reactor и GoogleTest
target_link_libraries(tests PRIVATE reactor gtest_main)

# ----------------------------
# Регистрируем тест для CTest
# ----------------------------
add_test(NAME AllTests COMMAND tests)  # Все тесты запускаются через исполняемый файл "tests"


# Добавляем флаги для gcc/g++
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()
