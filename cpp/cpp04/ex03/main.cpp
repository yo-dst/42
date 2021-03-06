#include <iostream>
#include "AMateria.hpp"
#include "ICharacter.hpp"
#include "MateriaSource.hpp"
#include "Character.hpp"
#include "Ice.hpp"
#include "Cure.hpp"

int	main(int ac, char *av[]) {
	(void)ac;
	(void)av;

	IMateriaSource* src = new MateriaSource();
	//dynamic_cast<MateriaSource *>(src)->showMaterias();
	src->learnMateria(new Ice());
	src->learnMateria(new Cure());
	//dynamic_cast<MateriaSource *>(src)->showMaterias();
	ICharacter* me = new Character("me");
	AMateria* tmp;
	tmp = src->createMateria("ice");
	me->equip(tmp);
	tmp = src->createMateria("cure");
	me->equip(tmp);
	ICharacter* bob = new Character("bob");
	me->use(0, *bob);
	me->use(1, *bob);
	delete bob;
	delete me;
	delete src;
	return 0;
}
