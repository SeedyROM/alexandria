#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "wal.h"
#include "memtable.h"

#define MAX_TEST_CASES 3

int main(int argc, char **argv) {
    struct test_data {
        char *key;
        char *value;
    };

    struct test_data t[MAX_TEST_CASES] = {
        {
            .key = "yoted",
            .value = "bloated"
        },
        {
            .key = "goated",
            .value = "quoted"
        },
        {
            .key = "soupy",
            .value = "me"
        }
    };


    // ===============================================================================
    // memtable
    // ===============================================================================

    struct memtable *mt = memtable_new(128, 0.5);

    for (int i = 0; i < MAX_TEST_CASES; i++) {
        struct test_data test = t[i];

        memtable_insert(
            mt,
            time(NULL),
            strlen(test.key) + 1,
            strlen(test.value) + 1,
            (u_int8_t *) test.key,
            (u_int8_t *) test.value
        );
    }

    memtable_free(mt);

    // ===============================================================================
    // WAL
    // ===============================================================================

    struct wal *w = wal_open("./yoted.log");
    assert(w != NULL);

    for (int i = 0; i < MAX_TEST_CASES; i++) {
        struct test_data test = t[i];
        struct wal_entry *e = wal_entry_new(
            time(NULL),
            strlen(test.key) + 1,
            strlen(test.value) + 1,
            (u_int8_t *) test.key,
            (u_int8_t *) test.value
        );
        wal_append(w, e);
        free(e);
    }

    // TODO: This needs to be a function
    struct wal_entry e;
    e.key = NULL;
    e.value = NULL;
    fseek(w->file, 0, SEEK_SET);

    for (int i = 0; i < MAX_TEST_CASES; i++) {
        struct test_data test = t[i];
        wal_read(w, &e);

        assert(strcmp(test.key, (char *) e.key) == 0);
        assert(strcmp(test.value, (char *) e.value) == 0);
    }

    wal_free(w);

    // ===============================================================================

    return 0;
}

