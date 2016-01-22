#ifndef __SOLUTION__
#define __SOLUTION__

#include "data.hpp"

//#include <random> //compatible avec une compilation std++0x

#include <cstdlib>
#include <cmath>
const double PI  =3.141592653589793238463;

//foncteur : générateur de nombre aléatoire suivant une loi normale
class loi_normale
{
	private :
	
	float U;
	float V;
	
	public :
	
	loi_normale()
	{
		U = 1;
		V = 1;
	}
	
	temps operator()(temps moyenne = 0, temps variance = 1) // opérateur -> 1er '()' avec 2 arguments (*, *)
	{ 
		//randomize U & V (using U(0,1))
		U = ((double)std::rand() / (double)RAND_MAX);
		V = ((double)std::rand() / (double)RAND_MAX);
		
		
		return ((temps) sqrt(-2.0 * log((double) U)) * cos( (double) V * (2.0 * PI) ))
				* variance + moyenne;
	}
};



class solution
{
	private	:
	
	data* donnees;
	
	temps total;
	
	std::vector <int> tournee; //vecteur fixé lors d'une évaluation, peut être sujet à des changements entre deux évaluations
	
	std::vector <temps> arrivee; //vecteur temporaire de calculs lors d'une évaluation

	
	public :
	
	solution(std::string & filename, data * d);
	
	int 	get_tournee(int index);
	temps 	get_arrivee(int index);
	
	bool 	check_deterministe();
	bool	check_reverse_deterministe();
	
	//void create_distances_normal(temps taux_pourcent);
	
	bool 	check_normal(temps taux_pourcent);
	
	
	// check_realiste : envoie des données à d'autres classes pas encore faites
		
	void display(std::ostream & os);
	
};


#endif
