#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "wal.h"

/**
 * An entry in our memtable skiplist
 */
struct memtable_entry {
    u_int64_t timestamp;
    u_int32_t key_size;
    u_int32_t value_size;
    u_int8_t *key;
    u_int8_t *value;

    unsigned int level;
    struct memtable_entry **forward;
};

/**
 * Create a new memtable entry
 * @param level
 * @param timestamp
 * @param key_size
 * @param value_size
 * @param key
 * @param value
 * @return
 */
struct memtable_entry *
memtable_entry_new(unsigned int level, u_int64_t timestamp, u_int32_t key_size, u_int32_t value_size, u_int8_t *key,
                   u_int8_t *value);

/**
 * Compare two memtable entries keys.
 * @param lhs
 * @param rhs
 * @return
 */
int memtable_entry_comparator(struct memtable_entry *lhs, struct memtable_entry *rhs);

/**
 * The memtable struct, this is defined as a `log2(capacity)` level skip-list.
 */
struct memtable {
    double p;
    unsigned int capacity;
    unsigned int max_levels;
    unsigned int level;
    struct memtable_entry *head;
};

/**
 * Create a new memtable
 * @param wal_path
 * @param capacity
 * @param p
 * @return
 */
struct memtable *memtable_new(unsigned int capacity, double p);

/**
 * Determine a coin toss, using `drand48` as randomness
 * @param mt
 * @return
 */
bool memtable_coin_flip(struct memtable *mt);

/**
 * Insert a value into the memtable probabilistically
 * @param mt
 * @param entry
 */
struct memtable_entry *
memtable_insert(struct memtable *mt, u_int64_t timestamp, u_int32_t key_size, u_int32_t value_size, u_int8_t *key,
                u_int8_t *value);

/**
 * Search the memtable for a key
 * @param mt
 * @param key_size
 * @param key
 * @return
 */
struct memtable_entry *memtable_search(struct memtable *mt, u_int32_t key_size, u_int8_t *key);

/**
 * Remove an item from the memtable
 * @param mt
 * @param key_size
 * @param key
 */
void memtable_remove(struct memtable *mt, u_int32_t key_size, u_int8_t *key);

/**
 * Free the memtable and it's resources
 * @param mt
 */
void memtable_free(struct memtable *mt);