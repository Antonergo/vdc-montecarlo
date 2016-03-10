#ifndef __STATS__
#define __STATS__

#include "data.hpp"
#include "solution.hpp"
#include <iostream>
#include <vector>

/*
Une instance statistique par test réel
*/
class Statistique
{
	private :

	std::vector <temps> arrivee; //le vecteur de l'importance extreme

	int position_fail; // si >0, l'echec s'est fait sur cette position dans la tournee

    temps tps_attendu;

	public :

	Statistique(){}
	Statistique(solution* sol);

	~Statistique(){}

	void set_arrivee(std::vector <temps> & valeurs);
	
	int get_position_fail() 		{return position_fail;}
	temps get_tps_attendu()			{return tps_attendu;}
	temps get_arrivee(int index) 	{return arrivee[index];}
	
	
	void generer (solution * sol);
	void display(std::ostream & os);
};

/*
Une instance de tableau par solution (1 solution = 1 tournee, il peut y avoir plusieurs solutions)
*/
class Tableau
{
	private :

	data * donnees;

	int nb_iterations;

	solution * soluce;


    //résultats finaux :

    std::vector <int> nombre_echecs;
	int pire_index;
	
    std::vector <temps> moyenne_arrivee;

    temps moyenne_attente;

	public :

	std::vector <Statistique*> stats;

	Tableau(data * donnees, solution * sol, int iterations);
	~Tableau();

	void create_SVG_component(std::ostream & os);
    void calculate_results();
    void print_results(std::ostream & os);
};

void create_HTML(std::ostream & os, std::vector <Tableau> & all_results); //à mettre dans le main ?











#endif
