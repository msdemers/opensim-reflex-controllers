#ifndef OPENSIM_ReflexController_H_
#define OPENSIM_ReflexController_H_
/* -------------------------------------------------------------------------- *
 *                       OpenSim:  ReflexController.h                         *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2013 Stanford University and the Authors                *
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


//============================================================================
// INCLUDE
//============================================================================
#include <OpenSim/Simulation/Control/Controller.h>

// to export class as part of a plugin:
#include "osimReflexesDLL.h" 


namespace OpenSim { 

//=============================================================================
//=============================================================================
/**
 * ReflexController is a concrete controller that excites muscles in response
 * to muscle lengthening to simulate a stretch reflex. This controller is 
 * meant to serve as an example how to implement a controller as a plugin in
 * OpenSim. It is intended for demonstrative purposes only. 

 *
 * @author  Ajay Seth
 * @version 1.0
 */
class OSIMREFLEXES_API ReflexController : public Controller {
OpenSim_DECLARE_CONCRETE_OBJECT(ReflexController, Controller);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
    /** @name Property declarations 
    These are the serializable properties associated with a ReflexController.*/
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
	ReflexController();
	// Uses default (compiler-generated) destructor, copy constructor and copy 
    // assignment operator.

	/** Convenience constructor 
	* @param gain		gain on the stretch response
	*/
	ReflexController(double gain);

	/** Compute the controls for actuators (muscles)
	 *  This method defines the behavior for ReflexController controller 
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
};	// END of class ReflexController

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_ReflexController_H_


