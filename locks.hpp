#include <cstddef>
#include <atomic>   
#include <iostream>
#include <vector>
#include <mutex>
#include <omp.h>
#include <algorithm>

using LLint = long long int;

class BoulangerieLock {
private:
  const std::size_t no_of_threads;
  volatile LLint *number;
  volatile bool *choosing;

public:
  BoulangerieLock(std::size_t num_threads);
  ~BoulangerieLock();
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
  bool check(LLint a, LLint b, LLint c, LLint d);
};

class LamportBakeryHerlihyLock {
private:
  const std::size_t no_of_threads;
  volatile bool *flag;
  volatile LLint *label;

public:
  LamportBakeryHerlihyLock(std::size_t num_threads);
  ~LamportBakeryHerlihyLock();
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};

class LamportBakeryOriginalLock {
private:
  const std::size_t no_of_threads;
  volatile bool *flag;
  volatile LLint *label;

public:
  LamportBakeryOriginalLock(std::size_t num_threads);
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};

class PetersonsFilterLock {
private:
  const std::size_t no_of_threads;
  volatile LLint *level;
  volatile LLint *victim;

public:
  PetersonsFilterLock(std::size_t num_threads);
  ~PetersonsFilterLock();
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};


class PetersonsNode {
private:
  const std::size_t no_of_threads;
  volatile bool *flags;
  std::atomic_size_t victim;

public:
  PetersonsNode *leftChild;
  PetersonsNode *rightChild;
  PetersonsNode *parent;

public:
  PetersonsNode(PetersonsNode *par, std::size_t num_threads);
  ~PetersonsNode();
  void lock(std::size_t tid);
  void unlock(std::size_t tid);

private:
  bool isVictim(std::size_t tid);
  bool isAnotherFlag(std::size_t tid);
};

class PetersonsTree{
private:
    const std::size_t no_of_threads;
    PetersonsNode* leafLockForThread(std::size_t tid);
    std::vector<PetersonsNode*> growTree(std::vector<PetersonsNode*> leaves);
    bool validatePow2(std::size_t threads);

public:
    PetersonsTree(std::size_t num_threads);
    ~PetersonsTree();
    PetersonsNode *root;
    std::vector<PetersonsNode*> leaves;
    void lock(std::size_t tid);
    void unlock(std::size_t tid);
};

class C11Lock{
private:
  std::mutex m;

public:
  C11Lock(std::size_t num_threads);
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};

class OpenMPLock{
private:
  omp_lock_t o;

public:
  OpenMPLock(std::size_t num_threads);
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};


class TestAndSetLock {
    private:
        std::atomic<bool> lock_ = {false};

    public:
        // ~test_set_Lock();
        TestAndSetLock(std::size_t num_threads);
        void lock(std::size_t tid);
        void unlock(std::size_t tid);
};

// TestAndSetLock::~TestAndSetLock() {
//     lock_ = {false};
// }


class TestAndTestAndSetLock {
    private:
        std::atomic<bool> lock_ = {false};

    public:
        // ~TestAndTestAndSetLock();
        TestAndTestAndSetLock(std::size_t num_threads);
        void lock(std::size_t tid);
        void unlock(std::size_t tid);
};

// TestAndTestAndSetLock::~TestAndTestAndSetLock() {
//     lock_ = {false};
// }
