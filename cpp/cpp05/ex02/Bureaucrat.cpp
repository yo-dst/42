#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

Bureaucrat::Bureaucrat(void) : 
	name("anonyme"),
	grade(150) {}

Bureaucrat::Bureaucrat(std::string const name, int const grade) :
	name(name),
	grade(grade) {
	if (grade > 150) {
		throw (Bureaucrat::GradeTooLowException());
	} else if (grade < 1) {
		throw (Bureaucrat::GradeTooHighException());
	}
}

Bureaucrat::Bureaucrat(Bureaucrat const & src) {
	*this = src;
}

Bureaucrat::~Bureaucrat(void) {}

Bureaucrat &	Bureaucrat::operator=(Bureaucrat const & src) {
	this->grade = src.grade;
	return (*this);
}

std::ostream &	operator<<(std::ostream & ostrm, Bureaucrat const & ros) {
	ostrm << ros.getName() << ", bureaucrat grade " << ros.getGrade();
	return (ostrm);
}

std::string	Bureaucrat::getName(void) const {
	return (this->name);
}

int	Bureaucrat::getGrade(void) const {
	return (this->grade);
}

void	Bureaucrat::incrGrade(void) {
	if (this->grade == 1) {
		throw (Bureaucrat::GradeTooHighException());
	} else {
		this->grade--;
	}
}

void	Bureaucrat::decrGrade(void) {
	if (this->grade == 150) {
		throw (Bureaucrat::GradeTooLowException());
	} else {
		this->grade++;
	}
}

void	Bureaucrat::signForm(Form & frm) const {
	try {
		frm.beSigned(*this);
		std::cout << this->name << " signed " << frm.getName() << std::endl;
	} catch (std::exception & e) {
		std::cout << this->name << " couldn’t sign " << frm.getName() << " because " << e.what() << std::endl;
	}
}

void	Bureaucrat::executeForm(Form const & form) const {
	try {
		form.execute(*this);
		std::cout << this->name << " executed " << form.getName() << std::endl;
	} catch (std::exception & e) {
		std::cerr << e.what() << '\n';
	}
}

