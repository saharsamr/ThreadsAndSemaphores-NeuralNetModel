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
  for(int i = 0; i < numOfInputs; i++)
    layers[0].push_back(new Node());
}

void NeuralNet::setWeightsAndBiases(int numOfLayers, vector<int>& layersNeuronNum){
  ifstream weightsFile("inputs/weights.txt");
  ifstream biasesFile("inputs/biases.txt");
  for(int i = 1; i < numOfLayers; i++){
    for(int j = 0; j < layersNeuronNum[i]; j++){
      vector<float> weights;  float temp, bias;
      for(int k = 0; k < layersNeuronNum[i-1]; k++){
        weightsFile >> temp;
        weights.push_back(temp);
      }
      biasesFile >> bias;
      layers[i].push_back(new Node(weights, bias));
    }
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
    nextLayerRead.push_back(new sem_t);
  }
}
