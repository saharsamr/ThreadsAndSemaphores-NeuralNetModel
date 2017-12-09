#ifndef NEURALNET_H
#define NEURALNET_H

#include <vector>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include "Node.h"

typedef float (*activationFunc) (float*, float*);

class NeuralNet{
public:
  NeuralNet(int numOfLayers, std::vector<int>& layersNeuronNum);
  void readInputs(std::string fileName);
  void computeOutputParallel(activationFunc func);
  void computeOutputSerial(activationFunc func);
private:
  void addInputNodes(int numOfInputs);
  void setWeightsAndBiases(int numOfLayers, std::vector<int>& layersNeuronNum);
  void defineThreads(int numOfLayers, std::vector<int>& layersNeuronNum);
  void createSemaphores(int numOfLayers);
  void readInputs();
  std::vector< std::vector<Node*> > layers;
  std::vector<sem_t*> readyForNextLayer;
  std::vector<sem_t*> nextLayerRead;
  std::vector< std::vector<pthread_t*> > nodeThreads;
};

#endif
