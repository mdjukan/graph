#pragma once
#include "vector.h"

class EmptyQueueError {};

//FIFO queue put, get
//kruzni niz -> put, get O(1) -> realokcija je O(n)
//AAAAAAAAAAAAAAAAAA
//^^                 -> realokaija -> O(n)
//lf
//AAAAAAAAAAAAAAAAAAxxxxxxxxxxxxxxxxxxxxx
//^                ^

template<typename T>
class Queue {
	private:
		Vector<T> elems;
	public:
		void put(T value) {
			elems.push_back(value);
		}

		T get() {
			if (elems.empty()) {
				throw EmptyQueueError();
			}

			T value = elems[0];
			elems.erase(0); //O(n)
			return value;
		}

		bool empty() {
			return elems.empty();
		}
};
