#include "NeuralNet.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

NeuralNet::NeuralNet(int numOfLayers, vector<int>& layersNeuronNum){
  addInputNodes(layersNeuronNum[0]);
  setWeightsAndBiases(numOfLayers, layersNeuronNum);
  defineThreads(numOfLayers, layersNeuronNum);
  createSemaphores(numOfLayers);
}

void NeuralNet::addInputNodes(int numOfInputs){
  vector<Node*> temp;
  for(int i = 0; i < numOfInputs; i++)
    temp.push_back(new Node());
  layers.push_back(temp);
}

void NeuralNet::setWeightsAndBiases(int numOfLayers, vector<int>& layersNeuronNum){
  ifstream weightsFile("inputs/weights.txt");
  ifstream biasesFile("inputs/biases.txt");
  for(int i = 1; i < numOfLayers; i++){
    vector<Node*> temp_;
    for(int j = 0; j < layersNeuronNum[i]; j++){
      vector<float> weights;  float temp, bias;
      for(int k = 0; k < layersNeuronNum[i-1]; k++){
        weightsFile >> temp;
        weights.push_back(temp);
      }
      biasesFile >> bias;
      temp_.push_back(new Node(weights, bias));
    }
    layers.push_back(temp_);
  }
}

void NeuralNet::defineThreads(int numOfLayers, vector<int>& layersNeuronNum){
  for(int i = 0; i < numOfLayers; i++){
    vector<pthread_t*> temp;
    for(int j = 0; j < layersNeuronNum[i]; j++)
      temp.push_back(new pthread_t);
    nodeThreads.push_back(temp);
  }
}

void NeuralNet::createSemaphores(int numOfLayers){
  for(int i = 1; i < numOfLayers; i++){
    readyForNextLayer.push_back(new sem_t);
    if(sem_init(readyForNextLayer[i-1], 0, 0) != 0)
      cerr << "semaphore creation failed.\n";
    nextLayerRead.push_back(new sem_t);
    if(sem_init(nextLayerRead[i-1], 0, 0) != 0)
      cerr << "semaphore creation failed.\n";
  }
}
