//
// Created by user on 21.08.2021.
//

#include "Backpropagation.h"
#include "../Utils/dataStructures/Bitmap.h"
#include "../Network/Network.h"
cn::Backpropagation::Backpropagation(Network &_network, float _learningRate, int _miniBatchSize) :
        network(_network),
        learningRate(_learningRate),
        miniBatchSize(_miniBatchSize),
        iteration(0)
        {}

void cn::Backpropagation::propagate(const cn::Bitmap<float> &target) {
    if(!(iteration % miniBatchSize)){
        memorizedWeights.clear();
        memorizedBiases.clear();
        memorizedWeights.resize(network.getLearnables()->size(), std::vector<float>());
        memorizedBiases.resize(network.getLearnables()->size(), std::vector<float>());
        for(int i = 0; i < memorizedWeights.size(); i ++){
            memorizedWeights[i].resize(network.getLearnables()->at(i)->weightsCount(), 0);
        }
        for(int i = 0; i < memorizedBiases.size(); i ++){
            memorizedBiases[i].resize(network.getLearnables()->at(i)->biasesCount(), 0);
        }
    }
    network.resetMemoization();
    OutputLayer *layer = network.getOutputLayer();
    layer->setTarget(&target);
    const Bitmap<float> &output = *network.getOutput(layer->id());
    if(output.size() != target.size()){
        throw std::logic_error("Backpropagation, invalid target!");
    }
    for(int k = 0; k < network.getLearnables()->size(); k ++){
        Learnable *learnable = network.getLearnables()->at(k);
        std::vector<float> weightsGradient = learnable->getWeightsGradient();
        std::vector<float> biasesGradient = learnable->getBiasesGradient();
        for(int i = 0; i < weightsGradient.size(); i ++){
            memorizedWeights[k][i] += learnable->getWeight(i) - learningRate * weightsGradient[i];
        }
        for(int i = 0; i < biasesGradient.size(); i ++){
            memorizedBiases[k][i] += learnable->getBias(i) - learningRate * biasesGradient[i];
        }
    }

    if(!((iteration + 1) % miniBatchSize)){
        for(int k = 0; k < network.getLearnables()->size(); k ++) {
            Learnable *learnable = network.getLearnables()->at(k);
            std::vector<float> &layerWeightGradients = memorizedWeights[k];
            std::vector<float> &layerBiasesGradients = memorizedBiases[k];

            for(int i = 0; i < layerWeightGradients.size(); i ++) {
                learnable->setWeight(i, layerWeightGradients[i] / miniBatchSize);
            }
            for(int i = 0; i < layerBiasesGradients.size(); i ++) {
                learnable->setBias(i, layerBiasesGradients[i] / miniBatchSize);
            }
        }
    }

    iteration++;
}

float cn::Backpropagation::getError(const cn::Bitmap<float> &target) const {
    float error = 0;
    OutputLayer *layer = network.getOutputLayer();
    const Bitmap<float> &output = *network.getOutput(layer->id());
    for(int i = 0; i < target.w(); i ++){
        error += 0.5*std::pow(target.getCell(i, 0, 0) - output.getCell(i, 0, 0), 2);
    }
    return error;
}
