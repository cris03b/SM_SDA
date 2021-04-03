#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>
#include <iostream>
using namespace std;

//O(n)
SMIterator::SMIterator(const SortedMap& m) : map(m){
	//TODO - Implementation
	int current = m.root;
	elm_v = m.elm;
	left_v = m.left;
	right_v = m.right;
	if (m.length > 0) {
		while (left_v[current] != -1) {
			// ultimul element, cel mai din stanga, e cel mai mic (primul elm) din container
			current = left_v[current];
			
		}
	}
	current_pos = current;
	first_pos = current;
	root_pos = m.root;
	
}

//theta(1)
void SMIterator::first(){
	//TODO - Implementation
	if (valid()) {
		current_pos = first_pos;
	}
	else {
		current_pos = -1;
	}
	
}

//theta(1)
void SMIterator::last() {
	if (valid()) {
		int current = root_pos;
		while (right_v[current] != -1) {
			current = right_v[current];
		}
		current_pos = current;
	}
	else {
		current_pos = -1;
	}
}


//O(n)
void SMIterator::next(){
	//TODO - Implementation
	if (!valid()) {
		throw::exception();
	}
	bool ok = false;
	if (valid()) {
		stack<int> s; //stack ajutator care tine pozitiile nodurilor 
		int current, prev;
		current = root_pos; //pozitia nodului curent
		prev = -1; //pozitia nodului anterior celui curent
		while (current != -1) {
			// adauga in stack toate elementele doar de pe ramura stanga
			s.push(current);
			current = left_v[current];
		}
		while (!s.empty()) {
			current = s.top();
			s.pop();
			if (prev == current_pos) {
				current_pos = current;
				ok = true;
				break;
			}
			prev = current;
			current = right_v[current];
			while (current != -1) {
				// adauga in stack toate elementele doar de pe ramura stanga a subarborelui drept
				s.push(current);
				current = left_v[current];
			}
		}
	}
	if (!ok) {
		// daca nu s-a gasit => pozitie invalida
		current_pos = -1;
	}
	
}

//O(n)
// pe dos fata de functia de next => se incepe cautarea de la elm del mai din dreapta (cel mai mare)
// pana ajunge sa fie egal cu nodul la care se cauta prev-ul, variabila prev fiind cea cautata
void SMIterator::prev() {
	if (!valid()) {
		throw::exception();
	}
	bool ok = false;
	if (valid()) {
		stack<int> s; //stack ajutator care tine pozitiile nodurilor 
		int current, prev;
		current = root_pos; //pozitia nodului curent
		prev = -1; //pozitia nodului anterior celui curent
		while (current != -1) {
			// adauga in stack toate elementele doar de pe ramura dreapta
			s.push(current);
			current = right_v[current];
		}
		while (!s.empty()) {
			current = s.top();
			s.pop();
			if (current == current_pos) {
				current_pos = prev;
				ok = true;
				break;
			}
			prev = current;
			current = left_v[current];
			while (current != -1) {
				// adauga in stack toate elementele doar de pe ramura dreapta a subarborelui stang
				s.push(current);
				current = right_v[current];
			}
		}
	}
	if (!ok) {
		// daca nu s-a gasit => pozitie invalida
		current_pos = -1;
	}
}

//theta(1)
bool SMIterator::valid() const{
	//TODO - Implementation
	if (current_pos != -1) {
		return true;
	}
	return false;
}

//theta(1)
TElem SMIterator::getCurrent() const{
	//TODO - Implementation
	if (valid()) {
		return elm_v[current_pos];
	}
	else {
		throw::exception();
	}
	return NULL_TPAIR;
}


