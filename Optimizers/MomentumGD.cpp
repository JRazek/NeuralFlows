//
// Created by user on 05.09.2021.
//

#include "MomentumGD.h"

cn::MomentumGD::MomentumGD(cn::Network &_network, int _samplesCount, double _learningRate):network(_network), samplesCount(_samplesCount), learningRate(_learningRate) {}