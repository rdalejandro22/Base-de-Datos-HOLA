#include <iostream>
#include <vector> 
#include <string>

using namespace std;

template <class T>
struct Menor{
	inline bool operator()(T a, T b){
		return a<b;
	}
};

template<class T>
class MayorNum{
public:
	inline bool operator()(T a , T b){
		return a.Number > b.Number;
	}
};

template<class T>
struct MenorNum
{
	inline bool operator()(T* a , T* b){
		return a->val.Number < b->val.Number;
	}
};


template<class T>
class MenorPais
{
public:
	inline bool operator()(T a , T b){
		return a.Country < b.Country;
	}
};

struct Player
{
	unsigned int Number;
	string Surname;
	char Position;
	unsigned int Age;
	string Country;
	Player(){};
	Player(unsigned int number, string surname, char position, unsigned int age, string country)
	{
		Number = number;
		Surname = surname;
		Position = position;
		Age = age;
		Country = country;
	}
	bool operator==(Player A){
		if (this->Number != A.Number) return 0;
		if (this->Surname != A.Surname) return 0;
		if (this->Position != A.Position) return 0;
		if (this->Country != A.Country) return 0;
		if (this->Age != A.Age) return 0;
		return 1;
	}
	bool operator!=(Player A){
		if (this->Number != A.Number) return 1;
		if (this->Surname != A.Surname) return 1;
		if (this->Position != A.Position) return 1;
		if (this->Country != A.Country) return 1;
		if (this->Age != A.Age) return 1;
		return 0;
	}
};

ostream& operator<< (ostream& out, const Player& A){
	out << "Apellido: " << A.Surname << endl;
	out << "Numero: " << A.Number << endl;
	out << "Posicion: " << A.Position << endl;
	out << "Edad: " << A.Age << endl;
	out << "Pais: " << A.Country << endl;
	out << endl;
	return out;
}

template<class T>
class Nodo
{
public:
	T val;
	Nodo <T> *next, *countryN;
	Nodo(T val, Nodo<T> *sig)
	{
		this->val = val;
		next = sig;
	}
};

template<class T>
class LinkedList
{
public:
	Nodo <T> *head;
	LinkedList()
	{
		head = NULL;
	};
	LinkedList(T tam)
	{
		Nodo <T>*aux = new Nodo<T>(1, NULL);
		head = aux;
		for (int i = 3; i <= tam; i=i+2)
		{
			aux->next = new Nodo<T>(i, NULL);
			aux = aux->next;
		}
		aux->next = NULL;
	};
	void pop_back(){
		Nodo <T>**ptr = &head;
		while ( (*ptr)->next){
			ptr = &((*ptr)->next);
		}
		delete *ptr;
		*ptr = 0;
	}
	void print()
	{
		Nodo <T>*temp;
		temp = head;
		while(temp)
		{
			cout << temp->val;
			temp = temp->next;
		}
		//cout << endl;
	}
	bool find(T x, Nodo <T>**&ptr)
	{
		ptr = &head;
		/*while(*ptr != NULL && cmp((*ptr)->val, x))
		{
			ptr = &((*ptr)->next);
		}*/
		while ( *ptr != NULL && (*ptr)->val != x){
			ptr = &((*ptr)->next);
		}
		return *ptr && (*ptr)->val == x;
	}
	bool insert(T x)
	{
		Nodo <T>**ptr = &head;
		while ( *ptr != NULL ){
			ptr = &((*ptr)->next);
		}
		/*if (find(x, ptr))
		{
			return 0;
		}*/
		//Nodo <T>*temp = new Nodo<T>(x, NULL);
		*ptr = new Nodo<T>(x, *ptr);
		//temp->next = *ptr;
		//(*ptr) = temp;
	}
	bool del(T x)
	{
		Nodo <T>**ptr;
		if(!find(x, ptr))
		{
			return 0;
		}
		Nodo <T>*del = *ptr;
		*ptr = (*ptr)->next;
		delete ptr;
		return 1;
	}
};
