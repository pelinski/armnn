//
// Copyright © 2020 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "BackendProfiling.hpp"
#include "RegisterBackendCounters.hpp"

namespace armnn
{

namespace profiling
{

std::unique_ptr<IRegisterBackendCounters>
    BackendProfiling::GetCounterRegistrationInterface(uint16_t currentMaxGlobalCounterID)
{
    return std::make_unique<RegisterBackendCounters>(RegisterBackendCounters(currentMaxGlobalCounterID, m_BackendId));
}

std::unique_ptr<ISendTimelinePacket> BackendProfiling::GetSendTimelinePacket()
{
    return m_ProfilingService.GetSendTimelinePacket();
}

IProfilingGuidGenerator& BackendProfiling::GetProfilingGuidGenerator()
{
    // The profiling service is our Guid Generator.
    return m_ProfilingService;
}

CounterStatus BackendProfiling::GetCounterStatus(uint16_t backendCounterId)
{
    uint16_t globalCounterId = m_ProfilingService.GetCounterMappings().GetGlobalId(backendCounterId, m_BackendId);
    CaptureData captureData = m_ProfilingService.GetCaptureData();

    CounterStatus counterStatus(backendCounterId, globalCounterId, false, 0);

    if (captureData.IsCounterIdInCaptureData(globalCounterId))
    {
        counterStatus.m_Enabled = true;
        counterStatus.m_SamplingRateInMicroseconds = captureData.GetCapturePeriod();
    }

    return counterStatus;
}

std::vector<CounterStatus> BackendProfiling::GetActiveCounters()
{
    CaptureData captureData = m_ProfilingService.GetCaptureData();

    const std::vector<uint16_t>& globalCounterIds = captureData.GetCounterIds();
    std::vector<CounterStatus> activeCounterIds;

    for (auto globalCounterId : globalCounterIds) {
        // Get pair of local counterId and backendId using globalCounterId
        const std::pair<uint16_t, armnn::BackendId>& backendCounterIdPair =
                ProfilingService::Instance().GetCounterMappings().GetBackendId(globalCounterId);
        if (backendCounterIdPair.second == m_BackendId)
        {
            activeCounterIds.emplace_back(backendCounterIdPair.first,
                                          globalCounterId,
                                          true,
                                          captureData.GetCapturePeriod());
        }
    }

    return activeCounterIds;
}

bool BackendProfiling::IsProfilingEnabled() const
{
    return m_ProfilingService.IsProfilingEnabled();
}

}    // namespace profiling
}    // namespace armnn
