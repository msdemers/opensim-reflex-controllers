#ifndef OPENSIM_MuscleFiberStretchController_H_
#define OPENSIM_MuscleFiberStretchController_H_
/* -------------------------------------------------------------------------- *
 *                       OpenSim:  MuscleFiberStretchController.h             *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2012 Stanford University and the Authors                *
 * Author(s): Ajay Seth     *
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


//============================================================================
// INCLUDE
//============================================================================
#include <OpenSim/Simulation/Control/Controller.h>
#include "MusclePathStretchController.h"

// to export class as part of a plugin:
#include "osimReflexesDLL.h" 


namespace OpenSim { 

//=============================================================================
//=============================================================================
/**
 * MuscleFiberStretchController is a concrete controller that excites muscles in response
 * to muscle lengthening. Thus, it behaves like a simple stretch reflex. 
 * This controller responds to non-zero lengthening velocities and to stretch lengths 
 * beyond the specified normalized_rest_length.  Since this controller monitors the 
 * muscle fiber only, the provided rest length is interpretted as a ratio of 
 * desired fiber rest length to optimal fiber length.

 *
 * @author  Matt DeMers
 * @version 1.0
 */
	class OSIMREFLEXES_API MuscleFiberStretchController : public MusclePathStretchController {
OpenSim_DECLARE_CONCRETE_OBJECT(MuscleFiberStretchController, MusclePathStretchController);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
    /** @name Property declarations 
    These are the serializable properties associated with a MuscleFiberStretchController.*/
    /**@{**/  	
	/*
	OpenSim_DECLARE_PROPERTY(gain, double, 
		"Factor by which the stretch response is scaled." );
	*/

//=============================================================================
// METHODS
//=============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION AND DESTRUCTION
	//--------------------------------------------------------------------------
	/** Default constructor. */
	//MuscleFiberStretchController();
	// Uses default (compiler-generated) destructor, copy constructor and copy 
    // assignment operator.
	

	/** Compute the controls for actuators (muscles)
	 *  This method defines the behavior for MuscleFiberStretchController controller 
	 *
	 * @param s			system state 
	 * @param controls	writable model controls
	 */
	void computeControls(const SimTK::State& s, SimTK::Vector &controls) const OVERRIDE_11;


private:
	// Connect properties to local pointers.  */
	//void constructProperties();
	/*
	// ModelComponent interface to connect this component to its model
	void connectToModel(Model& aModel);
	*/

	//=============================================================================
};	// END of class MuscleFiberStretchController

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_MuscleFiberStretchController_H_


