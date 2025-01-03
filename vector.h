#ifndef VECTOR_H
#define VECTOR_H

template <typename T> 
class vector{
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
    
};

#endif
