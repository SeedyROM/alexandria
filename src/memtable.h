#pragma once

#include <stdlib.h>
#include "wal.h"

struct memtable_entry {
    u_int64_t timestamp;
    u_int32_t key_size;
    u_int32_t value_size;
    u_int8_t *key;
    u_int8_t *value;

    struct memtable_entry **forward;
};

struct memtable_entry *
memtable_entry_new(u_int64_t timestamp, u_int32_t key_size, u_int32_t value_size, u_int8_t *key, u_int8_t *value);

struct memtable {
    unsigned int levels;
    float p;
    unsigned int level;
    struct memtable_entry *head;
    struct wal *log;
};

struct memtable *memtable_new(const char *wal_path, unsigned int levels, float p);
void memtable_insert(struct memtable *mt, struct memtable_entry *entry);
struct memtable_entry *memtable_search(struct memtable *mt, u_int32_t key_size, u_int8_t *key);
void memtable_remove(struct memtable *mt, u_int32_t key_size, u_int8_t *key);
void memtable_free(struct memtable *mt);