#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include"RemovablePriQueue.h"

// Cancellation struct to store patient's id and time of cancellation

struct Cancellation {
	int PID;       // Patient ID
	int HID; // Hospital ID where patient cancelled 
	int cancelTime;      // Cancellation time

	// Constructor
	Cancellation(int id, int time, int HID)
		: PID(id), cancelTime(time), HID(HID) {}
};

class Organizer
{
private:

	// Lists
	LinkedQueue<Patient*> AllPatients;
	LinkedQueue<Cancellation*> AllCancellations;

	LinkedQueue<Patient*> FinishedPatients;

	RemovablePriQueue<Car*> OutCars; // TODO: Should be removable priority queue
	priQueue<Car*> BackCars;

	Hospital* Hospitals; // Pointer to a dynamic array of hospitals.
	int NumHospitals; //  Number of hospitals

	// Other members

	int NumOutCars, NumBackCars;
	int NumFinishedPatients;
	int TotalNumRequests;
	int NumCancellations;

	int TotalNumEP, TotalNumSP, TotalNumNP;

	int TotalNumSC, TotalNumNC;

	// Phase 2 data members
	int AvgWaitTime;
	int AvgBusyTime;
	int Utilization;

public:
	// Constructor
	Organizer() : NumOutCars(0), NumBackCars(0), NumFinishedPatients(0), TotalNumRequests(0), TotalNumEP(0), TotalNumSP(0), TotalNumNP(0), TotalNumNC(0), TotalNumSC(0) {}

	// Utility functions
	void AddPatient(PatientType type, int rT, int PID, int HID, int HD, int severity = -1)
	{
		// Creating the patient
		Patient* p = new Patient(type, rT, PID, HID, HD, severity);
		// Enqueuing patient
		AllPatients.enqueue(p);

		// Updating total number of patients in the simulation

		if (type == PatientType::Ep)
			TotalNumEP++;
		else if (type == PatientType::NP)
			TotalNumNP++;
		else if (type == PatientType::SP)
			TotalNumSP++;
	}

	void AddCancellation(int PID, int CancellationTime, int HID)
	{
		Cancellation* c = new Cancellation(PID, CancellationTime, HID);
		AllCancellations.enqueue(c);
	}

	// Omar: Function to create the hospitals given the num of hospials.

	// Setters
	void SetNumHospitals(int n) { NumHospitals = n; }
	void SetTotalNumRequests(int n) { TotalNumRequests = n; }
	void SetNumCancellations(int n) { NumCancellations = n; }

	// Getters
};
