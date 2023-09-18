#pragma once
#include "Renderer.h"
#include "Color.h"
#include "Light.h"
#include "Math_Header.h"
#include "Math.h"
#include "Vector.h"
#include "ImageBuffer.h"
ImageBuffer* RenderLightingPass(ImageBuffer* lightBuffer, Light* lightSource[], int totalLights);