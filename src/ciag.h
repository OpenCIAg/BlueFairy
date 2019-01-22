#ifndef __CIAG__
#define __CIAG__

namespace ciag {
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
}

#endif