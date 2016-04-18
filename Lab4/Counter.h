#pragma once
class Counter
{	
public:
	unsigned int shared;
	unsigned int weak;

	Counter() :shared(0), weak(0){};
	Counter(Counter & c) :shared(c.shared), weak(c.weak){};
	Counter(std::nullptr_t c) :shared(0), weak(0) {};
	Counter(int shared, int weak) :shared(shared), weak(weak) {};
	~Counter() {};

	Counter& operator=(const Counter& rhs) { shared = rhs.shared; weak = rhs.weak; };

	int IncreaseShared() { return ++shared; };
	int IncreaseWeak() { return ++weak; };

	int DecreaseShared() { return (shared == 0) ? (shared) : (--shared); };
	int DecreaseWeak() { return (weak == 0) ? (weak) : (--weak); };
};


