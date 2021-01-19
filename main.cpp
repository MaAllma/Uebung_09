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

		

		//das einlesen der Zahlenfolge funktioniert noch nicht so ganz :/
		//meine Idee war es das CharArray durchzugehen und jedes char einzeln als Zahl zu konvertieren, aber das scheint so nicht ganz zu gehen
		//auch die Erkennung, dass es sich um eine 0 handelt klappt noch nicht so ganz :/

		/* <--- 1. argument: Wurffolge ---> */
		char* numbers = argv[1];
		size_t length = 3;

		//Zur Kontrolle:
		std::cout  << "lenght: " <<  length << std::endl;
		std::cout << "numbers: " << numbers << std::endl;

		//char stückweise durchgehen, zu int konvertieren und in den sequence vector einfügen
		for (int i = 0; i < length; i++) {

			//Konvertieren des char zu int
			char c = numbers[i];
			int number = (int)c;
			number = number - 48;
			
			//überprüfen ob die Zahlenfolge nur aus 0en oder 1en besteht
			if (number != 0 || number != 1){

				//zum Verständniss::
				std::cout << "char: " << c << std::endl;
				std::cout << "int: " << number << std::endl;

				//wirkliche Fehlermeldung:
				std::cerr << "The sequence can only contain 0 or 1\n";
				//return -1;
			}
			
			//Sequenz in der die Münzwurfabfolge gespeichert wird
			sequence.push_back(number);
			std::cout << "Sequence size" << sequence.size() << std::endl;
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

		//Vector in dem die Abfolge der Münzen gespeichert wird = Ausgabe des Programms
		std::vector<char> result;

		//letzte Münze (statt end)
		char last_coin;
		//max der beiden Zeilen
		if (table(0, sequence.size()) > table(1, sequence.size())) last_coin = 'f';
		else last_coin = 'g';
		//füge die letzte Münze vorne in das Ergebnis an
		result.insert(result.begin(), last_coin);

		
		//Backtracking:
		for (size_t i = sequence.size(); i > 1; i--){
			//rückwärts durch die Matrix gehen
			//wähle das Feld der aktuell letzten Münze und suche von dort aus das Maximum
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
			//füge die Münze vorne in das Ergebnis an
			result.insert(result.begin(), last_coin);
			
		}

		//zu Kontrolle
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
		//std:: cerr << argc;
		std::cerr << "Run program with:\n" <<
		"- a sequence of coin toss,\n" <<
		"- a probability for starting with the fair coin,\n" <<
		"- a probability for changing the coin,\n" <<
		"- a probability to get the number with the manipulated coin" << std::endl;
			return -3;
		
	}

	
	
	return 0;
}

