#include <memory.h>
#include <math.h>
#include "memtable.h"

struct memtable_entry *
memtable_entry_new(unsigned int level, u_int64_t timestamp, u_int32_t key_size, u_int32_t value_size, u_int8_t *key,
                   u_int8_t *value) {
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

    entry->level = level;
    entry->forward = malloc(sizeof(struct memtable_entry *) * (level + 1));
    memset(entry->forward, 0, sizeof(struct memtable_entry *) * (level + 1));

    return entry;
}

int memtable_entry_comparator(struct memtable_entry *lhs, struct memtable_entry *rhs) {
    unsigned int bytes_to_compare = (
            (lhs->key_size) > (rhs->key_size) ?
            lhs->key_size : rhs->key_size
    ); // TODO: This will read garbage memory...
    return strncmp((char *) lhs->key, (char *) rhs->key, bytes_to_compare);
}

struct memtable *memtable_new(unsigned int capacity, double p) {
    struct memtable *mt = malloc(sizeof(struct memtable));
    if (mt == NULL) { return NULL; }

    if (p < 0 || p > 1) {
        fprintf(stderr, "Memtable probability must be a value where 0 < p < 1");
        return NULL;
    }

    mt->p = p;
    mt->capacity = capacity;
    mt->max_levels = (unsigned int) (log(mt->capacity) / log(2));
    mt->level = 0;
    mt->head = NULL;

    return mt;
}


bool memtable_coin_flip(struct memtable *mt) {
    double r = drand48();
    return r >= mt->p;
}

void memtable_insert(struct memtable *mt, u_int64_t timestamp, u_int32_t key_size, u_int32_t value_size, u_int8_t *key,
                     u_int8_t *value) {
    struct memtable_entry *entry = memtable_entry_new(
        mt->level,
        timestamp,
        key_size,
        value_size,
        key,
        value
    );

    // If our skiplist is empty insert at the head.
    if (mt->head == NULL) {
        mt->head = entry;
        return;
    }


}

struct memtable_entry *memtable_search(struct memtable *mt, u_int32_t key_size, u_int8_t *key) {
    struct memtable_entry *head = mt->head;

    if (head == NULL) {

    }
}

void memtable_remove(struct memtable *mt, u_int32_t key_size, u_int8_t *key) {

}

void memtable_free(struct memtable *mt) {
    // TODO: Clean up nodes
    free(mt);
}
