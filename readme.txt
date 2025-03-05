Лабораторная работа 1: dirwalk

Сборка:
    make debug    # Компиляция с флагами отладки
    make release  # Компиляция с флагами релизной версии
    make clean    # Очистка файлов сборки

Запуск:
    ./build/debug/dirwalk . -f   # Вывод всех файлов в текущей директории
    ./build/debug/dirwalk . -d   # Вывод всех каталогов
    ./build/debug/dirwalk . -s   # Сортировка
