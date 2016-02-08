#ifndef __STATS__
#define __STATS__

#include "data.hpp"

class Statistique
{
	private :

	static data * donnees;
	std::vector <temps> arrivee; //le vecteur de l'importance extreme
	std::vector <temps> attente_vehicule; // (fen_deb - arrivee > 0)
	
	int position_fail; // si >0, l'echec s'est fait sur ce moment la 
	
	public :
	
	Statistique();
	~Statistique();
	
	static void set_data(data * donnees);
	 
};

//void print_SVG(std::vector <statistique> tableau;) //nouvelle classe ?












#endif
