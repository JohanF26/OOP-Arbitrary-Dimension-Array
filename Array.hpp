#ifndef ARRAY_CS540
#define ARRAY_CS540
#include <stdexcept>
#include <exception>
#include <assert.h>

//#include <cstdarg>

namespace cs540{

class OutOfRange: public std::exception {
    const char* what() const throw() {
        return "Error: indices provided are out of range for this array.";
    }
};

//template <typename T, size_t... Dims> class Array;
//Class to retrieve first dimension
template <typename T, size_t CurrentDim, size_t... Dims>
class Array{
public:
    size_t dimSize;
    Array<T, Dims...> nextDimArrays[CurrentDim];
    typedef Array<T, Dims...> nextDim;

    class FirstDimensionMajorIterator{
    public:
        //position
        size_t pos;
        size_t dimSize;
        Array<T, CurrentDim, Dims...>* arrPtr;
        typename nextDim::FirstDimensionMajorIterator nextDimIt;
        int finishFlag;


        void setFinish(){
            finishFlag = 1;
        }
        void resetFinish(){
            finishFlag = 0;
        }
        int getFinish(){
            return finishFlag;
        }
        //Must have correct default constructor
        FirstDimensionMajorIterator(): pos(0), dimSize(CurrentDim), finishFlag(0) {}
        //Copy constructor (implicit will probably be correct)
        FirstDimensionMajorIterator(const FirstDimensionMajorIterator &itToCopy) = default;
        //     pos = itToCopy.pos;
        //     dimSize = itToCopy.dimSize;
        //     arrPtr = itToCopy.arrPtr;
        // }
        FirstDimensionMajorIterator(Array<T, CurrentDim, Dims...>* arr, bool endIt){
            pos = endIt? CurrentDim : 0;
            finishFlag = endIt;
            arrPtr = arr;
            dimSize = arr->dimSize;
            nextDimIt = endIt? arr->nextDimArrays[0].fmend() : arr->nextDimArrays[0].fmbegin();
        }
        //Assignment Operator (implicit will prob be correct)
        FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &itToAssign) = default;
        //equality-inequality functions
        //compiler says they must take excatly one arg
        bool operator==(const FirstDimensionMajorIterator &fdmi1){
            bool retVal = ((fdmi1.pos == pos) && (fdmi1.arrPtr == arrPtr) && (finishFlag == fdmi1.finishFlag));
            if((retVal) && finishFlag){
                retVal = true;
            } else if(retVal){
                retVal = (nextDimIt == fdmi1.nextDimIt);
            }
            return retVal;
        }
        bool operator!=(const FirstDimensionMajorIterator &fdmi1){
            bool retVal = ((fdmi1.pos == pos) && (fdmi1.arrPtr == arrPtr) && (finishFlag == fdmi1.finishFlag));
            if((retVal) && finishFlag){
                retVal = true;
            } else if(retVal){
                retVal = (nextDimIt == fdmi1.nextDimIt);
            }
            return (!retVal);
        }
        //Increments the iterator one element and returns incremented iterator (preincrement)
        FirstDimensionMajorIterator &operator++(){
            //Array<T, Dims...> tempArr = *arrPtr;
            //This calls the ++ on the next FDMI
            nextDimIt++;
            //if it is the end of th array
            if(nextDimIt.pos == nextDimIt.dimSize){
                nextDimIt.pos = 0;
                pos++;
                (pos%dimSize)? resetFinish() : setFinish();

                updateNextDim();
            }

            //return incremented iterator
            return (*this);
            //tempArr.fmbegin()

        }
        //Increments the iterator one element and returns an iterator pointing to element prior
        FirstDimensionMajorIterator operator++(int){
            //auto tempFdmi= arrPtr->fmbegin();
            FirstDimensionMajorIterator thisFdmi(*this);
            //increment
            ++(*this);
            //return iterator pointing to element prior
            return thisFdmi;

        }

        void updateNextDim(){
            if(pos != dimSize){
                nextDimIt.arrPtr = &(arrPtr->nextDimArrays[pos]);
            } else{
                nextDimIt.arrPtr = &(arrPtr->nextDimArrays[0]);
            }
            nextDimIt.updateNextDim();
        }
        //Returns reference to the T at this position in the array
        T &operator*() const{
            //dereferece array pointer and index array
            //Array<T,Dims...> tempArr = *arrPtr;
            //T tempT = tempArr[pos];
            //return tempT;
            return *nextDimIt;
        }
    };

    /*
    class LastDimensionMajorIterator{
    public:
        size_t pos;
        Array<T, CurrentDim>* arrPtr;
        //Must have correct default constructor
        LastDimensionMajorIterator(): pos(0), arrPtr(nullptr){}
        //Copy constructor (implicit will probably be correct)
        LastDimensionMajorIterator(const LastDimensionMajorIterator &) = default;
        //Assignment Operator (implicit will prob be correct)
        LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &) = default;
        //equality-inequality functions
        bool operator==(const LastDimensionMajorIterator &ldmi1){
            bool retVal = (ldmi1.pos == pos) && (ldmi1.arrPtr == arrPtr);
            return retVal;
        }
        bool operator!=(const LastDimensionMajorIterator &ldmi1){
            bool retVal = !((ldmi1.pos == pos) && (ldmi1.arrPtr == arrPtr));
            return retVal;
        }
        //Increments the iterator one element and returns incremented iterator (preincrement)
        LastDimensionMajorIterator &operator++(){
            if((pos = (pos+1)%((*arrPtr).dimSize)) == 0){
                //we must call increment on nextDimArrays since it is last dimension
                for(size_t i = 0; i < (*arrPtr).dimSize; i++){
                    ++arrPtr[i];
                }
            }
            return *this;
        }
        //Increments the iterator one element and returns an iterator poinrting to element prior
        LastDimensionMajorIterator operator++(int){
            //FirstDimensionMajorIterator tempFdmi = (*arrPtr).fmbegin();
            LastDimensionMajorIterator thisLdmi = *this;
            //increment
            pos++;
            //return iterator pointing to element prior
            return thisLdmi;
        }
        //Returns reference to the T at this position in the array
        T &operator*() const{
            return ((*arrPtr)[pos]);
        }
    };
    */
    class LastDimensionMajorIterator{
    public:
        //position
        size_t pos;
        size_t dimSize;
        Array<T, CurrentDim, Dims...>* arrPtr;
        typename nextDim::LastDimensionMajorIterator nextDimIt;
        int finishFlag;

        void setFinish(){
            finishFlag = 1;
        }
        void resetFinish(){
            finishFlag = 0;
        }
        int getFinish(){
            return finishFlag;
        }
        //Must have correct default constructor
        LastDimensionMajorIterator(): pos(0), dimSize(CurrentDim), finishFlag(0) {}
        //Copy constructor (implicit will probably be correct)
        LastDimensionMajorIterator(const LastDimensionMajorIterator &itToCopy) = default;
        //     pos = itToCopy.pos;
        //     dimSize = itToCopy.dimSize;
        //     arrPtr = itToCopy.arrPtr;
        // }
        LastDimensionMajorIterator(Array<T, CurrentDim, Dims...>* arr, bool endIt){
            pos = endIt? CurrentDim : 0;
            finishFlag = endIt;
            arrPtr = arr;
            dimSize = arr->dimSize;
            nextDimIt = endIt? arr->nextDimArrays[0].lmend() : arr->nextDimArrays[0].lmbegin();
        }
        //Assignment Operator (implicit will prob be correct)
        LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &itToAssign) = default;
        //     pos = itToAssign.pos;
        //     arrPtr = itToAssign.arrPtr;
        //     dimSize = itToAssign.dimSize;
        //     return *this;
        // }
        //equality-inequality functions
        //compiler says they must take excatly one arg
        bool operator==(const LastDimensionMajorIterator &ldmi1){
            bool retVal = (((ldmi1.pos == pos) || (ldmi1.finishFlag && (pos == 0))) && (ldmi1.arrPtr == arrPtr) && (finishFlag == ldmi1.finishFlag) && (nextDimIt == ldmi1.nextDimIt));
            if((retVal) && finishFlag){
                retVal = true;
            } else if(retVal){
                retVal = (nextDimIt == ldmi1.nextDimIt);
            }
            return retVal;
        }
        bool operator!=(const LastDimensionMajorIterator &ldmi1){
            bool retVal = (((ldmi1.pos == pos) || (ldmi1.finishFlag && (pos == 0))) && (ldmi1.arrPtr == arrPtr) && (finishFlag == ldmi1.finishFlag) && (nextDimIt == ldmi1.nextDimIt));
            if((retVal) && finishFlag){
                retVal = true;
            } else if(retVal){
                retVal = (nextDimIt == ldmi1.nextDimIt);
            }
            return (!retVal);
        }
        //Increments the iterator one element and returns incremented iterator (preincrement)
        LastDimensionMajorIterator &operator++(){
            //Array<T, Dims...> tempArr = *arrPtr;
            //This calls the ++ on the next FDMI
            pos++;
            //if it is the end of th array
            if(pos == dimSize){
                setFinish();
                pos = pos%dimSize;
                nextDimIt++;
            }

            updateNextDim();
            //return incremented iterator
            return (*this);
            //tempArr.fmbegin()

        }
        //Increments the iterator one element and returns an iterator pointing to element prior
        LastDimensionMajorIterator operator++(int){
            //auto tempFdmi= arrPtr->fmbegin();
            LastDimensionMajorIterator thisFdmi(*this);
            //increment
            ++(*this);
            //return iterator pointing to element prior
            return thisFdmi;

        }

        void updateNextDim(){
            if(pos != dimSize){
                nextDimIt.arrPtr = &(arrPtr->nextDimArrays[pos]);
            } else{
                nextDimIt.arrPtr = &(arrPtr->nextDimArrays[0]);
            }
            nextDimIt.updateNextDim();
        }
        //Returns reference to the T at this position in the array
        T &operator*() const{
            //dereferece array pointer and index array
            //Array<T,Dims...> tempArr = *arrPtr;
            //T tempT = tempArr[pos];
            //return tempT;
            return *nextDimIt;
        }
    };

    typedef T ValueType;
    //default constructor
    Array(){
        //throw error at compile time if any dimension is 0
        static_assert(CurrentDim != 0, "Dimensions cannot be zero.");
        dimSize = CurrentDim;
        //nextDimArrays = Array<T, Dims...>[dimSize];
    }
    //Copy constructors with same dimensionality
    //(non-template and template)
    Array(const Array &arrToCopy){
        static_assert(CurrentDim != 0, "Dimensions cannot be zero.");
        dimSize = arrToCopy.dimSize;
        //nextDimArrays = new Array<T, Dims...>[dimSize];
        for(size_t i = 0; i < dimSize; i++)
            nextDimArrays[i] = arrToCopy.nextDimArrays[i];

    }
    template <typename U>
    Array(const Array<U, CurrentDim, Dims...> &arrToCopy){
        static_assert(CurrentDim != 0, "Dimensions cannot be zero.");
        dimSize = arrToCopy.dimSize;
        //nextDimArrays = new Array<T, Dims...>[dimSize];
        for(size_t i = 0; i < dimSize; i++)
            nextDimArrays[i] = arrToCopy.nextDimArrays[i];

    }
    //assignment operator with same dim and no-op self-assign
    //(non-template and template)
    Array &operator=(const Array &arrToAssign){
        if(this != &arrToAssign){
            assert(dimSize == arrToAssign.dimSize);
            for(size_t i = 0; i < CurrentDim; i++)
                nextDimArrays[i] = arrToAssign.nextDimArrays[i];
        }
        return *this;

    }
    template <typename U>
    Array &operator=(const Array<U, CurrentDim, Dims...> &arrToAssign){
        if(this != ((Array<T, CurrentDim, Dims...>*) &arrToAssign)){
            assert(dimSize == arrToAssign.dimSize);
            for(size_t i = 0; i < CurrentDim; i++)
                nextDimArrays[i] = arrToAssign.nextDimArrays[i];
        }
        return *this;
    }
    //index into the array with range-checking
    //throws OutOfRange exception if any indices are out of range
    //T &operator[size_t i_1][size_t i_2]...[size_t i_D];
    //const T &operator[size_t i_1][size_t i_2]...[size_t i_D] const;
    //need this
    Array<T, Dims...> &operator[](size_t index){
        if(!(index < CurrentDim && index >= 0)){ throw OutOfRange(); }
        return nextDimArrays[index];
    }
    const Array<T, Dims...> &operator[](size_t index) const{
        if(!(index < CurrentDim && index >= 0)){ throw OutOfRange(); }
        return nextDimArrays[index];
    }

    //Iterators
    //Returns an object pointing to the first element in the first dim
    FirstDimensionMajorIterator fmbegin(){
        // FirstDimensionMajorIterator* tempFm = new FirstDimensionMajorIterator();
        // tempFm->pos = 0;
        // tempFm->arrPtr = &nextDimArrays[dimSize];
        // return *tempFm;
        return FirstDimensionMajorIterator(this, false);
    }
    //Returns an object pointing one past the last element in the first dim
    FirstDimensionMajorIterator fmend(){
        // FirstDimensionMajorIterator* tempFm = new FirstDimensionMajorIterator();
        // tempFm->pos = CurrentDim;
        // tempFm->arrPtr = &nextDimArrays[dimSize];
        // return *tempFm;
        return FirstDimensionMajorIterator(this, true);
    }
    //Returns an object pointing to the first element in the last dim
    LastDimensionMajorIterator lmbegin(){
        // LastDimensionMajorIterator* tempLm = new LastDimensionMajorIterator();
        // tempLm->pos = 0;
        // tempLm->arrPtr = &nextDimArrays[dimSize];
        // return *tempLm;
        return LastDimensionMajorIterator(this, false);
    }
    //Returns an object pointing to the last element in the last dim
    LastDimensionMajorIterator lmend(){
        // LastDimensionMajorIterator* tempLm = new LastDimensionMajorIterator();
        // tempLm->pos = CurrentDim;
        // tempLm->arrPtr = &nextDimArrays[dimSize];
        // return *tempLm;
        return LastDimensionMajorIterator(this, true);
    }

    //~Array(){}

};

template <typename T, size_t CurrentDim>
class Array<T, CurrentDim> {
public:
    size_t dimSize;
    T baseArray[CurrentDim];

/*
    class FirstDimensionMajorIterator{
    public:
        //position
        size_t pos;
        T* arrPtr;
        Array<T, CurrentDim>* APtr;
        //Must have correct default constructor
        FirstDimensionMajorIterator(): pos(0), arrPtr(nullptr), APtr(nullptr){}
        //Copy constructor (implicit will probably be correct)
        FirstDimensionMajorIterator(const FirstDimensionMajorIterator &itToCopy){
            pos = itToCopy.pos;
            arrPtr = itToCopy.arrPtr;
            APtr = itToCopy.APtr;
        }
        FirstDimensionMajorIterator(T* arr, bool endIt){
            pos = endIt? CurrentDim-1 : 0;
            arrPtr = arr;
        }
        //Assignment Operator (implicit will prob be correct)
        FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &itToAssign){
            pos = itToAssign.pos;
            arrPtr = itToAssign.arrPtr;
            APtr = itToAssign.APtr;
            return *this;
        }
        //equality-inequality functions
        bool operator==(const FirstDimensionMajorIterator &fdmi1){
            bool retVal = (fdmi1.pos == pos) && (fdmi1.arrPtr == arrPtr);
            return retVal;
        }
        bool operator!=(const FirstDimensionMajorIterator &fdmi1){
            bool retVal = !((fdmi1.pos == pos) && (fdmi1.arrPtr == arrPtr));
            return retVal;
        }
        //Increments the iterator one element and returns incremented iterator (preincrement)
        FirstDimensionMajorIterator &operator++(){
            //Array<T, Dims...> tempArr = *arrPtr;
            //base class increments current position
            //FirstDimensionMajorIterator thisFdmi(*this);
            //thisFdmi.pos++;
            pos++;
            //return incremented iterator
            return (*this);
            //tempArr.fmbegin()

        }
        //Increments the iterator one element and returns an iterator pointing to element prior
        FirstDimensionMajorIterator operator++(int){
            FirstDimensionMajorIterator thisFdmi = *this;
            //increment
            pos++;
            //return iterator pointing to element prior
            return thisFdmi;

        }
        //Returns reference to the T at this position in the array
        T &operator*() const{
            //dereferece array pointer and index array
            return (arrPtr[pos]);
        }
    };
*/
    class FirstDimensionMajorIterator{
    public:
        //position
        size_t pos;
        size_t dimSize;
        Array<T, CurrentDim>* arrPtr;
        T* objPtr;
        int finishFlag;

        void setFinish(){
            finishFlag = 1;
        }
        void resetFinish(){
            finishFlag = 0;
        }
        int getFinish(){
            return finishFlag;
        }
        //Must have correct default constructor
        FirstDimensionMajorIterator(): pos(0), dimSize(CurrentDim), finishFlag(0) {}
        //Copy constructor (implicit will probably be correct)
        FirstDimensionMajorIterator(const FirstDimensionMajorIterator &itToCopy) = default;
        //     pos = itToCopy.pos;
        //     arrPtr = itToCopy.arrPtr;
        //     dimSize = itToCopy.dimSize;
        //     objPtr = itToCopy.objPtr;
        // }
        FirstDimensionMajorIterator(Array<T, CurrentDim>* arr, bool endIt){
            pos = endIt? CurrentDim : 0;
            finishFlag = endIt;
            arrPtr = arr;
            dimSize = arr->dimSize;
            objPtr = &(arr->baseArray[0]);
        }
        //Assignment Operator (implicit will prob be correct)
        FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &itToAssign) = default;
        //    pos = itToAssign.pos;
        //    arrPtr = itToAssign.arrPtr;
        //    dimSize = itToAssign.dimSize;
        //    objPtr = itToAssign.objPtr;
        //    return *this;

        //}
        //equality-inequality functions
        //compiler says they must take excatly one arg
        bool operator==(const FirstDimensionMajorIterator &fdmi1){
            bool retVal = ((fdmi1.pos == pos) && (fdmi1.arrPtr == arrPtr) && (finishFlag == fdmi1.finishFlag));
            if((retVal) && finishFlag){
                retVal = true;
            } else if(retVal){
                retVal = (objPtr == fdmi1.objPtr);
            }
            return retVal;
        }
        bool operator!=(const FirstDimensionMajorIterator &fdmi1){
            bool retVal = ((fdmi1.pos == pos) && (fdmi1.arrPtr == arrPtr) && (finishFlag == fdmi1.finishFlag));
            if((retVal) && finishFlag){
                retVal = true;
            } else if(retVal){
                retVal = (objPtr == fdmi1.objPtr);
            }
            return (!retVal);
        }
        //Increments the iterator one element and returns incremented iterator (preincrement)
        FirstDimensionMajorIterator &operator++(){
            //Array<T, Dims...> tempArr = *arrPtr;
            //increment first
            pos++;
            (pos%dimSize)? resetFinish() : setFinish();
            //if it is the end of th array
            updateNextDim();

            //return incremented iterator
            return (*this);
            //tempArr.fmbegin()

        }
        //Increments the iterator one element and returns an iterator pointing to element prior
        FirstDimensionMajorIterator operator++(int){
            //auto tempFdmi= arrPtr->fmbegin();
            FirstDimensionMajorIterator thisFdmi(*this);
            //increment
            ++(*this);
            //return iterator pointing to element prior
            return thisFdmi;

        }

        void updateNextDim(){
            if(pos != dimSize){
                objPtr = &(arrPtr->baseArray[pos]);
            } else{
                objPtr = &(arrPtr->baseArray[0]);
            }
        }
        //Returns reference to the T at this position in the array
        T &operator*() const{
            //dereferece array pointer and index array
            //Array<T,Dims...> tempArr = *arrPtr;
            //T tempT = tempArr[pos];
            //return tempT;
            return *objPtr;
        }
    };
/*
    class LastDimensionMajorIterator{
    public:
        size_t pos;
        T* arrPtr;
        //Must have correct default constructor
        LastDimensionMajorIterator(): pos(0), arrPtr(nullptr){}
        //Copy constructor (implicit will probably be correct)
        LastDimensionMajorIterator(const LastDimensionMajorIterator &) = default;
        //Assignment Operator (implicit will prob be correct)
        LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &) = default;
        //equality-inequality functions
        bool operator==(const LastDimensionMajorIterator &ldmi1){
            bool retVal = (ldmi1.pos == pos) && (ldmi1.arrPtr == arrPtr);
            return retVal;
        }
        bool operator!=(const LastDimensionMajorIterator &ldmi1){
            bool retVal = !((ldmi1.pos == pos) && (ldmi1.arrPtr == arrPtr));
            return retVal;
        }
        //Increments the iterator one element and returns incremented iterator (preincrement)
        LastDimensionMajorIterator &operator++(){
            //base class increments current position
            //LastDimensionMajorIterator thisLdmi = *this;
            //thisLdmi.pos++;
            pos++;
            //return incremented iterator
            return (*this);
        }
        //Increments the iterator one element and returns an iterator poinrting to element prior
        LastDimensionMajorIterator operator++(int){
            //FirstDimensionMajorIterator tempFdmi = (*arrPtr).fmbegin();
            LastDimensionMajorIterator thisLdmi = *this;
            //increment
            pos++;
            //return iterator pointing to element prior
            return thisLdmi;
        }
        //Returns reference to the T at this position in the array
        T &operator*() const{
            return arrPtr[pos];
        }
    };
*/

    class LastDimensionMajorIterator{
    public:
        //position
        size_t pos;
        size_t dimSize;
        Array<T, CurrentDim>* arrPtr;
        T* objPtr;
        int finishFlag;

        void setFinish(){
            finishFlag = 1;
        }
        void resetFinish(){
            finishFlag = 0;
        }
        int getFinish(){
            return finishFlag;
        }
        //Must have correct default constructor
        LastDimensionMajorIterator(): pos(0), dimSize(CurrentDim), finishFlag(0) {}
        //Copy constructor (implicit will probably be correct)
        LastDimensionMajorIterator(const LastDimensionMajorIterator &itToCopy) = default;
        //     pos = itToCopy.pos;
        //     arrPtr = itToCopy.arrPtr;
        //     dimSize = itToCopy.dimSize;
        //     objPtr = itToCopy.objPtr;
        // }
        LastDimensionMajorIterator(Array<T, CurrentDim>* arr, bool endIt){
            pos = endIt? CurrentDim : 0;
            finishFlag = endIt;
            arrPtr = arr;
            dimSize = arr->dimSize;
            objPtr = &(arr->baseArray[0]);
        }
        //Assignment Operator (implicit will prob be correct)
        LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &itToAssign) = default;
        //     pos = itToAssign.pos;
        //     arrPtr = itToAssign.arrPtr;
        //     dimSize = itToAssign.dimSize;
        //     objPtr = itToAssign.objPtr;
        //     return *this;
        // }
        //equality-inequality functions
        //compiler says they must take excatly one arg
        bool operator==(const LastDimensionMajorIterator &ldmi1){
            bool retVal = (((ldmi1.pos == pos) || (ldmi1.finishFlag && (pos == 0))) && (ldmi1.arrPtr == arrPtr) && (finishFlag == ldmi1.finishFlag));
            if((retVal) && finishFlag){
                retVal = true;
            } else if(retVal){
                retVal = (objPtr == ldmi1.objPtr);
            }
            return retVal;
        }
        bool operator!=(const LastDimensionMajorIterator &ldmi1){
            bool retVal = (((ldmi1.pos == pos) || (ldmi1.finishFlag && (pos == 0))) && (ldmi1.arrPtr == arrPtr) && (finishFlag == ldmi1.finishFlag));
            if((retVal) && finishFlag){
                retVal = true;
            } else if(retVal){
                retVal = (objPtr == ldmi1.objPtr);
            }
            return (!retVal);
        }
        //Increments the iterator one element and returns incremented iterator (preincrement)
        LastDimensionMajorIterator &operator++(){
            //Array<T, Dims...> tempArr = *arrPtr;
            //This calls the ++ on the next FDMI
            pos++;
            (pos%dimSize)? resetFinish() : setFinish();
            pos = pos%dimSize;
            //if it is the end of th array
            updateNextDim();

            //return incremented iterator
            return (*this);
            //tempArr.fmbegin()

        }
        //Increments the iterator one element and returns an iterator pointing to element prior
        LastDimensionMajorIterator operator++(int){
            //auto tempFdmi= arrPtr->fmbegin();
            LastDimensionMajorIterator thisFdmi(*this);
            //increment
            ++(*this);
            //return iterator pointing to element prior
            return thisFdmi;

        }

        void updateNextDim(){
            if(pos != dimSize){
                objPtr = &(arrPtr->baseArray[pos]);
            } else{
                objPtr = &(arrPtr->baseArray[0]);
            }
        }
        //Returns reference to the T at this position in the array
        T &operator*() const{
            //dereferece array pointer and index array
            //Array<T,Dims...> tempArr = *arrPtr;
            //T tempT = tempArr[pos];
            //return tempT;
            return *objPtr;
        }
    };

    typedef T ValueType;
    //default constructor
    Array(){
        //throw error at compile time if any dimension is 0
        static_assert(CurrentDim != 0, "Dimensions cannot be zero.");
        dimSize = CurrentDim;
        //baseArray = new T[dimSize];
    }
    //Copy constructors with same dimensionality
    //(non-template and template)
    Array(const Array &arrToCopy){
        static_assert(CurrentDim != 0, "Dimensions cannot be zero.");
        dimSize = CurrentDim;
        //baseArray = new T[dimSize];
        for(size_t i = 0; i < dimSize; i++)
            baseArray[i] = arrToCopy.baseArray[i];

    }
    template <typename U>
    Array(const Array<U, CurrentDim> &arrToCopy){
        static_assert(CurrentDim != 0, "Dimensions cannot be zero.");
        dimSize = CurrentDim;
        //baseArray = new T[dimSize];
        for(size_t i = 0; i < dimSize; i++)
            baseArray[i] = arrToCopy.baseArray[i];

    }
    //assignment operator with same dim and no-op self-assign
    //(non-template and template)
    Array &operator=(const Array &arrToAssign){
        if(this != &arrToAssign){
            assert(dimSize == arrToAssign.dimSize);
            for(size_t i = 0; i < dimSize; i++)
                baseArray[i] = arrToAssign.baseArray[i];
        }
        return *this;

    }
    template <typename U>
    Array &operator=(const Array<U, CurrentDim> &arrToAssign){
        //lacks a cast
        if(this != ((Array<T, CurrentDim>*) &arrToAssign)){
            assert(dimSize == arrToAssign.dimSize);
            for(size_t i = 0; i < dimSize; i++)
                baseArray[i] = arrToAssign.baseArray[i];
        }
        return *this;
    }
    //index into the array with range-checking
    //throws OutOfRange exception if any indices are out of range
    //T &operator[size_t i_1][size_t i_2]...[size_t i_D];
    //const T &operator[size_t i_1][size_t i_2]...[size_t i_D] const;
    //need this
    //base class returns an object of type T on operator[]
    T &operator[](size_t index){
        if(!(index < CurrentDim && index >= 0)){ throw OutOfRange(); }
        return baseArray[index];
    }
    //base class returns an object of type T on operator[]
    const T &operator[](size_t index) const{
        if(!(index < CurrentDim && index >= 0)){ throw OutOfRange(); }
        return baseArray[index];
    }

    //Iterators
    //Returns an object pointing to the first element in the first dim
    FirstDimensionMajorIterator fmbegin(){
        return FirstDimensionMajorIterator(this, false);
    }
    //Returns an object pointing one past the last element in the first dim
    FirstDimensionMajorIterator fmend(){
        return FirstDimensionMajorIterator(this, true);
    }
    //Returns an object pointing to the first element in the last dim
    LastDimensionMajorIterator lmbegin(){
        // LastDimensionMajorIterator* tempLm = new LastDimensionMajorIterator();
        // tempLm->pos = 0;
        // tempLm->arrPtr = &baseArray[dimSize];
        // return *tempLm;
        return LastDimensionMajorIterator(this, false);
    }
    //Returns an object pointing to the last element in the last dim
    LastDimensionMajorIterator lmend(){
        // LastDimensionMajorIterator* tempLm = new LastDimensionMajorIterator();
        // tempLm->pos = CurrentDim;
        // tempLm->arrPtr = &baseArray[dimSize];
        // return *tempLm;
        return LastDimensionMajorIterator(this, true);
    }

};

}

#endif
