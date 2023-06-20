#include <iostream>
using namespace std;
class Model
{
private:
    float coefficients[3];
    float intercept;
public:
    Model();
    ~Model();
    float predict(float, float, float);
    float predict2(float, float, float);
};

