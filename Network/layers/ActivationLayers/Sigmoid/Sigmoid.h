//
// Created by user on 06.09.2021.
//

#ifndef NEURALNETLIBRARY_SIGMOID_H
#define NEURALNETLIBRARY_SIGMOID_H
#include <cmath>
#include "../../interfaces/Layer.h"


namespace cn {
    class Sigmoid : public Layer{
    public:
//        constexpr static double e = M_E;

        Sigmoid(Vector3<int> _inputSize);
        Sigmoid(const JSON &json);
        virtual void CPURun(const Tensor<double> &input) override;
        virtual double getChain(const Vector4<int> &inputPos) override;
        virtual JSON jsonEncode() const override;
        static double sigmoid(double x);
        static double diff(double x);
        std::unique_ptr<Layer> getCopyAsUniquePtr() const noexcept override;
        virtual std::unique_ptr<Layer> reproduce(const Layer *net, int seed = 1) const override;
    };
}

#endif //NEURALNETLIBRARY_SIGMOID_H
