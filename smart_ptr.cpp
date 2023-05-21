//
// Created by Michael Sarto on 21/05/23.
//

#include <cstddef>
#include "smart_ptr.h"
typedef size_t diffptr_t;
template <class T>

class smart_ptr{
private:
    T const* ptr;
    size_t * cnt; //Siccome è condiviso deve essere un pointer e non un value
    diffptr_t offset;
    void destroy(){
        --cnt;
        if(*cnt==0){
            delete ptr;
            delete cnt;
        }
    }
public:
    smart_ptr(T* t_, size_t* cnt_ , diffptr_t& offset_):ptr(t_), cnt(cnt_),offset(offset_){}
    smart_ptr(T* ptr_):ptr(ptr_),cnt(new size_t (1)),offset(0){}
    smart_ptr(const smart_ptr<T> &x):ptr(x.ptr),cnt(x.cnt),offset(x.offset){
        ++(*cnt);
    }
    ~smart_ptr(){
        destroy();
    }


    smart_ptr<T> operator=(const smart_ptr<T>&test) {
        if(ptr!=test.ptr){
            destroy();
            ++(*test.cnt);
            ptr=test.ptr;
            offset=test.offset;
            cnt=test.cnt;
        }
        return *this;
    }

    const T& operator*(){
        return *(ptr + offset);
    }

    const T& operator[](diffptr_t offset) const {
        return *(ptr + offset);
    }


    const T& operator*() const{
        return *(ptr);
    }

    bool operator==(const smart_ptr<T>&test)const {
        return (ptr==test.ptr) && offset==test.offset;
    }

    bool operator!=(const smart_ptr<T>&test) const{
        return !(*this==test);
    }
    explicit operator T*(){ //Operatore di conversione -> diventa quello che è dopo "operator"
        return ptr;
    }

    explicit operator const T*()const{
        return ptr;
    }

    smart_ptr<T> operator*(const diffptr_t &t){
        return new smart_ptr<T>(ptr+t);
    }

    smart_ptr<T> operator+( int &t) {
        return  smart_ptr<T>(ptr, cnt,(diffptr_t )offset+t);
    }
    smart_ptr<T> operator-(const diffptr_t& t)const {
        return *this +(-t);
    }

    smart_ptr<T>& operator +=(const diffptr_t& t){
        this->offset+=t;
        return *this;
    }
    smart_ptr<T>& operator-=(const diffptr_t&t){
        return *this +=(-t);
    }

    smart_ptr<T> operator++(int){ //postincrement
        smart_ptr<T> *tmp=new smart_ptr<T>(*this);
        ++(*this);
        return *tmp;
    }

    smart_ptr<T>&  operator++(){ //preincremento
        return (*this)+=1;
    }

    smart_ptr<T> operator--(int){
        smart_ptr<T>tmp(*this);
        --(*this);
        return  tmp;
    }

    smart_ptr<T>& operator--(){
        return (*this)-=1;
    }

    const T* operator->()const {
        return ptr+offset;
    }

    T* operator->()
    {
        return const_cast<T*>((*this).operator->());
    }
};

template <class Pointer>
void demo(Pointer p)
{
    Pointer p2(p);
    p = p2;
    *p;		// in C, C++ ed altri linguaggi imperativi non è necessario utilizzare il risultato di una chiamata a funzione o operatore
    (p + 2);
    p2 = p + 2;
    ++p2;
    p++;
    --p2;
    p += 1;
    p -= 2;

}


int main()
{
    int* a = new int[10];
    demo(a);



    smart_ptr<int> b(a);
    demo(b);

    smart_ptr<double> d(new double(1029));
    demo(d);

}