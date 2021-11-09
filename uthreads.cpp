#include "uthreads.h"
#include <stdio.h>
#include <csetjmp>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <list>
#include <deque>
#include <algorithm>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>


typedef unsigned long address_t;

using namespace std;

#define JB_SP 6

#define JB_PC 7

#define RESTART_TIME 100


//=========================================== structs =====================================//


/**
 * the thread struct
 */
struct u_thread{

    int id;
    char stack[STACK_SIZE];
    int total_quant;
    sigjmp_buf env;
};

struct mutex_lock_s{
    bool lock;
    int lock_id;
};
struct sigaction sa = {0};

struct itimerval timer;


//============================= private function declaration ========================//


/**
 * this function find id in our STL
 * @param id
 * @return 0 if id not exist, -1 out of boundary, -2 id exist
 */
int find(int id);

/**
 * this function implement destructor to the uthread library and delete all the dynamic allocation
 */
void destructor();

/**
 * this function erase id from the library id data structure
 * @param q - pointer to the structure
 * @param tid - the id to erase
 * @return -1 if success, 0 otherwise
 */
int erase(std::deque<int> *q, int tid);

/**
 * this function making
 * @param sig
 */
void context_switch(int sig);

int block_signals();

int unblock_signals();


//=================================== data structure =================================//


/**
 * min heap with the available ids'
 */
static priority_queue<int, vector<int>, greater<int>> id_min_heap;

/**
 * deque with the ready thread
 */
static std::deque<int> *ready_que;

/**
 * deque with the waiting thread
 */
static std::deque<int> *waiting_que;

static std::deque<int> *mutex_lock_que;


/**
 * vector with all the threads
 */
static std::vector<u_thread*> *threads;

sigset_t set_t;


//===================================== global variable ===============================//


/**
 * the quantum usecs that we get from the user
 */
int g_quantum_usecs;

/**
 * the current thread
 */
int g_current_thread;

/**
 * the total quantum the all the thread running together
 */
int g_total_quantum;


//===================================== global flags ===============================//


/**
 * flag that tell if the mutex is lock or not
 */
mutex_lock_s *mutex_lock;

/**
 * flag that tell if thread 0 running now or not
 */
bool thread_0;

/**
 * flag that tell the current thread is terminate or not
 */
bool terminate_id;

/**
 * flag that tell the current thread is block or not
 */
bool block_id;

/**
 * flag that tell the current thread is mutex_block or not
 */
bool mutex_block;


//================================ u_thread library API ==============================//


/**
 * this function translate location to address
 * @param addr the address that we get
 * @return the translate address
 */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
                 "rol    $0x11,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}

/**
 * this function initializes the thread library
 * @param quantum usecs - the quantum that every thread going to run
 * @return 0 if success, -1 otherwise
 */
int uthread_init(int quantum_usecs)
{
    block_signals();
    if (quantum_usecs <= 0)
    {
        cerr << "thread library error: quantum usecs need to be positive number" << endl;
        unblock_signals();
        return -1;
    }

    terminate_id = false;

    block_id = false;

    mutex_block = false;

    g_quantum_usecs = quantum_usecs;

    g_total_quantum = 1;

    threads = new std::vector<u_thread*>(MAX_THREAD_NUM);

    ready_que = new std::deque<int>();

    waiting_que = new std::deque<int>();

    mutex_lock_que = new std::deque<int>();

    mutex_lock = new mutex_lock_s{false, -1};

    if (threads == nullptr or ready_que == nullptr or waiting_que == nullptr or
        mutex_lock_que == nullptr or mutex_lock == nullptr)
    {
        cerr << "system error: malloc failed." <<endl;
        exit(1);
    }

    for (int i = 1; i < MAX_THREAD_NUM; ++i)
    {
        id_min_heap.push(i);
    }

    //creating thread 0
    auto *new_thread= new u_thread;

    new_thread->id = 0;
    new_thread->total_quant = 1;
    (*threads)[0] = new_thread;
    thread_0 = true;
    g_current_thread = 0;

    unblock_signals();
    return 0;
}

/**
 * this function creates a new thread, whose entry point is the function
 * f with the signature void f(void). The thread is added to the end of the READY threads list.
 * @param f
 * @return On success, return the ID of the created thread. On failure, return -1
 */
int uthread_spawn(void (*f)(void))
{
    block_signals();
    address_t sp, pc;

    //check if there is a free id.
    if (id_min_heap.empty())
    {
        cerr << "thread library error: you reached the max number of threads" << endl;
        unblock_signals();
        return -1;
    }

    auto *new_thread= new u_thread;

    //set the thread id.
    new_thread->id = id_min_heap.top();
    id_min_heap.pop();
    new_thread->total_quant = 0;


    //set the pc and sp of the thread.
    sp = (address_t)new_thread->stack + STACK_SIZE - sizeof(address_t);
    pc = (address_t)f;

    //set env to the thread.
    sigsetjmp(new_thread->env, 1);
    (new_thread->env->__jmpbuf)[JB_SP] = translate_address(sp);
    (new_thread->env->__jmpbuf)[JB_PC] = translate_address(pc);
    sigemptyset(&new_thread->env->__saved_mask);

    //push the thread to the and of the ready list.
    ready_que->push_back(new_thread->id);
    (*threads)[new_thread->id] = new_thread;

    if (thread_0)
    {
        sa.sa_handler = &context_switch;
        if (sigaction(SIGVTALRM, &sa,NULL) < 0) {
            cerr << "system error: sigaction failed." <<endl;
            exit(1);
        }

        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = g_quantum_usecs;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = g_quantum_usecs;

        // Start a virtual timer. It counts down whenever this process is executing.
        if (setitimer (ITIMER_VIRTUAL, &timer, NULL)) {
            cerr << "system error: setitimer failed." <<endl;
            exit(1);
        }

        thread_0 = false;

    }

    unblock_signals();
    return new_thread->id;
}

/**
 * This function terminates the thread with ID tid and deletes it from all relevant control structures.
 * All the resources allocated by the library for this thread should be released.
 * @param thread id
 * @return he function returns 0 if the thread was successfully terminated and -1 otherwise.
 */
int uthread_terminate(int tid)
{
    block_signals();
    if(tid == 0)
    {

        destructor();
        exit(0);

    }

    if(tid == g_current_thread)
    {

        terminate_id = true;
        delete (*threads)[tid];
        (*threads)[tid] = nullptr;
        id_min_heap.push(g_current_thread);
        if (mutex_lock->lock_id == tid)
        {
                uthread_mutex_unlock();

        }
        context_switch(RESTART_TIME);

    }

    int find_res = find(tid);

    if(find_res == -1)
    {
        cerr << "thread library error: thread id not exist" << endl;
        unblock_signals();
        return -1;
    }

    if(find_res == -2)
    {
        cerr << "thread library error: the thread id is invalid (out of boundary)" << endl;
        unblock_signals();
        return -1;
    }

    delete (*threads)[tid];
    (*threads)[tid] = nullptr;

    if(erase(ready_que, tid))
    {
        erase(waiting_que, tid);
        erase(mutex_lock_que, tid);
    }

    //returning the id to be relevant
    id_min_heap.push(tid);
    unblock_signals();

    return 0;
}

/**
 * This function blocks the thread with ID tid. n addition, it is an error to try blocking the main thread (tid == 0).
 * If a thread blocks itself.
 * @param thraed id
 * @return On success, return 0. On failure, return -1.
 */
int uthread_block(int tid)
{

    block_signals();
    //cannot block thread_0
    if (tid == 0)
    {
        cerr << "thread library error: blocking the main thread is not allowed\n";
        unblock_signals();
        return -1;
    }

    int find_res = find(tid);

    if(find_res == -1)
    {
        cerr << "thread library error: thread id not exist" << endl;
        unblock_signals();
        return -1;
    }

    if(find_res == -2)
    {
        cerr << "thread library error: the thread id is invalid (out of boundary)" << endl;
        unblock_signals();
        return -1;
    }

    if(g_current_thread == tid)
    {
        block_id = true;
        waiting_que->push_back(tid);
        context_switch(RESTART_TIME);
        block_signals();

    }

    if(!erase(ready_que, tid) || find(mutex_lock_que->cbegin(), mutex_lock_que->cend(), tid) != mutex_lock_que->cend())
    {
        waiting_que->push_back(tid);
    }
    unblock_signals();
    return 0;
}

/**
 * This function resumes a blocked thread with ID tid and moves it to the READY state.
 * @param thread id
 * @return On success, return 0. On failure, return -1.
 */
int uthread_resume(int tid)
{

    block_signals();
    int find_res = find(tid);

    if(find_res == -1)
    {
        cerr << "thread library error: thread id not exist" << endl;
        unblock_signals();
        return -1;
    }

    if(find_res == -2)
    {
        cerr << "thread library error: the thread id is invalid (out of boundary)" << endl;
        unblock_signals();
        return -1;
    }

    if(!erase(waiting_que, tid) &&
            (find(mutex_lock_que->cbegin(), mutex_lock_que->cend(), tid) == mutex_lock_que->cend()))
    {
        ready_que->push_back(tid);
    }
    unblock_signals();
    return 0;
}

/**
 * his function tries to acquire a mutex. If the mutex is unlocked, it locks it and returns.
 * @return On success, return 0. On failure, return -1.
 */
int uthread_mutex_lock()
{

    block_signals();
    if (mutex_lock->lock_id == g_current_thread)
    {
        cerr << "thread library error: a thread cant lock the mutex twice." << endl;
        unblock_signals();
        return -1;
    }
    while (mutex_lock->lock)
    {
        mutex_block = true;
        context_switch(RESTART_TIME);
        block_signals();
    }
    mutex_lock->lock = true;
    mutex_lock->lock_id = g_current_thread;
    unblock_signals();
    return 0;
}

/**
 * This function releases a mutex. If there are blocked threads waiting for this mutex,
 * one of them (no matter which one) moves to READY state.
 * @return On success, return 0. On failure, return -1.
 */
int uthread_mutex_unlock()
{
    block_signals();
    if (!mutex_lock->lock)
    {
        cerr << "thread library error: a thread cant unlock the mutex if it is already unlock." << endl;
        unblock_signals();
        return -1;
    }
    if (mutex_lock->lock_id != g_current_thread)
    {
        cerr << "thread library error: a thread cant unlock the mutex if another thread lock it." << endl;
        unblock_signals();
        return -1;
    }
    mutex_lock->lock = false;
    mutex_lock->lock_id = -1;
    if (!mutex_lock_que->empty())
    {
        int unblock_id = mutex_lock_que->front();
        mutex_lock_que->pop_front();

        if (find(waiting_que->cbegin(), waiting_que->cend(), unblock_id) == waiting_que->cend())
        {
            ready_que->push_back(unblock_id);
        }
    }
    unblock_signals();
    return 0;
}


/**
 * This function returns the thread ID of the calling thread.
 * @return The ID of the calling thread
 */
int uthread_get_tid()
{
    return g_current_thread;
}

/**
 * This function returns the total number of quantums that were started since the library was initialized,
 * including the current quantum
 * @return The total number of quantums.
 */
int uthread_get_total_quantums()
{
    return g_total_quantum;
}

/**
 * his function returns the number of quantums the thread with ID tid was in RUNNING state.
 * @param thread id
 * @return n success, return the number of quantums of the thread with ID tid. On failure, return -1.
 */
int uthread_get_quantums(int tid)
{
    int find_res = find(tid);


    if(find_res == -1)
    {
        cerr << "thread library error: thread id not exist" << endl;
        return -1;
    }

    if(find_res == -2)
    {
        cerr << "thread library error: the thread id is invalid (out of boundary)" << endl;
        return -1;
    }
    return (*threads)[tid]->total_quant;
}


//==================================== private function ====================================//

int block_signals()
{
    if (sigemptyset(&set_t))
    {
        cerr << "system error: sigemptyset failed." <<endl;
        exit(1);
    }
    if (sigaddset(&set_t, SIGVTALRM))
    {
        cerr << "system error: sigaddset failed." <<endl;
        exit(1);
    }
    if (sigprocmask(SIG_BLOCK, &set_t, nullptr))
    {
        cerr << "system error:sigprocmask failed." <<endl;
        exit(1);
    }
    return 0;
}

int unblock_signals()
{
    if (sigprocmask(SIG_UNBLOCK, &set_t, nullptr))
    {
        cerr << "system error:sigprocmask failed." <<endl;
        exit(1);
    }
    return 0;
}
/**
 * see above
 */
int find(const int id)
{
    if (id < 0 || MAX_THREAD_NUM <= id)
    {
        return -2;
    }

    auto pq = id_min_heap;
    while(!pq.empty()){
        int temp=pq.top();
        pq.pop();
        if (id == temp)
        {
            return -1;
        }
    }
    return 0;
}

/**
 * see above
 */
int erase(std::deque<int> *q, int tid)
{

    for (auto it = q->begin(); it != q->end(); ++it)
        if(tid == *it)
        {
            q->erase(it, it+1);
            return 0;
        }
    return -1;
}

/**
 * see above
 */
void destructor()
{
    for (int i=0; i < MAX_THREAD_NUM; ++i)
    {
        if ((*threads)[i] != nullptr)
        {
            delete (*threads)[i];
            (*threads)[i] = nullptr;
        }
    }
    delete threads;
    delete ready_que;
    delete waiting_que;
    delete mutex_lock_que;
}

/**
 * see above
 */
void context_switch(int sig)
{

    block_signals();
    if (ready_que->empty())
    {
        g_total_quantum += 1;
        (*threads)[g_current_thread]->total_quant += 1;
        unblock_signals();
        return;
    }

    if (sig == RESTART_TIME)
    {
        if (setitimer(ITIMER_VIRTUAL, &timer, NULL)) {
            cerr << "system error: setitimer failed." <<endl;
            exit(1);
        }
    }

    int new_id = ready_que->front();
    ready_que->pop_front();
    int old_thread = g_current_thread;
    g_current_thread = new_id;

    if (!terminate_id)
    {
        if (block_id)
        {
            waiting_que->push_back(old_thread);

        } else if (mutex_block) {

            mutex_lock_que->push_back(old_thread);

        }else{

            ready_que->push_back(old_thread);

        }


        int res = sigsetjmp((*threads)[old_thread]->env, 1);
        if (res == 1)
        {
            unblock_signals();
            return;
        }

    }


    mutex_block = false;
    block_id = false;
    terminate_id = false;

    g_total_quantum += 1;

    (*threads)[new_id]->total_quant += 1;

    unblock_signals();

    siglongjmp((*threads)[new_id]->env, 1);
}