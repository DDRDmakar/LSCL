/*
 * 
 * Copyright 2019 Nikita Makarevich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#ifndef _H_LSCL_SCRIPT
#define _H_LSCL_SCRIPT

#include <signal.h>
#include <string.h>
#include <list>

#include "defines.hpp"

std::string get_script_output(std::string &script);

class Script
{
private:
	unsigned int _timeout_process_check_number;
	useconds_t   _timeout_us_process_check_period;
	
protected:
	
public:
	
	std::string  interpreter;
	int          timeout_ms_stdout_polling;
	int          script_terminate_signal;
	std::list<std::string> flags;
	
	Script(
		const std::string      &interpreter                 = LSCL_SCRIPT_INTERPRETER,
		unsigned int           timeout_ms_process_check     = LSCL_SCRIPT_TIMEOUT_MS_PROCESS_CHECK,
		unsigned int           timeout_process_check_number = LSCL_SCRIPT_TIMEOUT_PROCESS_CHECK_NUMBER,
		int                    timeout_ms_stdout_polling    = LSCL_SCRIPT_TIMEOUT_MS_STDOUT_POLLING,
		int                    script_terminate_signal      = LSCL_SCRIPT_TERMINATE_SIGNAL,
		std::list<std::string> *flags_in                    = nullptr
	);
	
	std::string execute(const std::string &script) const;
	
};

#endif // _H_LSCL_SCRIPT
