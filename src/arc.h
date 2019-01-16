#ifndef __ARC__
#define __ARC__

template<typename T>
void clean(T *& pointer) {
    delete pointer;
    pointer = NULL;
}

template<typename T>
void safeClean(T *& pointer) {
    if(pointer == NULL){
        return;
    }
    clean(pointer);
}

#endif