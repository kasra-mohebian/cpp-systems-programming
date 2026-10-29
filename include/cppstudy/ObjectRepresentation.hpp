#pragma once
#include <vector>
namespace object_representation {
void printBinary(float value);
void printBinary(double value);
void push(std::vector<unsigned char> &stack, float value);
void push(std::vector<unsigned char> &stack, double value);
bool pop(std::vector<unsigned char> &stack, float &value);
bool pop(std::vector<unsigned char> &stack, double &value);
} // namespace object_representation
