/* --------------------------------------------------------------------------*
*                      OpenSim:  DelayedPathReflexController.cpp                     *
* -------------------------------------------------------------------------- *
* The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
* See http://opensim.stanford.edu and the NOTICE file for more information.  *
* OpenSim is developed at Stanford University and supported by the US        *
* National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
* through the Warrior Web program.                                           *
*                                                                            *
* Copyright (c) 2005-2013 Stanford University and the Authors                *
* Author(s): Ajay Seth                                                       *
*                                                                            *
* Licensed under the Apache License, Version 2.0 (the "License"); you may    *
* not use this file except in compliance with the License. You may obtain a  *
* copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
*                                                                            *
* Unless required by applicable law or agreed to in writing, software        *
* distributed under the License is distributed on an "AS IS" BASIS,          *
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied    *
* See the License for the specific language governing permissions and        *
* limitations under the License.                                             *
* -------------------------------------------------------------------------- */



//=============================================================================
// INCLUDES
//=============================================================================
#include "DelayedPathReflexController.h"
#include <OpenSim/Simulation/Model/Muscle.h> 

// This allows us to use OpenSim functions, classes, etc., without having to
// prefix the names of those things with "OpenSim::".
using namespace OpenSim;
using namespace std;
using namespace SimTK;


//=============================================================================
// CONSTRUCTOR(S) AND DESTRUCTOR
//=============================================================================
//_____________________________________________________________________________
/* Default constructor. */
DelayedPathReflexController::DelayedPathReflexController()
{
	constructProperties();
}

/* Convenience constructor. */
DelayedPathReflexController::DelayedPathReflexController(double gain, double delay)
{
	constructProperties();
	set_gain(gain);
	set_delay(delay);
}


/*
* Construct Properties
*/
void DelayedPathReflexController::constructProperties()
{
	constructProperty_gain(1.0);
	constructProperty_delay(0.0);
}

void DelayedPathReflexController::addToSystem(SimTK::MultibodySystem& system) const
{
	Super::addToSystem(system);
	DelayedPathReflexController* mutableThis = const_cast<DelayedPathReflexController *>(this);

	// Create underlying simtk measures to compute muscle speeds and the
	// delayed signal of these speeds.
	// Since they are used to compute controls, they invalidate the dynamics stage.
	// These speeds require a state realized to Velocity stage and must be computed after Velocities in 
	// the MatterSubsystem.  Therefore, we will add the muscleSpeed measure to the force subsytem.
	// TO DO:  find a better way to order computations appropriately without adding a measure to arbitrary, 
	// later sybsystems.
	
	
	
	for (int i = 0; i < getActuatorSet().getSize(); ++i)
	{
		Measure_<double>::Result muscleSpeed(system.updDefaultSubsystem(), Stage::Velocity, Stage::Acceleration);
		//Measure_<double>::Result muscleSpeed2(system.updDefaultSubsystem(),
		//	Stage::Velocity, Stage::Acceleration);
		mutableThis->_indices_muscle_speeds_measures.push_back(muscleSpeed.getSubsystemMeasureIndex());

		// Create underlying simtk measure to compute delayed muscle lengths
		Measure_<double>::Delay delayedSpeed(system.updDefaultSubsystem());
		delayedSpeed.setSourceMeasure(muscleSpeed);
		delayedSpeed.setDelay(get_delay());

		mutableThis->_indices_delayed_speeds_measures.push_back(delayedSpeed.getSubsystemMeasureIndex());

	}

}
void DelayedPathReflexController::connectToModel(Model &model)
{
	Super::connectToModel(model);

	// get the list of actuators assigned to the reflex controller
	Set<Actuator>& actuators = updActuators();
	muscleStretchVelocityHistory.setSize(0);
	muscleStretchVelocityHistory.setMemoryOwner(true);

	int cnt = 0;

	while (cnt < actuators.getSize()){
		Muscle *musc = dynamic_cast<Muscle*>(&actuators[cnt]);
		// control muscles only
		if (!musc){
			cout << "DelayedPathReflexController:: WARNING- controller assigned a non-muscle actuator ";
			cout << actuators[cnt].getName() << " which will be ignored." << endl;
			actuators.remove(cnt);
		}
		else
		{
			PiecewiseLinearFunction muscleVelocity;
			muscleVelocity.setName(musc->getName());
			muscleStretchVelocityHistory.cloneAndAppend(muscleVelocity);
			cnt++;
		}
	}


}

//=============================================================================
// COMPUTATIONS
//=============================================================================
//_____________________________________________________________________________
/**
* Compute the controls for muscles under influence of this reflex controller
*
* @param s			current state of the system
* @param controls	system wide controls to which this controller can add
*/

void DelayedPathReflexController::realizeVelocity(const SimTK::State& state) const
{
	DelayedPathReflexController* mutableThis = const_cast<DelayedPathReflexController *>(this);

	// get time
	double time = state.getTime();

	// get the list of actuators assigned to the reflex controller
	const Set<Actuator>& actuators = getActuatorSet();

	// First must determine each muscles lengthening speed and store it in the measure's result
	//-----------------------------------------------------------------------------------------
	// muscle lengthening speed
	double speed = 0;
	// muscle length
	double length = 0;
	// max muscle lengthening (stretch) speed
	double max_speed = 0;
	// normalized and positive (lengthening) path speed
	double normalized_speed;
	//reflex control
	double control = 0;

	//SimTK::Vector normalized_speeds(actuators.getSize());
	
	for (int i = 0; i < actuators.getSize(); ++i){
		const Muscle *musc = dynamic_cast<const Muscle*>(&actuators[i]);
		//speed = musc->getLengtheningSpeed(state);
		length = musc->getLength(state);

		//// unnormalize muscle's maximum contraction velocity (fib_lengths/sec) 
		max_speed = musc->getOptimalFiberLength()*musc->getMaxContractionVelocity();
		//// only positive (lengthening) velocity produces a stretch signal
		normalized_speed = 0.5*(fabs(speed) + speed) / max_speed;
		//muscleStretchVelocityHistory.get(musc->getName()).addPoint(time, normalized_speed);
		// assume that the number and order of muscles does not change
		// store the muscle lengthening speeds in a vecotor
		// Prepare to access and modify the muscle measures in the simtk system
		Measure_<double>::Result muscleSpeed = Measure_<double>::Result::getAs(getModel().getMultibodySystem().getDefaultSubsystem().getMeasure(_indices_muscle_speeds_measures[i]));
		//Measure_<double>::Result muscleSpeed = Measure_<double>::Result::getAs(updSystem().updDefaultSubsystem().getMeasure(_indices_muscle_speeds_measures[i]));
		//muscleSpeed.setValue(state,normalized_speed);
		muscleSpeed.setValue(state, length);

	}

}
//_____________________________________________________________________________
/**
* Compute the controls for muscles under influence of this reflex controller
*
* @param s			current state of the system
* @param controls	system wide controls to which this controller can add
*/
void DelayedPathReflexController::computeControls(const State& s, Vector &controls) const
{
	//this->realizeDynamics(s);
	// get time
	double time = s.getTime();

	// get the list of actuators assigned to the reflex controller
	const Set<Actuator>& actuators = getActuatorSet();

	/**/
	// First must determine each muscles lengthening speed and store it in the measure's result
	//-----------------------------------------------------------------------------------------
	// muscle lengthening speed
	double speed = 0;
	// max muscle lengthening (stretch) speed
	double max_speed = 0;
	// normalized and positive (lengthening) path speed
	double normalized_speed;
	//reflex control
	double control = 0;

	SimTK::Vector normalized_speeds(actuators.getSize()), delayed_speeds(actuators.getSize());
	
	for (int i = 0; i < actuators.getSize(); ++i){
		const Muscle *musc = dynamic_cast<const Muscle*>(&actuators[i]);
		speed = musc->getLengtheningSpeed(s);
		// unnormalize muscle's maximum contraction velocity (fib_lengths/sec) 
		max_speed = musc->getOptimalFiberLength()*musc->getMaxContractionVelocity();
		// only positive (lengthening) velocity produces a stretch signal
		normalized_speed = 0.5*(fabs(speed) + speed) / max_speed;
		muscleStretchVelocityHistory.get(musc->getName()).addPoint(time, normalized_speed);
		// assume that the number and order of muscles does not change
		
		control = get_gain()*muscleStretchVelocityHistory.get(musc->getName()).calcValue(SimTK::Vector(1, time - get_delay()));

		SimTK::Vector actControls(1, control);
		// add reflex controls to whatever controls are already in place.
		musc->addInControls(actControls, controls);
	}

	/*
	//  Retrieve all the delayed, scaleed muscle speeds from the measure
	//  Assign the delayed, scaled muscle speeds as the control values
	double control;
	double speed;
	for (int i = 0; i < actuators.getSize(); ++i)
	{
		const Muscle *musc = dynamic_cast<const Muscle*>(&actuators[i]);

		// Prepare to access and modify the muscle measures in the simtk system
		//Measure_<double>::Result muscleSpeed = Measure_<double>::Result::getAs(updSystem().updDefaultSubsystem().getMeasure(_indices_muscle_speeds_measures[i]));
		Measure_<double>::Delay delayedSpeed = Measure_<double>::Delay::getAs(updSystem().updDefaultSubsystem().getMeasure(_indices_delayed_speeds_measures[i]));

		//control = get_gain()*muscleStretchVelocityHistory.get(musc->getName()).calcValue(SimTK::Vector(1,time - get_delay()));
		//speed = muscleSpeed.getValue(s);
		control = get_gain()*delayedSpeed.getValue(s);

		SimTK::Vector actControls(1, control);
		// add reflex controls to whatever controls are already in place.
		musc->addInControls(actControls, controls);
	}
	*/
}

