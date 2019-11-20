#pragma once

class ParkingMeter
{
public:
	ParkingMeter() = delete;
	inline ParkingMeter(double minutes) { time_paid = minutes; };
	inline double getTime() const { return this->time_paid; };
private:
	double time_paid;
};