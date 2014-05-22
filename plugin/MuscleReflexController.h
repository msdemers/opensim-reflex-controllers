#ifndef OPENSIM_MuscleReflexController_H_
#define OPENSIM_MuscleReflexController_H_
/* -------------------------------------------------------------------------- *
 *                       OpenSim:  MuscleReflexController.h                             *
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

// to export class as part of a plugin:
#include "osimPluginDLL.h" 


namespace OpenSim { 

//=============================================================================
//=============================================================================
/**
 * MuscleReflexController is a concrete controller that excites muscles in response
 * to muscle lengthening to simulate a stretch reflex. This controller is 
 * meant to serve as an example how to implement a controller as a plugin in
 * OpenSim. It is intended for demonstrative purposes only. 

 *
 * @author  Ajay Seth
 * @version 1.0
 */
class OSIMPLUGIN_API MuscleReflexController : public Controller {
OpenSim_DECLARE_CONCRETE_OBJECT(MuscleReflexController, Controller);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
    /** @name Property declarations 
    These are the serializable properties associated with a MuscleReflexController.*/
    /**@{**/  	
	OpenSim_DECLARE_PROPERTY(gain, double, 
		"Factor by which the stretch response is scaled." );

//=============================================================================
// METHODS
//=============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION AND DESTRUCTION
	//--------------------------------------------------------------------------
	/** Default constructor. */
	MuscleReflexController();
	// Uses default (compiler-generated) destructor, copy constructor and copy 
    // assignment operator.

	/** Convenience constructor 
	* @param gain		gain on the stretch response
	*/
	MuscleReflexController(double gain);

	/** Compute the controls for actuators (muscles)
	 *  This method defines the behavior for MuscleReflexController controller 
	 *
	 * @param s			system state 
	 * @param controls	writable model controls
	 */
	void computeControls(const SimTK::State& s, SimTK::Vector &controls) const OVERRIDE_11;


private:
	// Connect properties to local pointers.  */
	void constructProperties();
	// ModelComponent interface to connect this component to its model
	void connectToModel(Model& aModel);

	//=============================================================================
};	// END of class MuscleReflexController

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_MuscleReflexController_H_


