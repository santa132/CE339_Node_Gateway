#include "Model.h"
#include <cmath>
Model::Model()
{
    coefficients[0] = -0.18952351634421533 ;
    coefficients[1] = -0.01859829227504317; 
    coefficients[2] = 6.072414290817279e-05;
    intercept = -0.002527506580184905;
}
Model::~Model()
{
}
float Model::predict(float in1, float in2, float in3) // so với 0.5
{
    float prediction = this->intercept;
    prediction += this->coefficients[0]*in1;
    prediction += this->coefficients[1]*in2;
    prediction += this->coefficients[2]*in3;
    float P = 1 / (1 + exp(-prediction));
    return P;
}
float Model::predict2(float in1, float in2, float in3) // so với 0
{
    float prediction = this->intercept;
    prediction += this->coefficients[0]*in1;
    prediction += this->coefficients[1]*in2;
    prediction += this->coefficients[2]*in3;

    return prediction;
}
