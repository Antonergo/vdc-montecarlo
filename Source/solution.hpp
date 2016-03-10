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

	temps start_opti;
	temps start_wait;
	temps total; //total théorique des solutions de Solomon
	temps time_waited; //temps d'attente totale dans un parcours de tournee stochastique
	
	int index_echec;
	int id_depot;
	std::vector <int> tournee; //vecteur fixé lors d'une évaluation, peut être sujet à des changements entre deux évaluations
	std::vector <temps> arrivee; //vecteur temporaire de calculs lors d'une évaluation -> sert pour l'extraction dans les statistiques.


	public :

	solution(std::string & filename, data * d);
	solution(temps temps_total, std::vector<int> tournee_entree, data * d);
	
	
	std::vector<int> 	& get_tournee() {return tournee;}
	std::vector<temps> 	& get_arrivee() {return arrivee;}
	
	int 	get_tournee(int index) {return tournee[index];}
	temps 	get_arrivee(int index) {return arrivee[index];}

	temps	get_start_opti() {return start_opti;}
	temps	get_start_wait() {return start_wait;}
	
	temps	get_time_waited() {return time_waited;}
	int		get_index_echec() {return index_echec;}
	
	bool 	check_deterministe(temps start = 0);
	bool	check_reverse_deterministe(temps start = 0);

	bool 	check_normal(temps start, temps taux_pourcent);

	void 	display(std::ostream & os);

};


#endif
