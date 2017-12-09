#include "NeuralNet.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

NeuralNet::NeuralNet(int numOfLayers, vector<int>& layersNeuronNum){
  layers = vector< vector<Node*> > (numOfLayers);
  addInputNodes(layersNeuronNum[0]);
  setWeightsAndBiases(numOfLayers, layersNeuronNum);
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
