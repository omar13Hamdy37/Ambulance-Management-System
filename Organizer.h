#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include"RemovablePriQueue.h"
#include <fstream>
using namespace std;

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


	int NumHospitals; //  Number of hospitals

	// Other members

	Hospital** Hospitals; // Pointer to a dynamic array of hospitals.
	int** DistanceMatrix;
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

	void AddPatientFinishList(Patient* p)
	{
		FinishedPatients.enqueue(p);
		NumFinishedPatients++;
	}

	// Omar: Function to create the hospitals given the num of hospials.
	void CreateHospitals(int num)
	{
		Hospitals = new Hospital*[num];
		for (int i = 1; i <= num; i++)
			Hospitals[i - 1] = new Hospital(i);  //set id to i
	}

	void LoadFile(string fileName)
	{
		int ScarSpeed, NcarSpeed, ScarNum, NcarNum, ReqNum, ReqTime,
			PID, HID, PatientDistance, CancellationReqNum, CancelTime;
		string PT;
		PatientType PTenum;

		ifstream file(fileName);

		if (!file.is_open()) {
			cout << "Error opening file: " << fileName << endl;
			return;
		}


		file >> NumHospitals >> ScarSpeed >> NcarSpeed;

		CreateHospitals(NumHospitals);

		DistanceMatrix = new int* [NumHospitals];
		for (int i = 0; i < NumHospitals; i++)
			DistanceMatrix[i] = new int[NumHospitals];

		for (int i = 0; i < NumHospitals; i++)
		{
			for (int j = 0; j < NumHospitals; j++)
				file >> DistanceMatrix[i][j];
		}
		
		for (int i = 0; i < NumHospitals; i++)
		{
			file >> ScarNum >> NcarNum;
			for (int j = 0; j < ScarNum; j++)
			{
				Car *C = new Car(j + 1, CarType::SC, ScarSpeed, i + 1);
				Hospitals[i]->AddSCar(C);
			}
			for (int j = 0; j < NcarNum; j++)
			{
				Car* C = new Car(j + 1 + ScarNum, CarType::NC, NcarSpeed, i + 1);
				Hospitals[i]->AddNCar(C);
			}
		}

		file >> ReqNum;
		for (int i = 0; i < ReqNum; i++)
		{
			file >> PT >> ReqTime >> PID >> HID >> PatientDistance;

			Patient* P;

			if (PT == "NP")
				PTenum = PatientType::NP;
			else if (PT == "SP")
				PTenum = PatientType::SP;
			else if (PT == "EP")
				PTenum = PatientType::Ep;
			else
				cout << "invalid patient type";

			if (PTenum == PatientType::Ep)
			{
				int Severity;
				file >> Severity;
				P = new Patient(PTenum, ReqTime, PID, HID, PatientDistance, Severity);
			}
			else
				P = new Patient(PTenum, ReqTime, PID, HID, PatientDistance);

			AllPatients.enqueue(P);
		}

		file >> CancellationReqNum;

		for (int i = 0; i < CancellationReqNum; i++)
		{
			file >> CancelTime >> PID >> HID;

			Cancellation* C = new Cancellation(PID, CancelTime, HID);
			AllCancellations.enqueue(C);
		}
		
		file.close();
	}

	// Setters
	void SetNumHospitals(int n) { NumHospitals = n; }
	void SetTotalNumRequests(int n) { TotalNumRequests = n; }
	void SetNumCancellations(int n) { NumCancellations = n; }

	// Getters

	//Destructor
	~Organizer()
	{
		for (int i = 0; i < NumHospitals; i++) {
			// Delete dynamically allocated memory in DistanceMatrix
			delete[] DistanceMatrix[i];
		}
		delete[] DistanceMatrix;

		Patient* p;
		while (!AllPatients.isEmpty()) {
			AllPatients.dequeue(p);  // Remove patient from queue
			delete p;  // Free memory
		}

		Cancellation* c;
		while (!AllCancellations.isEmpty()) {
			AllCancellations.dequeue(c);  // Remove cancellation from queue
			delete c;  // Free memory
		}

	}
};
