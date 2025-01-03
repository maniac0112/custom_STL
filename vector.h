
#ifndef VECTOR_H
#define VECTOR_H

#include <memory>          // For std::allocator
#include <initializer_list> // For std::initializer_list
#include <algorithm>       // For std::copy
#include <iostream>        // For std::cout (used in reserve())

template <typename T> 
class vector{
    
private: 
    int length; 
    int capacity; 
    T* ptr; 
    std::allocator<T> allocator; 
    
    void reserve(int new_capacity){
        std::cout<<"resized \n";
        
        T* new_ptr = allocator.allocate(new_capacity);
        
        for (int i=0; i<length; i++){
            allocator.destroy(ptr + i);
        }
        
        // Deallocate old memory and update pointers
        allocator.deallocate(ptr, capacity);
        ptr = new_ptr; 
        capacity = new_capacity;
    }
    
    struct iterator{
        
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&; 
        using iterator_category = std::random_access_iterator_tag;
        
        iterator(T* i_ptr){
            t_ptr = i_ptr; 
        } 
        
        reference operator*() const {
            return *t_ptr; 
        }
        
        pointer operator->() {
            return t_ptr; 
        }
        
        iterator& operator++(){
            //pre increment
            t_ptr++;
            return *this; 
        }
        
        iterator operator++(int){
            //post increment
            iterator tmp = *this; 
            t_ptr++;
            return tmp; 
        }

        iterator& operator--() {
            t_ptr--;
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            t_ptr--;
            return temp;
        }

        iterator operator+(difference_type offset) const {
            return iterator(t_ptr + offset);
        }

        iterator operator-(difference_type offset) const {
            return iterator(t_ptr - offset);
        }

        difference_type operator-(const iterator& other) const {
            return t_ptr - other.t_ptr;
        }
        
        iterator& operator+=(difference_type offset) {
            t_ptr += offset;
            return *this;
        }

        iterator& operator-=(difference_type offset) {
            t_ptr -= offset;
            return *this;
        }
        
        reference operator[](difference_type index) const {
            return *(t_ptr + index);
        }
        
        bool operator==(const iterator& other) const { return t_ptr == other.t_ptr; }
        bool operator!=(const iterator& other) const { return t_ptr != other.t_ptr; }
        bool operator<(const iterator& other) const { return t_ptr < other.t_ptr; }
        bool operator>(const iterator& other) const { return t_ptr > other.t_ptr; }
        bool operator<=(const iterator& other) const { return t_ptr <= other.t_ptr; }
        bool operator>=(const iterator& other) const { return t_ptr >= other.t_ptr; }
        
    private:
        T* t_ptr;
    };

public: 
    
    vector(): length{0}, capacity{0}, ptr{nullptr}{
    }
    
    vector(int n): length{n}, capacity{2*n}{
        ptr = allocator.allocate(2*n);
    }
    
    vector(const vector& arr):length{arr.length}, capacity{arr.capacity}{
        ptr = allocator.allocate(arr.capacity);
        for (int i=0; i<length; i++){
            allocator.construct(ptr+i,arr[i]);
        }
    }
    
    
    vector(vector&& arr):length{arr.length}, capacity{arr.capacity}{
        ptr = arr.ptr; 
        arr.ptr = nullptr; 
    }
    
    vector(std::initializer_list<T> init) 
        : length{static_cast<int>(init.size())}, capacity{2*length} {
            
        ptr = allocator.allocate(capacity);
        std::copy(init.begin(), init.end(), ptr);
    }
    

    vector<T>& operator=(const vector<T>& arr){
        //perform resizing only if neccesarry
        
        if (arr.capacity>capacity){
            reserve(arr.capacity);
        }
        
        length = arr.length;
        
        for (int i=0; i<length; i++){
            allocator.construct(ptr + i, *(arr.ptr+i));
        }
        
        return *this;
    }
    
     vector<T>& operator=(vector<T>&& arr){
        //move assignment
        
        if (this==&arr){
            return *this;
        }
        
        reserve(0);
        
        length = arr.length;
        capacity = arr.capacity;
        ptr = arr.ptr; 
        arr.ptr = nullptr; 
        
        return *this;
    }
    
    void push_back(const T& obj){
        
        if (length == capacity) {
            reserve(capacity == 0 ? 1 : 2 * capacity);
        }
        
        allocator.construct(ptr+length, obj);
        length++;
        
    }
    
    void pop_back(){
        allocator.destroy(ptr+length-1);
        length--;
    }
    
    
    T& operator[](int n){
        return *(ptr + n);
    }
    
    int size(){
        return length; 
    }
    
    ~vector(){
        for (int i=0; i<length; i++){
            allocator.destroy(ptr+i);
        }
        allocator.deallocate(ptr, capacity);
    }
    
    iterator begin(){
        return iterator{ptr};
    }
    
    iterator end(){
        return iterator{ptr+length};
    }
};

#endif
