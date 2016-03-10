//google test
//#include <gtest/gtest.h>
/*
 //exemples de Gtests
  EXPECT_STREQ(alex.getNom().c_str(), "Alex");
  EXPECT_EQ(alex.getGenre(), HOMME);
*/


#include <fstream>
#include <iostream>
#include <ctime>

#include "data.hpp"
#include "solution.hpp"
#include "stats.hpp"

//#include <iomanip>
//#include <chrono>


// main function
int main (int argc, char * argv[])
{
  std::srand(std::time(0));
	
  //std::string prefix("Instances/Data_Cordeau/"); //Windows
  //std::string soluce("Instances/Solution_Cordeau/");

  std::string prefix("../Instances/Data_Cordeau/"); //Linux
  std::string soluce("../Instances/Solution_Cordeau/");

  std::string filename("c101"); //argument de départ (modifiable par user input)
  
  std::string output("../Resultats_"+ filename +".txt");
  
  std::ofstream os (output.c_str(), std::ofstream::out);
  
  int max_iter = 10000;
  
  // check the command line
  if (argc > 2)
  {
    std::cerr << "Error: arguments attendus (instance name)" << std::endl;
    return 1;
  }
  else if (argc == 2)
  {
    filename = argv[1];
  }

  /*
  double norm;
  int index;
  int tab [100] = {0};

  // démarrer les tests sous loi normale
  for (int i = 0; i< 1000000; i++)
  {
     norm = loi_normale()();
     index = (int)(norm*10.0 + 50.0);
     if (index < 0) index=0;
     if (index >= 100) index=99;

     tab[index]++;
  }

  for (index = 0; index< 100; index++)
  {
     std::cout << index-50 << "/10 -> " << tab[index] << std::endl;
  }
  */



  // lire l'instance
  data d(prefix, filename);


  //afficher l'instance
  std::cout << "nom du fichier : " << filename << std::endl;
  d.afficherData(os);
  d.calculerDistances();
  //d.afficherDistances(std::cout);
  //std::cout << data << std::endl; //surcharge de << pas encore fait


  //creer solutions (et Tableaux de sortie)
  std::vector <solution*> v_sols;
  std::vector <Tableau*> v_tab;
  v_sols.clear();
  v_sols.resize(d.get_nb_vehicules());
  v_tab.clear();
  v_tab.resize(d.get_nb_vehicules());

  std::string soluce_path = soluce + filename + ".res";
  std::string tmp;

  std::ifstream is(soluce_path.c_str());
  if (!is)
  {
	  std::cerr << "Error: unable to open file \'" << filename << "\'" << std::endl;
	  exit (EXIT_FAILURE);
  }

  is >> tmp; //on ecarte le premier nombre, qui semble etre le makespan

  for (int i = 0; i< d.get_nb_vehicules(); ++i)
  {
	temps total = 0;
	std::vector <int> tournee_tmp;
	tournee_tmp.clear();

	int tournee_depot = -1;
	int tournee_element = -1;

	is >> tmp >> tmp >> total >> tmp;
	std::cout << "total : " << total << std::endl;

	is >> tournee_depot >> tmp;
	tournee_tmp.push_back(tournee_depot);

	while (tournee_element != tournee_depot)
	{
		is >> tournee_element >> tmp;
		tournee_tmp.push_back(tournee_element);
	}

	solution * s = new solution(total, tournee_tmp, &d);
	s->display(std::cout);

	v_sols[i] = s;
	
  }

  is.close();

  
  //tester les solutions

  for (unsigned i = 0; i< v_sols.size(); ++i)
  {
	solution & sol = *v_sols[i];
	
	std::cout << "\n\n\nSolution numero " << i << std::endl;
	sol.display(std::cout);
	// tester la solution de manière déterministe
	
	sol.check_deterministe(d.get_fen_deb(0));
	sol.check_reverse_deterministe(d.get_fen_fin(0));

	std::cout << "debut optimisé et temps d'attente total : " << sol.get_start_opti() << " , " << sol.get_start_wait() << std::endl;


	//creer dans tableau max_iter Statistique, qui ont chacun un vecteur d'arrivee egale a nb_sommets de data
	Tableau * tab = new Tableau(&d, &sol, max_iter);
	
	for (int j = 0; j < max_iter; ++j)
	{
		//std::cout << "Sol " << i << "\tIter " << j << "\t";
		sol.check_normal(sol.get_start_opti(), 30); //test à 10% de variance, des résultats sont produits !!

		Statistique * stat = new Statistique(&sol);
		
		//stat->display(std::cout);
		
		tab->stats[j] = stat;

		
	}

    //calculer et afficher les resultats.
	tab->calculate_results();
	
	os << "Tournee " << i << std::endl;
	tab->print_results(os);
	
	v_tab[i] = tab;
  
  }

  
  
  
  
  //fin : libérer ressources

	for (unsigned i = 0; i< v_sols.size(); ++i)
	{
		delete(v_sols[i]);
	  
	}
	for(unsigned i=0; i < v_tab.size(); ++i)
	{
		delete(v_tab[i]);
	}

  return 0;
}
