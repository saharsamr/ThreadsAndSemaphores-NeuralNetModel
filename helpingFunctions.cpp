#include "helpingFunctions.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void getNetFeatures(int& numOfLayers, vector<int>& layersNeuronNum){
  int neuronNum;
  cin >> numOfLayers;
  for(int i = 0; i < neuronNum; i++){
    cin >> neuronNum;
    layersNeuronNum.push_back(neuronNum);
  }
}
