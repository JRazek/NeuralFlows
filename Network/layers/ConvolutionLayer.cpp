//
// Created by jrazek on 27.07.2021.
//
#include "ConvolutionLayer.h"
#include "../Network.h"

cn::ConvolutionLayer::ConvolutionLayer(int _id, Network &_network, int _kernelSizeX, int _kernelSizeY,
                                       int _kernelsCount, const DifferentiableFunction &_activationFunction,
                                       int _paddingX, int _paddingY, int _strideX, int _strideY) :
        kernelSize(_kernelSizeX, _kernelSizeY, network->getInputSize(_id).z),
        kernelsCount(_kernelsCount),
        activationFunction(_activationFunction),
        paddingX(_paddingX),
        paddingY(_paddingY),
        strideX(_strideX),
        strideY(_strideY),
        biases(kernelsCount),
        cn::Learnable(_id, _network, _kernelsCount) {

    Vector3<int> inputSize = network->getInputSize(_id);

    if(inputSize.x < kernelSize.x || inputSize.y < kernelSize.y){
        throw std::logic_error("kernel must not be larger than input!");
    }
    kernels.reserve(_kernelsCount);

    for(int i = 0; i < _kernelsCount; i ++){
        kernels.emplace_back(kernelSize);
    }

    int oX = Utils::afterConvolutionSize(kernelSize.x, inputSize.x, paddingX, strideX);
    int oY = Utils::afterConvolutionSize(kernelSize.y, inputSize.y, paddingY, strideY);
    int oZ = kernelsCount;
    outputSize = Vector3<int>(oX, oY, oZ);
}

cn::Bitmap<float> cn::ConvolutionLayer::run(const Bitmap<float> &input) {
    _input = &input;
    if(input.size() != network->getInputSize(__id)){
        throw std::logic_error("CLayer fed with wrong input size!");
    }

    for(int i = 0; i < kernelsCount; i ++){
        Bitmap<float> layer = Utils::sumBitmapLayers(Utils::convolve(kernels[i], input, paddingX, paddingY, strideX, strideY));
        beforeActivation->setLayer(i, layer.data());
    }
    Bitmap<float> result = beforeActivation.value();
    for(auto it = beforeActivation->data(); it != beforeActivation->data() + beforeActivation->w() * beforeActivation->h() * beforeActivation->d(); ++it){
        int index = it - beforeActivation->data();
        *(result.data() + index) = activationFunction.func(*it);
    }
    return result;
}

void cn::ConvolutionLayer::randomInit() {
    for(auto &k : kernels){
        for(auto it = k.data(); it != k.data() + k.w() * k.h() * k.d(); ++it){
            *it = network->getRandom(-1, 1);
        }
    }
    for(auto &b : biases){
        b = network->getRandom(-5, 5);
    }
}

float cn::ConvolutionLayer::getChain(const Vector3<int> &inputPos) {
    if(getMemoState(inputPos)){
        return getMemo(inputPos);
    }
    Bitmap<float> paddedInput = Utils::addPadding(*_input, paddingX, paddingY);

    auto validPos = [this](const Vector2<int> &kernelPos, const Bitmap<float> &bitmap){
        return kernelPos.x >= 0 && kernelPos.y >= 0 && kernelPos.x + kernelSizeX - 1 < bitmap.w() && kernelPos.y + kernelSizeY - 1 < bitmap.h();
    };

    float result = 0;

    Vector3<int> inputPosPadded(inputPos.x - paddingX, inputPos.y - paddingY, inputPos.z);
    for(int c = 0; c < kernelsCount; c++){
        for(int y = 0; y < kernelSizeY; y++){
            for(int x = 0; x < kernelSizeX; x++){
                Vector2<int> kernelPos(inputPosPadded.x - x, inputPosPadded.y - y);
                if(validPos(kernelPos, paddedInput)){
                    Vector2<int> shift = Vector2<int>(inputPosPadded.x, inputPosPadded.y) - kernelPos;
                    float weight = kernels[c].getCell(shift.x, shift.y, inputPosPadded.z);
                    Vector3<int> outputPos (kernelPos.x / strideX, kernelPos.y / strideY, inputPosPadded.z);
                    result += weight * network->getChain(__id + 1, outputPos);
                }
            }
        }
    }
    setMemo(inputPos, result);
    return result;
}

float cn::ConvolutionLayer::diffWeight(int weightID) {
    int kSize = kernelSizeX * kernelSizeY * kernelSizeZ;
    Bitmap<float> paddedInput = Utils::addPadding(*_input, paddingX, paddingY);
    Vector3<int> weightPos = kernels[weightID / kSize].indexToVector(weightID % kSize);
    int kID = weightID / kSize;

    float result = 0;
    for(int y = 0; y < output->h() - kernelSizeY; y++){
        for(int x = 0; x < output->w() - kernelSizeX; x++){
            int inputX = x * strideX + weightPos.x;
            int inputY = y * strideY + weightPos.y;
            float inputValue = _input->getCell(inputX, inputY, weightPos.z);
            Vector3<int> nextPos (x, y, kID);
            result += inputValue * activationFunction.derive(beforeActivation->getCell(nextPos)) * network->getChain(__id + 1, nextPos);
        }
    }
    return result;
}

int cn::ConvolutionLayer::weightsCount() const {
    return kernelSizeX * kernelSizeY * kernelSizeZ * kernelsCount;
}

std::vector<float> cn::ConvolutionLayer::getGradient() {
    std::vector<float> gradient(weightsCount());
    for(int i = 0; i < weightsCount(); i ++){
        gradient[i] = diffWeight(i);
    }
    return gradient;
}

void cn::ConvolutionLayer::setWeight(int weightID, float value) {

}

float cn::ConvolutionLayer::getWeight(int weightID) const {
    int kSize = kernelSizeX * kernelSizeY * kernelSizeZ;
    return *kernels[weightID / (kSize)].data() + (weightID % kSize);
}

