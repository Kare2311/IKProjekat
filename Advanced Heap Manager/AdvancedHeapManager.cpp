#include "AdvancedHeapManager.h"

// Konstruktor
AdvancedHeapManager::AdvancedHeapManager(size_t heapCount, size_t heapSize)
    : heapCount(heapCount), currentHeapIndex(0) {
    for (size_t i = 0; i < heapCount; ++i) {
        heaps.push_back(Heap(heapSize));
    }
}

// Funkcija za alokaciju memorije
void* AdvancedHeapManager::allocate(size_t size, int userID) {
    // Prolazimo kroz sve heap-ove da bismo pronašli prvi koji ima dovoljno slobodne memorije
    for (size_t i = 0; i < heapCount; ++i) {
        size_t heapIndex = (currentHeapIndex + i) % heapCount;

        // Proveravamo da li trenutni heap može da alocira traženu količinu memorije
        void* allocatedMemory = heaps[heapIndex].allocate(size, userID);

        // Ako je alokacija uspela, vratimo pokazivač na alociranu memoriju
        if (allocatedMemory != nullptr) {
            currentHeapIndex = (heapIndex + 1) % heapCount;  // Ažuriraj trenutni index heap-a za sledeću alokaciju
            return allocatedMemory;
        }
    }

    // Ako nijedan heap ne može da obezbedi memoriju, vraćamo nullptr
    return nullptr;
}

//Metoda allocate pokušava da alocira memoriju u različitim heap - ovima u cikličnoj naredbi i vraća pokazivač na alociranu memoriju.

// Funkcija za dealokaciju memorije
bool AdvancedHeapManager::free(void* address) {
    for (auto& heap : heaps) {
        if (heap.free(address)) {
            return true;  // Ako je dealokacija uspela
        }
    }
    return false;  // Ako nije pronađena adresa za dealokaciju
}

// Metoda free pokušava da oslobodi memoriju na datoj adresi u svim heap-ovima i vraća true ako je oslobađanje uspešno. nisam siguran dal sam dobro razume da ovako treba da se radi