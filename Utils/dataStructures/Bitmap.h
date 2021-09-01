//
// Created by jrazek on 30.07.2021.
//

#include <algorithm>
#include "../Utils.h"
#ifndef NEURALNETLIBRARY_BITMAP_H
#define NEURALNETLIBRARY_BITMAP_H


namespace cn {
    template<typename T>
    class Bitmap {
    protected:

        /**
         * data should be stored in 0th format described in Utils.h file
         */
        T * dataP;
        int _w, _h, _d;
    public:
        Bitmap(int w, int h, int d);
        Bitmap(const Vector3<int> &s);
        Bitmap(int w, int h, int d, const T *data, int inputType = 0);
        Bitmap(const Vector3<int> &s, const T *data, int inputType = 0);
        Bitmap(const Bitmap &bitmap);
        Bitmap(Bitmap &&bitmap);
        Bitmap();
        ~Bitmap();
        T getCell(int col, int row, int depth) const;
        T getCell(const Vector3<int> &c) const;
        void setCell(int col, int row, int depth, T val);
        void setCell(const Vector3<int> &c, T b);
        void setData(const T * data, int inputType = 0);
        void setLayer(int layerID, T *input);
        [[nodiscard]] int getDataIndex(int col, int row, int depth) const;
        [[nodiscard]] int getDataIndex(const Vector3<T> &v) const;
        Vector3<int> indexToVector(int index) const;

        Bitmap<T> &operator=(const Bitmap<T> &other);
        Bitmap<T> &operator=(Bitmap<T> &&other);
        bool belongs(const Vector3<int> &point) const;
        T * data() const;
        int w() const;
        int h() const;
        int d() const;
        Vector3<int> size() const;
    };
}

/////DEFINITIONS/////
template<typename T>
cn::Bitmap<T>::Bitmap(int w, int h, int d): _w(w), _h(h), _d(d) {
    if(_w < 1 || _h < 1 || _d < 1)
        throw std::logic_error("invalid bitmap size!");
    dataP = new T [_w * _h * _d];
}

template<typename T>
cn::Bitmap<T>::Bitmap(const Bitmap<T> &bitmap): Bitmap(bitmap.w(), bitmap.h(), bitmap.d()) {
    std::copy(bitmap.dataP, bitmap.dataP + _w * _h * _d, dataP);
}

template<typename T>
T cn::Bitmap<T>::getCell(int col, int row, int depth) const {
    return dataP[getDataIndex(col, row, depth)];
}

template<typename T>
void cn::Bitmap<T>::setCell(int col, int row, int depth, T val) {
    dataP[getDataIndex(col, row, depth)] = val;
}

template<typename T>
T *cn::Bitmap<T>::data() const{
    return dataP;
}

template<typename T>
cn::Bitmap<T>::~Bitmap() {
    delete [] dataP;
}

template<typename T>
cn::Bitmap<T>::Bitmap(int _w, int _h, int _d, const T *data, int inputType): Bitmap(_w, _h, _d) {
    setData(data, inputType);
}

template<typename T>
int cn::Bitmap<T>::getDataIndex(int col, int row, int depth) const{
    if(!belongs({col, row, depth})){
        throw std::invalid_argument("cell does not belong to bitmap!");
    }
    return depth * _w * _h + row * _w + col;
}

template<typename T>
cn::Bitmap<T> &cn::Bitmap<T>::operator=(const cn::Bitmap<T> &other) {
    if(&other != this) {
        delete[] dataP;
        _w = other.w();
        _h = other.h();
        _d = other.d();
        dataP = new T[_w * _h * _d];
        std::copy(other.data(), other.data() + _w * _h * _d, dataP);
    }
    return *this;
}

template<typename T>
cn::Bitmap<T> &cn::Bitmap<T>::operator=(cn::Bitmap<T> &&other){
    if(&other != this) {
        delete [] dataP;
        _w = other.w();
        _h = other.h();
        _d = other.d();
        dataP = other.dataP;
        other.dataP = nullptr;
    }
    return *this;
}

template<typename T>
cn::Bitmap<T>::Bitmap(cn::Bitmap<T> &&bitmap):_w(bitmap.w()), _h(bitmap.h()), _d(bitmap.d()){
    dataP = bitmap.dataP;
    bitmap.dataP = nullptr;
}

template<typename T>
void cn::Bitmap<T>::setData(const T *data, int inputType) {
    cn::Utils::convert(data, dataP, _w, _h, _d, inputType, 0);
}

template<typename T>
void cn::Bitmap<T>::setLayer(int layerID, T *input) {
    std::copy(input, input + _w * _h, dataP + _w * _h * layerID);
}


template<typename T>
int cn::Bitmap<T>::w() const {
    return _w;
}

template<typename T>
int cn::Bitmap<T>::h() const {
    return _h;
}

template<typename T>
int cn::Bitmap<T>::d() const {
    return _d;
}

template<typename T>
T cn::Bitmap<T>::getCell(const Vector3<int> &c) const {
    return getCell(c.x, c.y, c.z);
}

template<typename T>
int cn::Bitmap<T>::getDataIndex(const Vector3<T> &v) const {
    return getDataIndex(v.x, v.y, v.z);
}

template<typename T>
Vector3<int> cn::Bitmap<T>::indexToVector(int index) const{
    return {index % _w, (index / _w) % _w, index / (_w * _h)};
}

template<typename T>
void cn::Bitmap<T>::setCell(const Vector3<int> &c, T b) {
    setCell(c.x, c.y, c.z, b);
}

template<typename T>
Vector3<int> cn::Bitmap<T>::size() const {
    return Vector3<int>(_w, _h, _d);
}

template<typename T>
cn::Bitmap<T>::Bitmap(const Vector3<int> &s):Bitmap(s.x, s.y, s.z) {}

template<typename T>
cn::Bitmap<T>::Bitmap(const Vector3<int> &s, const T *data, int inputType):Bitmap(s.x, s.y, s.z, data, inputType) {}

template<typename T>
cn::Bitmap<T>::Bitmap():Bitmap(0, 0, 0) {
    dataP = nullptr;
}

template<typename T>
bool cn::Bitmap<T>::belongs(const Vector3<int> &p) const {
    return p.x >= 0 && p.x < _w && p.y >= 0 && p.y < _h && p.z >= 0 && p.z < _d;
}

//


#endif //NEURALNETLIBRARY_BITMAP_H
