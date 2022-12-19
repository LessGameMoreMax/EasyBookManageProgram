#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
#include <string>
#include <iostream>
namespace sablin{
	
class program_is_running : public std::logic_error{
public:
	explicit program_is_running(const std::string &s):
		std::logic_error(s) {}
};	
	
}
#endif
