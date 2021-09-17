//
// Created by jrazek on 27.07.2021.
//

#ifndef NEURALNETLIBRARY_LAYER_H
#define NEURALNETLIBRARY_LAYER_H

#include <vector>
#include <optional>
#include "../../../Utils/dataStructures/Bitmap.h"
#include "../../../Utils/interfaces/JSONEncodable.h"

namespace cn {

    class FFLayer;

    template<typename T>
    struct Vector3;
    class Network;
    class Layer : public JSONEncodable{
    protected:

        std::optional<Bitmap<double>> output;

        Layer *prevLayer = nullptr;
        Layer *nextLayer = nullptr;

        Vector3<int> inputSize;
        Vector3<int> outputSize;

        std::optional<Bitmap<bool>> memoizationStates;
        std::optional<Bitmap<double>> memoizationTable;

        int __id;


        /**
         * todo - each layer will have 4 files - .h, .cpp .cuh and .cu
         */
        virtual void CUDARun() = 0;
        virtual void CPURun() = 0;

    public:
        Layer(int _id, Vector3<int> _inputSize);

        /**
         *
         * @param _input to process
         */
        virtual void run(const Bitmap<double> &_input) = 0;

        virtual double getChain(const Vector3<int> &inputPos) = 0;

        virtual ~Layer() = default;

        void resetMemoization();
        void setMemo(const Vector3<int> &pos, double val);
        bool getMemoState(const Vector3<int> &pos) const;
        double getMemo(const Vector3<int> &pos) const;
        Vector3<int> getOutputSize() const;

        [[maybe_unused]] int id() const;

        //for development purposes only. To delete in future
        virtual JSON jsonEncode() const override;

        virtual std::unique_ptr<Layer> getCopyAsUniquePtr() const = 0;

        static std::unique_ptr<cn::Layer> fromJSON(const cn::JSON &json);

        void setPrevLayer(Layer *_prevLayer);

        const std::optional<Bitmap<double>> &getOutput() const;
        virtual const std::optional<Bitmap<double>> &getInput() const;

        void setNextLayer(Layer *_nextLayer);
    };
}


#endif //NEURALNETLIBRARY_LAYER_H
