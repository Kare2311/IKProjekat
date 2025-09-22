#pragma once
#ifndef ADVANCED_HEAP_MANAGER_H
#define ADVANCED_HEAP_MANAGER_H

#include <windows.h> 
#include "Heap.h"

class AdvancedHeapManager {
private:
    Heap* m_heaps;
    size_t m_heap_count;
    size_t m_current_heap_idx;

    CRITICAL_SECTION m_lock;     // Kriticna sekcija za sinhronizaciju pristupa AHM-u

public:
    AdvancedHeapManager(size_t heapCount, size_t heapSize);
    ~AdvancedHeapManager();


    void* allocate(size_t size, int thread_id);
    void free(void* ptr);

    void get_heap_stats(size_t heap_index, size_t& used_size, size_t& total_size) const;

    size_t get_heap_index(void* ptr) const;

    Heap* get_heap(size_t index);
};

#endif // ADVANCED_HEAP_MANAGER_H