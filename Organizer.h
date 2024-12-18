#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include"RemovablePriQueue.h"
#include <fstream>
#include"UI.h"
using namespace std;

// Cancellation struct to store patient's id and time of cancellation

struct Cancellation {
	int PID;       // Patient ID
	int HID; // Hospital ID where patient cancelled 
	int cancelTime;      // Cancellation time

	// Constructor
	Cancellation(int id, int time, int HID)
		: PID(id), cancelTime(time), HID(HID) {
	}
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
	int timestep;

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
	// returns hospital pointer given its ID
	Hospital* getHospital(int HID)
	{
		// Hospital is stored at HID - 1
		return Hospitals[HID - 1];
	}

	void AddCancellation(int PID, int CancellationTime, int HID)
	{
		Cancellation* c = new Cancellation(PID, CancellationTime, HID);
		AllCancellations.enqueue(c);
	}


	void AddPatientFinishedList(Patient* p)
	{
		FinishedPatients.enqueue(p);
		NumFinishedPatients++;
	}

	int getTimeStep ()const { return timestep; }
	int setTimeStep(int t) { timestep = t; }

	// Omar: Function to create the hospitals given the num of hospials.
	void CreateHospitals(int num)
	{
		Hospitals = new Hospital * [num];
		for (int i = 1; i <= num; i++)
			Hospitals[i - 1] = new Hospital(i);  //set id to i
	}

	void LoadFile(string fileName)
	{
		float ScarSpeed, NcarSpeed;
		int ScarNum, NcarNum, ReqNum, ReqTime,
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
			TotalNumSC += ScarNum;
			TotalNumNC += NcarNum;
			for (int j = 0; j < ScarNum; j++)
			{
				Car* C = new Car(j + 1, CarType::SC, ScarSpeed, i + 1);
				Hospitals[i]->AddSCar(C);
				
			}
			for (int j = 0; j < NcarNum; j++)
			{
				Car* C = new Car(j + 1 + ScarNum, CarType::NC, NcarSpeed, i + 1);
				Hospitals[i]->AddNCar(C);
			}
			Hospitals[i]->setTotalNumNcars(NcarNum);
			Hospitals[i]->setTotalNumScars(ScarNum);
			
		}

		file >> ReqNum;
		TotalNumRequests = ReqNum;
		for (int i = 0; i < ReqNum; i++)
		{
			file >> PT >> ReqTime >> PID >> HID >> PatientDistance;

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
				AddPatient(PTenum, ReqTime, PID, HID, PatientDistance, Severity);
			}
			else
				AddPatient(PTenum, ReqTime, PID, HID, PatientDistance);
		}

		file >> CancellationReqNum;
		NumCancellations = CancellationReqNum;
		for (int i = 0; i < CancellationReqNum; i++)
		{
			file >> CancelTime >> PID >> HID;

			AddCancellation(PID, CancelTime, HID);
		}

		file.close();
	}

	// Setters
	void SetNumHospitals(int n) { NumHospitals = n; }
	void SetTotalNumRequests(int n) { TotalNumRequests = n; }
	void SetNumCancellations(int n) { NumCancellations = n; }

	// Getters
	int GetTotalNumReq() { return TotalNumRequests; }
	int GetTotalNumCanellation() { return NumCancellations; }
	int GetTotalNumFinished() { return NumFinishedPatients; }
	int GetNumHospitals() { return NumHospitals; }

	RemovablePriQueue<Car*>& GetOutCars() { return OutCars; }
	priQueue<Car*>& GetBackCars() { return BackCars; }
	LinkedQueue<Patient*>& GetFinished() { return FinishedPatients; }

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

	// Function that takes time step and checks if there is a request.
	// If there is a request, enqueue the patient to its hospital.
	// Returns true if the time step matches the request time

	bool AllocatePatient(int time,Patient*& patient)
	{
		// If AllPatients list is empty return false.
		if (AllPatients.peek(patient) == false)
		{
			return false;
		}
		else
		{
			// If the "top" patient's requeset time is the ssame as this time step we move handle it.
			if (patient->getRequestTime()==time)
			{
				AllPatients.dequeue(patient);
				return true;
			}
			// Else all patient requests for this time step have been allocated
			return false;
		}
	}
	void HandleHospital(PatientType type) {

		if (type == PatientType::SP) {
			Patient* RemovedPatient; 
			for (int i = 0; i < NumHospitals; i++) {
				if (Hospitals[i]->RemoveSP(RemovedPatient))
				{
					AddPatientFinishedList(RemovedPatient); 
				}

			}
		}
		else if (type == PatientType::Ep) {
			Patient* RemovedPatient;
			int severity;
			for (int i = 0; i < NumHospitals; i++) {
				if (Hospitals[i]->RemoveEP(RemovedPatient, severity))
				{
					AddPatientFinishedList(RemovedPatient);
				}
			}
		}
		else if (type == PatientType::NP) {
			Patient* RemovedPatient;
			for (int i = 0; i < NumHospitals; i++) {
				if (Hospitals[i]->RemoveNP(RemovedPatient))
				{
					AddPatientFinishedList(RemovedPatient);
				}


			}
		}
	}

	// Handles the transferring of cars from hospital to out.
	void HandleCars(CarType type) {
		// Checks the type of the car 

		if (type == CarType::SC) {
			Car* movedcar;
			// For each hospital
			for (int i = 0; i < NumHospitals; i++) {

				// If I can remove a car i will do so and enqueue it.
				if(Hospitals[i]->RemoveSCar(movedcar))
				{
					int pri = 1;
					OutCars.enqueue(movedcar, pri);
					NumOutCars++;
				}

			}
		}
		else if (type == CarType::NC) {
			Car* movedcar;

			for (int i = 0; i < NumHospitals; i++) {

				if(Hospitals[i]->RemoveNCar(movedcar))
				{
					int pri = 1;
					OutCars.enqueue(movedcar, pri);
					NumOutCars++;
				}


			}
		}
	}
	// Handles moving cars from out to back
	void MoveOutToBack(int Timestep) {
		Car* tomove = nullptr;
		Patient* P = nullptr;
		int  pri;
		
		while (OutCars.peek(tomove, pri)) // pri = (-1) * picktime
		{
			if (pri * -1 != Timestep)
				break; //car shouldnt be returned to backlist yet
			
			P = tomove->getAssignedPatient();
			if (!P)
				break;
			P->setPickupTime(pri * -1);
			P->setFinishTime(P->getPickupTime() + P->getHospitalDistance() / tomove->getSpeed());
			tomove->updateBusyTime(P->getPickupTime() - P->getAssignmentTime());
			OutCars.dequeue(tomove, pri);
			int newPri = -1 * P->getFinishTime();
			tomove->setStatus(CarStatus::Loaded);
			BackCars.enqueue(tomove, newPri); //least finishtime should be first in queue
			NumBackCars++;
			NumOutCars--;
		}

		// Checks if outcars has a car
		/*if (OutCars.dequeue(tomove, pri))
		{
			BackCars.enqueue(tomove, pri);
			NumOutCars--;
			NumBackCars++;
		}*/
	}
	// Moves one car from back list to its hospital
	void MoveBackToFree(int Timestep) { 

		Car* tomove = nullptr;
		Patient* P = nullptr;
		int pri;

		// loop on all back cars with same timestep
		while (BackCars.peek(tomove, pri))
		{

			if ((pri * -1) != Timestep || (P = tomove->getAssignedPatient()))
				break;

			if (!BackCars.dequeue(tomove, pri))
				break;

			NumBackCars--;
			tomove->updateBusyTime(P->getFinishTime() - P->getPickupTime());
			P->setCarId(-1); // -1: is not assigned a car
			AddPatientFinishedList(P);
			tomove->setAssignedPatient(nullptr);
			tomove->setStatus(CarStatus::Ready);
			int hid = tomove->getHID();

			Hospital* H = Hospitals[hid - 1];
			CarType Ctype = tomove->getType();
			if (Ctype == CarType::NC)
			{
				H->AddNCar(tomove);
			}
			else if (Ctype == CarType::SC)
			{
				H->AddSCar(tomove);
			}
			H = nullptr;
			tomove = nullptr;
			P = nullptr;
		}
	}
	// Assigning a car, removing it from its hospital and puting it in OutCars
	bool MoveFreeToOut(int Timestep, int hid)
	{
		Patient* AssignedP = nullptr;
		Car* AssignedC = nullptr;
		int pri;

		if (hid < 1 || hid > NumHospitals)
			return false;

		Hospital* H = Hospitals[hid - 1];

		//First handle EP
		//

		//second handle SP
		//

		//Third handle NP
		if (H->AssignNP(Timestep))
		{
			H->RemoveNCar(AssignedC);  // if patient is assigned successfully then Ncar is assigned a patient
			AssignedP = AssignedC->getAssignedPatient();
			int Picktime = AssignedP->getAssignmentTime() + AssignedP->getHospitalDistance() / AssignedC->getSpeed();
			pri = -1 * Picktime; //least pick time has higher priority
			AssignedC->setStatus(CarStatus::Assigned);
			OutCars.enqueue(AssignedC, pri);
			NumOutCars++;
			return true;
		}
	}
	bool CancelRequest(int Timestep)
	{
		Cancellation* CancelReq = nullptr;
		if (!AllCancellations.dequeue(CancelReq))
		{
			return false;
		}
		Car* Car = nullptr;
		Patient* P = nullptr;
		int pri;

		int hid = CancelReq->HID;
		if (hid < 1 || hid > NumHospitals) { //check validity of hid
			delete CancelReq;
			return false;  
		}
		Hospital* H = Hospitals[hid - 1];
		H->GetNPlist()->removeItem(P, CancelReq->PID);

		if (!P)
			return false;

		if (Timestep > P->getAssignmentTime() + P->getHospitalDistance() / H->getNCarspeed())
			return false; //patient cannot cancel a request while in a car ***waiting P only***

		int Carid = P->getCarId();

		if (Carid == -1) { // no car assigned
			delete CancelReq; CancelReq = nullptr;
			delete P; P = nullptr;
			return true; // No car to handle
		}

		//getting car that was assigned this patient
		if (OutCars.removeItem(Car, pri, Carid))
		{
			Car->setAssignedPatient(nullptr);
			Car->updateBusyTime((Timestep - P->getAssignmentTime()) * 2);
			P->setCarId(-1); //car removed from patient
			BackCars.enqueue(Car, pri);
			NumOutCars--;
			NumBackCars++;
		}
		CancelReq = nullptr;
		P = nullptr;
		return true;
	}
	void CallUI(int timestep) {
		UI call;
		call.PrintOutput(timestep, Hospitals, &OutCars, &BackCars, &FinishedPatients, NumFinishedPatients, NumOutCars, NumBackCars, NumHospitals);
	}

};
