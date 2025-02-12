#pragma once

#include <vector>

namespace DSP
{
    class FirstOrderAllPass
    {
    public:
        FirstOrderAllPass() = default;
        ~FirstOrderAllPass() = default;
        float process(const float& inputSample, int channel);
        void setFilterCoefficient(const float& cutoffFrequency, float sampleRate);

    private:
        std::vector<float> prevInput = { 0.0f, 0.0f };
        std::vector<float> prevOutput = { 0.0f, 0.0f };
        float coefficient = 0.0f;
    };
}