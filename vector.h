#pragma once

#include <cstdlib>
#include <iostream>

class IndexOutOfRange {};
class InvalidArgument {};

#define INIT_CAP 1
#define REALLOC_COEF 2
template<typename T>
class Vector {
private:
	T *m_data;
	size_t m_size;
	size_t m_cap;

	void realloc() {
		if (m_cap==0) {
			m_cap = INIT_CAP;
		} else {
			m_cap *= REALLOC_COEF;
		}

		T *new_data = new T[m_cap];
		if (new_data==nullptr) {
			std::cerr << "allocation failed" << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i=0; i<m_size; i++) {
			new_data[i] = std::move(m_data[i]);
		}

		delete []m_data;
		m_data = new_data;
	}

public:
	~Vector() {
		delete []m_data;
	}

	Vector(int n, T value) {
		if (n<=0) {
			throw InvalidArgument();
		}

		m_data = new T[n];
		if (m_data==nullptr) {
			std::cerr << "allocation failed" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_size = n;
		m_cap = n;

		for (int i=0; i<n; i++) {
			m_data[i] = value;
		}
	}

	Vector(const Vector &other) {
		m_data = new T[other.m_cap];
		m_size = other.m_size;
		m_cap = other.m_cap;

		for (int i=0; i<m_size; i++) {
			m_data[i] = other.m_data[i];
		}
	}

	Vector(Vector &&other) {
		m_data = other.m_data;
		other.m_data = nullptr;

		m_size = other.m_size;
		other.m_size = 0;

		m_cap = other.m_cap;
		other.m_cap = 0;
	}

	//T = Vector<String>
	//levi oparand je this
	//desni operand jednakosti je other
	// leva strana oslobadja svoju memoriju i upisuje kopiju memorije drugog
	Vector& operator=(const Vector &other) {
		delete []m_data;

		m_data = new T[other.m_cap];
		m_cap = other.m_cap;
		m_size = other.m_size;

		for (int i=0; i<m_size; i++) {
			m_data[i] = other.m_data[i];
		}

		return *this;
	}

	//Vector<string> 
	//new std::string[INIT_CAP]
	Vector() {
		m_data = new T[INIT_CAP];
		if (m_data==nullptr) {
			std::cerr << "allocation failed" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_size = 0;
		m_cap = INIT_CAP;
	}

	T& operator[](int idx) {
		if (idx<0 || idx>=m_size) {
			throw IndexOutOfRange();
		}

		return m_data[idx];
	}

	//T = Vector<string>
	void push_back(T value) {
		if (m_size==m_cap) {
			realloc();
		}

		//Vector<Vector<string>>
		//T = Vector<string>
		//m_data[m_size] <- leva stran
		//value <- desna strana jednakosti
		//<< poziva operator =
		m_data[m_size] = value;
		m_size += 1;
	}

	void insert(int idx, T value) {
		if (idx<0 || idx>m_size) {
			throw IndexOutOfRange();
		}

		if (m_size==m_cap) {
			realloc();
		}

		//idx, idx+1, ...
		for (int i=m_size-1; i>=idx; i--) {
			m_data[i+1] = m_data[i];
		}

		m_data[idx] = value;
		m_size += 1;
	}

	//move sematics -> move constructor -> 
	//slicna copy constructoru
	//
	void erase(int idx) {
		if (idx<0 || idx>=m_size) {
			throw IndexOutOfRange();
		}

		for (int i=idx+1; i<m_size; i++) {
			m_data[i-1] = std::move(m_data[i]);
		}

		m_size -= 1;
	}

	//removes elements [from, to)
	void erase(int from, int to) {
		if (from>to || from<0 || to>m_size) {
			throw IndexOutOfRange();
		}

		if (m_size==1) {
			clear();
			return;
		}

		for (int i=to; i<m_size; i++) {
			m_data[from + (i-to)] = std::move(m_data[i]);
		}

		m_size -= to-from;

	}

	void clear() {
		for (int i=0; i<m_size; i++) {
			m_data[i].~T();
		}

		m_size = 0;
	}

	bool empty() {
		return m_size==0;
	}

	size_t size() const {
		return m_size;
	}
};

/*
int main() {
	Vector<int> xs;
	xs.push_back(1);
	xs.push_back(2);
	xs.push_back(3);
	xs.push_back(4);
	xs.push_back(5);

	for (int i=0; i<xs.size(); i++) {
		std::cout << xs[i] << " ";
	}
	std::cout << std::endl;

	//[1, 1) -> / ==> 1, 2, 3, 4, 5
	xs.erase(1, 1);

	for (int i=0; i<xs.size(); i++) {
		std::cout << xs[i] << " ";
	}
	std::cout << std::endl;
}
*/
