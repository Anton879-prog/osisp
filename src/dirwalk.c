/*#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#define MAXPATHLEN 4096

int flag_l = 0; // Флаг для символических ссылок
int flag_d = 0; // Флаг для каталогов
int flag_f = 0; // Флаг для обычных файлов
int flag_s = 0; // Флаг для сортировки

void add_entry(const char *path) {
    printf("%s\n", path); // Выводим путь
}

int dirwalk(char *path) {
    DIR *d;
    struct dirent *dir;
    struct stat sb;
    char fullpath[MAXPATHLEN];
    int total = 0;

    if ((d = opendir(path)) == NULL) {
        perror("opendir");
        return 0;
    }
    
    while ((dir = readdir(d))) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }

        // Формируем полный путь
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, dir->d_name);

        // Получаем информацию о файле
        if (stat(fullpath, &sb) == -1) {
            perror("stat");
            continue;
        }

        // Фильтруем по типам в зависимости от флагов
        if (flag_l && S_ISLNK(sb.st_mode)) {
            add_entry(fullpath); // Символические ссылки
        } else if (flag_d && S_ISDIR(sb.st_mode)) {
            add_entry(fullpath); // Каталоги
        } else if (flag_f && S_ISREG(sb.st_mode)) {
            add_entry(fullpath); // Обычные файлы
        } else if (!flag_l && !flag_d && !flag_f) {
            // Если флаги не указаны, выводим всё
            add_entry(fullpath);
        }

        // Если это каталог, рекурсивно обрабатываем его
        if (S_ISDIR(sb.st_mode)) {
            total += dirwalk(fullpath);
        } else {
            total++;
        }
    }
    
    closedir(d);
    return total;
}

int main(int argc, char *argv[]) {
    int opt;
    char *start_dir = "."; // По умолчанию начинаем с текущей директории

    // Обрабатываем флаги
    while ((opt = getopt(argc, argv, "ldfs")) != -1) {
        switch (opt) {
            case 'l':
                flag_l = 1;
                break;
            case 'd':
                flag_d = 1;
                break;
            case 'f':
                flag_f = 1;
                break;
            case 's':
                flag_s = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-d] [-f] [-s] [dir]\n", argv[0]);
                return 1;
        }
    }

    // Если в аргументах есть директория, используем её, иначе текущая директория
    if (optind < argc) {
        start_dir = argv[optind];
    }

    // Если включен флаг сортировки, добавляем сортировку
    if (flag_s) {
        // Тут можно добавить код сортировки, если нужно
    }

    // Запускаем обход директории
    printf("Scanning directory: %s\n", start_dir);
    int total = dirwalk(start_dir);
    printf("Total items: %d\n", total);
    
    return 0;
}
*/ 
#define _XOPEN_SOURCE 700
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>


#define MAXPATHLEN 4096
#define MAX_ENTRIES 1000 // Максимальное количество файлов для сортировки

int flag_l = 0; // Флаг для символических ссылок
int flag_d = 0; // Флаг для каталогов
int flag_f = 0; // Флаг для обычных файлов
int flag_s = 0; // Флаг для сортировки

// Массив для хранения найденных файлов (если нужна сортировка)
char *entries[MAX_ENTRIES];
int entry_count = 0;

// Функция добавления в массив (если включена сортировка)
void add_entry(const char *path) {
    if (flag_s) {
        if (entry_count < MAX_ENTRIES) {
            entries[entry_count++] = strdup(path);
        }
    } else {
        printf("%s\n", path);
    }
}

// Функция сравнения для qsort()
int compare_entries(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Основная функция обхода директорий
int dirwalk(const char *path) {
    DIR *d;
    struct dirent *dir;
    struct stat sb;
    char fullpath[MAXPATHLEN];
    int total = 0;

    if ((d = opendir(path)) == NULL) {
        perror("opendir");
        return 0;
    }
    
    while ((dir = readdir(d))) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }

        // Формируем полный путь
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, dir->d_name);

        // Получаем информацию о файле
        if (lstat(fullpath, &sb) == -1) { // lstat позволяет корректно работать с символическими ссылками
            perror("lstat");
            continue;
        }

        // Фильтруем по типу файла в зависимости от флагов
        if ((flag_l && S_ISLNK(sb.st_mode)) || 
            (flag_d && S_ISDIR(sb.st_mode)) || 
            (flag_f && S_ISREG(sb.st_mode)) || 
            (!flag_l && !flag_d && !flag_f)) {
            add_entry(fullpath);
        }

        // Рекурсивный вызов для каталогов
        if (S_ISDIR(sb.st_mode)) {
            total += dirwalk(fullpath);
        } else {
            total++;
        }
    }

    closedir(d);
    return total;
}

int main(int argc, char *argv[]) {
    int opt;
    const char *start_dir = "."; // По умолчанию текущая директория

    // Разбираем аргументы командной строки
    while ((opt = getopt(argc, argv, "ldfs")) != -1) {
        switch (opt) {
            case 'l': flag_l = 1; break;
            case 'd': flag_d = 1; break;
            case 'f': flag_f = 1; break;
            case 's': flag_s = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-d] [-f] [-s] [dir]\n", argv[0]);
                return 1;
        }
    }

    // Если передана директория, используем её
    if (optind < argc) {
        start_dir = argv[optind];
    }

    printf("Scanning directory: %s\n", start_dir);
    int total = dirwalk(start_dir);

    // Если включена сортировка, сортируем и выводим
    if (flag_s) {
        qsort(entries, entry_count, sizeof(char *), compare_entries);
        for (int i = 0; i < entry_count; i++) {
            printf("%s\n", entries[i]);
            free(entries[i]); // Освобождаем память
        }
    }

    printf("Total items: %d\n", total);
    return 0;
}
