#pragma once
#include "Patient.h"
#include "Car.h"
#include "priQueue.h"
#include "LinkedQueue.h"
#include "RemovableQueue.h"
#include "RemovablePriQueue.h"

class Hospital
{
private:
	// Normal data members
	int HID;

	int TotalNumScars, TotalNumNcars;

	int AvailableNumScars, AvailableNumNCars;

	int NumSPRequests, NumNPRequests, NumEPRequests;

	bool HasUnassignedEP;

	// Lists
	LinkedQueue<Patient*> SPlist;
	RemovableQueue<Patient*> NPlist;  // NP's have the ability to cancel.
	priQueue<Patient*> EPlist;       // Sorting emergency patients using priQueue

	//LinkedQueue<Patient*> NpWaitList;
	LinkedQueue<Car*> Scars;
	LinkedQueue<Car*> Ncars;
	LinkedQueue<Car*> AssignedCars;

public:
	// Constructor

	Hospital(int HID) : HID(HID)
	{
		NumSPRequests = 0; NumEPRequests = 0; NumNPRequests = 0;
		AvailableNumScars = 0; AvailableNumNCars = 0;
	}
	// Add Cars
	void AddSCar(Car* car)
	{
		Scars.enqueue(car);
		AvailableNumScars++;
	}
	void AddNCar(Car* car)
	{
		Ncars.enqueue(car);
		AvailableNumNCars++;
	}
	// Remove Cars
	bool RemoveSCar(Car*& car)
	{
		if (Scars.isEmpty())
		{
			return false;
		}
		AvailableNumScars--;
		return Scars.dequeue(car);
	}

	bool RemoveNCar(Car*& car)
	{
		if (Ncars.isEmpty())
		{
			return false;
		}
		AvailableNumNCars--;
		return Ncars.dequeue(car);
	}

	// Add Request

	void AddSP(Patient* p)
	{
		SPlist.enqueue(p);
		NumSPRequests++;
	}
	void AddNP(Patient* p)
	{
		NPlist.enqueue(p);
		NumNPRequests++;
	}

	void AddEP(Patient* p, int severity)
	{
		EPlist.enqueue(p, severity);
		NumEPRequests++;
	}

	// Remove Request

	bool RemoveSP(Patient*& p)
	{
		if (SPlist.isEmpty())
		{
			p = nullptr;
			return false;
		}
		NumSPRequests--;
		return SPlist.dequeue(p);
	}

	bool RemoveNP(Patient*& p)
	{
		if (NPlist.isEmpty())
		{
			p = nullptr;
			return false;
		}
		NumNPRequests--;
		return NPlist.dequeue(p);
	}

	bool RemoveEP(Patient*& p, int& severity)
	{
		if (EPlist.isEmpty())
		{
			p = nullptr;
			return false;
		}
		NumEPRequests--;
		return EPlist.dequeue(p, severity);
	}

	// Setters
	void setTotalNumScars(int totalScars) {
		TotalNumScars = totalScars;
		AvailableNumScars = totalScars;
	}

	void setTotalNumNcars(int totalNcars) {
		TotalNumNcars = totalNcars;
		AvailableNumNCars = totalNcars;
	}

	void setAvailableNumNcars(int n) {
		AvailableNumNCars = n;
	}
	void setAvailableNumScars(int n) {
		AvailableNumScars = n;
	}
	void setHasUnassignedEP(bool b)
	{
		HasUnassignedEP = b;
	}

	// Getters
	int getTotalNumScars() const {
		return TotalNumScars;
	}

	int getTotalNumNcars() const {
		return TotalNumNcars;
	}

	int getAvailableNumScars() const {
		return AvailableNumScars;
	}

	int getAvailableNumNCars() const {
		return AvailableNumNCars;
	}

	int getNumSPRequests() const {
		return NumSPRequests;
	}

	int getNumEPRequests() const {
		return NumEPRequests;
	}

	int getNumNPRequests() const {
		return NumNPRequests;
	}

	bool getHasUnassignedEP() const {
		return HasUnassignedEP;
	}
	bool getEPlistEmpty()const {
		return EPlist.isEmpty();
	}

	priQueue<Patient*>* GetEPlist() {
		return &EPlist;
	}
	RemovableQueue<Patient*>* GetNPlist() {
		return &NPlist;
	}
	LinkedQueue<Patient*>* GetSPlist() {
		return &SPlist;
	}
	LinkedQueue<Car*>* GetSCar() {
		return &Scars;
	}
	LinkedQueue<Car*>* GetNCar() {
		return &Ncars;
	}

	int getNCarspeed() {
		Car* C = nullptr;
		Ncars.peek(C);
		return C->getSpeed();
	}
	int getSCarspeed() {
		Car* C = nullptr;
		Scars.peek(C);
		return C->getSpeed();
	}

	////initial thought for assign normal patient..  TOREVISE
	//bool RemovefromNpWait(Patient* P)
	//{
	//	if (NpWaitList.isEmpty())
	//	{
	//		P = nullptr;
	//		return false;
	//	}
	//	NumNPRequests--;
	//	return NpWaitList.dequeue(P);
	//}
	//void AddToNpWait(Patient* P)
	//{
	//	NpWaitList.enqueue(P);
	//}
	//bool AssignNP(int timestep)
	//{
	//	Patient* P = nullptr;
	//	Car* C = nullptr;

	//	// Check if there are available Ncars
	//	if (AvailableNumNCars > 0)
	//	{
	//		// Try to get a patient from the waitlist first
	//		if (!RemovefromNpWait(P))
	//		{
	//			// If waitlist is empty remove from nplist
	//			if (!RemoveNP(P))
	//			{
	//				return false;
	//			}
	//		}

	//		//Peak the first available Ncar   -- Organizer is responsible for removing the car...
	//		if (Ncars.peek(C) && P != nullptr)
	//		{
	//			C->setAssignedPatient(P);
	//			P->setCarId(C->getCarID());
	//			P->setAssignmentTime(timestep);
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		// If no cars are available add the patient to waitlist
	//		if (RemoveNP(P))
	//		{
	//			AddToNpWait(P);
	//			return false;  
	//		}
	//	}

	//	return false; 
	//}
	// Operator overloading

	// overloading to allow the comparison of Hospitals with int ID. To see if the Hospital object has that ID.
	bool operator==(int ID) const {
		return HID == ID;
	}
	// Check if hospital has available cars
	bool CarAvailable()
	{
		return !(Ncars.isEmpty() && Scars.isEmpty());
	}
	void AddAssignedCar(Car* C)
	{
		AssignedCars.enqueue(C);
	}
	bool getAssignedCar(Car* C)
	{
		if (AssignedCars.isEmpty())
			return false;

		AssignedCars.dequeue(C);
		return true;
	}
	// A function that handles all current requests for the hospital
	void HandlePatients()
	{
		// The patient that will be removed from requests list
		Patient* p;
		int severity;
		// The car of the patient
		Car* c;

		// Patient-Car assignment will stop, if no more requests present or no cars are available
		while (!EPlist.isEmpty() && !(Scars.isEmpty() && Ncars.isEmpty()))
		{
			// We have the patient
			RemoveEP(p, severity);
			// We have the car. (if both empty we wouldn't enter while loop)
			if (!Ncars.isEmpty())
			{
				RemoveNCar(c);
			}
			else
			{
				RemoveSCar(c);
			}
			//TODO: USE OMAR'S FUNCTION THAT HANDLES PICKING PATIENTS.
			c->setAssignedPatient(p);
			AddAssignedCar(c);
		}

		// Here we decide if the Hospital has unassigned EP (to be reassigned later)

		if (EPlist.isEmpty())
		{
			HasUnassignedEP = false;
		}
		else
		{
			HasUnassignedEP = true;
		}

		// Patient-Car assignment will stop, if no more requests present or no cars are available
		while (!SPlist.isEmpty() && !Scars.isEmpty())
		{
			// We have the patient
			RemoveSP(p);
			// We have the car. (if empty we wouldn't enter while loop)
			RemoveSCar(c);
			//TODO: USE OMAR'S FUNCTION THAT HANDLES PICKING PATIENTS.
			c->setAssignedPatient(p);
			AddAssignedCar(c);
		}

		// Patient-Car assignment will stop, if no more requests present or no cars are available
		while (!NPlist.isEmpty() && !Ncars.isEmpty())
		{
			// We have the patient
			RemoveNP(p);
			// We have the car. (if empty we wouldn't enter while loop)
			RemoveNCar(c);
			//TODO: USE OMAR'S FUNCTION THAT HANDLES PICKING PATIENTS.
			c->setAssignedPatient(p);
			AddAssignedCar(c);
		}
	}

};
