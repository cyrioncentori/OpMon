
#ifndef EVOLUTION_HPP
#define EVOLUTION_HPP

#include <iostream>
#include "../objects/Espece.hpp"
#include "../objects/Pokemon.hpp"

using namespace std;

class Evolution{
public:
	virtual ~Evolution();
	Evolution(int evo);
	virtual bool checkEvolve(Pokemon const& toCheck) const = 0;
	Espece* getEvolution();
	void checkEvo();

protected:
	Espece *toEvolve;
	int evo;
};

#endif /* EVOLUTION_HPP */
