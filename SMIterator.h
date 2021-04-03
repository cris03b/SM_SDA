#pragma once
#include "SortedMap.h"
#include <stack>

//DO NOT CHANGE THIS PART
class SMIterator{
	friend class SortedMap;
private:
	const SortedMap& map;
	SMIterator(const SortedMap& mapionar);

	//TODO - Representation
	// elm_v, left_v, right_v = pointeri la array urile elm/left/right din SM
	// root_pos = pozitia radacinii, current_pos = pozitie curenta, first_pos = prima pozitie din SM (cel mai mic elm (dc rel: <=))
	TElem* elm_v;
	int* left_v, * right_v;
	int root_pos, current_pos, first_pos;

public:
	void first();
	void next();
	void prev();
	void last();
	bool valid() const;
    TElem getCurrent() const;
};

