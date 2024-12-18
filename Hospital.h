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

	// Lists
	LinkedQueue<Patient*> SPlist;
	RemovableQueue<Patient*> NPlist;  // NP's have the ability to cancel.
	priQueue<Patient*> EPlist;       // Sorting emergency patients using priQueue

	LinkedQueue<Patient*> NpWaitList;
	LinkedQueue<Car*> Scars;
	LinkedQueue<Car*> Ncars;

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

	//initial thought for assign normal patient..  TOREVISE
	bool RemovefromNpWait(Patient* P)
	{
		if (NpWaitList.isEmpty())
		{
			P = nullptr;
			return false;
		}
		NumNPRequests--;
		return NpWaitList.dequeue(P);
	}
	void AddToNpWait(Patient* P)
	{
		NpWaitList.enqueue(P);
	}
	bool AssignNP(int timestep)
	{
		Patient* P = nullptr;
		Car* C = nullptr;

		// Check if there are available Ncars
		if (AvailableNumNCars > 0)
		{
			// Try to get a patient from the waitlist first
			if (!RemovefromNpWait(P))
			{
				// If waitlist is empty remove from nplist
				if (!RemoveNP(P))
				{
					return false;
				}
			}

			//Peak the first available Ncar   -- Organizer is responsible for removing the car...
			if (Ncars.peek(C) && P != nullptr)
			{
				C->setAssignedPatient(P);
				P->setCarId(C->getCarID());
				P->setAssignmentTime(timestep);
				return true;
			}
		}
		else
		{
			// If no cars are available add the patient to waitlist
			if (RemoveNP(P))
			{
				AddToNpWait(P);
				return false;  
			}
		}

		return false; 
	}
	// Operator overloading

	// overloading to allow the comparison of Hospitals with int ID. To see if the Hospital object has that ID.
	bool operator==(int ID) const {
		return HID == ID;
	}
};
