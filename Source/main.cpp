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
  //std::string soluce("Instances/Solutions/Sols.txt");

  std::string prefix("../Instances/Data/"); //Linux
  std::string soluce("../Instances/Solutions/Sols.txt");

  std::string filename("rc_207.4.txt"); //argument de départ


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
  //d.display(std::cout);

  //std::cout << data << std::endl; //surcharge de << pas encore fait


  // lire la solution
  solution s(soluce, &d);

  s.display(std::cout);
  // tester la solution de manière déterministe
  if (!s.check_deterministe(0)) //set start min
  {
	  std::cout << "la verification deterministe a echouee, il y a peu de chance qu'une vérification aleatoire reussisse" << std::endl;
  }
  else
  {
	  std::cout << "la verification deterministe a reussie, on fait a l'envers." << std::endl;

      s.check_reverse_deterministe(s.get_start_min()); //set start max
	  
	  std::cout << "debut min et début max de départ : " << s.get_start_min() << " , " << s.get_start_max() << std::endl;
	  
	  //s.check_normal(s.get_start_min(), 10); //1 seul test à 10% de variance

  }


  return 0;
}
