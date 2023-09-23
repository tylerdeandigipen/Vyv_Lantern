#pragma once
#include "ImageBuffer.h"
#include "Light.h"

ImageBuffer* RenderLightingPass(ImageBuffer* outputBuffer, ImageBuffer* inputBuffer, Light* lightSource[], int totalLights);
