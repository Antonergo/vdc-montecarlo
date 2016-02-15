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
	} while ((!is.eof()) && str != filename); //on s'arrête sur soit la bonne ligne de trouvée, soit EOF.
	if (is.eof())
	{
		std::cerr << "ERROR, solution not found for : " << filename << std::endl;
	}
	else
	{
      //donner le numéro de dépot
      id_depot = 0;
	  //sortir le temps total et se caler sur la tournée.
	  is >> total >> str;
	  //creer la tournee

	  tournee.push_back(id_depot);
	  for (int i = 0; i< nb_sommets - 1; ++i)
	  {
		is >> client;
		tournee.push_back(client);
	  }
  	  tournee.push_back(id_depot);
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


bool 	solution::check_deterministe(temps start)
{
	temps total_cout = 0.0;
	temps total_wait = 0.0;
	
	temps distance = 0.0;
	
	temps temps_courant = start;
	int prec;
	int cour;
	unsigned index = 0;

	bool res = true;

	if (!tournee.size())
	{
	    std::cerr << "Erreur : solution vide" << std::endl;
		exit (EXIT_FAILURE);
	}

	std::cout << "depart au temps " << start << std::endl;
		
	while (res && index < tournee.size() - 1)
	{
		prec = tournee[index];
		cour = tournee[index+1];
		
		distance = donnees->get_dist(prec,cour);
		
		temps_courant += distance; //todo : prendre en compte le temps de service ? -- dans les graphes !!
		total_cout += distance;

        std::cout << "(" << prec << " -> " << cour << ") : "<< distance  << ", arrivee a : " <<  temps_courant << std::endl;

		//Le cout ne dépend pas du temps passé, mais uniquement de la distance parcourue
		if ( temps_courant < donnees->get_fen_deb(cour) )
		{
		    total_wait += donnees->get_fen_deb(cour) - temps_courant;
			temps_courant = donnees->get_fen_deb(cour);

			std::cout << "WAIT ... ";
		}
		else

		if ( temps_courant > donnees->get_fen_fin(cour) )
		{
			res = false;

			//std::cout << "Erreur de fenetre de temps au " << index << "e client (" << cour << ")" << std::endl;
		}

		++index;
	}
	
	start_min = temps_courant;
	
	std::cout << "distance totale : " << total << " (estime) ; " << total_cout << " (calcul) ; plus " << total_wait << " (waiting)" << std::endl;

	return res;
}

bool    solution::check_reverse_deterministe(temps end)
{
    temps total_cout = 0.0;
	temps total_overlimit = 0.0;
	temps temps_courant = end;
	temps distance = 0.0;
	int prec;
	int cour;
	unsigned index = tournee.size() - 1;

	bool res = true;

	if (!tournee.size())
	{
	    std::cerr << "Erreur: solution vide" << std::endl;
		exit (EXIT_FAILURE);
	}
	
	std::cout << "'fin' au temps " << end << std::endl;

	while (res && index > 0)
	{
		prec = tournee[index];
		cour = tournee[index-1];
		
		distance = donnees->get_dist(cour,prec);
		
		temps_courant -= distance; //todo : enlever aussi le temps de service
		total_cout += distance;

        std::cout << "(" << prec << " <- " << cour << ") distance = "  << distance << ", arrivee a : " <<  temps_courant << std::endl;

		//a l'envers, si on arrive après la fin, on y revient, en y ajoutant l'overlimit.
		if ( temps_courant > donnees->get_fen_fin(cour) )
		{
		    total_overlimit += temps_courant - donnees->get_fen_fin(cour);
			temps_courant = donnees->get_fen_fin(cour);

			std::cout << "OVERLIMIT ... ";
		}
		else

		if ( temps_courant < donnees->get_fen_deb(cour) )
		{
		    //issue impossible, vu que le check déterministe a réussi
		    //ce serait donc un bug de ma part .-.
			res = false;

			//std::cout << "Erreur de fenetre de temps au " << index << "e client (" << cour << ")" << std::endl;
		}

		--index;
	}
	
	start_max = temps_courant;
	
	std::cout << "distance totale : " << total << " (estime) ; " << total_cout << " (calcul) ; plus " << total_overlimit << " (overlimit)" << std::endl;

	return res;
}

bool 	solution::check_normal(temps start, temps taux)
{
	temps total_cout = 0.0;
	temps temps_courant = start;
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


		//Le cout ne dépend pas du temps passé, mais uniquement de la distance parcourue
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

		//tester si quand on échoue que les temps correspondent bien ! (ça a l'air convaincant)
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
		//sortir l'index/l'ID Client (préalablement enregistré dans notre instance
	}
	return res;
}

