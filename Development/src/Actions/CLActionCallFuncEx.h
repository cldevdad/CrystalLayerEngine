/*
Crystal Layer
Copyright (C) 2018 Colin Payette <colin@cpayette.com>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#ifndef _INCLUDE_CLACTIONCALLFUNCEX_H_
#define _INCLUDE_CLACTIONCALLFUNCEX_H_

#include "..\Actions\CLAction.h"
#include "..\Core\CLTypes.h"
#include <functional>

typedef std::function<void(void*, int)> CLVoidFunctionEx;

/**
*   An action that calls a function with user data (void*, int) arguments
*/
class CLActionCallFuncEx : public CLAction
{
public:

    //! Allows cloning through NewCopy()
	DLLEXPORT CL_CLONEABLE(CLActionCallFuncEx)
    //! Copy constructor                                  
		DLLEXPORT CLActionCallFuncEx(const CLActionCallFuncEx& action);
    //! Constructor that takes a callback and arguments
	DLLEXPORT CLActionCallFuncEx(CLVoidFunctionEx function, void* pData, int nData);
    //! Destructor
	DLLEXPORT ~CLActionCallFuncEx();

	DLLEXPORT void Start();           //!< Starts the delay action
	DLLEXPORT void Finish();          //!< Finishes the delay action
	DLLEXPORT void Update(float dt);  //!< Updates the delay action

private:

    CLVoidFunctionEx    m_FunctionCB;
    void*               m_pVoidData;
    int                 m_IntData;
};

#endif // _INCLUDE_CLACTIONCALLFUNCEX_H_