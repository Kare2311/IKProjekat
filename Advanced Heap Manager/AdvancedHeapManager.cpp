#include "AdvancedHeapManager.h"
#include "Heap.h"
#include <sstream>

AdvancedHeapManager::AdvancedHeapManager(size_t heapCount, size_t heapSize)
    : heapCount(heapCount), currentHeapIndex(0) {
    heaps.resize(heapCount, Heap(heapSize));  // Kreiranje heap-ova sa datim veličinama
}

AdvancedHeapManager::AdvancedHeapManager()
    : heapCount(0), currentHeapIndex(0) {}

void* AdvancedHeapManager::allocate(size_t size, int userID) {
    std::lock_guard<std::mutex> guard(lock); // Zastita pristupa pomocu mutex-a

    // Prolazimo kroz heap-ove koristeći Round Robin algoritam
    for (size_t i = 0; i < heapCount; ++i) {
        Heap& currentHeap = heaps[(currentHeapIndex + i) % heapCount];
        void* allocatedMemory = currentHeap.allocate(size, userID);

        if (allocatedMemory != nullptr) {
            // Ako je memorija uspešno alocirana, ažuriraj Round Robin indeks
            currentHeapIndex = (currentHeapIndex + 1) % heapCount;
            return allocatedMemory;
        }
    }

    // Ako nije bilo uspešne alokacije na bilo kojem heap-u
    return nullptr;
}

bool AdvancedHeapManager::free(void* address) {
    std::lock_guard<std::mutex> guard(lock);  // Zastita pristupa pomocu mutex-a

    // Prolazimo kroz sve heap-ove i pozivamo free za svaki
    for (Heap& heap : heaps) {
        if (heap.free(address)) {
            return true;  // Ako je uspešno oslobođeno u jednom od heap-ova
        }
    }

    // Ako nije pronađen odgovarajući blok
    return false;
}

std::string AdvancedHeapManager::getMemoryState() const {
    std::ostringstream output;

    for (size_t i = 0; i < heaps.size(); ++i) {
        const Heap& heap = heaps[i];
        output << "Heap " << i + 1 << ":\n";
        output << "  Total Size: " << heap.getTotalSize() << " bytes\n";
        output << "  Used Size: " << heap.getUsedSize() << " bytes\n";
        output << "  Free Size: " << heap.getFreeSize() << " bytes\n";

        for (const auto& block : heap.getBlocks()) {
            output << "    Block Address: " << block.address
                << ", Size: " << block.size
                << ", " << (block.free ? "Free" : "Allocated")
                << "\n";
        }
        output << "------------------------------------------------------------\n";
    }

    return output.str();
}

