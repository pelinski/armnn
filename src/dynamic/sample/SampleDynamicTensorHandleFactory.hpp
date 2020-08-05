//
// Copyright © 2020 Arm Ltd and Contributors. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include "SampleMemoryManager.hpp"

#include <armnn/backends/ITensorHandleFactory.hpp>

namespace armnn
{

constexpr const char * SampleDynamicTensorHandleFactoryId() { return "Arm/SampleDynamic/TensorHandleFactory"; }

class SampleDynamicTensorHandleFactory : public ITensorHandleFactory
{

public:
    SampleDynamicTensorHandleFactory(std::shared_ptr<SampleMemoryManager> mgr)
    : m_MemoryManager(mgr),
      m_ImportFlags(static_cast<MemorySourceFlags>(MemorySource::Malloc)),
      m_ExportFlags(static_cast<MemorySourceFlags>(MemorySource::Malloc))
    {}

    std::unique_ptr<ITensorHandle> CreateSubTensorHandle(ITensorHandle& parent,
                                                         TensorShape const& subTensorShape,
                                                         unsigned int const* subTensorOrigin) const override;

    std::unique_ptr<ITensorHandle> CreateTensorHandle(const TensorInfo& tensorInfo) const override;

    std::unique_ptr<ITensorHandle> CreateTensorHandle(const TensorInfo& tensorInfo,
                                                      DataLayout dataLayout) const override;

    std::unique_ptr<ITensorHandle> CreateTensorHandle(const TensorInfo& tensorInfo,
                                                      const bool IsMemoryManaged) const override;

    std::unique_ptr<ITensorHandle> CreateTensorHandle(const TensorInfo& tensorInfo,
                                                      DataLayout dataLayout,
                                                      const bool IsMemoryManaged) const override;

    static const FactoryId& GetIdStatic();

    const FactoryId& GetId() const override;

    bool SupportsSubTensors() const override;

    MemorySourceFlags GetExportFlags() const override;

    MemorySourceFlags GetImportFlags() const override;

private:
    mutable std::shared_ptr<SampleMemoryManager> m_MemoryManager;
    MemorySourceFlags m_ImportFlags;
    MemorySourceFlags m_ExportFlags;
};

} // namespace armnn

