//
// Created by jrazek on 27.07.2021.
//

#include "Layer.h"
#include "../../Network.h"

cn::Layer::Layer(int _id, Network &_network): __id(_id), network(&_network){
    const Vector3<int> inputSize = network->getInput(__id).size();
    memoizationStates.emplace(Bitmap<bool>(inputSize));
    memoizationTable.emplace(Bitmap<float>(inputSize));
    resetMemoization();
}

[[maybe_unused]] int cn::Layer::id() const {
    return __id;
}

void cn::Layer::setMemo(const Vector3<int> &pos, float val) {
    memoizationStates->setCell(pos, true);
    memoizationTable->setCell(pos, val);
}

void cn::Layer::resetMemoization() {
    std::fill(memoizationStates->data(), memoizationStates->data() + memoizationStates->w() * memoizationStates->h() * memoizationStates->d(), false);
}

float cn::Layer::getMemo(const Vector3<int> &pos) const {
    return memoizationTable->getCell(pos);
}

bool cn::Layer::getMemoState(const Vector3<int> &pos) const {
    return memoizationStates->getCell(pos);
}

Vector3<int> cn::Layer::getOutputSize() const {
    return outputSize;
}

