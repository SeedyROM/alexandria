#pragma once

#include <stdio.h>
#include <stdlib.h>

struct wal_entry {
    u_int32_t crc32;
    u_int64_t timestamp;
    u_int32_t key_size;
    u_int32_t value_size;
    u_int8_t *key;
    u_int8_t *value;
};

struct wal_entry *wal_entry_new(u_int64_t timestamp,
                                u_int32_t key_size,
                                u_int32_t value_size,
                                u_int8_t *key,
                                u_int8_t *value);

void wal_entry_free(struct wal_entry *entry);

u_int32_t wal_entry_crc(struct wal_entry *entry);

struct wal {
    FILE *file;
};

struct wal *wal_open(const char *path);

int wal_append(struct wal *log, struct wal_entry *entry);

int wal_read(struct wal *log, struct wal_entry *entry);

void wal_free(struct wal *log);