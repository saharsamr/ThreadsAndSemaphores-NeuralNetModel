#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>

class Node{
public:
  Node(std::vector<float> weights_, float bias_): weights(weights_), bias(bias_) {}
  Node() {}
  float computeNodeResult ();
private:
  std::vector<float> weights;
  std::vector<float> inputs;
  float bias;
};

#endif
