#include <cstdlib>
#include <algorithm>
#include "stats.hpp"

void Statistique::set_arrivee(std::vector <temps> & valeurs)
{
	arrivee = valeurs;
}

Statistique::Statistique(solution * sol)
{
	this->arrivee = sol->get_arrivee();
	this->tps_attendu = sol->get_time_waited();
	this->position_fail = sol->get_index_echec();
}

void Statistique::display(std::ostream & os)
{
	if (position_fail == -1)
	{
		//reussite
		os << "Reussite : tps attendu = " << tps_attendu << std::endl;
	}
	else
	{
		//echec
		os << "Echec a la position " << position_fail << std::endl;
	}

	
	for (unsigned i = 0; i< arrivee.size(); ++i)
	{
		os << arrivee[i] << " ";
	}
	os << std::endl;
	
}




Tableau::Tableau(data * d, solution * sol, int iterations) //générer ici les tests réels ?
{
	donnees = d;
	soluce = sol;
	stats.clear();
	if (iterations > 0 && iterations < 100000)
	{
		stats.resize(iterations);
		nb_iterations = iterations;
	}
	else
	{
		std::cout << "nombre d'iterations invalide : entrez un nombre entre 1 et 99999" << std::endl;
		nb_iterations = 0;
	}
	
}

Tableau::~Tableau()
{
	//supprimmer les données dynamique
	for (unsigned i = 0; i< stats.size(); ++i)
	{
		if(stats[i] != NULL)
			delete(stats[i]);
	}
	
	
}

void Tableau::calculate_results()
{
	int taille_tournee = soluce->get_tournee().size();
	int parcours_arrivee;
	
	std::cout << "*** calcul des resultats ***" << std::endl;
	
	temps somme_attente = 0.0;
	temps diviseur_attente = 0.0;
	moyenne_attente = 0.0;
	
	std::vector <temps> somme_arrivee;
	somme_arrivee.resize(taille_tournee);
	//remplir de 0.0
	
	std::vector <temps> diviseur_arrivee;
	diviseur_arrivee.resize(taille_tournee);
	//remplir de 0.0
	
	moyenne_arrivee.resize(taille_tournee);
	
	nombre_echecs.resize(taille_tournee);
	//remplir de 0
	
	//std::cout << "vecteurs mis en place" << std::endl;
	
	//calculer moy arrivee, moy attente, et client le plus genant.
	for (int i = 0; i < nb_iterations; ++i)
	{
		//std::cout << "iter " << i << std::endl;
		
		int pos_fail = stats[i]->get_position_fail();
		
		//std::cout << "position d'echec : " << pos_fail << std::endl;
		
		if (pos_fail == -1)
		{
			parcours_arrivee = taille_tournee;
			
			somme_attente += stats[i]->get_tps_attendu();
			diviseur_attente ++;
		}
		else
		{
			parcours_arrivee = pos_fail;
			
			nombre_echecs[ pos_fail ] ++;
		}
		
		//std::cout << "parcours du vecteur d'arrivee : ";
		
		for (int j=0; j < parcours_arrivee - 1; j++)
		{
			//std::cout << stats[i]->get_arrivee(j) << " ";
			
			somme_arrivee[j] += stats[i]->get_arrivee(j);
			diviseur_arrivee[j] ++;
		}
		
		//std::cout << std::endl;
		
	}
	
	//diviser, faire la moyenne, et trouver le client le plus gênant
	int nombre_fail_max = 0;
	int index_fail_max = 0;
	
	std::cout << "disposition des echecs : ";
	
	for (int j=0; j < taille_tournee; j++)
	{
		
		if (diviseur_arrivee[j] > 0)
		{
			moyenne_arrivee[j] = somme_arrivee[j] / diviseur_arrivee[j];
		}
		else
		{
			moyenne_arrivee[j] = -1;
		}
		
		std::cout << nombre_echecs[j] << " ";
		
		if (nombre_echecs[j] > nombre_fail_max)
		{
			nombre_fail_max = nombre_echecs[j];
			index_fail_max = j;
		}
	}
	std::cout << std::endl;
	
	if ( diviseur_attente > 0)
	{
		moyenne_attente = somme_attente / diviseur_attente;
	}
	else
	{
		moyenne_attente = -1;
	}
	
	pire_index = index_fail_max;
	
	//std::cout << "nb de reussites : " << diviseur_attente << "\nclient chiant : " << index_fail_max << " (" << soluce->get_tournee(index_fail_max) << ")" << std::endl;
	
}

void Tableau::print_results(std::ostream & os)
{
    //print nb iter, moy arrivee, moy attente, et client le plus genant.
    
    os << "\nRESULTATS : \nnombre iterations : " << nb_iterations << std::endl;
    
    /*
    for (int i = 0; i< nb_iterations; ++i)
    {
		stats[i]->display(os);
	}
    */
    
	os << "moyenne d'arrivee : ";
    for (unsigned i=0; i< moyenne_arrivee.size(); ++i)
    {
		os << moyenne_arrivee[i] << " ";
	}
	os << std::endl;
	
	os << "pire client : " << pire_index << "(" << nombre_echecs[pire_index] << " echecs)" << std::endl;
    os << "moyenne d'attente pour ceux qui ont reussi : " << moyenne_attente << std::endl;
}






void Tableau::create_SVG_component(std::ostream & os)
{
	//attributs de départ : largeur (fixe ?), hauteur (dépend du nombre de clients dans une tournee).

	os << "<svg " << ">" << std::endl;

	//rectangle des fenetre de temps (jaune -- vert -- rouge), avec des lignes de séparation entre 2 clients

	//si deterministe : polyline

	//si non deterministe : lignes de moyenne, et taux de fail

	os << "</svg>" << std::endl;
}


void create_HTML(std::ostream & os, std::vector <Tableau> all_results)
{
	os << "<!DOCTYPE html>" << std::endl;
	os << "<html>" << std::endl;
	os << "<body>" << std::endl;
	os << "<h2>"<< /* titre << */ "</h2>" << std::endl;

	//boucle aff tableau
	//"for each" tableau t, t.create_SVG_component(os);
	for (unsigned i = 0; i < all_results.size(); ++i)
	{
		Tableau & tab = all_results[i];

		tab.create_SVG_component(os);
	}

	os << "</body>" << std::endl;
	os << "</html>" << std::endl;
}
