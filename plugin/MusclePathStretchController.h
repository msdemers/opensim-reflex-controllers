#ifndef OPENSIM_MusclePathStretchController_H_
#define OPENSIM_MusclePathStretchController_H_
/* -------------------------------------------------------------------------- *
 *                       OpenSim:  MusclePathStretchController.h                   *
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
#include "osimReflexesDLL.h" 


namespace OpenSim { 

//=============================================================================
//=============================================================================
/**
 * MusclePathStretchController is a concrete controller that excites muscles in response
 * to muscle lengthening to simulate. Thus, it behaves like a simple stretch reflex. 
 * This controller responds to non-zero lengthening velocities and to stretch lengths 
 * beyond the specified normalized_rest_length.  Since this controller monitors the 
 * entire muscle path (muscle fiber + tendon), the provided rest length is interpretted 
 * as a ratio of desired path rest length to muscle neutral length 
 * (fiber length + tendon slack length).

 *
 * @author  Matt DeMers
 * @version 1.0
 */
class OSIMREFLEXES_API MusclePathStretchController : public Controller {
OpenSim_DECLARE_CONCRETE_OBJECT(MusclePathStretchController, Controller);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
    /** @name Property declarations 
    These are the serializable properties associated with a MusclePathStretchController.*/
    /**@{**/  	
	OpenSim_DECLARE_PROPERTY(gain_length, double, 
		"Control gain applied to stretch length" );
	OpenSim_DECLARE_PROPERTY(gain_velocity, double,
		"Control gain applied to stretch velocity");
	OpenSim_DECLARE_PROPERTY(normalized_rest_length, double,
		"The intended rest length of the muscle, after which the,"
		" controller responds to stretch. Rest length is interpreted as a "
		"ratio of the muscle rest length to the muscle neutral length.");

//=============================================================================
// METHODS
//=============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION AND DESTRUCTION
	//--------------------------------------------------------------------------
	/** Default constructor. */
	MusclePathStretchController();
	// Uses default (compiler-generated) destructor, copy constructor and copy 
    // assignment operator.

	/** Convenience constructor 
	* @param rest_length desired length of the muscle
	* @param gain_l		gain on the stretch length response
	* @param gain_v     gain on the stretch velocity response
	*/
	MusclePathStretchController(double rest_length, double gain_l, double gain_v);

	/** Compute the controls for actuators (muscles)
	 *  This method defines the behavior for MusclePathStretchController controller 
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
};	// END of class MusclePathStretchController

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_MusclePathStretchController_H_


