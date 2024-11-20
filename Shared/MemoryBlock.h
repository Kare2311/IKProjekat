#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <cstddef> // za size_t

struct MemoryBlock {
    void* address;    // Adresa bloka memorije
    size_t size;      // Veličina bloka memorije
    bool free;        // Status: true ako je slobodan, false ako je zauzet
    int userID;       // ID korisnika koji koristi ovaj blok
};

#endif // MEMORY_BLOCK_H