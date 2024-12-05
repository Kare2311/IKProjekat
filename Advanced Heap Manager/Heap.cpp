#include "Heap.h"

#include <sstream>

Heap::Heap(size_t size) : totalSize(size), usedSize(0) {
    // Inicijalizacija heap-a sa pocetnim slobodnim blokom
    blocks.push_back({ nullptr, size, true, -1 });
}

// Funkcija za alokaciju memorije
void* Heap::allocate(size_t size, int userID) {
    std::lock_guard<std::mutex> guard(lock);  // Zastita pristupa pomocu mutex-a

    // Pronadji prvi slobodan blok koji je dovoljno veliki
    for (auto& block : blocks) {
        if (block.free && block.size >= size) {
            // Ako je velicina bloka veca od potrebne, podelimo blok na dva
            if (block.size > size) {
                // Podela bloka na dva dela 
                void* newAddress = static_cast<char*>(block.address) + size;  // Adresa novog bloka
                size_t newSize = block.size - size;  // Novi blok sa preostalom memorijom
                
                //TO DO: da se podeli iako je premali i onda da se svi slobodni blokovi spoje ako moze
                if (newSize >= MIN_BLOCK_SIZE) {// MIN_BLOCK_SIZE je minimalna velicina za novi blok
                    // Postavljamo velicinu prvobitnog bloka na velicinu koju je korisnik trazio
                    block.size = size;  

                    // Kreiramo novi slobodan blok 
                    MemoryBlock newBlock(newAddress, newSize, true, -1); 

                    // Dodajemo novi blok na kraj heap-a
                    blocks.push_back(newBlock);
                }
            }

            // Dodeljujemo memoriju korisniku
            block.free = false;  // Ovaj blok je zauzet
            block.userID = userID;  // Postavljamo ID korisnika
            usedSize += block.size;  // Azuriramo zauzetost heap-a
            return block.address;  // Vracamo adresu zauzetog bloka
        }
    }

    // Ako nije pronadjen odgovarajuci blok
    return nullptr;
}

// Funkcija za dealokaciju memorije
bool Heap::free(void* address) {
    std::lock_guard<std::mutex> guard(lock);  // Zastita pristupa pomocu mutex-a

    for (auto& block : blocks) {
        if (block.address == address && !block.free) {
            usedSize -= block.size;  // Azuriraj zauzetost heap-a
            block.free = true;
            block.address = nullptr;
            block.userID = -1;  // Resetovanje korisnickog ID-a
            return true;  // Uspelo je dealociranje

            //TO DO: ako oko ovog bloka ima slobodnih da se spoje u 1 ako nisu dostigli max size npr

        }
    }
    return false;  // Ako nije pronadjena adresa
}

// Funkcija za dobijanje zauzete velicine
size_t Heap::getUsedSize() const {
    return usedSize;
}

// Funkcija za dobijanje slobodne velicine
size_t Heap::getFreeSize() const {
    return totalSize - usedSize;

}


Heap::Heap(Heap&& other) noexcept
    : blocks(std::move(other.blocks)),
    totalSize(other.totalSize),
    usedSize(other.usedSize),
    roundRobinIndex(other.roundRobinIndex) {
    // Nema potrebe za kopiranjem mutex-a, jer on neće biti premešten
    // Mutex objekti se ne premeštaju, oni ostaju vezani za izvorni objekat
    // Ovaj deo možete ostaviti bez promena
}

// Move dodela
Heap& Heap::operator=(Heap&& other) noexcept {
    if (this != &other) {
        blocks = std::move(other.blocks);
        totalSize = other.totalSize;
        usedSize = other.usedSize;
        roundRobinIndex = other.roundRobinIndex;
        // Nema potrebe za kopiranjem mutex-a, jer on neće biti premešten
    }
    return *this;
}
