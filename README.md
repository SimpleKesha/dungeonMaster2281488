Ниже — решение задачи 1 варианта 7: процедурная реализация битового множества на C.

Выбор слова: uint64_t.
Почему: одно слово обрабатывает 64 элемента за одну операцию; 64 — степень двойки, поэтому номер слова и номер бита считаются как value >> 6 и value & 63; тип беззнаковый, чтобы сдвиги и маски не зависели от знаковости. Хвост последнего слова закрывается маской last_mask, чтобы биты за границей диапазона оставались нулевыми.

bitset.h

```c
#ifndef BITSET_H
#define BITSET_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    BITSET_OK = 0,
    BITSET_ERR_INVALID_ARGUMENT,
    BITSET_ERR_OUT_OF_RANGE,
    BITSET_ERR_RANGE_MISMATCH,
    BITSET_ERR_NO_MEMORY,
    BITSET_ERR_NOT_FOUND
} BitSetError;

typedef struct {
    uint64_t *words;
    size_t range_size;
    size_t word_count;
    uint64_t last_mask;
} BitSet;

BitSetError bitset_create(size_t range_size, BitSet *out_set);
void bitset_destroy(BitSet *set);

BitSetError bitset_add(BitSet *set, size_t value);
BitSetError bitset_remove(BitSet *set, size_t value);
BitSetError bitset_contains(const BitSet *set, size_t value, bool *out_contains);

BitSetError bitset_union(const BitSet *a, const BitSet *b, BitSet *result);
BitSetError bitset_intersection(const BitSet *a, const BitSet *b, BitSet *result);
BitSetError bitset_difference(const BitSet *a, const BitSet *b, BitSet *result);
BitSetError bitset_complement(BitSet *set);

size_t bitset_count(const BitSet *set);
void bitset_clear(BitSet *set);
void bitset_fill(BitSet *set);

#endif
```

bitset.c

```c
#include "bitset.h"
#include <stdlib.h>

static uint64_t make_last_mask(size_t range_size) {
    size_t rem = range_size % 64;
    if (range_size == 0) {
        return 0;
    }
    if (rem == 0) {
        return UINT64_MAX;
    }
    return (UINT64_C(1) << rem) - 1;
}

static bool words_are_valid(const BitSet *set) {
    return set != NULL && (set->word_count == 0 || set->words != NULL);
}

static unsigned popcount64(uint64_t x) {
    unsigned count = 0;
    while (x != 0) {
        x &= x - 1;
        ++count;
    }
    return count;
}

static BitSetError check_value(const BitSet *set, size_t value) {
    if (!words_are_valid(set)) {
        return BITSET_ERR_INVALID_ARGUMENT;
    }
    if (value >= set->range_size) {
        return BITSET_ERR_OUT_OF_RANGE;
    }
    return BITSET_OK;
}

static BitSetError check_binary(const BitSet *a, const BitSet *b, BitSet *result) {
    if (!words_are_valid(a) || !words_are_valid(b) || !words_are_valid(result)) {
        return BITSET_ERR_INVALID_ARGUMENT;
    }
    if (a->range_size != b->range_size || a->range_size != result->range_size) {
        return BITSET_ERR_RANGE_MISMATCH;
    }
    return BITSET_OK;
}

BitSetError bitset_create(size_t range_size, BitSet *out_set) {
    if (out_set == NULL) {
        return BITSET_ERR_INVALID_ARGUMENT;
    }

    out_set->words = NULL;
    out_set->range_size = range_size;
    out_set->word_count = range_size / 64 + (range_size % 64 != 0 ? 1 : 0);
    out_set->last_mask = make_last_mask(range_size);

    if (out_set->word_count > 0) {
        out_set->words = (uint64_t *)calloc(out_set->word_count, sizeof(uint64_t));
        if (out_set->words == NULL) {
            return BITSET_ERR_NO_MEMORY;
        }
    }

    return BITSET_OK;
}

void bitset_destroy(BitSet *set) {
    if (set == NULL) {
        return;
    }
    free(set->words);
    set->words = NULL;
    set->range_size = 0;
    set->word_count = 0;
    set->last_mask = 0;
}

BitSetError bitset_add(BitSet *set, size_t value) {
    BitSetError err = check_value(set, value);
    if (err != BITSET_OK) {
        return err;
    }

    size_t word_index = value >> 6;
    size_t bit_index = value & 63;
    set->words[word_index] |= (UINT64_C(1) << bit_index);
    return BITSET_OK;
}

BitSetError bitset_remove(BitSet *set, size_t value) {
    BitSetError err = check_value(set, value);
    if (err != BITSET_OK) {
        return err;
    }

    size_t word_index = value >> 6;
    size_t bit_index = value & 63;
    uint64_t mask = UINT64_C(1) << bit_index;

    if ((set->words[word_index] & mask) == 0) {
        return BITSET_ERR_NOT_FOUND;
    }

    set->words[word_index] &= ~mask;
    return BITSET_OK;
}

BitSetError bitset_contains(const BitSet *set, size_t value, bool *out_contains) {
    if (out_contains == NULL) {
        return BITSET_ERR_INVALID_ARGUMENT;
    }

    BitSetError err = check_value(set, value);
    if (err != BITSET_OK) {
        return err;
    }

    size_t word_index = value >> 6;
    size_t bit_index = value & 63;
    *out_contains = (set->words[word_index] & (UINT64_C(1) << bit_index)) != 0;
    return BITSET_OK;
}

BitSetError bitset_union(const BitSet *a, const BitSet *b, BitSet *result) {
    BitSetError err = check_binary(a, b, result);
    if (err != BITSET_OK) {
        return err;
    }

    for (size_t i = 0; i < a->word_count; ++i) {
        result->words[i] = a->words[i] | b->words[i];
    }
    if (result->word_count > 0) {
        result->words[result->word_count - 1] &= result->last_mask;
    }
    return BITSET_OK;
}

BitSetError bitset_intersection(const BitSet *a, const BitSet *b, BitSet *result) {
    BitSetError err = check_binary(a, b, result);
    if (err != BITSET_OK) {
        return err;
    }

    for (size_t i = 0; i < a->word_count; ++i) {
        result->words[i] = a->words[i] & b->words[i];
    }
    if (result->word_count > 0) {
        result->words[result->word_count - 1] &= result->last_mask;
    }
    return BITSET_OK;
}

BitSetError bitset_difference(const BitSet *a, const BitSet *b, BitSet *result) {
    BitSetError err = check_binary(a, b, result);
    if (err != BITSET_OK) {
        return err;
    }

    for (size_t i = 0; i < a->word_count; ++i) {
        result->words[i] = a->words[i] & ~b->words[i];
    }
    if (result->word_count > 0) {
        result->words[result->word_count - 1] &= result->last_mask;
    }
    return BITSET_OK;
}

BitSetError bitset_complement(BitSet *set) {
    if (!words_are_valid(set)) {
        return BITSET_ERR_INVALID_ARGUMENT;
    }

    for (size_t i = 0; i < set->word_count; ++i) {
        set->words[i] = ~set->words[i];
    }
    if (set->word_count > 0) {
        set->words[set->word_count - 1] &= set->last_mask;
    }
    return BITSET_OK;
}

size_t bitset_count(const BitSet *set) {
    if (!words_are_valid(set) || set->word_count == 0) {
        return 0;
    }

    size_t total = 0;
    for (size_t i = 0; i < set->word_count; ++i) {
        uint64_t word = set->words[i];
        if (i + 1 == set->word_count) {
            word &= set->last_mask;
        }
        total += popcount64(word);
    }
    return total;
}

void bitset_clear(BitSet *set) {
    if (!words_are_valid(set)) {
        return;
    }
    for (size_t i = 0; i < set->word_count; ++i) {
        set->words[i] = 0;
    }
}

void bitset_fill(BitSet *set) {
    if (!words_are_valid(set)) {
        return;
    }
    for (size_t i = 0; i < set->word_count; ++i) {
        set->words[i] = UINT64_MAX;
    }
    if (set->word_count > 0) {
        set->words[set->word_count - 1] &= set->last_mask;
    }
}
```

demo.c

```c
#include "bitset.h"
#include <stdio.h>

static const char *err_str(BitSetError err) {
    switch (err) {
        case BITSET_OK: return "OK";
        case BITSET_ERR_INVALID_ARGUMENT: return "INVALID_ARGUMENT";
        case BITSET_ERR_OUT_OF_RANGE: return "OUT_OF_RANGE";
        case BITSET_ERR_RANGE_MISMATCH: return "RANGE_MISMATCH";
        case BITSET_ERR_NO_MEMORY: return "NO_MEMORY";
        case BITSET_ERR_NOT_FOUND: return "NOT_FOUND";
        default: return "UNKNOWN";
    }
}

static void print_contains(const BitSet *set, size_t value) {
    bool result = false;
    BitSetError err = bitset_contains(set, value, &result);
    if (err == BITSET_OK) {
        printf("contains(%zu) = %s\n", value, result ? "true" : "false");
    } else {
        printf("contains(%zu) -> %s\n", value, err_str(err));
    }
}

#define CREATE_OR_RETURN(var, size)                                      \
    do {                                                                 \
        BitSetError e_ = bitset_create((size), &(var));                  \
        if (e_ != BITSET_OK) {                                           \
            fprintf(stderr, "create failed: %s\n", err_str(e_));         \
            return 1;                                                    \
        }                                                                \
    } while (0)

int main(void) {
    const size_t N = 130;

    BitSet a, b, empty, single, uni, inter, diff, comp, other;

    CREATE_OR_RETURN(a, N);
    CREATE_OR_RETURN(b, N);
    CREATE_OR_RETURN(empty, N);
    CREATE_OR_RETURN(single, N);
    CREATE_OR_RETURN(uni, N);
    CREATE_OR_RETURN(inter, N);
    CREATE_OR_RETURN(diff, N);
    CREATE_OR_RETURN(comp, N);
    CREATE_OR_RETURN(other, 128);

    printf("Range size = %zu (not multiple of 64)\n", N);
    printf("A after create: count = %zu\n", bitset_count(&a));

    printf("add(%zu) on A -> %s\n", N, err_str(bitset_add(&a, N)));

    bitset_add(&a, 0);
    bitset_add(&a, 63);
    bitset_add(&a, 64);
    bitset_add(&a, N - 1);

    printf("A = {0, 63, 64, 129}: count = %zu\n", bitset_count(&a));
    print_contains(&a, 0);
    print_contains(&a, N - 1);
    print_contains(&a, N);

    printf("remove(0) first    -> %s\n", err_str(bitset_remove(&a, 0)));
    printf("remove(0) repeated -> %s\n", err_str(bitset_remove(&a, 0)));
    printf("A after repeated remove: count = %zu\n", bitset_count(&a));

    bitset_add(&single, 42);
    printf("single = {42}: count = %zu\n", bitset_count(&single));
    bitset_remove(&single, 42);
    printf("single after remove: count = %zu\n", bitset_count(&single));

    bitset_add(&b, 1);
    bitset_add(&b, 64);
    bitset_add(&b, 65);
    bitset_add(&b, N - 1);
    printf("B = {1, 64, 65, 129}: count = %zu\n", bitset_count(&b));

    bitset_union(&a, &b, &uni);
    printf("A | B: count = %zu\n", bitset_count(&uni));

    bitset_intersection(&a, &b, &inter);
    printf("A & B: count = %zu\n", bitset_count(&inter));

    bitset_difference(&a, &b, &diff);
    printf("A \\ B: count = %zu\n", bitset_count(&diff));

    bitset_union(&empty, &a, &comp);
    size_t before = bitset_count(&comp);
    bitset_complement(&comp);
    size_t after = bitset_count(&comp);
    printf("complement(A): before = %zu, after = %zu, expected = %zu\n",
           before, after, N - before);

    bitset_fill(&comp);
    printf("fill(comp): count = %zu\n", bitset_count(&comp));
    bitset_clear(&comp);
    printf("clear(comp): count = %zu\n", bitset_count(&comp));

    printf("union A with other range 128 -> %s\n",
           err_str(bitset_union(&a, &other, &uni)));

    bitset_destroy(&a);
    bitset_destroy(&b);
    bitset_destroy(&empty);
    bitset_destroy(&single);
    bitset_destroy(&uni);
    bitset_destroy(&inter);
    bitset_destroy(&diff);
    bitset_destroy(&comp);
    bitset_destroy(&other);

    return 0;
}
```

Сборка:

```bash
gcc -std=c11 -Wall -Wextra -pedantic bitset.c demo.c -o demo
./demo
```

Важное соглашение: bitset_remove возвращает BITSET_ERR_NOT_FOUND при повторном удалении. Это позволяет явно показать ошибочный/краевой случай в демонстрационной программе. Если нужна идемпотентная семантика, можно возвращать BITSET_OK и в этой ветке.

Ошибка «Нет правила для сборки цели 'all'» означает, что Qt Creator не может найти Makefile или не понимает, как собрать проект. Судя по скриншоту, вы просто открыли папку с файлами .c и .h как «существующий проект», но Qt Creator не создал для них систему сборки.

Вот как это исправить:

· Создайте файл проекта: В корневой папке с кодом создайте текстовый файл CMakeLists.txt и вставьте в него базовый код для сборки C-проекта:
  ```cmake
  cmake_minimum_required(VERSION 3.16)
  project(BitSet C)
  set(CMAKE_C_STANDARD 11)
  
  add_executable(demo main.c bitset.c)
  ```
· Переоткройте проект: Закройте текущий проект в Qt Creator (Файл -> Закрыть проект), затем откройте сам файл CMakeLists.txt через Файл -> Открыть файл или проект.
· Настройте комплект (Kit): В левой панели нажмите на значок «Проекты» (или Ctrl+5). Убедитесь, что выбран рабочий комплект (например, Desktop Qt ... MinGW или GCC). Если рядом с комплектом есть предупреждающий знак, проверьте путь к компилятору gcc / g++.
· Задайте каталог сборки: В тех же настройках «Проекты» проверьте поле «Каталог сборки» (Build directory). Укажите папку build внутри каталога с исходниками (например, .../BitSet/build), чтобы Qt Creator точно создал там Makefile.

Если после этих шагов ошибка сохранится, напишите, какой компилятор выбран в комплекте — подскажу, как проверить его правильность.
