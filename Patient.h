#pragma once
//#include "Car.h"

enum class PatientType {
	NP,
	SP,
	Ep
};

class Patient
{
private:
	int PID; // Patient ID
	int requestTime; // Time patient requested service
	int pickupTime; // Time patient is picked up by car
	int waitingTime; // Calculated from pickupTime - requestTime

	int finishTime; // (might not be needed) Time when patient has reached the hospital
	int assignmentTime; // Time a car is assigned to a patient

	int HID; // ID of nearest hospital to patient
	float hospitalDistance; // Distance between patient and nearest hospital

	//Car* AssignedCar;
	int CarID; //id of assigned car.  -1 if not assigned aa c
	PatientType type; // NP or SP or EP

	int severity; // -1 if not an EP

public:
	// Constructor
	Patient(PatientType type, int rT, int PID, int HID, int HD, int severity = -1) : PID(PID), requestTime(rT), HID(HID), hospitalDistance(HD), type(type), severity(severity), CarID(-1)
	{
	}

	// Setters
	void setPickupTime(int time)
	{
		pickupTime = time;
		calculateWaitingTime();
	}
	void setAssignmentTime(int time) { assignmentTime = time; }
	void setFinishTime(int time) { finishTime = time; }
	void setWaitingTime(int time) { waitingTime = time; }
	void setHID(int id) { HID = id; }
	void setHospitalDistance(float distance) { hospitalDistance = distance; }
	void setCarId(int id) { CarID = id; }
	// Utility functions

	void calculateWaitingTime() { waitingTime = pickupTime - requestTime; }

	// Getters
	int getPID() const { return PID; }
	int getRequestTime() const { return requestTime; }
	int getPickupTime() const { return pickupTime; }
	int getWaitingTime() const { return waitingTime; }
	int getFinishTime() const { return finishTime; }
	int getAssignmentTime() const { return assignmentTime; }
	int getHID() const { return HID; }
	int getSeverity() const { return severity; }
	float getHospitalDistance() const { return hospitalDistance; }
	PatientType getType() const { return type; }
	int getCarId() const { return CarID; }

	// Operator overloading
	//
	// overloading to allow the comparison of Patients with int ID. To see if the patient object has that ID.
	bool operator==(int ID) const {
		return PID == ID;
	}
	//operator overloading to cout patient ID
	friend std::ostream& operator<<(std::ostream& output, const Patient& p) {
		output << p.getPID();
		return output;
	}
};
