#ifndef _Fixed_hpp_
#define _Fixed_hpp_

#include <iostream>

class Fixed {
	public:
		Fixed(void);
		Fixed(int const x);
		Fixed(float const f);
		Fixed(Fixed const & src);
		~Fixed(void);
		Fixed &	operator=(Fixed const & src);
	
	public:
		int 	getRawBits(void) const;
		void 	setRawBits(int const raw);
		float 	toFloat(void) const;
		int 	toInt(void) const;
		void	showRange(void) const;

	private:
		int					_bits;
		static int const	_nbBitsFracPart;
};

std::ostream &	operator<<(std::ostream &ostrm, Fixed const & src);

#endif
