//
// Created by jrazek on 24.09.2021.
//
#include "../../Network.h"
#include "RecurrentLayer.h"
#include "../InputLayer/InputLayer.h"
#include "RecurrentOutputLayer/RecurrentOutputLayer.h"

std::unique_ptr<cn::Layer> cn::RecurrentLayer::getCopyAsUniquePtr() const noexcept{
    return std::make_unique<RecurrentLayer>(*this);
}

cn::RecurrentLayer::RecurrentLayer(const Vector3<int> &_inputSize, std::vector<std::unique_ptr<Layer>> &&layers) :
ComplexLayer(_inputSize),
internalLayers(std::move(layers)),
identity(inputSize){
    outputSize = inputSize;
    std::fill(identity.data(), identity.data() + identity.size().multiplyContent(), 0);
    internalLayers.push_back(std::make_unique<InputLayer>(InputLayer(inputSize)));
}

void cn::RecurrentLayer::CPURun(const cn::Tensor<double> &_input) {
    for(auto &l : internalLayers)
        l->incTime();
    Tensor<double> res = Utils::elementWiseSum(_input, _time == 0 ? identity : output[getTime() - 1]);
    const Tensor<double> *input = &res;
    for(u_int i = 0; i < internalLayers.size(); i ++){
        internalLayers[i]->CPURun(*input);
        input = &internalLayers[i]->getOutput(getTime());
    }
    output.push_back(*input);

    //todo check this!!!!!!
}

double cn::RecurrentLayer::getChain(const Vector4<int> &inputPos) {
//    return internalLayers[0]->getChain(inputPos) * nextLayer->getChain();
}

cn::JSON cn::RecurrentLayer::jsonEncode() const {
    JSON structure;
    structure["input_size"] = inputSize.jsonEncode();
    structure["type"] = "rcl";
    structure["internal_layers"] = std::vector<JSON>();
    for(auto &l : internalLayers){
        structure["internal_layers"].push_back(l->jsonEncode());
    }
    return structure;
}

cn::RecurrentLayer::RecurrentLayer(const cn::JSON &json) :
RecurrentLayer(Vector3<int>(json.at("input_size"))) {
    for(auto &layerJSON : json.at("internal_layers")){
        std::unique_ptr<Layer> layer = Layer::fromJSON(layerJSON);
        internalLayers.push_back(std::move(layer));
    }
}

cn::RecurrentLayer::RecurrentLayer(const cn::RecurrentLayer &recurrentLayer): ComplexLayer(recurrentLayer) {
    for(const std::unique_ptr<Layer> &l : recurrentLayer.internalLayers){
        internalLayers.push_back(l.get()->getCopyAsUniquePtr());
    }
}

cn::RecurrentLayer::RecurrentLayer(const Vector3<int> &_inputSize) : ComplexLayer(_inputSize), identity(inputSize)  {
    outputSize = inputSize;
    //todo should the size be fixed??
    std::fill(identity.data(), identity.data() + identity.size().multiplyContent(), 0);
}

void cn::RecurrentLayer::ready() {
    internalLayers.push_back(std::make_unique<RecurrentOutputLayer>(RecurrentOutputLayer(inputSize, *this)));
    Network::linkLayers(internalLayers);
}

double cn::RecurrentLayer::getChainFromChild(const cn::Vector4<int> &inputPos) {
    return nextLayer->getChain({inputPos.x, inputPos.y, inputPos.z, getTime() - 1});
}

