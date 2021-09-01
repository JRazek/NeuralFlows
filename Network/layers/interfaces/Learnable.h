//
// Created by jrazek on 24.08.2021.
//

#ifndef NEURALNETLIBRARY_LEARNABLE_H
#define NEURALNETLIBRARY_LEARNABLE_H
#include "Layer.h"

namespace cn {
    class Network;
    class Learnable : public Layer{
    protected:
        int neuronsCount;
    public:
        int getNeuronsCount() const;
        virtual int weightsCount() const = 0;
        virtual int biasesCount() const = 0;
        virtual void setWeight(int weightID, float value) = 0;
        virtual float getWeight(int weightID) const = 0;
        virtual void setBias(int neuronID, float value) = 0;
        virtual float getBias(int neuronID) const = 0;

        virtual void randomInit() = 0;
        Learnable(int id, Network &network, int neuronsCount);
        virtual std::vector<float> getWeightsGradient() = 0;
        virtual std::vector<float> getBiasesGradient() = 0;
    };
}

#endif //NEURALNETLIBRARY_LEARNABLE_H
