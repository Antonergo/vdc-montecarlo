#include <fstream> //ifstream
#include <iostream>
#include <math.h>
//#include <sstream>
#include <stdlib.h> //exit()

#include "data.hpp"


data::data(std::string filename)
{
	temps valeur;
	temps tmp;
	
	std::string poubelle;
	std::string str;
	
	// open the instance
	std::ifstream is(filename.c_str());
	if (!is)
	{
	std::cerr << "Error: unable to open file \'" << filename << "\'" << std::endl;
	exit (EXIT_FAILURE);
	}

	name = filename.substr(filename.size()-4,4);
	
	// Première ligne
	is >> poubelle;
	is >> nb_vehicules;
	is >> nb_clients;
	is >> nb_depots;
	nb_sommets = nb_clients + nb_depots;
	
	// Deuxième ligne
	is >> poubelle;
	is >> capacite_vehicules;
	/*
	xVector.resize(nb_sommets);
	yVector.resize(nb_sommets);
	fen_deb.resize(nb_sommets);
	fen_fin.resize(nb_sommets);*/
	
	// Les infos du dépot
	is >> poubelle;
	
	is >> valeur;
	xVector.push_back(valeur);
	//is >> xVector[i];
	
	is >> valeur;
	yVector.push_back(valeur);
	//is >> yVector[i];
	
	is >> poubelle;
	
	is >> poubelle;
	
	is >> poubelle;
	
	is >> poubelle;
	
	is >> tmp;
	fen_deb.push_back(tmp);
	//is >> fen_deb[i];
	
	is >> tmp;
	fen_fin.push_back(tmp);
	
	// Les infos des clients
	for(int i = 0; i < nb_clients; ++i)
	{
		is >> poubelle;
		
		is >> valeur;
		xVector.push_back(valeur); //X
		
		is >> valeur;
		yVector.push_back(valeur); //Y
		
		is >> service; //temps de service
		is >> poubelle; //capacité demandée (inutile ici)
		is >> poubelle; // ?
		is >> poubelle; // ?
		is >> poubelle; // ?
		
		is >> tmp;
		fen_deb.push_back(tmp); //fenetre debut
		
		is >> tmp;
		fen_fin.push_back(tmp); //fenetre fin
	}
	
	is.close();
	
	
	
	/*
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
	}*/
}

std::string data::get_name()
{
	return name;
}

int data::get_nb_depots()
{
	return nb_depots;
}

int data::get_nb_vehicules()
{
	return nb_vehicules;
}

int data::get_nb_clients()
{
	return nb_clients;
}

int data::get_nb_sommets()
{
	return nb_sommets;
}

temps data::get_dist(int a, int b)
{
	if (a < 0 || b < 0 || a >= nb_sommets || b >= nb_sommets)
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

temps data::get_service()
{
	return service;
}

void data::afficherData(std::ostream & os)
{
	os << "Nom de l'instance : " << name << std::endl;
	os << "Nombre de depots : " << nb_depots << std::endl;
	os << "Nombre de vehicules : " << nb_vehicules << std::endl;
	os << "Nombre de clients : " << nb_clients << std::endl;
	os << "Nombre total de sommets : " << nb_sommets << std::endl << std::endl;
	os << "Temps de service : " << service << std::endl << std::endl;
	
	os << "id\t" << "x\t" << "y\t" << "deb\t" << "fin" << std::endl;
	for(int i = 0; i < nb_sommets; ++i)
	{
		os << i << "\t" << xVector[i] << "\t" << yVector[i] << "\t" << fen_deb[i] << "\t" << fen_fin[i] << std::endl;
	}
}

void data::afficherDistances(std::ostream & os)
{	
	os << "Matrice des distances" << std::endl;
	for(unsigned int i = 0; i < distances.size(); ++i)
	{
		if(i!= 0 && i%nb_sommets == 0)
		{
			os << std::endl;
		}
		
		os << distances[i] << " ";
	}
}

void data::calculerDistances()
{
	double dist;
	/*
	// Calcul des distances entre le dépôt et les clients
	for(int i = 0; i < nb_sommets; ++i)
	{
		distances.push_back( sqrt(pow(xVector[0]-xVector[i], 2) + pow(yVector[0]-yVector[i], 2)) );
	}*/
	
	// Calcul des distances entre chaque client
	for(int i = 0; i < nb_sommets; ++i)
	{
		for(int j = 0; j < nb_sommets; ++j)
		{
			//dist = sqrt(pow(xVector[i]-xVector[j], 2) + pow(yVector[i]-yVector[j], 2));
			dist = sqrt( (xVector[i]-xVector[j])*(xVector[i]-xVector[j]) + (yVector[i]-yVector[j])*(yVector[i]-yVector[j]) );
			if(i != 0)
			{
				distances.push_back(dist + service);
			}
			else // On considère le dépôt
			{
				distances.push_back(dist);
			}
		}
	}
}

/*
void data::display(std::ostream & os)
{
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
}*/
