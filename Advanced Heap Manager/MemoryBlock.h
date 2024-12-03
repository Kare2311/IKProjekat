#pragma once
#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H
#define MIN_BLOCK_SIZE 5

#include <cstddef> // za size_t

struct MemoryBlock {
    void* address;    // Adresa bloka memorije
    size_t size;      // Velicina bloka memorije
    bool free;        // Status: true ako je slobodan, false ako je zauzet
    int userID;       // ID korisnika koji koristi ovaj blok

    // Konstruktor za inicijalizaciju
    MemoryBlock(void* addr, size_t s, bool f, int id)
        : address(addr), size(s), free(f), userID(id) {}
};

#endif // MEMORY_BLOCK_H
