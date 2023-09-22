#pragma once
#include "ImageBuffer.h"
#include "Light.h"

ImageBuffer* RenderLightingPass(ImageBuffer* lightBuffer, Light* lightSource[], int totalLights);