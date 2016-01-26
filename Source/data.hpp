#ifndef __DATA__
#define __DATA__

#include <vector>
#include <string>
#include <iostream>

typedef float temps;


class data
{
	private	:
	
	std::string name;
	
	int nb_sommets;
	
	std::vector <temps> distances; //matrice
	
	std::vector <temps> fen_deb; //vecteur
	std::vector <temps> fen_fin; //vecteur
	
	
	
	
	public :
	
	data(std::string filename);
	
	std::string get_name();
	int   get_nb_sommets();
	temps get_dist(int a, int b);
	
	temps get_fen_deb(int client);
	temps get_fen_fin(int client);
	
	void display(std::ostream & os);
	
};


#endif
