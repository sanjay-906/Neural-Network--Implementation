#ifndef _NeuralNetwork_h_
#define _NeuralNetwork_h_

#include <iostream>
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include <cmath>

class Neuron;


typedef std::vector<Neuron> Layer;


struct connection{
	double weight;
	double deltaweight;
};


class Neuron{
	public:
		Neuron(unsigned numoutputs, unsigned index);
		double 	getoutputval(void) const;
		void 	setoutputval(double);
		void 	FeedForward(const Layer &prevlayer);
		void 	calcoutputgradients(double targetval);
		void 	calchiddengradients(const Layer &nextlayer);
		void 	updateinputweights(Layer &prevlayer);
		
	private:
		static double eta;
		static double alpha;
		static double activate(double x);
		static double derive(double x);

		unsigned 		m_index;
		double 			m_gradient;
		double 			m_outputval;
		static double	randomweight(void);
		std::vector<connection> m_outputweights;
		double sumDOW(const Layer &nextlayer)const;
};


class NeuralNetwork{
	public:
		NeuralNetwork(const std::vector<unsigned> &topology);
		void 	FeedForward(const std::vector<double> &inputvals);
		void 	BackProp(const std::vector<double> &targetvals);
		void 	getResults(std::vector<double> &results) const;
		double 	getrecentaverageerror(void)const;
		
	private:
		double m_error;
		double m_recentaverageerror;
		std::vector<Layer> m_layers;  //m_layers[layer_no][neuron_no]
		static double m_recentaveragesmoothingfactor;
};


#endif
