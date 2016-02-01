#include "solution.hpp"
#include "stats.hpp"

#include <cstring>

#include <iostream>
#include <fstream> //ifstream
//#include <sstream>
#include <stdlib.h> //exit()


solution::solution(std::string & solname, data * d)
{
	int nb_sommets;
	int client;
		std::string filename;
	// open the instance
  std::ifstream is(solname.c_str());
  if (!is)
  {
    std::cerr << "Error: unable to open file \'" << solname << "\'" << std::endl;
    exit (EXIT_FAILURE);
  }
  donnees = d;
  filename = donnees->get_name();
  nb_sommets = donnees->get_nb_sommets();
  tournee.clear();

  //std::cout << "nb_sommets = " << nb_sommets << std::endl;

  //trouver la bonne ligne (comparer filename)

	std::string str;
	do
	{
		is >> str;
	} while ((!is.eof()) && str != filename); //on s'arr�te sur soit la bonne ligne de trouv�e, soit EOF.
	if (is.eof())
	{
		std::cerr << "ERROR, solution not found for : " << filename << std::endl;
	}
	else
	{
	  //sortir le temps total et se caler sur la tourn�e.
	  is >> total >> str;
	  //creer la tournee

	  //tournee.push_back(id_depot);
	  for (int i = 0; i< nb_sommets - 1; ++i)
	  {
		is >> client;
		tournee.push_back(client);
	  }
  	  //tournee.push_back(id_depot);
	}
}

void solution::display(std::ostream & os)
{
	os << "cout total deterministe : " << total << "\nordre de tournee : ";
	for (unsigned i = 0; i< tournee.size(); ++i)
	{
		os << tournee[i] << " ";
	}
	os << std::endl;
}


bool 	solution::check_deterministe()
{
	temps total_cout = 0.0;
	temps temps_courant = 0.0;
	int prec;
	int cour;
	unsigned index = 1;

	bool res = true;


	if (!tournee.size())
	{
	    std::cerr << "Error: attempting to evaluate an empty solution" << std::endl;
		exit (EXIT_FAILURE);
	}

	prec = 0;
	cour = tournee[0];

	while (res && index < tournee.size())
	{
		//std::cout << "(" << prec << " -> " << cour << ") : "<< donnees->get_dist(prec,cour) << std::endl;

		temps_courant += donnees->get_dist(prec,cour);
		total_cout += donnees->get_dist(prec,cour);


		//Le cout ne d�pend pas du temps pass�, mais uniquement de la distance parcourue
		if ( temps_courant < donnees->get_fen_deb(cour) )
		{
			temps_courant = donnees->get_fen_deb(cour);

			//std::cout << "WAIT ... ";
		}
		else

		if ( temps_courant > donnees->get_fen_fin(cour) )
		{
			res = false;

			//std::cout << "Erreur de fenetre de temps au " << index << "e client (" << cour << ")" << std::endl;
		}

		prec = cour;
		cour = tournee[index]; //todo : put it on the beginning of the loop (and deal with Depot in beginning and end of thing)

		++index;
	}

	//std::cout << "distance totale : " << total << " (estime) ; " << total_cout << " (calcul)" << std::endl;

	return res;
}



bool 	solution::check_normal(temps taux)
{
	temps total_cout = 0.0;
	temps temps_courant = 0.0;
	temps dist_fixe;
	temps dist_norm;
	int prec;
	int cour;
	unsigned index = 1;

	bool res = true;

	//allouer la classe de stats

	if (!tournee.size())
	{
	    std::cerr << "Error: attempting to evaluate an empty solution" << std::endl;
		exit (EXIT_FAILURE);
	}

	prec = 0;
	cour = tournee[0];

	while (res && index <= tournee.size())
	{


		dist_fixe = donnees->get_dist(prec,cour);
		//RNG goes here
		dist_norm = loi_normale()(dist_fixe, dist_fixe*taux/100);
		if (dist_norm < 10.0)
		{
			dist_norm = 10.0;
		}

		std::cout << "(" << prec << " -> " << cour << ") : "<< dist_norm << std::endl;

		temps_courant += dist_norm;
		total_cout += dist_norm;


		//Le cout ne d�pend pas du temps pass�, mais uniquement de la distance parcourue
		if ( temps_courant < donnees->get_fen_deb(cour) )
		{
			//************ TODO : extraire le temps d'attente ici dans la classe Stats
			temps_courant = donnees->get_fen_deb(cour);

			std::cout << "WAIT ... ";
		}
		else

		if ( temps_courant > donnees->get_fen_fin(cour) )
		{
			res = false;

			std::cout << "Echec de fenetre de temps au " << index << "e client (" << cour << ")" << std::endl;

		//tester si quand on �choue que les temps correspondent bien ! (�a a l'air convaincant)
		}

		prec = cour;
		cour = tournee[index];

		++index;
	}


	if (res) //reussite du parcours
	{
		std::cout << "distance totale : " << total << " (deterministe) ; " << total_cout << " (calcule)" << std::endl;
	}
	else //echec du test
	{
		//sortir l'index/l'ID Client (pr�alablement enregistr� dans notre instance
	}
	return res;
}

