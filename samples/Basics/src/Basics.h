#pragma once

#include "runtime/Virtual.h"
#include "runtime/ClassWatcher.h"

class Basics {
public:
	Basics();

	// any function that has to be called from
	// outside the class should be marked as virtual
	// rt_virtual should probably be use instead when
	// the function doesn't need to be virtual has it
	// become a no-op when building in Release or Debug.
	rt_virtual void draw();
	
protected:

	// otherwise RT_DECL is the only thing
	// needed in the class declaration to mark the class
	// as runtime reloadable. Modifying the header is 
	// usually slower than modifying the cpp file.
	RT_DECL
};