#include "Funcs.hpp"
#include "RandomNumber.tpp"

unsigned short getFreshId(const std::set<unsigned short>& knownIds) {
	auto id = randNum<unsigned short>();
	return (knownIds.count(id)) ? getFreshId(knownIds) : id;
}

unsigned short getFreshId(const std::map<unsigned short, std::string>* knownIds) {
	auto id = randNum<unsigned short>();
	return (knownIds->count(id)) ? getFreshId(knownIds) : id;
}
