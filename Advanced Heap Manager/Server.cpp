#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <ctime>
#include <iomanip>
#include <process.h>
#include <windows.h>
#include "AdvancedHeapManager.h"
#include "ThreadPool.h"
#include "ClientHandler.h"

#pragma comment(lib, "Ws2_32.lib")

bool g_keep_running = true;

const int HEAP_COUNT_DEMO_MT = 5;
const size_t HEAP_SIZE_BYTES_DEMO_MT = 1024;
const int NUM_DEMO_THREADS = 3;
const int ALLOCATIONS_PER_DEMO_THREAD = 4;
AdvancedHeapManager g_ahm_demo_mt(HEAP_COUNT_DEMO_MT, HEAP_SIZE_BYTES_DEMO_MT);

DWORD WINAPI DemoMonitorThread(LPVOID lpParam) {
    while (g_keep_running) {
        printf("--- Detaljni prikaz stanja za %d heap-ova ---\n", HEAP_COUNT_DEMO_MT);
        for (int i = 0; i < HEAP_COUNT_DEMO_MT; ++i) {
            Heap* h = g_ahm_demo_mt.get_heap(i);
            if (h != nullptr) h->print_layout();
        }
        Sleep(2000);
    }
    return 0;
}

DWORD WINAPI DemoWorkerThread(LPVOID lpParam) {
    size_t thread_id = (size_t)(uintptr_t)lpParam;
    for (int i = 0; i < ALLOCATIONS_PER_DEMO_THREAD; ++i) {
        Sleep(500 + (rand() % 1000));
        size_t size_to_alloc = 50 + (rand() % 100);
        printf("\n>>> Nit %zu trazi %zu bajtova...\n", thread_id, size_to_alloc);
        g_ahm_demo_mt.allocate(size_to_alloc, (int)thread_id);
    }
    printf("\n<<< Nit %zu je zavrsila sa alokacijama.\n", thread_id);
    return 0;
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    HANDLE demo_workers[NUM_DEMO_THREADS];
    HANDLE h_monitor_thread = CreateThread(NULL, 0, DemoMonitorThread, NULL, 0, NULL);
    for (size_t i = 0; i < NUM_DEMO_THREADS; ++i) {
        demo_workers[i] = CreateThread(NULL, 0, DemoWorkerThread, (LPVOID)i, 0, NULL);
    }
    WaitForMultipleObjects(NUM_DEMO_THREADS, demo_workers, TRUE, INFINITE);
    printf("\nSve radnicke niti su zavrsile. Pritisni taster za finalni prikaz stanja.\n");
    g_keep_running = false;
    WaitForSingleObject(h_monitor_thread, INFINITE);
    printf("--- FINALNO STANJE HEAP-OVA ---\n");
    for (int i = 0; i < HEAP_COUNT_DEMO_MT; ++i) {
        Heap* h = g_ahm_demo_mt.get_heap(i);
        if (h != nullptr) h->print_layout();
    }
    for (size_t i = 0; i < NUM_DEMO_THREADS; ++i) CloseHandle(demo_workers[i]);
    CloseHandle(h_monitor_thread);
    system("pause");
    return 0;
}
