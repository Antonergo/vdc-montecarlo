#ifndef __STATS__
#define __STATS__

#include "data.hpp"

class Statistique
{
	private :

	static data * donnees;
	
	
	std::vector <temps> arrivee; //le vecteur de l'importance extreme
	
	int position_fail; // si >0, l'echec s'est fait sur ce moment la 
	
	public :
	
	Statistique();
	~Statistique();
	
	static void set_data(data * donnees);
	
};


class Tableau
{
	private :
	
	std::vector <Statistique> tableau;
	
	
	public :
	
	Tableau(int iterations = 10); //générer ici les stats ?
	~Tableau();
	
};

void print_SVG(std::vector <Tableau> all_results);











#endif
