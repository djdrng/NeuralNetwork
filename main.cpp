#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <cmath>
#include "Matrix.cpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//				Function to load the training images
//
//

#define TRAINING_SIZE 946
#define ITERATIONS 10
#define HIDDEN_NEURONS 20
#define OUTPUT_NEURONS 10
#define IMAGE_DIMENSION 32

void loadTraining(const char* filename, vector <vector <double>> &input, vector <vector <double>> &output)	{
	input.resize(TRAINING_SIZE);
	output.resize(TRAINING_SIZE);

	ifstream file(filename);
	if(file)	{
		string line;
		int n;

		for(int i = 0; i < TRAINING_SIZE; i++)	{
			for(int j = 0; j < IMAGE_DIMENSION; j++)	{
				getline(file, line);

				for(int k = 0; k < IMAGE_DIMENSION; k++)	
					input[i].push_back(atoi(line.substr(k, 1).c_str()));
			}
			getline(file, line);
			output[i].resize(OUTPUT_NEURONS);
			n = atoi(line.substr(0, 1).c_str());

			output[i][n] = 1;
		}
	}
	file.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//				Functions for training the neural network
//

Matrix X, W1, H, W2, Y, B1, B2, Y2, dJdB1, dJdB2, dJdW1, dJdW2, J;
double learningRate;

double random(double x)	{
	return (double)(rand() % 10000 + 1) / 10000 - 0.5;
}

double sigmoid(double x)	{
	return 1 / (1 + exp(-x));
}

double sigmoidPrime(double x)	{
	return exp(-x) / (pow(1 + exp(-x), 2));
}

double stepFunction(double x)	{
	if(x > 0.9)	
		return 1.0;
	if(x < 0.1)
		return 0.0;
	return x;
}

void init(int inputNeuron, int hiddenNeuron, int outputNeuron, double rate)	{
	learningRate = rate;

	W1 = Matrix(inputNeuron, hiddenNeuron);
	W2 = Matrix(hiddenNeuron, outputNeuron);
	B1 = Matrix(1, hiddenNeuron);
	B2 = Matrix(1, outputNeuron);

	W1 = W1.applyFunction(random);
	W2 = W2.applyFunction(random);
	B1 = B1.applyFunction(random);
	B2 = B2.applyFunction(random);
}

Matrix computeOutput(vector <double> input)	{
	X = Matrix({input});
	H = (X.dot(W1) + B1).applyFunction(sigmoid);
	Y = (H.dot(W2) + B2).applyFunction(sigmoid);
	return Y;
}

void learn(vector <double> expectedOutput)	{

	Y2 = Matrix({expectedOutput});

	dJdB2 = (Y - Y2) * ((H.dot(W2) + B2).applyFunction(sigmoidPrime));
	dJdB1 = dJdB2.dot(W2.transpose()) * ((X.dot(W1) + B1).applyFunction(sigmoidPrime));
	dJdW2 = H.transpose().dot(dJdB2);
	dJdW1 = X.transpose().dot(dJdB1);

	W1 = W1 - dJdW1 * learningRate;
	W2 = W2 - dJdW2 * learningRate;
	B1 = B1 - dJdB1 * learningRate;
	B2 = B2 - dJdB2 * learningRate;
}

void timeStamp(ostream& data)	{
	char* curTimeStamp;
	time_t curTime = time(NULL);
	
	struct tm* timeInfo = localtime(&curTime);

	data << "\n" << asctime(timeInfo) << "\n";
}

void outputData(ostream& data, vector <vector <double>> &input, vector <vector <double>> &output, const double learningRate)	{

	timeStamp(data);
	
	if(data)	{
		data << "\nLearning Rate : " << learningRate << " : Iterations : " << ITERATIONS << "\n";
		data << "\nExpected Output : Actual Output\n" ;

		for(int i = input.size() - OUTPUT_NEURONS; i < input.size(); i++)	{
			for(int j = 0; j < OUTPUT_NEURONS; j++)	
				data << output[i][j] << " ";
			data << ": " << computeOutput(input[i]).applyFunction(stepFunction) << "\n";
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//						Main Function
//
//

#define CYCLES 1

int main(const int argc, const char* argv[])	{

	srand(time(NULL));
	const char* dataFileName = "data";
	ofstream dataFile(dataFileName, fstream::app);


	for(int w = 0; w < CYCLES; w++)	{

		vector <vector <double>> inputVector, outputVector;

		loadTraining("training", inputVector, outputVector);
		learningRate = 0.5;
		init(IMAGE_DIMENSION * IMAGE_DIMENSION, HIDDEN_NEURONS, OUTPUT_NEURONS, learningRate);

		for(int i = 0; i < ITERATIONS; i++)	{
			for(int j = 0; j < inputVector.size() - OUTPUT_NEURONS; j++)	{
				computeOutput(inputVector[j]);
				learn(outputVector[j]);
			}
			cout << "#" << i + 1 << "/" << ITERATIONS << "\n";
		}

		outputData(dataFile, inputVector, outputVector, learningRate);
		learningRate += 0.1;
	}
}