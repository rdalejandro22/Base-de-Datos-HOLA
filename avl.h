#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include "lista.h"
using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;


template <class T>
void sortVector(vector<T>& vec){
	bool status = true;
	int n;
	n = vec.size();
	while (status){
		status = false;
		for(int i = 0; i < n-1; i++){
			if (vec.at(i)->val.Number > vec.at(i+1)->val.Number){
				T c = vec.at(i);
				vec.at(i) = vec.at(i+1);
				vec.at(i+1) = c;
				status = true;
			}
		}
		n--;
	}
}

template <class T, class M>
struct CNode{
	T m_x;
	CNode<T,M>* m_nodes[2];
	vector< Nodo<M>* > players; //countries, position, ages
	short height;
	ushort depth;
	CNode(T x){
		m_nodes[0] = m_nodes[1] = 0;
		m_x = x;
		height = 0;
		depth = 0;
	}
	void printList(){
		for(int i = 0; i < players.size(); i++){
			cout << players.at(i)->val;
		}
	}
	void addPlayer(Nodo<M>* player){
		players.push_back(player);
		sortVector< Nodo<M>* >(players);
	}
	void removePlayer(Nodo<M>* player){
		for(int i = 0; i < players.size(); i++)
		{
			if(players[i] == player)
			{
				players.erase(players.begin() + i);
			}
		}

	}
};

template <class T, class M>
void printList(list<CNode<T, M>* > xd){
	for(typename list<CNode<T, M>* >::iterator it = xd.begin(); it != xd.end(); it++){
		cout << (*it)->m_x << ' ';
	}
	cout << endl;
}

template<class T, class M, class C>
class AVL{
public:
	CNode<T, M>* m_root;
	C mc;
	AVL();
	bool find(T x, CNode<T, M>**& p);
	bool insert(T x);
	bool remove(T x);
	void inorder(CNode<T, M>* p);
	//private:
	CNode<T, M>** Rep(CNode<T, M>** p);
	short getHeight(CNode<T, M>* p);
	void printTree(CNode<T, M>* p);
private:
	bool findInPath(CNode<T, M>* p);
	bool findHeight(CNode<T, M>**& p, short& h);
	void updateDepth();
	void updateDepthR();
	void updateHeights(T x);
	list< CNode<T, M>* > path;
	short posInList(T val);
	void RR(CNode<T, M>** p);
	void LL(CNode<T, M>** p);
	void LR(CNode<T, M>** p);
	void RL(CNode<T, M>** p);
};

template<class T, class M, class C>
bool AVL<T,M,C>::findInPath(CNode<T, M>* p){
	for (typename list<CNode<T, M>* >::iterator it = path.begin(); it != path.end(); it++){
		if ( *it == p ) return 1;
	}
	return 0;
}

template<class T, class M, class C>
short AVL<T,M,C>::posInList(T val){
	short i = 1;
	typename list<CNode<T, M>* >::iterator it;
	for(it = path.begin() ; it != path.end() && (*it)->m_x != val; it++, i++);
	if (it == path.end() ) return -1;
	return i;
}

template<class T, class M, class C>
void AVL<T,M,C>::printTree(CNode<T, M>* p){
	if (!p) return;
	cout << "Valor: " << p->m_x << " prof: " << p->depth << endl;
	if (p->m_nodes[0]) cout << "Hijo izquierdo: " << p->m_nodes[0]->m_x << ' ';
	if (p->m_nodes[1]) cout << "Hijo derecho: " << p->m_nodes[1]->m_x << endl;
	printTree(p->m_nodes[0]);
	printTree(p->m_nodes[1]);
}

template<class T, class M, class C>
bool AVL<T,M,C>::findHeight(CNode<T, M>**& p, short& h){
	for(typename list<CNode<T, M>*>::reverse_iterator it = path.rbegin(); it != path.rend(); it++){
		if ((*it)->height == 2){
			p = &(*it);
			if ( (*it)->m_nodes[1]->height == -1) h = 3;
			else h = 2;
			return 1;
		} else if ((*it)->height == -2){
			p = &(*it);
			if ( (*it)->m_nodes[0]->height == 1) h = -3;
			else h = -2;
			return 1;
		}
	}
	return 0;
}

template<class T, class M, class C>
void AVL<T,M,C>::RR(CNode<T, M>** p){
	typename list<CNode<T, M>*>::iterator it = path.end();
	bool LR = 0, RL = 0;
	if ( (*p)->m_nodes[1] && (*p)->m_nodes[1]->m_nodes[1] && (*p)->m_nodes[1]->m_nodes[1]->height == -1) LR = 1;
	if ( (*p)->height  == 1) {RL = 1; LR = 0;}
	while ( (*it) != (*p) ){
		it--;
	}
	CNode<T, M>* temp, *father;
	if (*it != m_root){ //si es que se tiene que balancear
		it--;
		if (RL){
			while( *p != (*it)->m_nodes[0]) it--; //busca en el camino hasta encontrar el padre de p
			temp = *it;
		} else 
			//temp = *(--it); //fuera de la raiz
			temp = *(it);
	}
	CNode<T, M>* gfather = *p;
	father = (*p)->m_nodes[1];
	if (!findInPath(father) ) path.push_back(father);
	CNode<T, M> *bl = father->m_nodes[0];
	father->m_nodes[0] = gfather;
	gfather->m_nodes[1] = bl;
	if (m_root == gfather) m_root = father;
	else {
		if (RL){
			temp->m_nodes[0] = father;
		} else if (LR && father->height == 1) temp->m_nodes[0] = father;
		else temp->m_nodes[1] = father;
	}
}

template<class T, class M, class C>
void AVL<T,M,C>::LL(CNode<T, M>** p){
	bool LR = 0, RL = 0;
	typename list<CNode<T, M>*>::iterator it = path.end();
	if ((*p)->m_nodes[0] && (*p)->m_nodes[0]->m_nodes[0] && (*p)->m_nodes[0]->m_nodes[0]->height == 1 ) RL = 1;
	if ( (*p)->height == -1) {LR = 1; RL = 0;}
	while ( (*it) != (*p)){
		it--;
	}
	CNode<T, M>* temp, *father;
	if (*it != m_root){ //si es que se tiene que balancear
		--it;
		if (LR){
			while( *p != (*it)->m_nodes[1]) it--; //busca en el camino hasta encontrar el padre de p
			temp = *it;
		} else 
			temp = (*it);
	}
	CNode<T, M>* gfather = *(p);
	father = (*p)->m_nodes[0];
	CNode<T, M> *br = father->m_nodes[1];
	if (!findInPath(father) ) path.push_back(father);
	father->m_nodes[1] = gfather;
	gfather->m_nodes[0] = br;
	if (m_root == gfather){
		m_root = father;
	}
	else {
		if (LR) {
			temp->m_nodes[1] = father;
		}
		else if (RL && father->height == -1) temp->m_nodes[1] = father;
		else temp->m_nodes[0] = father;
	}
}

template<class T, class M, class C>
void AVL<T,M,C>::LR(CNode<T, M>** p){
	if (!findInPath( (*p)->m_nodes[1] ) ) path.push_back((*p)->m_nodes[1]);
	LL( &(*p)->m_nodes[1]);
	RR(p);
}

template<class T, class M, class C>
void AVL<T,M,C>::RL(CNode<T, M>** p){
	if (!findInPath( (*p)->m_nodes[0] ) ) path.push_back((*p)->m_nodes[0]);
	RR( &(*p)->m_nodes[0]);
	LL(p);
}

template<class T, class M, class C>
CNode<T, M>** AVL<T,M,C>::Rep(CNode<T, M>** p){
	for(p = &(*p)->m_nodes[0]; *p && (*p)->m_nodes[1]; p = &(*p)->m_nodes[1]){ //the university-one like
		path.push_back(*p);
	}
	/*for(p = &(*p)->m_nodes[1]; *p && (*p)->m_nodes[0]; p = &(*p)->m_nodes[0]){ //VisuAlgo like
	path.push_back(*p);
	}*/
	path.push_back(*p);
	return p;
}

template<class T, class M, class C>
short AVL<T,M,C>::getHeight(CNode<T, M>* p){
	//if (p->m_x == x) return 0;
	short left, right;
	if (p->m_nodes[0]) left = (p)->m_nodes[0]->depth;
	else return (p->depth);//left = 0;
	if (p->m_nodes[1]) {
		right = p->m_nodes[1]->depth;
	}
	else return -p->depth;//right = 0;
	return right - left;
}

template<class T, class M, class C>
void AVL<T,M,C>::updateHeights(T x){
	for(typename list<CNode<T, M>* >::iterator it = path.begin(); it != path.end(); it++){
		(*it)->height = getHeight(*it);
	}
}

template<class T, class M, class C>
void AVL<T,M,C>::updateDepth(){
	CNode<T, M>** p;
	bool balance = 0;
	ushort lDepth, rDepth;
	typename list<CNode<T, M>* >::reverse_iterator rit = path.rbegin();
	while(rit != path.rend() ){
		if ( !(*rit)->m_nodes[0] && !(*rit)->m_nodes[1] ){
			(*rit)->depth = 0;
			rit++;
			continue;
		}
		if ( (*rit)->m_nodes[0] ) lDepth = (*rit)->m_nodes[0]->depth;
		else lDepth = 0;
		if ( (*rit)->m_nodes[1] ) rDepth = (*rit)->m_nodes[1]->depth;
		else rDepth = 0;
		(*rit)->depth = max(lDepth, rDepth) + 1;
		(*rit)->height = getHeight(*rit);
		if ((*rit)->height == 2){
			p = &(*rit);
			balance = 1;
			if ( getHeight((*rit)->m_nodes[1]) == -1){
				LR(p);
			}
			else{
				RR(p);
			}
		} else if ((*rit)->height == -2){
			p = &(*rit);
			balance = 1;
			if ( getHeight((*rit)->m_nodes[0]) == 1){
				RL(p);
			}
			else{
				LL(p);
			}
		}
		if (balance){
			break;
		}
		rit++;
	}
}

template<class T, class M, class C>
void AVL<T,M,C>::updateDepthR(){
	ushort lDepth, rDepth;
	typename list<CNode<T, M>* >::iterator it = path.begin();
	while(it != path.end() ){
		if ( !(*it)->m_nodes[0] && !(*it)->m_nodes[1] ){
			(*it)->depth = 0;
			it++;
			continue;
		}
		if ( (*it)->m_nodes[0] ) lDepth = (*it)->m_nodes[0]->depth;
		else lDepth = 0;
		if ( (*it)->m_nodes[1] ) rDepth = (*it)->m_nodes[1]->depth;
		else rDepth = 0;
		(*it)->depth = max(lDepth, rDepth) + 1;
		(*it)->height = getHeight(*it);
		it++;
	}
}


template<class T, class M, class C>
AVL<T,M,C>::AVL(){
	this->m_root = 0;
}

template<class T, class M, class C>
bool AVL<T,M,C>::find(T x, CNode<T, M>**& p){
	for(p = &m_root; *p && (*p)->m_x != x; p = &((*p)->m_nodes[mc((*p)->m_x, x)])){
		path.push_back(*p);
	}
	return !!*p;
}

template<class T, class M, class C>
bool AVL<T,M,C>::insert(T x){
	CNode<T, M>** p;
	if(find(x, p)){
		path.clear();
		return 0;
	}
	*p = new CNode<T, M>(x);
	path.push_back(*p);
	updateDepth();
	updateDepthR();
	path.clear();
	return 1;
}

template<class T, class M, class C>
bool AVL<T,M,C>::remove(T x){
	CNode<T, M>** p;
	if(!(find(x,p))){
		path.clear();
		return 0;
	}
	path.push_back(*p);
	if ((*p)->m_nodes[0] && (*p)->m_nodes[1]){
		CNode<T, M>** q = Rep(p);
		(*p)->m_x = (*q)->m_x;
		p = q;
	}
	CNode<T, M>* temp = *p;
	*p = (*p)->m_nodes[(*p)->m_nodes[1]!=0];
	delete temp;
	path.pop_back();
	updateDepth();
	updateDepthR(); //this is needed when we do a balance after erasing
	path.clear(); //because a balanced node may not be a part of the path
	return 1;
}

template<class T, class M, class C>
void AVL<T,M,C>::inorder(CNode<T, M>* p){
	if (!p) return;
	inorder(p->m_nodes[0]);
	cout << p->m_x << " ";
	inorder(p->m_nodes[1]);
}
