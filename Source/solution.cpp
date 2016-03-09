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

	  //sortir le temps total théorique (optionnel)
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

	//une fois la tournee complete, on donne la bonne taille du vecteur d'arrivee

	arrivee.resize(tournee.size());
}

solution::solution(temps temps_total, std::vector<int> tournee_entree, data * d)
{
	donnees = d;

	total = temps_total;

	tournee = tournee_entree;

	std::cout << "tournee cree de taille " << tournee.size() << std::endl;

	arrivee.resize(tournee.size());
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

	arrivee[0] = start;

	while (res && index < tournee.size() - 1)
	{
		prec = tournee[index];
		cour = tournee[index+1];

		distance = donnees->get_dist(prec,cour);

		temps_courant += distance; //todo : prendre en compte le temps de service ? -- dans les graphes !!
		total_cout += distance;

        std::cout << "(" << prec << " -> " << cour << ") : "<< distance  << ", arrivee a : " <<  temps_courant << std::endl;

		//ICI : exporter temps_courant dans un vecteur de statistiques à la position [index]
		arrivee[index+1] = temps_courant;

		if ( temps_courant < donnees->get_fen_deb(cour) )
		{
		    total_wait += donnees->get_fen_deb(cour) - temps_courant;
			temps_courant = donnees->get_fen_deb(cour);

			std::cout << "attente jusqu'a " << temps_courant << " ... ";
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
	start_max = total_wait;

	std::cout << "distance totale : " << total << " (estime) ; " << total_cout << " (calcul) ; plus " << total_wait << " (waiting)" << std::endl;

	return res;
}

bool    solution::check_reverse_deterministe(temps end)
{
    temps total_cout = 0.0;
	temps total_overlimit = 0.0;
	temps temps_courant = end + 0.00001; //ajout pour éviter un glissement d'arrondi dérrière une fenetre de temps
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

			std::cout << "retard jusqu'a " << temps_courant << " ... ";
		}
		else

		if ( temps_courant < donnees->get_fen_deb(cour) )
		{
		    //issue impossible, vu que le check déterministe a réussi
		    //ce serait donc un bug de ma part .-.
			res = false;

			std::cout << "ERREUR FATALE de fenetre de temps au " << index << "e client (" << cour << ") temps minimum requis :" << donnees->get_fen_deb(cour)  << std::endl;
		}

		--index;
	}

	start_min = temps_courant;

	std::cout << "distance totale : " << total << " (estime) ; " << total_cout << " (calcul) ; plus " << total_overlimit << " (overlimit)" << std::endl;

	return res;
}

bool 	solution::check_normal(temps start, temps taux)
{
	temps total_cout = 0.0;
	temps total_wait = 0.0;
	temps temps_courant = start;
	temps dist_fixe;
	temps tps_service = 0.0;
	temps dist_norm;
	int prec;
	int cour;
	unsigned index = 0;
	int position_fail = 0;

	bool res = true;

	//allouer la classe de stats

	if (!tournee.size())
	{
	    std::cerr << "Error: attempting to evaluate an empty solution" << std::endl;
		exit (EXIT_FAILURE);
	}

	std::cout << "debut tournee au temps : " << start << std::endl;

	while (res && index < tournee.size() - 1)
	{
		prec = tournee[index];
		cour = tournee[index+1];

		if (index == 0)
		{ //depart depuis le dépot : pas de temps de service
		    tps_service = 0.0;
		}
		else
        {
            tps_service = donnees->get_service();
        }

		dist_fixe = donnees->get_dist(prec,cour);


		//appliquer RNG ici
		dist_norm = tps_service + loi_normale()(dist_fixe - tps_service, (dist_fixe - tps_service)*taux/100);
		if (dist_norm < donnees->get_service()) //si par pure chance (ou variance abusément élevée), on a le temps de trajet parfait (temps de distance négatif), on se cale sur la valeur du temps de service
		{
			dist_norm = donnees->get_service();
		}


		temps_courant += dist_norm;
		total_cout += dist_norm - tps_service;

        std::cout << "(" << prec << " -> " << cour << ") : "<< dist_norm  << ", arrivee a : " <<  temps_courant << std::endl;

		//Le cout ne dépend pas du temps passé, mais uniquement de la distance parcourue
		if ( temps_courant < donnees->get_fen_deb(cour) )
		{
		    total_wait += donnees->get_fen_deb(cour) - temps_courant;
			temps_courant = donnees->get_fen_deb(cour);

			std::cout << "attente jusqu'a " << temps_courant << " ... ";
		}
		else

		if ( temps_courant > donnees->get_fen_fin(cour) )
		{
			res = false;

			position_fail = index;
		}

		++index;
	}


	if (res) //reussite du parcours
	{
		std::cout << "distance totale (moins les couts) : " << total_cout << "  attente totale : " << total_wait << std::endl;
	}
	else //echec du test
	{
		//sortir l'index/l'ID Client (préalablement enregistré dans notre instance)

		std::cout << "Erreur de fenetre de temps au " << position_fail << "e client (" << tournee[position_fail] << ")" << std::endl;
	}
	return res;
}

