#pragma once
#include "Patient.h"

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
    CarType type;
    float speed;
    CarStatus status;
    Patient* assignedPatient;

   

    

    int busyTime; // Total time car was in use.

public:
    // Constructor
    Car(CarType t, float sp,int busy = 0)
        : type(t), speed(sp), status(CarStatus::Ready), assignedPatient(nullptr), busyTime(0) {}


    // Setters

    void setStatus(CarStatus st) { status = st; }
    void setAssignedPatient(Patient* patient) { assignedPatient = patient; status = CarStatus::Assigned; }
    void PickPatient() { status = CarStatus::Loaded; }



    // Getters
    CarType getType() const { return type; }
    float getSpeed() const { return speed; }
    CarStatus getStatus() const { return status; }
    Patient* getAssignedPatient() const { return assignedPatient; }
    int getBusyTime() const { return busyTime; }

    // Utility functions

    // Phase 2 code
    // ok bos awalan ama patient yeba assigned yeba sa3tha assignment time bt3 patient updated
    // patient picked up.. update pickup time of patient -- update busy time bt3 el ragel -- change status
    //  patient dropped up - update finish time -- shoof change status
    // shoof hat calc busy time ezay law had canclled 
    //
    //int calculatePatientFinishTime()
    //{
    //    return assignedPatient->getPickupTime() + assignedPatient->getHospitalDistance() / speed;
    //}
    //
    //int calculatePatientPickupTime()
    //{
    //    return assignedPatient->getAssignmentTime() + assignedPatient->getHospitalDistance() / speed;
    //}
    //
    //void updateBusyTime()
    //{
    //    
    //    busyTime +=  - assignedPatient->getAssignmentTime();
    //}



};