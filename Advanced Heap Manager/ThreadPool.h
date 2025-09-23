#pragma once
#include <windows.h>

class ThreadPool {
public:
    
    ThreadPool(int num_threads);
   
    ~ThreadPool();

    
    void submit_task(void* task);

private:
    // Statička funkcija koju svaka nit u pool-u izvršava
    static DWORD WINAPI worker_thread_start(LPVOID param);

   
    int m_num_threads;
    HANDLE* m_threads; // Niz handle-ova za radničke niti


    void** m_task_queue;
    int m_queue_capacity;
    int m_head;
    int m_tail;

 
    CRITICAL_SECTION m_queue_lock; // Štiti pristup kružnom baferu
    HANDLE m_tasks_available;      // Semafor koji signalizira da ima novih zadataka
    HANDLE m_space_available;      // Semafor koji signalizira da ima mesta u baferu

    bool m_stop_threads; // Flag za zaustavljanje niti
};