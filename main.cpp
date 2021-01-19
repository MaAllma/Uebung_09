#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "Matrix.h"
#include <math.h>       /* log10 */
#include <algorithm>    // std::max


/**
 *  Münzwurffolge
 */
std::vector<int> sequence; 

/**
 *  a_begin,fair
 */
float transition_begin_fair;

/**
 *  a_i,j
 */
float transition_changeCoin;

/**
 *  p_gezinkt(0)
 */
float p_manipulated_number;


/**
 * returns the probability to get a 0 or a 1 with the given coin
 * @param state is the coin
 * @param coin_result is eather 0 or 1 for number or head
 */
float probability (int state, int coin_result){
	switch (state)
	{
	case 0:
		return 0.5;
		break;
	case 1:
		if (coin_result == 0) return p_manipulated_number;
		else return 1 - p_manipulated_number;
		break;
	default: 
		return 0;
	}
}

/**
 * returns the probability of the given transition
 */
float transition(int from, int to){
	if (from == 2){
		if (to == 0){
			 return transition_begin_fair;
		}
		else return 1 - transition_begin_fair;
	} else 
		if (from != to) return transition_changeCoin;
		else return 1 - transition_changeCoin;
}




int main(int argc, char* argv[])
{
	
	//programm is run with:
		// Münzwurffolge = eine Zahl aus 1 und 0
		// a_begin,fair = eine Zahl zwischen 0 und 1
		// a_i,j = Zahl zwischen 0 und 1
		// p_gezinkt(0) = Zahl zwischen 0 und 1

	if(argc == 5) {
		
		//reading

		/* <--- 1. argument: Wurffolge ---> */
		std::string sequenceString = argv[1];

		for (int i = 0; i < sequenceString.size(); i++) {

			//Konvertieren des char zu int
			int number = (int)sequenceString[i];
			number = number - 48;
			
			//checks if the numbers are 1 or 0
			if (number != 1 && number != 0){

				std::cerr << "The sequence can only contain 0 or 1\n";
				return -1;
			}

			sequence.push_back(number);
		}


		/* <--- 2. argument: a_begin,fair ---> */
		transition_begin_fair = (float)strtod((argv[2]),NULL);
		if (transition_begin_fair < 0 || transition_begin_fair > 1){
			std::cerr << "The probability must be between 0 and 1.\n";
			return -1;
		}

		/* <--- 3. argument: a_i,j ---> */
		transition_changeCoin = (float)strtod((argv[3]),NULL);
		if (transition_changeCoin < 0 || transition_changeCoin > 1){
			std::cerr << "The probability must be between 0 and 1.\n";
			return -1;
		}


		/* <--- 4. argument: p_gezinkt(0)---> */
		p_manipulated_number = (float)strtod((argv[4]),NULL);
		if (p_manipulated_number < 0 || p_manipulated_number > 1){
			std::cerr << "The probability must be between 0 and 1.\n";
			return -1;
		}

		//initialize table
			//rows = verschiedene Zustände: gezinkt, fair, begin
			//cols = Länge der Sequenz + eine Spalte zur Initialisierung
		Matrix table(3, sequence.size()+1);

		/* <--- row 0 ---> */

		//fair
		table(0,0) = 0;
		//gezinkt
		table(1,0) = 0;
		//begin
		table(2,0) = 1;

		/* <--- row 1 ---> */
		for (size_t z = 0; z < 2; z++){
			table(z,1) = log10(probability(z,sequence[0])) + 1 + log10(transition(2,z));
		}

		/* <--- other rows ---> */
		for (size_t i = 2; i < sequence.size()+1; i ++){
			for (size_t z = 0; z < 2; z++){
				table(z,i) = log10(probability(z,sequence[i-1])) + std::max(table(0,i-1) + log10(transition(0,z)), table(1,i-1) + log10(transition(1,z)));
			}
		}

		/* <--- backtracking ---> */

		//Output = coin sequence
		std::vector<char> result;

		//last coin
		char last_coin;
		//max
		if (table(0, sequence.size()) > table(1, sequence.size())) last_coin = 'f';
		else last_coin = 'g';
		//adds last coin to the result 
		result.insert(result.begin(), last_coin);

		
		//Backtracking:
		for (size_t i = sequence.size(); i > 1; i--){
			//backwards through matrix
			//max
			if (last_coin == 'f'){
				if (table(0,i-1) + log10(transition(0,0)) > table(1,i-1) + log10(transition(1,0))){
					last_coin = 'f';
				}
				else last_coin = 'g';
			} else{
				if (table(0,i-1) + log10(transition(0,1)) > table(1,i-1) + log10(transition(1,1))){
					last_coin = 'f';
				}
				else last_coin = 'g';
			}
			//adds coin to result
			result.insert(result.begin(), last_coin);
			
		}

		//Table
		std::cerr << table.toString();

		//print result
		std::cerr << "The most likely coin sequence was: \n";
		for( char coin: result){
			std::cerr << coin << " ";
		}
		std::cerr << std::endl;
		return 0;


	//program is used with to little or to many arguments
	} else {
		std::cerr << "Run program with:\n" <<
		"- a sequence of coin toss,\n" <<
		"- a probability for starting with the fair coin,\n" <<
		"- a probability for changing the coin,\n" <<
		"- a probability to get the number with the manipulated coin" << std::endl;
			return -3;
		
	}

	
	
	return 0;
}

