#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include "helpingFunctions.h"
#include "Node.h"
#include "NeuralNet.h"

using namespace std;

int main(int argc, char const *argv[]){
  int numOfLayers;
  vector<int> layersNeuronNum;
  getNetFeatures(numOfLayers, layersNeuronNum);
  NeuralNet* net = new NeuralNet(numOfLayers, layersNeuronNum);
  // net->readInputs("InputFile.txt");
  return 0;
}
