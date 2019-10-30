
#include <iostream>
#include <sstream>
#include <string>
#include <wchar.h>
#include <sstream>
#include <limits.h>

#include <pthread.h>

#include <lscl/lscl.hpp>

void* lscl_routine(void *arg)
{
	while (true)
	{
		std::string s = 
		"{"
		",    kek:    kok"
		",    lel:    {lel: {lel: {lel:lol     }}   }"
		",    lesley: [one, two, three, {\"ключ\": \"значение\"}, five]"
		",    КЛЮЧИК   :    *(lesley.[3].ключ)"
		",    ключик0  :    *(lesley.[1])"
		",    ключик1  :    *(\"kek\")"
		",    ключик2  :    *(kek)"
		",    ключик3  :    *(lel.lel.lel.lel)"
		",    ключик4  :    *('lel'.lel.lel.lel)"
		",    ключик5  :    *(lel.'lel'.lel.lel)"
		",    ключик6  :    *(lel.lel.lel.'lel')"
		"}";
		std::stringstream ss(s);
		LSCL::Nodebuilder::Builder builder(ss);
		if (builder.getroot().size() != 11) { std::cout << "Error - map size" << std::endl; exit(1); }
		if (builder.getroot()["КЛЮЧИК"].get<std::string>() != "значение") { std::cout << "Error - КЛЮЧИК element" << std::endl; exit(1); }
		if (builder.getroot()["ключик0"].get<std::string>() != "two") { std::cout << "Error - ключик0 element" << std::endl; exit(1); }
		if (builder.getroot()["ключик1"].get<std::string>() != "kok") { std::cout << "Error - ключик1 element" << std::endl; exit(1); }
		if (builder.getroot()["ключик2"].get<std::string>() != "kok") { std::cout << "Error - ключик2 element" << std::endl; exit(1); }
		if (builder.getroot()["ключик3"].get<std::string>() != "lol") { std::cout << "Error - ключик3 element" << std::endl; exit(1); }
		if (builder.getroot()["ключик4"].get<std::string>() != "lol") { std::cout << "Error - ключик4 element" << std::endl; exit(1); }
		if (builder.getroot()["ключик5"].get<std::string>() != "lol") { std::cout << "Error - ключик5 element" << std::endl; exit(1); }
		if (builder.getroot()["ключик6"].get<std::string>() != "lol") { std::cout << "Error - ключик6 element" << std::endl; exit(1); }
	}
}

int main()
{
	pthread_t t1;
	if (pthread_create(&t1, NULL, lscl_routine, NULL))
	{
		std::cout << "Error creating CLI thread" << std::endl;
		return 1;
	}
	
	pthread_t t2;
	if (pthread_create(&t2, NULL, lscl_routine, NULL))
	{
		std::cout << "Error creating CLI thread" << std::endl;
		return 1;
	}
	
	pthread_t t3;
	if (pthread_create(&t3, NULL, lscl_routine, NULL))
	{
		std::cout << "Error creating CLI thread" << std::endl;
		return 1;
	}
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	
	return 0;
}
