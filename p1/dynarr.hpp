#ifndef DYNARR_H
#define DYNARR_H

class DynArr
{
    private:
        int capacity;
        int arrSize;
        int * intArr;
    public:
        DynArr();
        ~DynArr();
        int getCapacity();
        int getSize();
        void doubleSize();
        int add(int element);
        void deleteArr();
        void print();
};

#endif // DYNARR_H
