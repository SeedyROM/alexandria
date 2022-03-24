#include <memory.h>
#include "memtable.h"

struct memtable_entry *
memtable_entry_new(u_int64_t timestamp, u_int32_t key_size, u_int32_t value_size, u_int8_t *key, u_int8_t *value) {
    struct memtable_entry *entry = malloc(sizeof(struct memtable_entry));
    if (entry == NULL) { return NULL; }

    entry->timestamp = timestamp;
    entry->key_size = key_size;
    entry->value_size = value_size;

    entry->key = malloc(sizeof(u_int8_t) * key_size);
    if (entry->key == NULL) { return NULL; }
    memcpy(entry->key, key, key_size);

    entry->value = malloc(sizeof(u_int8_t) * value_size);
    if (entry->value == NULL) { return NULL; }
    memcpy(entry->value, value, value_size);

    return entry;
}

struct memtable *memtable_new(const char *wal_path, unsigned int levels, float p) {
    struct memtable *mt = malloc(sizeof(struct memtable));
    if (mt == NULL) { return NULL; }

    mt->log = wal_open(wal_path);
    if (mt->log == NULL) { return NULL; }

    mt->levels = levels;
    mt->p = p;
    mt->level = 0;
    mt->head = NULL;

    return mt;
}

void memtable_insert(struct memtable *mt, struct memtable_entry *entry) {
    struct wal_entry *wal_entry = wal_entry_new(entry->timestamp, entry->key_size, entry->value_size, entry->key, entry->value);
    wal_append(mt->log, wal_entry);
    free(wal_entry);
}

struct memtable_entry *memtable_search(struct memtable *mt, u_int32_t key_size, u_int8_t *key) {

}

void memtable_remove(struct memtable *mt, u_int32_t key_size, u_int8_t *key) {

}

void memtable_free(struct memtable *mt) {
    wal_free(mt->log);
    // TODO: Clean up nodes
    free(mt);
}
