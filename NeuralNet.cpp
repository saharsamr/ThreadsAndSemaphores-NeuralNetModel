#include "NeuralNet.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <semaphore.h>
#include <pthread.h>
#include <cstdlib>

using namespace std;

NeuralNet::NeuralNet(int numOfLayers, vector<int>& layersNeuronNum, int numOfSamples_){
  numOfInputParameters = layersNeuronNum[0];
  numOfSamples = numOfSamples_;
  addInputNode();
  setWeightsAndBiases(numOfLayers, layersNeuronNum);
  defineThreads(numOfLayers, layersNeuronNum);
  createSemaphores(numOfLayers);
}

void NeuralNet::addInputNode(){
  layers.push_back(vector<Node*> (1, new Node()));
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
    if(i == 0){
      temp.push_back(new pthread_t);
      nodeThreads.push_back(temp);
      continue;
    }
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

void NeuralNet::computeOutputParallel(activationFunc func){
  if(pthread_create(nodeThreads[0][0], NULL, this->readInputs, (void*)this) != 0){
    cerr << "Input layer: Thread creation failed.\n";
    exit(1);
  }

  for(int i = 1; i < nodeThreads.size() - 1; i++)
    for(int j = 0; j < nodeThreads[i].size(); j++){
      if(pthread_create(nodeThreads[i][j], NULL, this->calcHiddenNodeOutput, (void*)this) != 0){
        cerr << "Hidden layers: Thread creation failed.\n";
        exit(1);
      }
    }

  int outLayerIndx = nodeThreads.size() - 1;
  for(int i = 0; i < nodeThreads[outLayerIndx].size(); i++)
    if(pthread_create(nodeThreads[outLayerIndx][i], NULL, this->calcOutputNodeData, (void*)this) != 0){
      cerr << "Output layer: Thread creation failed.\n";
      exit(0);
    }
}

void* NeuralNet::readInputs(void* arg){
  NeuralNet *net = (NeuralNet*) arg;
  ifstream inputFile("InputFile.txt");
  vector<float> inputs(net->numOfInputParameters);
  for(int i = 0; i < net->numOfSamples; i++){
    for(int i = 0; i < inputs.size(); i++)
      inputFile >> inputs[i];
    if(!inputs[0])
      break;
    net->enterNewInput(inputs);
    for(int i = 0; i < net->layers[1].size(); i++)
      sem_post(net->readyForNextLayer[0]);
    for(int i = 0; i < net->layers[1].size(); i++)
      sem_wait(net->nextLayerRead[0]);
  }
  return NULL;
}


void NeuralNet::enterNewInput(std::vector<float> in_){
  layers[0][0]->setInput(in_);
}
