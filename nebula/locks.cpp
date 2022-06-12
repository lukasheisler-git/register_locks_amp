#include "locks.hpp"

BoulangerieLock::BoulangerieLock(std::size_t num_threads) : no_of_threads(num_threads){
    number = new std::atomic_size_t[no_of_threads];
    choosing = new std::atomic_bool[no_of_threads];
    std::fill(number, number + no_of_threads, 0);
    std::fill(choosing, choosing + no_of_threads, false);
}

BoulangerieLock::~BoulangerieLock(){
    delete [] number;
    delete [] choosing; 
}

bool BoulangerieLock::check(std::size_t a, std::size_t b, std::size_t c, std::size_t d){
    return ((a < c) || (a == c && b < d));
}

void BoulangerieLock::lock(std::size_t tid) {
    std::size_t i = tid;
    std::size_t Limit = no_of_threads;
    bool tmp_c = false;
    std::size_t prev_n(-1), tmp_n(-1);
    std::vector<std::size_t> num(no_of_threads,0);

    choosing[i] = true;
    for(std::size_t j = 0; j < no_of_threads; j++){
        if(j != i) num[j] = number[j];
    }

    num[i] = 1 + *std::max_element(num.begin(), num.end());
    number[i] = num[i];
    choosing[i] = false;

    if(number[i] == 1) Limit = i;

    //take ticket
    for (std::size_t j = 0; j < Limit; j ++) {
        if (j != i) {
            do{
                tmp_c = choosing[j];
            }while(tmp_c == true);

            tmp_n = -1;

            do{
                prev_n = tmp_n;
                tmp_n = number[j];
            }while((tmp_n != 0) && !check(num[i],i,tmp_n,j) && !(tmp_n != prev_n && prev_n != -1));
        }
    }
}

void BoulangerieLock::unlock(std::size_t tid) {
    number[tid] = 0;
    //std::cout << "Lock released by thread " << tid << std::endl;
}


LamportBakeryHerlihyLock::LamportBakeryHerlihyLock(std::size_t num_threads) : no_of_threads(num_threads){
    flag = new std::atomic_bool[no_of_threads];
    label = new std::atomic_size_t[no_of_threads];
    for(std::size_t i=0; i<no_of_threads; i++){
        flag[i] = false;
        label[i] = 0;
    }
}

LamportBakeryHerlihyLock::~LamportBakeryHerlihyLock(){
    delete [] flag;
    delete [] label;
}

void LamportBakeryHerlihyLock::lock(std::size_t tid) {
    std::size_t i = tid;
    flag[i] = true;
    std::size_t max = label[0];

    //take ticket
    for (std::size_t j = 1; j < no_of_threads; j ++) {
        if (label[j] > max) {
            max = label[j];
        }
    }
    label[i] = max + 1;

    //wait till smallest
    for (size_t j = 0; j < no_of_threads; j++) {
        if (i == j) {
            continue;
        }
        else{
            while (flag[j] && ((label[j] < label[i]) ||( label[i] == label[j] && j < i))) {};
        } 
    }
    
    //std::cout << "Lock obtained by thread " << tid << std::endl;
}

void LamportBakeryHerlihyLock::unlock(std::size_t tid) {
    flag[tid] = false;
    //std::cout << "Lock released by thread " << tid << std::endl;
}


LamportBakeryOriginalLock::LamportBakeryOriginalLock(std::size_t num_threads) : no_of_threads(num_threads){
    flag = new std::atomic_bool[no_of_threads];
    label = new std::atomic_size_t[no_of_threads];
    for(std::size_t i=0; i<no_of_threads; i++){
        flag[i] = false;
        label[i] = 0;
    }
}

void LamportBakeryOriginalLock::lock(std::size_t tid) {
    std::size_t i = tid;
    flag[i] = true;
    std::size_t max = label[0];

    //take ticket
    for (std::size_t j = 1; j < no_of_threads; j ++) {
        if (label[j] > max) {
            max = label[j];
        }
    }
    label[i] = max + 1;
    flag[i] = false;
    //wait till smallest
    for (size_t j = 0; j < no_of_threads; j++) {
        if (i == j) {
            continue;
        }
        else{
            while(flag[j]){};
            while (label[j] > 0 && ((label[j] < label[i]) ||( label[i] == label[j] && j < i))) {};
        } 
    }
    
    //std::cout << "Lock obtained by thread " << tid << std::endl;
}

void LamportBakeryOriginalLock::unlock(std::size_t tid) {
    label[tid] = 0;
    //std::cout << "Lock released by thread " << tid << std::endl;
}


PetersonsFilterLock::PetersonsFilterLock(std::size_t num_threads) : no_of_threads(num_threads){
  level = new std::atomic_size_t[no_of_threads];
  victim = new std::atomic_size_t[no_of_threads];
  for(std::size_t i=0; i<no_of_threads; i++){
    level[i] = 0;
    victim[i] = false;
  }
}

PetersonsFilterLock::~PetersonsFilterLock(){
    delete [] level;
    delete [] victim;
}

void PetersonsFilterLock::lock(std::size_t tid) {
  for(std::size_t i=1; i<no_of_threads; i++){
    level[tid] = i;
    victim[i] = tid;
    for(std::size_t k=0; k<no_of_threads; k++){
        while( k!=tid && level[k] >=i && victim[i] == tid) {}
    }
  }
  //std::cout << "Lock obtained by thread " << tid << std::endl;
}

void PetersonsFilterLock::unlock(std::size_t tid) {
  level[tid] = 0;
  //std::cout << "Lock released by thread " << tid << std::endl;
}


PetersonsNode::PetersonsNode(PetersonsNode *par, std::size_t num_threads) : no_of_threads(num_threads){
    parent = par; 
    flags = new std::atomic_bool[no_of_threads];
    for(std::size_t i=0; i<no_of_threads; i++){
        flags[i] = false;
    }
}

PetersonsNode::~PetersonsNode(){
    delete [] flags;
}

void PetersonsNode::lock(std::size_t tid) {
    flags[tid] = true;
    victim = tid;

    //spin until my flag is unset and I am not victim
    while(isAnotherFlag(tid) && isVictim(tid)){};
  //std::cout << "Lock obtained by thread " << tid << std::endl;
}

void PetersonsNode::unlock(std::size_t tid) {
  flags[tid] = false;
  //std::cout << "Lock released by thread " << tid << std::endl;
}

bool PetersonsNode::isVictim(std::size_t tid) {
    return victim == tid;
}

bool PetersonsNode::isAnotherFlag(std::size_t tid) {
    for(std::size_t i = 0; i < no_of_threads; i++){
        if(flags[i] && (i != tid))
            return true;
    }
    return false;
}

PetersonsTree::PetersonsTree(std::size_t num_threads) : no_of_threads(num_threads){
    if(validatePow2(no_of_threads))
    {
        root = new PetersonsNode(nullptr, no_of_threads);
        std::vector<PetersonsNode*> initList;
        initList.push_back(root);
        leaves = growTree(initList);
    }
    else{
        throw std::invalid_argument("Number of threads must be power of 2");
    }
}

PetersonsTree::~PetersonsTree(){
    leaves.clear();
}

void PetersonsTree::lock(std::size_t tid){
    PetersonsNode *currentNode = leafLockForThread(tid);
    while(currentNode != nullptr){
        currentNode->lock(tid);
        currentNode = currentNode->parent;
    }
}

void PetersonsTree::unlock(std::size_t tid){
    PetersonsNode *currentNode = leafLockForThread(tid);
    while(currentNode != nullptr){
        currentNode->unlock(tid);
        currentNode = currentNode->parent;
    }
}

PetersonsNode* PetersonsTree::leafLockForThread(std::size_t tid){
    return leaves[int(tid/2)];
}

std::vector<PetersonsNode*> PetersonsTree::growTree(std::vector<PetersonsNode*> leaves){
    if(leaves.size() == no_of_threads/2)
        return leaves;

    std::vector<PetersonsNode*> currentLeaves;
    for(PetersonsNode* node : leaves){
        node->leftChild = new PetersonsNode(node, no_of_threads);
        node->rightChild = new PetersonsNode(node, no_of_threads);
        currentLeaves.push_back(node->leftChild);
        currentLeaves.push_back(node->rightChild);
    }

    return growTree(currentLeaves);
}

bool PetersonsTree::validatePow2(std::size_t threads){
    return (threads & (threads-1)) == 0;
}

C11Lock::C11Lock(std::size_t num_threads){

}

void C11Lock::lock(std::size_t tid){
    m.lock();
}

void C11Lock::unlock(std::size_t tid){
    m.unlock();
}

OpenMPLock::OpenMPLock(std::size_t num_threads){
    omp_init_lock(&o);
}

void OpenMPLock::lock(std::size_t tid){
    omp_set_lock(&o);
}

void OpenMPLock::unlock(std::size_t tid){
    omp_unset_lock(&o);
}



TestAndSetLock::TestAndSetLock(std::size_t num_threads){}

void TestAndSetLock::lock(std::size_t tid) 
{
  while(lock_.exchange(true, std::memory_order_acquire));
}

void TestAndSetLock::unlock(std::size_t tid) 
{
  lock_.store(false, std::memory_order_release);
}

TestAndTestAndSetLock::TestAndTestAndSetLock(std::size_t num_threads){}

void TestAndTestAndSetLock::lock(std::size_t tid) 
{
  for (;;) 
  {
    if (!lock_.exchange(true, std::memory_order_acquire)) {
      break;
    }
    while (lock_.load(std::memory_order_relaxed));
  }
}

void TestAndTestAndSetLock::unlock(std::size_t tid) 
{
  //lock_.store(false);
  lock_.store(false, std::memory_order_release);
}