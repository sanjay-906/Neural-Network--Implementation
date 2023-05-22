#ifndef _TrainingData_h_
#define _TrainingData_h_

#include <fstream>
#include <sstream>
#include <vector>

class TrainingData{
	
	public:
		TrainingData(const std::string filename);
		bool 	 isEof(void);
		void 	 gettopology(std::vector<unsigned> &topology);
		unsigned getnextinputs(std::vector<double> &inputvals);
		unsigned gettargetoutputs(std::vector<double> &targetoutputvals);
	
	private:
		std::ifstream m_trainingdatafile;	
				
};


#endif
