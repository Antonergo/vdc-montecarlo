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
	Statistique(solution* sol, data* d);

	~Statistique();

	void set_arrivee(std::vector <temps> & valeurs);

	void generer (solution * sol);
};

/*
Une instance de tableau par solution (1 solution = 1 tournee, il peut y avoir plusieurs solutions)
*/
class Tableau
{
	private :

	static data * donnees;

	int nb_iterations;

	solution * sol;

	std::vector <Statistique*> tableau;


    //résultats finaux :

    std::vector <int> nombre_echecs;

    std::vector <temps> moyenne_arrivee;

    temps moyenne_attente;

	public :

	Tableau(int iterations = 10); //générer ici les stats ?
	~Tableau();

	static void set_data(data * donnees);

	void create_SVG_component(std::ostream & os);
    void calculate_results();
    void print_results(std::ostream & os);
};

void create_HTML(std::ostream & os, std::vector <Tableau> & all_results); //à mettre dans le main ?











#endif
