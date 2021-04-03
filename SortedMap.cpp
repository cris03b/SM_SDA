#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>
using namespace std;

//theta(n)
SortedMap::SortedMap(Relation r) {
	//TODO - Implementation
	rel = r;
	length = 0;
	cap = 500;
	elm = new TElem[cap];
	left = new int[cap];
	right = new int[cap];
	next_empty = new int[cap];
	root = -1;
	first_empty = 0;

	// initializare array uri goale
	for (int i = 0; i < cap; i++) {
		elm[i] = NULL_TPAIR;
		left[i] = -1;
		right[i] = -1;
		next_empty[i] = i + 1;
	}

	next_empty[cap - 1] = -1;
}

//O(n)
TValue SortedMap::add(TKey k, TValue v) {
	//TODO - Implementation
	if (isEmpty()) {
		// inserare primul elm => root
		elm[0].first = k;
		elm[0].second = v;
		root = 0;
		first_empty = 1;
		next_empty[0] = -2; //-2 => pozitia e ocupata, vor ramane in ordine doar poz libere
		length++;
		return NULL_TVALUE;
	}

	// daca s-a umplut array-ul trebuie marita cap si mutat in alta parte a memoriei
	if (first_empty == -1) {
		cap *= 2;
		// creare array-uri noi de dimensiune cap marita
		auto* new_elm = new TElem[cap];
		auto* new_left = new int[cap];
		auto* new_right = new int[cap];
		auto* new_next_empty = new int[cap];

		// copiere elemente in noile arrays
		for (int i = 0; i < cap; i++) {
			new_elm[i] = elm[i];
			new_left[i] = left[i] ;
			new_right[i] = right[i];
			new_next_empty[i] = next_empty[i];
		}

		// stergere arrays vechi din memorie
		delete[] elm;
		delete[] left;
		delete[] right;
		delete[] next_empty;

		// redenumire
		elm = new_elm;
		left = new_left;
		right = new_right;
		next_empty = new_next_empty;

		// initializare valori in a doua jumatate a array urilor
		for (int i = cap / 2; i < cap; i++) {
			elm[i] = NULL_TPAIR;
			left[i] = -1;
			right[i] = -1;
			next_empty[i] = i + 1;
		}
		next_empty[cap - 1] = -1;
		first_empty = cap / 2;
	}

	//cautare pozitie pentru inserare
	int current = root;
	int current_pos = -1;
	while (current != -1) {
		if (elm[current].first == k) {
			// daca sunt egale cheile => se schimba doar valoarea
			TValue old_value;
			old_value = elm[current].second;
			elm[current].second = v;
			return old_value;
		}
		if (rel(k, elm[current].first)) {
			// se cauta pozitia nodului anterior in subarborele stang
			current_pos = current;
			current = left[current];
		}
		else {
			// se cauta pozitia nodului anterior in subarborele drept
			current_pos = current;
			current = right[current];
		}
	}

	// verificare in functie de relatie, pentru a determina pozitia nodului ce trebuie adaugat

	if (rel(k, elm[current_pos].first)) {
		// elm va deveni copilul din stanga
		int pos = first_empty;
		elm[pos].first = k;
		elm[pos].second = v;
		left[current_pos] = pos; //se face legatura dintre parinte si noul elm
		first_empty = next_empty[first_empty]; //schimbare prima pozitie libera
		next_empty[pos] = -2;
		length++;
		return NULL_TVALUE;
	}

	else {
		// elm va deveni copilul din dreapta
		int pos = first_empty;
		elm[pos].first = k;
		elm[pos].second = v;
		right[current_pos] = pos; //se face legatura dintre parinte si noul elm
		first_empty = next_empty[first_empty]; //schimbare prima pozitie libera
		next_empty[pos] = -2;
		length++;
		return NULL_TVALUE;
	}
}

//O(n)
TValue SortedMap::search(TKey k) const {
	//TODO - Implementation
	int current = root;
	if (isEmpty()) {
		return NULL_TVALUE;
	}
	while (current != -1) {
		if (elm[current].first == k) {
			return elm[current].second;
		}
		if (rel(k, elm[current].first)) {
			// cautam in subarborele stang
			current = left[current];
		}
		else {
			// cautam in subarborele drept
			current = right[current];
		}
	}
	return NULL_TVALUE;
}

//O(n)
TValue SortedMap::remove(TKey k) {
	//TODO - Implementation
	if (isEmpty()) {
		return NULL_TVALUE;
	}
	int current = root;
	int parent = root;
	while (current != -1) {
		if (elm[current].first == k) {
			if (left[current] == -1 && right[current] == -1) {
				// stergere frunza
				if (current == root) {
					// stergere radacina => stergere intreg arbore
					root = -1;
					TValue deleted_node = elm[current].second;
					elm[current] = NULL_TPAIR;
					next_empty[current] = first_empty; //se face conexiunea cu sirul de empty
					first_empty = current;
					length--;
					return deleted_node;
				}
				if (rel(elm[current].first, elm[parent].first)) {
					// stergere copil stang
					left[parent] = -1;
				}
				else {
					// stergere copil drept
					right[parent] = -1;
				}
				
				TValue deleted_node = elm[current].second;
				elm[current] = NULL_TPAIR; //se sterge elm din lista
				next_empty[current] = first_empty; //se face conexiunea cu sirul de empty
				first_empty = current;
				length--;
				return deleted_node;
			}

			if (left[current] != -1 && right[current] != -1) {
				// stergere nod cu doi copii => nodul cu valoarea min din partea dreapta ii va lua locul
				// cautare pozitie a copilului min
				int pos = right[current];
				int parent_min = pos; //parintele copilului min
				while (left[pos] != -1) {
					parent_min = pos;
					pos = left[pos];
				}

				if (current == root) {
					// copilul lui va deveni radacina
					root = pos;
				}

				TValue deleted_node = elm[current].second;
				elm[current].first = elm[pos].first; //mutare valori de pe locul elm ce va fi sters
				elm[current].second = elm[pos].second;
				left[parent_min] = right[pos]; //elementele ramase din subarborele drept a elm vor deveni subarborele stang al parintelui
				elm[pos] = NULL_TPAIR; //se sterge elm din lista
				left[pos] = -1;
				right[pos] = -1;
				next_empty[pos] = first_empty; //se face conexiunea cu sirul de empty
				first_empty = pos;
				length--;
				return deleted_node;
			}

			if (left[current] != -1 || right[current] != -1) {
				// stergere nod care are un copil => copilul ii va lua locul, se sterge elm de pe poz copilului
				int pos;
				if (left[current] != -1) {
					// mutare valori copil stang in locul elm care va fi sters
					pos = left[current];
				}
				else {
					// mutare valori copil drept in locul elm care va fi sters
					pos = right[current];
				}
				if (current == root) {
					// copilul lui va deveni radacina
					root = pos;
				}
				TValue deleted_node = elm[current].second;
				elm[current].first = elm[pos].first; //mutare valori de pe locul elm ce va fi sters
				elm[current].second = elm[pos].second;
				left[current] = left[pos];
				right[current] = right[pos];
				elm[pos] = NULL_TPAIR; //se sterge elm din lista
				left[pos] = -1;
				right[pos] = -1;
				next_empty[pos] = first_empty; //se face conexiunea cu sirul de empty
				first_empty = pos;
				length--;
				return deleted_node;
			}

		}
		parent = current;
		if (rel(k, elm[current].first)) {
			// continua cautarea in subarborele stang
			current = left[current];
		}

		else {
			// continua cautarea in subarborele drept
			current = right[current];
		}
	}
	return NULL_TVALUE;
}

//theta(1)
int SortedMap::size() const {
	//TODO - Implementation
	if (!isEmpty()) {
		return length;
	}
	return 0;
}

//theta(1)
bool SortedMap::isEmpty() const {
	//TODO - Implementation
	if (length == 0) {
		return true;
	}
	return false;
}

SMIterator SortedMap::iterator() const {
	return SMIterator(*this);
}

//theta(1)
SortedMap::~SortedMap() {
	delete[] elm;
	delete[] left;
	delete[] right;
	delete[] next_empty;
	//TODO - Implementation
}
