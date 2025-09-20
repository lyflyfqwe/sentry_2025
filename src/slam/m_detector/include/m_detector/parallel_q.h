#ifndef PARALLEL_Q_H
#define PARALLEL_Q_H

#include <cassert>

namespace m_detector{
//双端队列
template<typename T> class Parallel_Q{
private:
    int counter_{0}, Q_len_;
    bool is_empty_, is_initialized_{false};
public:
    T* q_;
    int head_{0}, tail_{0};

    Parallel_Q(){
        is_initialized_ = false;
    }

    Parallel_Q(int len){
        Q_len_ = len;
        q_ = new T[Q_len_];
        is_initialized_ = true;
    }

    ~Parallel_Q(){
        if(is_initialized_) delete[] q_;
    }

    void init(int len){
        Q_len_ = len;
        q_ = new T[Q_len_];
        is_initialized_ = true;
    }

    void pop(){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        if(counter_ == 0) return;
        head_++;
        head_ %= Q_len_;
        counter_--;
        if(counter_ == 0) is_empty_ = true;
    }

    T front(){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        return q_[head_];
    }

    T back(){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        return q_[(tail_+Q_len_-1) % Q_len_];
    } 

    void clear(){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        head_ = tail_ = counter_ = 0;
        is_empty_ = true;
    }

    void push(T op){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        if(counter_ == Q_len_) pop();
        q_[tail_] = op;
        counter_++;
        if(is_empty_) is_empty_ = false;
        tail_++;
        tail_ %= Q_len_;
    }

    //元素入队，元素索引为 index % Q_len
    void pushPos(T& op, int index){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        if(counter_ == Q_len_) pop();
        counter_++;
        index %= Q_len_;
        q_[index] = op;
        if(is_empty_) is_empty_ = false;
        tail_++;
        tail_ %= Q_len_;
    }

    void pushParallel(T& op, int index){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        if(counter_ == Q_len_) pop();
        index %= Q_len_;
        q_[index] = op;
    }

    void pushParallelPrepare(int len){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        if(counter_ == Q_len_) pop();
        counter_ += len;
        if(is_empty_) is_empty_ = false;
        tail_ += len;
        tail_ %= Q_len_;
    }

    bool empty(){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        return is_empty_;
    }

    int size(){
        assert(is_initialized_ && "[m_detector] Queue is not initialized!");
        return counter_;
    }
};
} //namespace m_detector

#endif