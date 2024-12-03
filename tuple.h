#pragma once

template<typename T1, typename T2, typename T3>
class Tuple {
	public:
		T1 first;
		T2 second;
		T3 third;

		Tuple() = default;
		Tuple(T1 first, T2 second, T3 third) : first(first), second(second), third(third) {}
};
