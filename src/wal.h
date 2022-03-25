#pragma once

#include <stdio.h>
#include <stdlib.h>

/**
 * An entry in our write ahead log.
 */
struct wal_entry {
    u_int32_t crc32;
    u_int64_t timestamp;
    u_int32_t key_size;
    u_int32_t value_size;
    u_int8_t *key;
    u_int8_t *value;
};

/**
 * Construct a new WAL entry.
 * @param timestamp
 * @param key_size
 * @param value_size
 * @param key
 * @param value
 * @return
 */
struct wal_entry *wal_entry_new(u_int64_t timestamp,
                                u_int32_t key_size,
                                u_int32_t value_size,
                                u_int8_t *key,
                                u_int8_t *value);

/**
 * Free an entries resources.
 * @param entry
 */
void wal_entry_free(struct wal_entry *entry);

/**
 * Calculate the CRC32 for our WAL entry.
 * @param entry
 * @return
 */
u_int32_t wal_entry_crc(struct wal_entry *entry);

/**
 * The WAL
 */
struct wal {
    FILE *file;
};

/**
 * Open a new WAL at the specified location
 * @param path
 * @return
 */
struct wal *wal_open(const char *path);

/**
 * Append an entry into the WAL, aka persist to disk.
 * @param log
 * @param entry
 * @return
 */
int wal_append(struct wal *log, struct wal_entry *entry);

/**
 * Read WAL entries from the log file one by one,
 * this returns 1 when it fails to parse.
 * @param log
 * @param entry
 * @return
 */
int wal_read(struct wal *log, struct wal_entry *entry);

/**
 * Free the WAL and close the log file.
 * @param log
 */
void wal_free(struct wal *log);