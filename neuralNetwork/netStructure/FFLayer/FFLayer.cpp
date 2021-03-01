#include "FFLayer.h"
#include <netStructure/ConvolutionLayer/CLayer.h>
#include <Net.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

FFLayer::FFLayer(int id, Net * net, int inputVectorSize, int neuronsCount, ActivationFunction * f):
    inputVectorSize(inputVectorSize), activationFunction(f), 
    Layer(id, net, neuronsCount){
        for(int i = 0; i < neuronsCount; i ++){
            this->neurons.push_back(new Neuron(i));
        }
    }
FFLayer::Neuron::Neuron(int idInLayer):idInLayer(idInLayer){}
void FFLayer::initConnections(){
    Layer * prevLayer = Layer::net->layers[Layer::idInNet - 1];
    for(auto n : this->neurons){
        float randBias = (rand() % 1000)/100.f;
        int inputSize = this->inputVectorSize;
        for(int i = 0; i < inputSize; i ++){
            float randWeight = (rand() % 100)/100.f;
            n->inputEdges.push_back({i, randWeight});
        }
    }
}

FFLayer::~FFLayer(){
    for(auto n : this->neurons){
        delete n;
    }
    delete activationFunction;
}
void FFLayer::run(const std::vector<float> &input){
    if(this->neurons.size() <= 0){
        throw std::invalid_argument( "neurons not initiated in layer " + this->idInNet );
        return;
    }

    if(input.size() != this->neurons[0]->inputEdges.size()){
        throw std::invalid_argument( "wrong input size!" );
        return;
    }
    this->outputVector.clear();

    for(int i = 0; i < neurons.size(); i ++){
        Neuron * n = neurons[i];
        float sum = 0;
        for(int j = 0; j < n->inputEdges.size(); j ++){
            sum += input[j] * n->inputEdges[j].second;
        }
        sum += n->bias;
        
        float activatedVal = activationFunction->getValue(sum);
        this->outputVector.push_back(activatedVal);
    }
}