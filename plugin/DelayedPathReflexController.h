#ifndef OPENSIM_DelayedPathReflexController_H_
#define OPENSIM_DelayedPathReflexController_H_
/* ------------------------------------------------------------------------- *
*                      OpenSim: DelayedPathReflexController.h                 *
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


//============================================================================
// INCLUDE
//============================================================================
#include "OpenSim\Simulation\Control\Controller.h"
#include "OpenSim\Common\PiecewiseLinearFunction.h"
#include "OpenSim\Simulation\Model\Model.h"

// to export class as part of a plugin:
#include "osimReflexesDLL.h" 

namespace OpenSim {

	//=============================================================================
	//=============================================================================
	/**
	* DelayedPathReflexController is a concrete controller that excites muscles in 
	* response to muscle lengthening to simulate a simple, and delayed stretch 
	* reflex. This controller models lengthening as the stretch velocity of the
	* entire muscle-tendon path, delays the lengthening signal by a constant time
	* offset, then applies a gain to this delayed signal.  Since this reflex is not
	* aware of true muslce fiber velocity, fiber length, tendon force, or neurologic
	* sources of delay, it is not intended for research. It serves as an example how
	* to implement a controller in OpenSim. It is intended for demonstrative 
	* purposes only.
	*
	* @author  Matt DeMers
	*/
	class OSIMREFLEXES_API DelayedPathReflexController : public Controller {
		OpenSim_DECLARE_CONCRETE_OBJECT(DelayedPathReflexController, Controller);

	public:
		//=============================================================================
		// PROPERTIES
		//=============================================================================
		/** @name Property declarations
		These are the serializable properties associated with a DelayedPathReflexController.*/
		/**@{**/
		OpenSim_DECLARE_PROPERTY(gain, double,
			"Factor by which the stretch reflex is scaled.");
		OpenSim_DECLARE_PROPERTY(delay, double,
			"Time delay (seconds) between the musle stretch and the stretch reflex signal");

		//=============================================================================
		// METHODS
		//=============================================================================
		//--------------------------------------------------------------------------
		// CONSTRUCTION AND DESTRUCTION
		//--------------------------------------------------------------------------
		/** Default constructor. */
		DelayedPathReflexController();

		// Uses default (compiler-generated) destructor, copy constructor and copy 
		// assignment operator.

		/** Convenience constructor
		* @param gain		gain on the stretch response
		* @param delay      delay on the stretch response
		*/
		DelayedPathReflexController(double gain, double delay);

		/** Compute the controls for actuators (muscles)
		*  This method defines the behavior of the DelayedPathReflexController
		*
		* @param s			system state
		* @param controls	writable model controls
		*/
		void computeControls(const SimTK::State& s, SimTK::Vector &controls) const override;


	private:
		// Connect properties to local pointers.  */
		void constructProperties();
		// ModelComponent interface to add computational elements to the SimTK system
		void addToSystem(SimTK::MultibodySystem& system) const;
		// ModelComponent interface to connect this component to its model
		void connectToModel(Model& aModel);
		//=============================================================================
		// Private Members
		//=============================================================================
		mutable OpenSim::Set<PiecewiseLinearFunction> muscleStretchVelocityHistory;
        //mutable OpenSim::Set<PiecewiseLinearFunction> delayedMuscleStretchVelocityHistory;
		
		//=============================================================================
	};	// END of class DelayedPathReflexController

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_DelayedPathReflexController_H_


