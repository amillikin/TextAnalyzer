// TextAnalyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;
ifstream inFile;
ofstream outFile;

//Determines total number of 1 bits in a binary number. -ARM
int byteOnes(int byte) {
	int count;
	count = 0;
	while (byte > 0) {
		count++;
		byte &= (byte - 1);
	}
	return count;
}

void analyzeOctets() {
	int octets[640000] = { 0 };
	int bitVals[640000] = { 0 };


}

void analyzeTrigrams() {
	long int* trigrams = new long int[16777216];
	unsigned int trigramTotal = 0;
	char byteToCheck;

	while (inFile.get(byteToCheck)) {
		trigramTotal <<= 8;
		trigramTotal &= 0xffff00;
		trigramTotal |= byteToCheck;
		trigrams[trigramTotal] += 1;
	}

	//Write results to outfile.
	for (int i = 0; i < 16777216; i++) {
		if (trigrams[i] > 0) {
			outFile << i << "," << trigrams[i] << "," << endl;
		}
	}
	delete[] trigrams;
}

void analyzeDigrams() {
	int digrams[65536] = { 0 };
	unsigned int digramTotal = 0;
	char byteToCheck;

	while (inFile.get(byteToCheck)) {
		digramTotal <<= 8;
		digramTotal &= 0xff00;
		digramTotal |= byteToCheck;
		digrams[digramTotal] += 1;
	}

	//Write results to outfile.
	for (int i = 0; i < 65536; i++) {
		if (digrams[i] > 0) {
			outFile << i << "," << digrams[i] << "," << endl;
		}
	}
}

void analyzeSingleBytes() {
	int bytes[255] = { 0 };
	char byteToCheck;

	while (inFile.get(byteToCheck)) {
		bytes[byteToCheck] += 1;
	}

	//Write results to outfile.
	for (int i = 0; i < 255; i++) {
		if (bytes[i] > 0) {
			outFile << i << "," << bytes[i] << "," << endl;
		}
	}
}

void analyzeSingleBits() {
	int blockCounts[64] = { 0 };
	int tempCount = 0, blockOnes = 0;
	char byteToCheck;
	int numOfOnes = 0, totalZed = 0, totalOnes = 0, size = 0;
	streampos begin, end;

	begin = inFile.tellg();
	inFile.seekg(0, ios::end);
	end = inFile.tellg();
	size = (end - begin);
	inFile.seekg(0, ios::beg);

	while (inFile.get(byteToCheck)){
		numOfOnes = byteOnes(byteToCheck);
		totalOnes += numOfOnes;
		blockOnes += numOfOnes;
		tempCount++;
		if (tempCount == 8) {
			blockCounts[blockOnes] += 1;
			blockOnes = 0;
			tempCount = 0;
		}
	}

	totalZed = size - numOfOnes;
	//Write results to outfile.
	outFile << "totalOnes" << "," << "totalZed" << "," << endl;
	outFile << totalOnes << "," << totalZed << "," << endl;
	for (int i = 0; i < 64; i++) {
				if (blockCounts[i] > 0) {
			outFile << i << "," << blockCounts[i] << "," << endl;
		}
	}	

}

void prompt()
{
	cout << "Welcome to Aaron's Text Analyzer! Analyzes files and outputs CSV of occurrence frequencies." << endl;
	cout << "Accepted input: textanalyzer <frequency mode> <infile> <outfile>" << endl;
	cout << "Modes: single-bit (s), single-Byte (b), digram (d), trigram (t), octet (o) << endl";
}
int main(int argc, char* argv[]) {
	clock_t startTime = clock();
	double secondsElapsed;
	char freqMode;
	errno_t err;

	if (argc != 4) {
		cout << "Incorrect number of arguments supplied." << endl;
		prompt();
		return 1;
	}

	inFile.open(argv[2], ios::in | ios::binary);
	if (!inFile) {
		cout << "Can't open input file " << argv[4] << endl;
		prompt();
		return 1;
	}

	outFile.open(argv[3], ios::out);
	if (!outFile) {
		cout << "Can't open output file " << argv[5] << endl;
		prompt();
		return 1;
	}

	freqMode = argv[1][1];
	switch (freqMode) {
	case 's': cout << "Analyzing bits..." << endl;
		analyzeSingleBits();
		break;
	case 'b': cout << "Analyzing bytes..." << endl;
		analyzeSingleBytes();
		break;
	case 'd': cout << "Analyzing digrams..." << endl;
		analyzeDigrams();
		break;
	case 't': cout << "Analyzing trigrams..." << endl;
		analyzeTrigrams();
		break;
	case 'o': cout << "Analyzing octets..." << endl;
		analyzeOctets();
		break;
	default: cout << "Modes: single-bit (0), single-Byte (1), digram (2), trigram (3), octet (8)" << endl;
		return 1;
	}

	inFile.close();
	outFile.close();

	secondsElapsed = (clock() - startTime) / CLOCKS_PER_SEC;
	cout << secondsElapsed << endl;

	return 0;
}

