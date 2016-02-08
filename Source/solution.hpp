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

	temps start_min;
	temps start_max;
	temps total;

	int id_depot;
	std::vector <int> tournee; //vecteur fixé lors d'une évaluation, peut être sujet à des changements entre deux évaluations
	std::vector <temps> arrivee; //vecteur temporaire de calculs lors d'une évaluation


	public :

	solution(std::string & filename, data * d);

	int 	get_tournee(int index);
	temps 	get_arrivee(int index);

	temps	get_start_min() {return start_min;}
	temps	get_start_max() {return start_max;}
	
	bool 	check_deterministe(temps start = 0);
	bool	check_reverse_deterministe(temps start = 0);

	bool 	check_normal(temps start, temps taux_pourcent);

	void 	display(std::ostream & os);

};


#endif
