#include <iostream>
#include <fstream> //ifstream
//#include <sstream>
#include <stdlib.h> //exit()

#include "data.hpp"


data::data(std::string filename)
{
	// open the instance
  std::ifstream is(filename.c_str());
  if (!is)
  {
    std::cerr << "Error: unable to open file \'" << filename << "\'" << std::endl;
    exit (EXIT_FAILURE);
  }

  name = filename.substr (filename.size()-12,12);
  is >> nb_sommets;

  // read the distance matrice
  distances.resize(nb_sommets*nb_sommets);
  
  for (int i = 0; i< nb_sommets; ++i)
  {
  	for(int j=0; j< nb_sommets; ++j)
  	{
  		is >> distances[i*nb_sommets + j];
  	}
  }
  
  fen_deb.resize(nb_sommets);
  fen_fin.resize(nb_sommets);
  
  for (int i = 0; i< nb_sommets; ++i)
  {
  	is >> fen_deb[i];
  	is >> fen_fin[i];
  }
  
}

std::string data::get_name()
{
	return name;
}

int   data::get_nb_sommets()
{
	return nb_sommets;
}

temps data::get_dist(int a, int b)
{
	if (a < 0 || b < 0 ||
		a >= nb_sommets || b >= nb_sommets)
	{
		std::cerr << "ERROR : get distance, argument is out of range" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	return distances[a*nb_sommets + b];
}
	
temps data::get_fen_deb(int client)
{
	if (client < 0 || client >= nb_sommets)
	{
		std::cerr << "ERROR : get fenetre debut, argument is out of range" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	return fen_deb[client];
}
temps data::get_fen_fin(int client)
{
	if (client < 0 || client >= nb_sommets)
	{
		std::cerr << "ERROR : get fenetre fin, argument is out of range" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	return fen_fin[client];
}

void data::display(std::ostream & os)
{
	// get the instance name
  os << "NB sommets :" << nb_sommets << std::endl ;

  // read the distance matrice
  
  for (int i = 0; i< nb_sommets; ++i)
  {
  	for(int j=0; j< nb_sommets; ++j)
  	{
  		os << distances[i*nb_sommets + j] << " ";
  	}
  	os << std::endl;
  }
  
  
  for (int i = 0; i< nb_sommets; ++i)
  {
  	os << fen_deb[i] << " -> ";
  	os << fen_fin[i] << std::endl; 
  }
}
