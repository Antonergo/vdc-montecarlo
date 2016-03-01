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

//#include <iomanip>
//#include <chrono>


// main function
int main (int argc, char * argv[])
{
  //std::string prefix("Instances/Data/"); //Windows
  //std::string soluce("Instances/Solution_Simple/Sols.txt");

  std::string prefix("../Instances/Data_Cordeau/"); //Linux
  std::string soluce("../Instances/Solution_Cordeau/");

  std::string filename("c103"); //argument de départ (modifiable par user input)
  std::srand(std::time(0));

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
  data d(prefix + filename);


  //afficher l'instance
  std::cout << "nom du fichier : " << filename << std::endl;
  d.afficherData(std::cout);
  d.calculerDistances();
  //d.afficherDistances(std::cout);
  //std::cout << data << std::endl; //surcharge de << pas encore fait
  
  //creer solutions
  std::vector <solution*> v_sols;
  v_sols.clear();
  v_sols.resize(d.get_nb_vehicules());
  
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
  
  //tester les solutions
  
  for (unsigned i = 0; i< v_sols.size(); ++i)
  {
	solution & sol = *v_sols[i];
	// tester la solution de manière déterministe
	if (!sol.check_deterministe(0)) //set start min
	{
	  std::cout << "la verification deterministe a echouee, il y a peu de chance qu'une vérification aleatoire reussisse" << std::endl;
	}
	else
	{
	  std::cout << "la verification deterministe a reussie, on fait a l'envers." << std::endl;

	  sol.check_reverse_deterministe(sol.get_start_min()); //set start max
	  
	  std::cout << "debut min et début max de départ : " << sol.get_start_min() << " , " << sol.get_start_max() << std::endl;
	  
	  //sol.check_normal(s.get_start_min(), 30); //1 seul test à 30% de variance

	}
  
  }
  //fin : libérer ressources
  
  for (unsigned i = 0; i< v_sols.size(); ++i)
  {
	  delete(v_sols[i]);
  }

  return 0;
}
