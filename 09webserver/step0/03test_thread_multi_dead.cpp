#include <set>
#include <stdio.h>
#include <unistd.h>

#include "Mutex.h"
#include "Thread.h"

class Request;

class Inventory
{
public:
    void add(Request * req)
    {
        MutexLockGuard lock(mutex_);
        requests_.insert(req);
    }

    void remove(Request * req) __attribute__((noinline))
    {
        MutexLockGuard lock(mutex_);
        requests_.erase(req);
    }

    void printAll() const;

private:
    mutable MutexLock mutex_;
    std::set<Request *> requests_;
};

Inventory g_inventory;

class Request
{
public:
    void process() // __attribute__ ((noinline))
    {
        MutexLockGuard lock(mutex_);
        g_inventory.add(this);
        // ...
    }

    ~Request() __attribute__((noinline))
    {
        MutexLockGuard lock(mutex_);
        sleep(1);
        g_inventory.remove(this);
    }

    void print() const __attribute__((noinline))
    {
        MutexLockGuard lock(mutex_);
        // ...
    }

private:
    mutable MutexLock mutex_;
};

void Inventory::printAll() const
{
    MutexLockGuard lock(mutex_);
    sleep(1);
    for (std::set<Request *>::const_iterator it = requests_.begin();
         it != requests_.end();
         ++it)
    {
        (*it)->print();
    }
    printf("Inventory::printAll() unlocked\n");
}

/*
void Inventory::printAll() const
{
  std::set<Request*> requests
  {
    MutexLockGuard lock(mutex_);
    requests = requests_;
  }
  for (std::set<Request*>::const_iterator it = requests.begin();
      it != requests.end();
      ++it)
  {
    (*it)->print();
  }
}
*/

void threadFunc()
{
    Request * req = new Request;
    req->process();
    delete req;
}

int main()
{
    Thread thread(threadFunc);
    thread.start();
    usleep(1500 * 1000);
    g_inventory.printAll();
    thread.join();

    puts("=== over. ===");

    return 0;
}
