#include "NeuralNetwork.h"


Neuron::Neuron(unsigned numoutputs, unsigned index){
	for(unsigned c=0; c<numoutputs; c++){
		m_outputweights.push_back(connection());
		m_outputweights.back().weight= randomweight();
	}
	
	m_index=index;

}


double Neuron::getoutputval(void) const{
	return m_outputval;

}


void Neuron::setoutputval(double val){
	m_outputval=val;

}


void Neuron::FeedForward(const Layer &prevlayer){
	double sum=0.0;
	// sum the previous layer's outputs (which are this layer's inputs)
	// add bias to the summation
	
	for(unsigned n=0; n<prevlayer.size();n++){
		sum+=prevlayer[n].getoutputval()*prevlayer[n].m_outputweights[m_index].weight;
	}
	
	m_outputval= Neuron::activate(sum);
}


void Neuron::calcoutputgradients(double targetval){
	double delta= targetval-m_outputval;
	m_gradient= delta*Neuron::derive(m_outputval);
	
}


void Neuron::calchiddengradients(const Layer &nextlayer){
	double dow= sumDOW(nextlayer);
	m_gradient= dow*Neuron::derive(m_outputval);

}


void Neuron::updateinputweights(Layer &prevlayer){
	// the weights to be updated are int the connection container
	// in the neurons in the preceding layer
	for(unsigned n=0; n<prevlayer.size(); n++){
		Neuron &neuron= prevlayer[n];
		double olddeltaweight= neuron.m_outputweights[m_index].deltaweight;
		
		double newdeltaweight=
		//indeividual input, magnified by the gradient and train rate.
		eta*neuron.getoutputval()*m_gradient +alpha*olddeltaweight;
		//eta= learning rate, alpha= momentum
		
		neuron.m_outputweights[m_index].deltaweight= newdeltaweight;
		neuron.m_outputweights[m_index].weight += newdeltaweight;
	}

}


double Neuron::eta=0.01;
double Neuron::alpha= 0.5;


double Neuron::activate(double x){
	//tanh in [-1,1]
	return tanh(x);
	
}


double Neuron::derive(double x){
	//derivate of tanh(x)= 1-tanh^2(x)
	return 1.0 - x*x; //approximation
	
}


double Neuron::randomweight(void){
	return rand()/double(RAND_MAX);

}

double Neuron::sumDOW(const Layer &nextlayer)const{
	double sum=0.0;
	//sum our contributions at the errors at the nodes we feed
	for(unsigned n=0; n<nextlayer.size()-1; n++){
		sum+=m_outputweights[n].weight *nextlayer[n].m_gradient;
	}
	
	return sum;

}




NeuralNetwork::NeuralNetwork(const std::vector<unsigned> &topology){
	unsigned numlayers= topology.size(); 
	for(unsigned layernum=0; layernum<numlayers;layernum++){
		std::cout<<"Made a Layer"<<std::endl;
		m_layers.push_back(Layer());
		//if current layer is the outputlayer, then the no of outputs from that layer is 0 
		unsigned numoutputs= layernum== topology.size()-1? 0 : topology[layernum+1];
		for(unsigned neuronnum=0; neuronnum<=topology[layernum];neuronnum++){
			// <= for the extra bias nueron for each layer
			m_layers.back().push_back(Neuron(numoutputs, neuronnum));
			std::cout<<"Made a Neuron"<<std::endl;
		}
		// force the bias node's output value to 1.0
		m_layers.back().back().setoutputval(1.0);
	}

}


void NeuralNetwork::FeedForward(const std::vector<double> &inputvals){
	//checking if no of inputvals==no of neurons (removing the bias)
	assert(inputvals.size()==m_layers[0].size()-1);
	
	//Assign the values into the neurons
	for(unsigned i=0; i<inputvals.size();i++){
		m_layers[0][i].setoutputval(inputvals[i]);
	}
	
	//forward propagation
	for(unsigned layernum = 1; layernum<m_layers.size(); layernum++){
		Layer &prevlayer= m_layers[layernum-1];
		for(unsigned n=0; n<m_layers[layernum].size()-1; n++){
			m_layers[layernum][n].FeedForward(prevlayer);
		}
	}
	
}


void NeuralNetwork::BackProp(const std::vector<double> &targetvals){
	//calculate overall error  (RMS of Output Neuron errors)
	Layer &outputlayer= m_layers.back();
	m_error= 0.0;
	
	for(unsigned n=0; n<outputlayer.size();n++){
		double delta= targetvals[n]-outputlayer[n].getoutputval();
		m_error+=delta*delta;
	}
	m_error/= outputlayer.size()-1; //get avg error squared
	m_error= sqrt(m_error); //RMS
	
	//implement a recent average measurement (error to say how the NN is handilng the errors)
	m_recentaverageerror= (m_recentaverageerror*m_recentaveragesmoothingfactor + m_error)
							/(m_recentaveragesmoothingfactor + 1.0);
	
	//calculate output layer gradients
	for(unsigned n=0; n<outputlayer.size()-1;n++){
		outputlayer[n].calcoutputgradients(targetvals[n]);
	}
	
	//calculate gradients on hidden layer
	for(unsigned layernum= m_layers.size()-2; layernum>0; layernum--){
		Layer &hiddenlayer= m_layers[layernum];
		Layer &nextlayer= m_layers[layernum+1];
		
		for(unsigned n=0; n<hiddenlayer.size();n++){
			hiddenlayer[n].calchiddengradients(nextlayer);
		}
	}
	
	//for all layers, update connection weights
	for(unsigned layernum=m_layers.size()-1; layernum>0; layernum--){
		Layer &layer= m_layers[layernum];
		Layer &prevlayer= m_layers[layernum-1];
		for(unsigned n=0; n<m_layers.size()-1; n++){
			layer[n].updateinputweights(prevlayer);
		}
		
	}
	
}


void NeuralNetwork::getResults(std::vector<double> &resultvals) const{
	resultvals.clear();
	
	for(unsigned n=0; n<m_layers.back().size()-1; n++){
		resultvals.push_back(m_layers.back()[n].getoutputval());
	}

}


double NeuralNetwork::getrecentaverageerror(void)const{
	return m_recentaverageerror;

}

double NeuralNetwork::m_recentaveragesmoothingfactor = 100.0;

