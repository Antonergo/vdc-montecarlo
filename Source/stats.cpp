#include "stats.hpp"
#include <cstdlib>


void Statistique::set_arrivee(std::vector <temps> & valeurs)
{
	arrivee = valeurs;
}

Statistique::Statistique(solution * sol, data* d)
{

}


data* Tableau::donnees = NULL;


Tableau::Tableau(int iterations) //générer ici les tests réels ?
{

}

Tableau::~Tableau()
{}


void Tableau::set_data(data * ptr_donnees) //static
{
	donnees = ptr_donnees;
}

void Tableau::calculate_results()
{

}

void Tableau::print_results(std::ostream & os)
{
    //print nb iter, moy arrivee, moy attente, et client le plus genant.
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
