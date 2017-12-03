#include <iostream>
#include <fstream>
#include <sstream>
#include "avl.h"

using namespace std;

typedef unsigned int uint;
typedef stringstream ss;

uint to_uint(string x){
	uint ret;
	ss convi(x);
	convi >> ret;
	return ret;
}

class DataBase{
public:
	LinkedList<Player> Players;
	AVL<string, Player, Menor<string> > Countries;
	AVL<char, Player, Menor<char> > Positions;
	AVL<uint, Player, Menor<uint> > Ages;
	void insert(Player x){
		Players.insert(x); //inserting to the linked list
		Countries.insert(x.Country); //inserting the player's country to the Tree xdd
		Positions.insert(x.Position); //inserting the player's position to the Tree xdd
		Ages.insert(x.Age); //inserting the player's age to the Tree xdd
		CNode<string, Player >** p; //country node
		CNode<char, Player>** q; //position node
		CNode<uint, Player>** r; //ages node
		Nodo<Player>** ply; //node to the player in the linked list
		Countries.find(x.Country, p); 
		Positions.find(x.Position, q); 
		Ages.find(x.Age, r); 
		Players.find(x, ply);
		if (*p) (*p)->addPlayer(*ply); //Adding at the Countries Tree
		if (*q) (*q)->addPlayer(*ply); //Adding at the positions tree
		if (*r) (*r)->addPlayer(*ply); //Adding at the Ages tree
	}
	void remove(Player x){
		Nodo <Player> **ply;
		Players.find(x, ply);
		CNode<string, Player >** p;
		CNode<char, Player >** q;
		CNode<uint, Player>** r;
		Countries.find(x.Country, p);
		Positions.find(x.Position, q);
		Ages.find(x.Age, r); 
		(*p)->removePlayer(*ply);
		(*q)->removePlayer(*ply);
		(*r)->removePlayer(*ply);
		Players.del(x);
	}
	void printByCountry(string country){
		CNode<string, Player >** p;
		Countries.find(country, p);
		if (*p) (*p)->printList();
	}
	void printByPosition(char pos){
		CNode<char, Player>** p;
		Positions.find(pos, p);
		if (*p) (*p)->printList();
	}
	void printByAgeRange(uint min, uint max){
		CNode<uint, Player>** r; //ages node
		vector< Nodo<Player>* > m_ages;
		for(min; min <= max; min++){
			if (Ages.find(min, r) ){
				for(uint i = 0; i < (*r)->players.size(); i++){
					m_ages.push_back((*r)->players.at(i) ); //llena el vector con los jugadores 
				} //de edad min
			}
		}
		sortVector<Nodo<Player>* >(m_ages); //lo ordena
		for(uint i = 0; i < m_ages.size(); i++){
			cout << m_ages.at(i)->val; //imprime ese vectorcin
		}
	}
	void FillList(){ //llena la lista con los contenidos de players.csv
		ifstream players;
		string surname, country;
		uint number;
		uint age;
		Player Temp;
		players.open("players.csv");
		while( !players.eof() ){
			getline( players, country, ',' );
			Temp.Country = country;
			getline(players, surname, ',' );
			Temp.Position = surname[0];
			getline(players, surname, ',');
			Temp.Surname = surname;
			getline(players, surname, ',');
			Temp.Age = to_uint(surname);
			getline(players, surname);
			Temp.Number = to_uint(surname);
			insert(Temp);
		}
		Players.pop_back(); ///xd
		return;
	}
};


int main (int argc, char *argv[]) {
	//FillList();
	DataBase DB;
	DB.FillList();
	//DB.Players.print();
	//DB.printByCountry("Belgica");
	//DB.printByPosition('M');
	DB.printByAgeRange(32, 35);
	return 0;
}

