#include "TrafficLight.h"


TrafficLight::TrafficLight(void)
{
}


TrafficLight::~TrafficLight(void)
{
}

void TrafficLight::setMaterials()
{

	// In this function, you are supposed to assign values to the variables redOn, redOff,
	// yellowOn, yellowOff, greenOn, greenOff.
	
	redOn = materials["_Red_"];
	redOff = redOn;
	redOff.Kd[0] = 0.3;
	redOff.Kd[1] = 0.0;
	redOff.Kd[2] = 0.0;



	yellowOn = materials["_Yellow_"];
	yellowOff = yellowOn;
	yellowOff.Kd[0] = 0.3;
	yellowOff.Kd[1] = 0.3;
	yellowOff.Kd[2] = 0.0;



	greenOn = materials["_Green_"];
	greenOff = greenOn;
	greenOff.Kd[0] = 0.0;
	greenOff.Kd[1] = 0.3;
	greenOff.Kd[2] = 0.0;
}

void TrafficLight::setSignal(Signal signal)
{
	// You are supposed to assign the materials used in the ObjModel class based on
	// values of the input signal.

	if (signal == Green)
	{
		materials["_Green_"] = greenOn;;
		materials["_Red_"] = redOff;
		materials["_Yellow_"] = yellowOff;

	}
	else if (signal == Red)
	{
		materials["_Red_"] = redOn;
		materials["_Green_"] = greenOff;
		materials["_Yellow_"] = yellowOff;
		

	}
	else if (signal == Yellow)
	{
		materials["_Yellow_"] = yellowOn;
		materials["_Green_"] = greenOff;
		materials["_Red_"] = redOff;
		

	}
}

void TrafficLight::ReadFile(string fileName)
{
	ObjModel::ReadFile(fileName);
	setMaterials();
}