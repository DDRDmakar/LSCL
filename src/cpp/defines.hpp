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

#ifndef _H_LSCL_DEFINES
#define _H_LSCL_DEFINES

#define LSCL_SCRIPT_READ_BUFFER_SIZE 256
#define LSCL_SCRIPT_TERMINATE_SIGNAL SIGUSR1
#define LSCL_SCRIPT_INTERPRETER      "python3"
#define LSCL_SCRIPT_TIMEOUT_MS_PROCESS_CHECK        (10*1000) // 10 seconds
#define LSCL_SCRIPT_TIMEOUT_PROCESS_CHECK_NUMBER    (1000)
#define LSCL_SCRIPT_TIMEOUT_MS_STDOUT_POLLING       (100)
#define LSCL_SCRIPT_PYTHON_FLAGS                    { "-c" }

#define LSCL_BUILDER_TIMEOUT_JOIN_COUNT      100
#define LSCL_BUILDER_TIMEOUT_JOIN_PERIOD_MS  100*100 // 100ms * 100 = 10s

#endif // _H_LSCL_DEFINES
