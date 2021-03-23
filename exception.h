#ifndef EXCEPTION_H
#define EXCEPTION_H

struct Exception {
	std::string msg;
	Exception(std::string msg) : msg{msg} {}
};

#endif
