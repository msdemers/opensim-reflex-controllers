/* -------------------------------------------------------------------------- *
 *                      OpenSim:  MusclePathStretchController.cpp                             *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2012 Stanford University and the Authors                *
 * Author(s): Matt DeMers     *
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

// This line includes a large number of OpenSim functions and classes so that
// those things will be available to this program.
#include <OpenSim/OpenSim.h>
#include "MusclePathStretchController.h"

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

MusclePathStretchController::MusclePathStretchController()
{
	constructProperties();
}

/* Convenience constructor. */
MusclePathStretchController::MusclePathStretchController(double rest_length, double gain_l, double gain_v)
{
	constructProperties();
	set_normalized_rest_length(rest_length);
	set_gain_length(gain_l);
	set_gain_velocity(gain_v);
}


//=============================================================================
// SETUP PROPERTIES
//=============================================================================
//
/* Connect properties to local pointers.
 *
 * Properties should be given a meaningful name and an informative comment.
 * The name you give each property is the tag that will be used in the XML
 * file. The comment will appear before the property in the XML file.
 * In addition, the comments are used for tool tips in the OpenSim GUI.
 *
 * All properties are added to the property set. Once added, they can be
 * read in and written to files.
 ____________________________________________________________________________


 /**
 * Construct Properties
 */
void MusclePathStretchController::constructProperties()
{
	constructProperty_gain_length(1.0);
	constructProperty_gain_velocity(1.0);
	constructProperty_normalized_rest_length(1.0);
}

void MusclePathStretchController::connectToModel(Model &model)
{
	Super::connectToModel(model);

	// get the list of actuators assigned to the reflex controller
	Set<Actuator>& actuators = updActuators();

	int cnt=0;
 
	while(cnt < actuators.getSize()){
		Muscle *musc = dynamic_cast<Muscle*>(&actuators[cnt]);
		// control muscles only
		if(!musc){
			cout << "WARNING - controller named " << this->getName() << " assigned a non-muscle actuator";
			cout << actuators[cnt].getName() << " which will be ignored." << endl;
			actuators.remove(cnt);
		}else
			cnt++;
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
void MusclePathStretchController::computeControls(const State& s, Vector &controls) const
{	
	// get time
	double time = s.getTime();

	// get the list of actuators assigned to the reflex controller
	const Set<Actuator>& actuators = getActuatorSet();

	// save resused controller parameter
	double rest_length = get_normalized_rest_length();
	double k_l = get_gain_length();
	double k_v = get_gain_velocity();

	// muscle optimal fiber length
	double f_o = 1;
	// muscle length
	double length = 0;
	// muscle stretch
	double stretch;
	// muscle lengthening speed
	double speed = 0;
	// max muscle lengthening (stretch) speed
	double max_speed = 0;
	//reflex control
	double control = 0;

	for(int i=0; i<actuators.getSize(); ++i){
		const Muscle *musc = dynamic_cast<const Muscle*>(&actuators[i]);
		f_o = musc->getOptimalFiberLength();
		length = musc->getLength(s);
		// compute stretch beyond desired muscle-tendon length
		stretch = length - rest_length*(f_o + musc->getTendonSlackLength());
		// only positive stretch, normalized by optimal fiber length is used
		control = k_l * 0.5*(fabs(stretch) + stretch) / f_o;
		speed = musc->getLengtheningSpeed(s);
		// unnormalize muscle's maximum contraction velocity (fib_lengths/sec) 
		max_speed = f_o*musc->getMaxContractionVelocity();
		control += 0.5*k_v*(fabs(speed)+speed)/max_speed;

		SimTK::Vector actControls(1,control);
		// add reflex controls to whatever controls are already in place.
		musc->addInControls(actControls, controls);
	}
}

