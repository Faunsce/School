#pragma once
#include <string>
#include <map>
#include <array>
#include "ParkingMeter.hpp"
#include "ParkedCar.hpp"

class ParkedCar;
class ParkingMeter;

class PoliceOfficer
{
public:
	PoliceOfficer() = default;
	PoliceOfficer(std::string /* name */);
	~PoliceOfficer();
	double examine(ParkedCar& /* car */, const ParkingMeter& /* meter */);
	inline std::string getName() const { return m_name; };
	inline std::string getName(unsigned short badge) const { return taken_ids.at(badge); };
	inline unsigned short getBadge() const { return m_id; };
private:
	std::string m_name;
	unsigned short m_id;
	static std::map<unsigned short, std::string> taken_ids;
};
