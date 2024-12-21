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
	Loaded,
	Checkup,
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

	int checkupTime;
	bool failedCar;

	static int Sspeed;
	static int Nspeed;

public:
	// Constructor
	Car(int ID, CarType t, float sp, int HID, int checkup = 0)
		: CarID(ID), type(t), speed(sp), status(CarStatus::Ready), assignedPatient(nullptr), HID(HID), checkupTime(checkup), failedCar(false) {
		busyTime = 0;
	}

	// Setters

	void setStatus(CarStatus st) { status = st; }
	void setAssignedPatient(Patient* patient) { assignedPatient = patient; status = CarStatus::Assigned; }
	void PickPatient() { status = CarStatus::Loaded; }
	void setCarFailed(bool b) { failedCar = b; }
	static void setScarSpeed(int speed) { Sspeed = speed; }
	static void setNcarSpeed(int speed) { Nspeed = speed; }

	// Getters
	int getHID() const { return HID; }
	int getCarID() const { return CarID; }
	CarType getType() const { return type; }
	float getSpeed() const { return speed; }
	CarStatus getStatus() const { return status; }
	Patient* getAssignedPatient() const { return assignedPatient; }
	int getCheckupTime()const { return checkupTime; }
	int getBusyTime() const { return busyTime; }

	static int getScarSpeed() { return Sspeed; }
	static int getNcarSpeed() { return Nspeed; }

	bool getFailedCar() const { return failedCar; }

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

	int calculatePatientFinishTime()
	{
		return assignedPatient->getPickupTime() + assignedPatient->getHospitalDistance() / speed;
	}

	int calculatePatientPickupTime()
	{
		return assignedPatient->getAssignmentTime() + assignedPatient->getHospitalDistance() / speed;
	}

	void updateBusyTime(int time)
	{
		busyTime += time;
	}
};

int Car::Sspeed = 0;
int Car::Nspeed = 0;