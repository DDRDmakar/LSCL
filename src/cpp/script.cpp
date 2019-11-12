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

#include <unistd.h>
#include <iostream>
#include <sys/poll.h>
#include <sys/wait.h>
#include <pthread.h>

#include "script.hpp"
#include "nodebuilder.hpp"

/* 
 * since pipes are unidirectional, we need two pipes.
 * one for data to flow from parent's stdout to child's
 * stdin and the other for child's stdout to flow to
 * parent's stdin
 * 
 */

#define LSCL_NUM_PIPES         3

#define LSCL_PIPE_CHILD_STDIN  0
#define LSCL_PIPE_CHILD_STDOUT 1
#define LSCL_PIPE_CHILD_STDERR 2

// Always in a pipe[], pipe[0] is for read and pipe[1] is for write
#define LSCL_PIPE_RD 0
#define LSCL_PIPE_WR 1

#define LSCL_PARENT_RD_FD ( pipes[LSCL_PIPE_CHILD_STDOUT][LSCL_PIPE_RD] )
#define LSCL_PARENT_WR_FD ( pipes[LSCL_PIPE_CHILD_STDIN][LSCL_PIPE_WR]  )
#define LSCL_PARENT_ER_FD ( pipes[LSCL_PIPE_CHILD_STDERR][LSCL_PIPE_RD] )

#define LSCL_CHILD_RD_FD  ( pipes[LSCL_PIPE_CHILD_STDIN][LSCL_PIPE_RD]  )
#define LSCL_CHILD_WR_FD  ( pipes[LSCL_PIPE_CHILD_STDOUT][LSCL_PIPE_WR] )
#define LSCL_CHILD_ER_FD  ( pipes[LSCL_PIPE_CHILD_STDERR][LSCL_PIPE_WR] )

namespace LSCL
{

namespace Nodebuilder
{


struct Python_listener_args
{
	int child_stdout_fd;
	int child_stderr_fd;
	int timeout_ms_stdout_polling;
	bool *python_is_active;
	std::string *accum;
};

void* python_listener(void *a)
{
	// Thread - pipe listener
	
	Python_listener_args *args = (Python_listener_args*)a;
	
	char buffer[LSCL_SCRIPT_READ_BUFFER_SIZE];
	int count;
	bool python_is_active_2 = true;
	while (python_is_active_2)
	{
		python_is_active_2 = *(args->python_is_active);
		
		// Read from child’s stdout and stderr
		struct pollfd pfds[2];
		pfds[0].fd = args->child_stdout_fd;
		pfds[0].events = POLLIN;
		pfds[0].revents = 0;
		pfds[1].fd = args->child_stderr_fd;
		pfds[1].events = POLLIN;
		pfds[1].revents = 0;
		
		//std::cout << "Polling\n";
		poll(pfds, 3, args->timeout_ms_stdout_polling);
		
		if (pfds[0].revents & POLLIN)
		{
			std::cout << "Reading stdout\n";
			count = read(args->child_stdout_fd, buffer, LSCL_SCRIPT_READ_BUFFER_SIZE-1);
			buffer[count] = '\0';
			std::cout << buffer << std::endl;
			args->accum->append(buffer);
		}
		
		if (pfds[1].revents & POLLIN)
		{
			std::cout << "Reading stderr\n";
			count = read(args->child_stderr_fd, buffer, LSCL_SCRIPT_READ_BUFFER_SIZE-1);
			buffer[count] = '\0';
			std::cout << buffer << std::endl;
		}
	}
	
	std::cout << "Exiting python-listener\n";
	
	return NULL;
}

std::string Script::execute(const std::string &script) const
{
	std::string accum;
		
	int pipes[LSCL_NUM_PIPES][2];
	
	// pipes for parent to write and read
	pipe(pipes[LSCL_PIPE_CHILD_STDIN]);
	pipe(pipes[LSCL_PIPE_CHILD_STDOUT]);
	pipe(pipes[LSCL_PIPE_CHILD_STDERR]);
	
	pid_t child = fork();
	if (child < 0)
	{
		printf("Can't create child\n");
		return 0;
	}
	
	if(!child)
	{
		// New process - Script
		// TODO make interpreter configurable
		char *argv[] =
		{
			strdup("python3"),
			strdup("-c"),
			strdup(script.c_str()),
			nullptr
		};
		
		dup2(LSCL_CHILD_RD_FD, STDIN_FILENO);
		dup2(LSCL_CHILD_WR_FD, STDOUT_FILENO);
		dup2(LSCL_CHILD_ER_FD, STDERR_FILENO);
		
		// Close fds not required by child
		// Also, we don't want the exec'ed program to know these existed
		close(LSCL_CHILD_RD_FD);
		close(LSCL_CHILD_WR_FD);
		close(LSCL_CHILD_ER_FD);
		close(LSCL_PARENT_RD_FD);
		close(LSCL_PARENT_WR_FD);
		close(LSCL_PARENT_ER_FD);
		
		execvp(argv[0], argv);
		// Here we go only if error occured while running execv
		printf("[Cannot run %s]\n", argv[0]);
		exit(7);
	}
	else
	{
		// close fds not required by parent
		close(LSCL_CHILD_RD_FD);
		close(LSCL_CHILD_WR_FD);
		close(LSCL_CHILD_ER_FD);
		
		// Create python stdout listener thread
		bool python_is_active = true;
		pthread_t pylisten_thread;
		Python_listener_args python_listener_args = 
		{
			LSCL_PARENT_RD_FD,
			LSCL_PARENT_ER_FD,
			timeout_ms_stdout_polling,
			&python_is_active,
			&accum
		};
		if (
			pthread_create(
				&pylisten_thread,
				NULL,
				python_listener,
				&python_listener_args
			)
		)
		{
			printf("Error creating listener thread\n");
		}
		// Write python script into pipe
		//pyprog.push_back('\n');
		//write(LSCL_PARENT_WR_FD, pyprog.c_str(), pyprog.size());
		
		// Wait for python to exit
		int status = 0;
		int waitpid_return;
		printf("Waiting for child\n");
		
		unsigned int n_polls = timeout_process_check_number_;
		unsigned int n_checked = 0;
		do
		{
			waitpid_return = waitpid(child, &status, WNOHANG);
			if (!waitpid_return) usleep(timeout_us_process_check_period_);
			++n_checked;
		}
		while (
			!waitpid_return &&
			n_checked < n_polls &&
			python_is_active
		);
		
		printf("WIFEXITED = %d\n", WIFEXITED(status));
		printf("WIFSIGNALED = %d\n", WIFSIGNALED(status));
		printf("WCOREDUMP = %d\n", WCOREDUMP(status));
		printf("WIFSTOPPED = %d\n", WIFSTOPPED(status));
		
		printf("n_checked: %d\n", n_checked);
		
		*(python_listener_args.python_is_active) = false;
		
		if (waitpid_return && WIFEXITED(status))
		{
			printf("Python exited by signal %d\n", WEXITSTATUS(status));
			if (WEXITSTATUS(status) != 0)
			{
				printf("Python error\n");
			}
		}
		else
		{
			printf("Python is timed out!\n");
			kill(child, script_terminate_signal);
		}
		printf("joining listener thread\n");
		// Wait for the python stdout listener to finish
		if (pthread_join(pylisten_thread, NULL))
		{
			printf("Error joining listener thread\n");
		}
		printf("join successful\n");
	}
	
	std::cout << "Accum = " << std::endl;
	std::cout << accum << std::endl;
	
	printf("Returning 0\n");
	
	return accum;
}


Script::Script(
	const std::string      &interpreter,
	unsigned int           timeout_ms_process_check,
	unsigned int           timeout_process_check_number,
	int                    timeout_ms_stdout_polling,
	int                    script_terminate_signal,
	std::list<std::string> *flags_in
) :
	timeout_process_check_number_(timeout_process_check_number),
	timeout_us_process_check_period_((timeout_ms_process_check * 1000 / timeout_process_check_number_)),
	interpreter(interpreter),
	timeout_ms_stdout_polling(timeout_ms_stdout_polling),
	script_terminate_signal(script_terminate_signal)
{
	if (flags_in) // If we passed this argument
	{
		for (const std::string &e : *flags_in) flags.push_back(e);
	}
	else
	{
		flags = LSCL_SCRIPT_PYTHON_FLAGS;
	}
}

// Thread
void* script_processor(void *a)
{
	Builder::Executed_args *args = static_cast<Builder::Executed_args*>(a);
	try
	{
		// Run script and get its output
		Script script; // Script-runner object
		std::string script_result = script.execute(args->in);
		// Then parse script output
		std::stringstream ss(script_result);
		LSCL::Nodebuilder::Builder builder2(ss);
		args->out = builder2.release_root();
		args->done = true;
	}
	catch (Exception::Exception_nodebuilder &e)
	{
		Exception::Exception_nodebuilder nbe = e;
		args->e = e;
		args->done = true;
		args->out = nullptr;
	}
	catch (std::exception&)
	{
		args->e = Exception::Exception_nodebuilder("Exception thrown while processing script or script output");
		args->done = true;
		args->out = nullptr;
	}
	
	return NULL;
}


} // Namespace Nodebuilder

} // Namespace LSCL
