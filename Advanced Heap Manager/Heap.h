<<<<<<< Updated upstream
=======
﻿#pragma once
>>>>>>> Stashed changes
#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <mutex>  // Dodajemo ovo za mutex
#include "MemoryBlock.h"

class Heap {
private:
    std::vector<MemoryBlock> blocks; // Svi memorijski blokovi u ovom heap-u
    size_t totalSize;                // Ukupna veli?ina heap-a
    size_t usedSize;                 // Trenutno zauzeta veli?ina
    std::mutex lock;                 // Mutex za sinhronizaciju
    size_t roundRobinIndex;          // Indeks za Round Robin alokaciju

public:
<<<<<<< Updated upstream
    Heap(size_t size);               // Konstruktor za inicijalizaciju heap-a
    void* allocate(size_t size, int userID); // Funkcija za alokaciju memorije
    bool deallocate(void* address);         // Funkcija za dealokaciju memorije
    size_t getUsedSize() const;             // Vra?a trenutno zauzetu veli?inu
    size_t getFreeSize() const;             // Vra?a slobodnu veli?inu
=======
    Heap(size_t size); // Konstruktor
    Heap(const Heap&) = delete; // Onemogućavamo kopiranje
    Heap& operator=(const Heap&) = delete; // Onemogućavamo dodelu preko kopiranja

    Heap(Heap&&) noexcept; // Move konstruktor
    Heap& operator=(Heap&&) noexcept; // Move dodela

    void* allocate(size_t size, int userID);
    bool free(void* address);
    size_t getUsedSize() const;
    size_t getFreeSize() const;         // Vraca slobodnu velicinu

    size_t getTotalSize() const { return totalSize; }
    const std::vector<MemoryBlock>& getBlocks() const { return blocks; }


>>>>>>> Stashed changes
};

#endif // HEAP_H
