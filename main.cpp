#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream> 
#include "NeuralNetwork.h"
#include "TrainingData.h"

std::fstream fout("Results/output.txt");


void showvectorvals(std::string label, std::vector<double>&v){
	
	fout<<label<<" ";
	for(unsigned i=0; i<v.size(); i++){
		fout<<v[i]<<" ";
	}
	fout<<std::endl;
}


int main(){

	TrainingData traindata("Results/input.txt");

	std::vector<unsigned>topology;
	traindata.gettopology(topology);

	NeuralNetwork mynet(topology);

	std::vector<double> inputvals, targetvals, resultvals;
	int trainingpass=0;
	

	while(!traindata.isEof()){
		
		++trainingpass;
		fout<<std::endl<<"Pass: "<<trainingpass;
		
		//get new input data and feed it forward
		if (traindata.getnextinputs(inputvals)!=topology[0]){
			break;
		}
		showvectorvals(": Inputs:", inputvals);
		mynet.FeedForward(inputvals);
		
		//collect the net's actual results
		mynet.getResults(resultvals);
		showvectorvals("Outputs:", resultvals);
		
		//train the net what the outputs should have been:
		traindata.gettargetoutputs(targetvals);
		showvectorvals("Targets:", targetvals);
		assert(targetvals.size()==topology.back());
		 
		mynet.BackProp(targetvals);
		
		//report how well the training is working averaged over recent
		fout<<"Network recent average error:"<<mynet.getrecentaverageerror()<<std::endl;
			
	}
	
	std::cout<<std::endl<<"Done"<<std::endl;
	
}
