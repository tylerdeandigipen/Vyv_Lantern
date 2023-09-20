#include <fstream>
#include <math.h>
#include <algorithm>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

const int width = 240, height = 135;


struct color
{
    int r;
    int g;
    int b;
};

struct light
{
    int centerX;
    int centerY;
    float maxAngle;
    float minAngle;
    int intensity;
    float radialWeight;
    float angularWeight;
    float radialFalloffMult1;
    float radialFalloffMult2;
    float volumetricIntensity;
};

float clamp(float x, float upper, float lower)
{
    return min(upper, max(x, lower));
}

float smoothstep (float edge0, float edge1, float x) {
   // Scale, and clamp x to 0..1 range
   x = clamp((x - edge0) / (edge1 - edge0),1,0);

   return x * x * (3.0f - 2.0f * x);
}

float Distance(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

int main ()
{
    //Setting up the file
    ofstream img ("picture.ppm");
    img << "P3" << endl;
    img << width << " " << height << endl;
    img << "255" << endl;
    //Done with setup

    struct color buffer[width][height];

    struct light light1;
    light1.centerX = 30;
    light1.centerY = 67;
    light1.maxAngle = 135;
    light1.minAngle = 45;
    light1.intensity = 2;
    light1.radialWeight = 2;
    light1.angularWeight = 1;
    light1.radialFalloffMult1 = 0.3f;
    light1.radialFalloffMult2 = 0.0f;
    light1.volumetricIntensity = 0.5f;

    struct color black;
    black.r = 0;
    black.g = 0;
    black.b = 0;

    struct color white;
    white.r = 255;
    white.g = 125;
    white.b = 255;

    struct color player;
    player.r = 0;
    player.g = 256;
    player.b = 0;

    struct color lightBrown;
    lightBrown.r = 222;
    lightBrown.g = 184;
    lightBrown.b = 135;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if(x > 160 && x < 200 && y > 47 && y < 87)
            {
                buffer[x][y].r = lightBrown.r;
                buffer[x][y].g = lightBrown.g;
                buffer[x][y].b = lightBrown.b;
            }
            else
            {
                buffer[x][y].r = black.r;
                buffer[x][y].g = black.g;
                buffer[x][y].b = black.b;
            }
        }
    }

    //Fancy lighting code here
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float distance = Distance(light1.centerX, light1.centerY, x, y); //find distance from the center of the light
            float angle = atan2(x - light1.centerX, y - light1.centerY) * 57.295779f; //Find angle from point to center relative to x axis, magic number is 180 / pi

            float radialFalloff = 0;
            float angularFalloff = 0;

            //adjust angle to same angle with dif sign
            if(angle > 0 && light1.maxAngle < 0)
            {
                angle -= 360;
            }
            if(angle < 0 && light1.minAngle > 0)
            {
                angle += 360;
            }

            //calculate angularfalloff
            if(angle >= light1.minAngle && angle <= light1.maxAngle)
            {
                float midAngle = (light1.minAngle + light1.maxAngle) / 2;
                float tempMax = light1.maxAngle - midAngle;
                float tempMin = light1.minAngle - midAngle;

                angularFalloff = -1 * (((abs(angle - midAngle) - tempMax)) / (tempMax - tempMin));
            }

            //calculate radialfalloff
            radialFalloff = 1 / (1 + (light1.radialFalloffMult1* distance) + (light1.radialFalloffMult2 * (distance * distance)));

            //clamp both falloffs
            radialFalloff = clamp(light1.radialWeight * radialFalloff, 1, 0);
            angularFalloff = clamp(light1.angularWeight * angularFalloff, 1, 0);

            //Light objects
            float lightMultiplier = light1.intensity * radialFalloff * angularFalloff;
            buffer[x][y].r = (buffer[x][y].r * (clamp((white.r * lightMultiplier), 255,0) / 255));
            buffer[x][y].g = (buffer[x][y].g * (clamp((white.g * lightMultiplier), 255,0) / 255));
            buffer[x][y].b = (buffer[x][y].b * (clamp((white.b * lightMultiplier), 255,0) / 255));

            //Add volumetric lighting
            lightMultiplier *= light1.volumetricIntensity;
            buffer[x][y].r += (white.r * lightMultiplier);
            buffer[x][y].g += (white.g * lightMultiplier);
            buffer[x][y].b += (white.b * lightMultiplier);
        }
    }


    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            img << buffer[x][y].r << " " << buffer[x][y].g << " " << buffer[x][y].b << endl;
        }
    }

    system("open picture.ppm");
    return 0;
}