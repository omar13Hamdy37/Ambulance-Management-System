#pragma once
#include "Patient.h"
#include <iostream>
#include<string>
using namespace std;
enum class CarType {
	SC, // Special Car
	NC  // Normal CAr
};

enum class CarStatus {
	Ready,
	Assigned,
	Loaded
};

class Car
{
private:
	int CarID;
	CarType type;
	float speed;
	CarStatus status;
	Patient* assignedPatient;
	int HID; // ID of hospital car belongs to

	int busyTime; // Total time car was in use.

public:
	// Constructor
	Car(int ID, CarType t, float sp, int HID, int busy = 0)
		: CarID(ID), type(t), speed(sp), status(CarStatus::Ready), assignedPatient(nullptr), busyTime(busy), HID(HID) {}

	// Setters

	void setStatus(CarStatus st) { status = st; }
	void setAssignedPatient(Patient* patient) { assignedPatient = patient; status = CarStatus::Assigned; }
	void PickPatient() { status = CarStatus::Loaded; }

	// Getters
	int getHID() const { return HID; }
	int getCarID() const { return CarID; }
	CarType getType() const { return type; }
	float getSpeed() const { return speed; }
	CarStatus getStatus() const { return status; }
	Patient* getAssignedPatient() const { return assignedPatient; }
	int getBusyTime() const { return busyTime; }

	// Operator overloading

	// overloading to allow the comparison of Cars with int ID. To see if the car object has that ID.
	bool operator==(int ID) const {
		return CarID == ID;
	}

	friend std::ostream& operator<<(std::ostream& output, const Car& c) {
		CarType tp = c.getType();
		string car = "";
		if (tp == CarType::NC) {
			car += "N";
		}
		else
		{
			car += "S";
		}
		output << car << c.getCarID() << "_" << "H" << c.getHID();
		
		return output;
	}


	// Utility functions

	// Phase 2 code
	// ok bos awalan ama patient yeba assigned yeba sa3tha assignment time bt3 patient updated
	// patient picked up.. update pickup time of patient -- update busy time bt3 el ragel -- change status
	//  patient dropped up - update finish time -- shoof change status
	// shoof hat calc busy time ezay law had canclled
	//
	int calculatePatientFinishTime()
	{
	    return assignedPatient->getPickupTime() + assignedPatient->getHospitalDistance() / speed;
	}
	
	int calculatePatientPickupTime()
	{
	    return assignedPatient->getAssignmentTime() + assignedPatient->getHospitalDistance() / speed;
	}
	
	void updateBusyTime()
	{
	
	    busyTime +=  - assignedPatient->getAssignmentTime();
	}
};