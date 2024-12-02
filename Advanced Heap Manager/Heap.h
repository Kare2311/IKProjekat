#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <mutex>
#include "MemoryBlock.h"

class Heap {
private:
    std::vector<MemoryBlock> blocks; // Svi memorijski blokovi u ovom heap-u
    size_t totalSize;                // Ukupna veli?ina heap-a
    size_t usedSize;                 // Trenutno zauzeta veli?ina
    std::mutex lock;                 // Mutex za sinhronizaciju

public:
    Heap(size_t size);               // Konstruktor za inicijalizaciju heap-a
    void* allocate(size_t size, int userID); // Funkcija za alokaciju memorije
    bool deallocate(void* address);         // Funkcija za dealokaciju memorije
    size_t getUsedSize() const;             // Vra?a trenutno zauzetu veli?inu
    size_t getFreeSize() const;             // Vra?a slobodnu veli?inu
};

#endif // HEAP_H
