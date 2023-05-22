#include "TrainingData.h"


TrainingData::TrainingData(const std::string filename){
	m_trainingdatafile.open(filename.c_str());

}


bool TrainingData::isEof(void){
	return m_trainingdatafile.eof();	

}


void TrainingData::gettopology(std::vector<unsigned>&topology){
	std::string line;
	std::string label;
	
	getline(m_trainingdatafile, line);
	
	std::stringstream ss(line);
	ss>>label;
	if(this->isEof() or label.compare("topology:")!=0){
		abort();
	}
	
	while(!ss.eof()){
		unsigned n;
		ss>>n;
		topology.push_back(n);
	}
	
	return;

}


unsigned TrainingData::getnextinputs(std::vector<double> &inputvals){
	inputvals.clear();
	
	std::string line;
	getline(m_trainingdatafile, line);
	std::stringstream ss(line);
	
	std::string label;
	ss>>label;
	if(label.compare("in:")==0){
		double onevalue;
		while (ss>>onevalue){
			inputvals.push_back(onevalue);
		}
	}
	
	return inputvals.size();

}


unsigned TrainingData::gettargetoutputs(std::vector<double> &targetoutputvals){
	targetoutputvals.clear();
	
	std::string line;
	getline(m_trainingdatafile, line);
	std::stringstream ss(line);
	
	std::string label;
	ss>>label;
	if(label.compare("out:")==0){
		double onevalue;
		while (ss>>onevalue){
			targetoutputvals.push_back(onevalue);
		}
	}
	
	return targetoutputvals.size();

}
