#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Patient.h"
#include "Car.h"


struct Cancellation {
    int PID;       // Patient ID
    int cancelTime;      // Cancellation time 

    // Constructor 
    Cancellation(int id, int time)
        : PID(id), cancelTime(time) {}
};

class Organizer
{
private:
	LinkedQueue<Patient*> AllPatients;
    LinkedQueue<Cancellation> AllCancellations;





};
