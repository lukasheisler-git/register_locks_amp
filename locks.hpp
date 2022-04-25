#include <cstddef>
#include <atomic>   
#include <iostream>

class BoulangerieLock {
private:
  std::atomic_size_t no_of_threads;
  std::atomic_bool *flag;
  std::atomic_size_t *label;

public:
  BoulangerieLock(std::size_t num_threads);
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};

class LamportBakeryHerlihyLock {
private:
  std::atomic_size_t no_of_threads;
  std::atomic_bool *flag;
  std::atomic_size_t *label;

public:
  LamportBakeryHerlihyLock(std::size_t num_threads);
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};

class LamportBakeryOriginalLock {
private:
  std::atomic_size_t no_of_threads;
  std::atomic_bool *flag;
  std::atomic_size_t *label;

public:
  LamportBakeryOriginalLock(std::size_t num_threads);
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};

class PetersonsFilterLock {
private:
  std::atomic_size_t no_of_threads;
  std::atomic_size_t *level;
  std::atomic_size_t *victim;

public:
  PetersonsFilterLock(std::size_t num_threads);
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};


class PetersonsNode {
private:
  std::atomic_size_t no_of_threads;
  std::atomic_size_t *level;
  std::atomic_size_t *victim;

  PetersonsNode *leftChild;
  PetersonsNode *rightChild;
  PetersonsNode *parent;

public:
  PetersonsNode(PetersonsNode par, std::size_t num_threads);
  void lock(std::size_t tid);
  void unlock(std::size_t tid);
};

class PetersonsTree{
private:
    std::size_t no_of_threads;
    PetersonsNode leafLockForThread(std::size_t tid);
    std::list<PetersonsNode> growTree(std::list<PetersonsNode> nodes);
    bool validatePow2(std::size_t threads);

public:
    PetersonsTree(std::size_t num_threads);
    PetersonsNode root;
    void lock(std::size_t tid);
    void unlock(std::size_t tid);
    
    
};