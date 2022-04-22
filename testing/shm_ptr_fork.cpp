// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.
#include "./testing.hpp"

#include <pshm/shm_ptr.hpp>

#include <chrono>
#include <thread>

/**
 * @brief Wrapper around fork().
 *
 * @returns 0 to child process and child's pid to parent process.
 * @throws runtime_error on failure.
 */
int do_fork();

/**
 * @brief Wrapper around waitpid()/WEXITSTATUS().
 *
 * @param kid the process id to wait for.
 * @returns the exit status from the child.
 * @throws runtime_error on failure.
 */
int do_wait(int kid);

/**
 * @brief Handle the parent process.
 *
 * Creates a shared memory segment, waits for the child process to send data,
 * and reads it back.
 *
 * @param name shared memory object to open.
 * @param pid child process id.
 */
void parent(const string& name, int pid);

/**
 * @brief Handle the child process.
 *
 * Uses shm_ptr to attach to the parent's shared memory segment send data.
 *
 * @param name shared memory object to open.
 */
void child(const string& name);

#if defined(__unix__)
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int do_fork()
{
    pid_t kid = fork();
    if (kid == -1) {
        throw runtime_error(string("fork() failed ") + strerror(errno));
    }
    return kid;
}

int do_wait(int kid)
{
    int wstatus;
    int rc = waitpid(kid, &wstatus, 0);

    if (rc != kid)
        throw runtime_error("waitpid() returned " + to_string(rc) + ": " + strerror(errno));
    if (!WIFEXITED(wstatus))
        throw runtime_error(string("!WIFEXITED(): ") + strerror(errno));
    return WEXITSTATUS(wstatus);
}
#else
#error no fork or waitpaid implementation on this platform
#endif

namespace
{
    /* Data to send from child to parent. */
    const char* message = "Hello";

    /* Don't have semaphores yet: using a busy/wait and sleeps. */
    constexpr std::chrono::milliseconds delay(50);
    constexpr int max_ticks = 20;
} // namespace

void parent(const string& name, int pid)
{
    pshm::shm_ptr<TestStruct> ptr(name, pshm::flags::CREAT | pshm::flags::EXCL | pshm::flags::RDWR | pshm::flags::TRUNC);
    ptr->true_or_false = false;

    cout << "Waiting for child" << endl;
    {
        int ticks = 0;
        do {
            std::this_thread::sleep_for(delay);
            ticks++;
            cout << "parent: ptr->true_or_false:" << ptr->true_or_false << endl;
        } while (!ptr->true_or_false && ticks <= max_ticks);
    }
    if (do_wait(pid) != 0)
        throw TestFailure(name, "do_wait(): bad return");

    if (!ptr->true_or_false)
        throw TestFailure(name, "Parent timed out waiting for child");

    if (strlen(ptr->str) == 0)
        throw TestFailure(name, "no message from child");
    if (strcmp(ptr->str, message) != 0)
        throw TestFailure(name, "message was " + string(ptr->str) + " but " + string(message) + " was expected");
}

void child(const string& name)
{
    cout << "Waiting for parent" << endl;
    std::this_thread::sleep_for(delay);
    int ticks = 0;
    pshm::shm_ptr<TestStruct> ptr;
    do {
        ticks++;
        try {
            std::this_thread::sleep_for(delay);
            ptr = std::move(pshm::shm_ptr<TestStruct>(name, pshm::flags::RDWR));
            cout << "child attached to shared memory" << endl;
            break;
        } catch (std::exception& ex) {
            cout << "child waiting...: " << ex.what() << endl;
        }
    } while (ticks <= max_ticks);

    if (!ptr) {
        cout << "child timed out waiting for parent to create the shared memory object." << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Child sending message" << endl;
    strncpy(ptr->str, message, std::min(TestStruct::Bounds, strlen(message)));
    ptr->true_or_false = true;
    cout << "child: ptr->true_or_false:" << ptr->true_or_false << endl;
}

void run_test(const string& name)
{
    int pid;

    cout << "Forking" << endl;
    try {
        pid = do_fork();
    } catch (runtime_error& ex) {
        throw TestFailure(name, string("do_fork() failed: ") + ex.what());
    }

    if (pid == 0) {
        child(name);
        cout << "CHILD EXIT" << endl;
        exit(EXIT_SUCCESS);
    } else {
        parent(name, pid);
    }
}
