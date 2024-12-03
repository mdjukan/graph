#pragma once
#include "vector.h"

class EmptyStackError {
};

template<typename T>
class Stack {
	private:
		Vector<T> m_elems;
	public:
		void push(T value) {
			m_elems.push_back(value);
		}

		bool empty() {
			return m_elems.size()==0;
		}

		T pop() {
			if (empty()) {
				throw EmptyStackError();
			}

			T elem = m_elems[m_elems.size()-1];
			m_elems.erase(m_elems.size()-1);
			return elem;
		}

		T top() {
			if (empty()) {
				throw EmptyStackError();
			}

			return m_elems[m_elems.size()-1];
		}

		size_t size() {
			return m_elems.size();
		}

		void clear() {
			m_elems.clear();
		}
};

/*
   int main() {
   Stack<std::string> s;
   s.push("a");
   s.push("b");

   s.clear();

   s.push("cc");
   s.push("dd");

   while (!s.empty()) {
   std::cout << s.pop() << std::endl;
   }
   }
   */
