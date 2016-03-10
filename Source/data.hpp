#ifndef __DATA__
#define __DATA__

#include <vector>
#include <string>
#include <iostream>

typedef double temps;


class data
{
	private	:
	
	std::string name;
	
	int nb_depots;
	
	int nb_vehicules;
	int capacite_vehicules;
	
	int nb_clients;
	
	int nb_sommets;

	std::vector <temps> xVector;	// Vecteur des abcisses
	std::vector <temps> yVector;	// Vecteur des ordonnées
	
	std::vector <temps> fen_deb; 	// vecteur des heures d'ouverture
	std::vector <temps> fen_fin; 	// vecteur des heures de fermeture
	
	std::vector <temps> distances; 	// matrice des distances
	
	temps service;
	
	public :
	
	data(std::string prefix, std::string suffix);
	
	std::string get_name();
	
	int get_nb_depots();
	int get_nb_vehicules();
	int get_nb_clients();
	int get_nb_sommets();
	
	temps get_dist(int a, int b);
	temps get_fen_deb(int client);
	temps get_fen_fin(int client);
	temps get_service();
	
	void afficherData(std::ostream & os);
	void afficherDistances(std::ostream & os);
	void calculerDistances();
	void display(std::ostream & os);
};


#endif
