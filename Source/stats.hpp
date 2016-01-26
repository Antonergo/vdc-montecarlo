#ifndef __STATS__
#define __STATS__

#include "data.hpp"

class statistique
{
	private :

	static std::vector <temps> fen_deb;
	static std::vector <temps> fen_fin;
	
	std::vector <temps> arrivee;
	std::vector <temps> attente_vehicule; // (fen_deb - arrivee > 0)
	
	int failure; // si >0, il y a eu un echec
	
	public :
	
	statistique();
	~statistique();
};

//void print_SVG(std::vector <statistique> tableau;) //nouvelle classe ?












#endif
