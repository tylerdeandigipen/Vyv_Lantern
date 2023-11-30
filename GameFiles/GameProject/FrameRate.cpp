#include "FrameRate.h"

const float desiredFPS = 60.0f; /*Change to desired fps here*/
const float frameDelay = 1000.0f / desiredFPS; // Milliseconds per frame

void FrameRate::capFrame(float dt)
{
    float deltaTime = dt;
    if (deltaTime < frameDelay) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(frameDelay - deltaTime)));
    }
}

float FrameRate::CalculateAverageFrameRate(float* PreviousFrameLengths, uint32_t arraySize, float currentTime, float PreviousFrameBeginTime) {
    float AverageFrameLength = 0.0f;
    for (uint32_t FrameIndex = 1; FrameIndex < arraySize; ++FrameIndex) {
        AverageFrameLength += PreviousFrameLengths[FrameIndex - 1];
        PreviousFrameLengths[FrameIndex] = PreviousFrameLengths[FrameIndex - 1];
    }

    float dtThisFrame = (float)(currentTime - PreviousFrameBeginTime) / 1000.0f;
    PreviousFrameLengths[0] = dtThisFrame;
    AverageFrameLength += dtThisFrame;
    AverageFrameLength /= static_cast<float>(arraySize);

    return 1.0f / AverageFrameLength;
}

void FrameRate::UpdateWindowTitle(SDL_Window* window, float AverageFrameRate) {
    char WindowTextBuffer[128];
    sprintf_s(WindowTextBuffer, sizeof(WindowTextBuffer), "FPS: %.2f", AverageFrameRate);
    SDL_SetWindowTitle(window, WindowTextBuffer);
}