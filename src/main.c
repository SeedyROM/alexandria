#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "wal.h"
#include "memtable.h"

int main(int argc, char **argv) {
    struct memtable *mt = memtable_new(128, 0.5);

    memtable_insert(
        mt,
        time(NULL),
        5,
        5,
        (u_int8_t *) "yote",
        (u_int8_t *) "boat"
    );
    memtable_insert(
            mt,
            time(NULL),
            5,
            5,
            (u_int8_t *) "yote",
            (u_int8_t *) "boat"
    );

    return 0;
}

//int main() {
//    struct wal *w = wal_open("./yoted.log");
//    assert(w != NULL);
//
//    struct wal_entry *e = wal_entry_new(
//        time(NULL),
//        6,
//        5,
//        (u_int8_t *) "yoted",
//        (u_int8_t *) "king"
//    );
//
//    struct wal_entry *e1 = wal_entry_new(
//            time(NULL),
//            7,
//            6,
//            (u_int8_t *) "goated",
//            (u_int8_t *) "queen"
//    );
//
//    struct wal_entry *e2 = wal_entry_new(
//            time(NULL),
//            4,
//            7,
//            (u_int8_t *) "edy",
//            (u_int8_t *) "is gay"
//    );
//
//    wal_append(w, e);
//    wal_append(w, e1);
//    wal_append(w, e2);
//
//    wal_entry_free(e);
//    wal_entry_free(e1);
//    wal_entry_free(e2);
//
//    struct wal_entry e3;
//    e3.key = NULL;
//    e3.value = NULL;
//    fseek(w->file, 0, SEEK_SET);
//    while (wal_read(w, &e3)) {
//        printf("CRC32: %u\n", e3.crc32);
//        printf("Timestamp: %lu\n", e3.timestamp);
//        printf("Key: %s\n", (char *) e3.key);
//        printf("Value: %s\n\n", (char *) e3.value);
//    }
//
//    wal_free(w);
//    return 0;
//}
