#include "wal.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "crc.h"

struct wal_entry *wal_entry_new(u_int64_t timestamp,
                                u_int32_t key_size,
                                u_int32_t value_size,
                                u_int8_t *key,
                                u_int8_t *value) {
    struct wal_entry *entry = malloc(sizeof(struct wal_entry));
    if(entry == NULL) { return NULL; }

    entry->timestamp = timestamp;
    entry->key_size = key_size;
    entry->value_size = value_size;

    entry->key = malloc(sizeof(u_int8_t) * key_size);
    if(entry->key == NULL) { return NULL; }
    memcpy(entry->key, key, key_size);

    entry->value = malloc(sizeof(u_int8_t) * value_size);
    if(entry->value == NULL) { return NULL; }
    memcpy(entry->value, value, value_size);

    entry->crc32 = wal_entry_crc(entry);

    return entry;
}

void wal_entry_free(struct wal_entry *entry) {
    free(entry->key);
    free(entry->value);
    free(entry);
}

uint32_t wal_entry_crc(struct wal_entry *entry) {
    uint32_t crc = 0;
    crc32(&entry->timestamp, sizeof(u_int64_t), &crc);
    crc32(&entry->key_size, sizeof(u_int32_t), &crc);
    crc32(&entry->value_size, sizeof(u_int32_t), &crc);
    crc32(entry->key, entry->key_size, &crc);
    crc32(entry->value, entry->value_size, &crc);
    return crc;
}

struct wal *wal_open(const char *path) {
    struct wal *wal = malloc(sizeof(struct wal));
    if (wal == NULL) {
        return NULL;
    }
    wal->file = NULL;
    FILE *f = fopen(path, "w+");
    if (f <= 0) {
        fprintf(stderr, "Failed to open wal with file: %s", path);
        return NULL;
    }
    wal->file = f;
    return wal;
}

int wal_append(struct wal *log, struct wal_entry *entry) {
    fwrite(&entry->crc32, sizeof(u_int32_t), 1, log->file);
    fwrite(&entry->timestamp, sizeof(u_int64_t), 1, log->file);
    fwrite(&entry->key_size, sizeof(u_int32_t), 1, log->file);
    fwrite(&entry->value_size, sizeof(u_int32_t), 1, log->file);
    fwrite(entry->key, entry->key_size, 1, log->file);
    fwrite(entry->value, entry->value_size, 1, log->file);
}

int wal_read(struct wal *log, struct wal_entry *entry) {
    unsigned long read = 0;

    u_int32_t crc32;
    u_int64_t timestamp;
    u_int32_t key_size;
    u_int32_t value_size;
    u_int8_t *key = NULL;
    u_int8_t *value = NULL;

    if(entry->key != NULL) { free(entry->key); }
    if(entry->value != NULL) { free(entry->value); }

    read = fread(&crc32, sizeof(u_int32_t), 1, log->file);
    if (read == 0) { return 0; }
    read = fread(&timestamp, sizeof(u_int64_t), 1, log->file);
    if (read == 0) { return 0; }
    read = fread(&key_size, sizeof(u_int32_t), 1, log->file);
    if (read == 0) { return 0; }
    read = fread(&value_size, sizeof(u_int32_t), 1, log->file);
    if (read == 0) { return 0; }

    key = malloc(sizeof(u_int8_t) * key_size);
    read = fread(key, key_size, 1, log->file);
    if (read == key_size) { return 0; }

    value = malloc(sizeof(u_int8_t) * value_size);
    read = fread(value, value_size, 1, log->file);
    if (read == value_size) { return 0; }

    entry->crc32 = crc32;
    entry->timestamp = timestamp;
    entry->key_size = key_size;
    entry->value_size = value_size;
    entry->key = key;
    entry->value = value;

    return 1;
}

void wal_free(struct wal *log) {
    fclose(log->file);
    free(log);
}