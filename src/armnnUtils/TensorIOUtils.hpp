//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <armnn/Tensor.hpp>

#include <boost/format.hpp>
#include <mapbox/variant.hpp>

namespace armnnUtils
{

template<typename TContainer>
inline armnn::InputTensors MakeInputTensors(const std::vector<armnn::BindingPointInfo>& inputBindings,
                                            const std::vector<TContainer>& inputDataContainers)
{
    armnn::InputTensors inputTensors;

    const size_t numInputs = inputBindings.size();
    if (numInputs != inputDataContainers.size())
    {
        throw armnn::Exception(boost::str(boost::format("The number of inputs does not match number of "
                                                        "tensor data containers: %1% != %2%")
                                          % numInputs
                                          % inputDataContainers.size()));
    }

    for (size_t i = 0; i < numInputs; i++)
    {
        const armnn::BindingPointInfo& inputBinding = inputBindings[i];
        const TContainer& inputData = inputDataContainers[i];

        mapbox::util::apply_visitor([&](auto&& value)
        {
            if (value.size() != inputBinding.second.GetNumElements())
            {
               throw armnn::Exception(boost::str(boost::format("The input tensor has incorrect size "
                                                               "(expected %1% got %2%)")
                                                 % inputBinding.second.GetNumElements()
                                                 % value.size()));
            }

            armnn::ConstTensor inputTensor(inputBinding.second, value.data());
            inputTensors.push_back(std::make_pair(inputBinding.first, inputTensor));
        },
        inputData);
    }

    return inputTensors;
}

template<typename TContainer>
inline armnn::OutputTensors MakeOutputTensors(const std::vector<armnn::BindingPointInfo>& outputBindings,
                                              std::vector<TContainer>& outputDataContainers)
{
    armnn::OutputTensors outputTensors;

    const size_t numOutputs = outputBindings.size();
    if (numOutputs != outputDataContainers.size())
    {
        throw armnn::Exception(boost::str(boost::format("Number of outputs does not match number of "
                                                        "tensor data containers: %1% != %2%")
                                          % numOutputs
                                          % outputDataContainers.size()));
    }

    for (size_t i = 0; i < numOutputs; i++)
    {
        const armnn::BindingPointInfo& outputBinding = outputBindings[i];
        TContainer& outputData = outputDataContainers[i];

        mapbox::util::apply_visitor([&](auto&& value)
        {
            if (value.size() != outputBinding.second.GetNumElements())
            {
                throw armnn::Exception("Output tensor has incorrect size");
            }

            armnn::Tensor outputTensor(outputBinding.second, value.data());
            outputTensors.push_back(std::make_pair(outputBinding.first, outputTensor));
        },
        outputData);
    }

    return outputTensors;
}

} // namespace armnnUtils
