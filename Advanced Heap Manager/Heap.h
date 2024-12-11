#pragma once
#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <mutex>
#include "MemoryBlock.h"

class Heap {
private:
    size_t totalSize;             // Ukupna veličina heap-a
    size_t usedSize;              // Zauzeta veličina heap-a
    std::vector<MemoryBlock> blocks;  // Blokovi memorije u heap-u
    std::mutex lock;              // Mutex za zaštitu operacija unutar heap-a

public:
    Heap(size_t size);            // Konstruktor
    Heap(const Heap&) = delete;   // Zabranjujemo kopiranje  //Ovaj konstruktor je obrisan (deleted). To znači da nije dozvoljeno da se objekti klase Heap kopiraju. Kopiranje objekta ove klase nije moguće
    Heap& operator=(const Heap&) = delete;  // Zabranjujemo dodelu kopiranjem   //Slično kao kopirni konstruktor, ovaj operator dodela je takođe obrisan. To znači da nije moguće dodeliti jedan objekat klase Heap drugom putem dodeljivanja, jer bi to moglo izazvati nepoželjno kopiranje resursa

    // Move konstruktor  umesto pravljenja kopije uradi se move
    Heap(Heap&& other) {
        totalSize = other.totalSize;
        usedSize = other.usedSize;
        blocks = std::move(other.blocks);  // Premestamo vektor blokova
        // Mutex ne premestamo, samo ga ostavljamo u izvornom objektu
    }
     
    // Move dodela operator  Ovaj operator se poziva kada se dodeljuje vrednost objekta Heap iz drugog objekta Heap
    Heap& operator=(Heap&& other) {
        if (this != &other) {  // Provera da se ne premesti u sebe
            totalSize = other.totalSize;
            usedSize = other.usedSize;
            blocks = std::move(other.blocks);  // Premestamo vektor blokova
            // Mutex ne premestamo, samo ga ostavljamo u izvornom objektu
        }
        return *this;
    }

    void* allocate(size_t size, int userID);  // Alokacija memorije
    bool free(void* address);             // Dealokacija memorije
    size_t getUsedSize() const;    // Zauzeta veličina
    size_t getFreeSize() const;    // Slobodna veličina
};

#endif // HEAP_H

//Greška koju dobijaš(attempting to reference a deleted function) ukazuje na to da se pokušava kopirati objekat klase Heap, 
//ali konstruktor kopije za tu klasu nije definisan ili je izričito obrisan.Ovo se često dešava kada klasa koristi članove poput std::mutex, koji nisu kopirljivi.