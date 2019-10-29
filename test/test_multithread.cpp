
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
		//if (builder.root_->size() != 11) { std::cout << "Error"; return 1; }
		
		//CMP_NUM(builder.root_->size(), 11, "Map size");
		//CMP_NUM(builder.references_.size(), 8, "Number of references");
		//CMP_TYPE(Node(builder.root_)["КЛЮЧИК"].get_type(), LSCL::NODETYPE_LINK, "element КЛЮЧИК type");
		//TEST_BOOL( ((Link*)(builder.root_->at("КЛЮЧИК")))->linked != nullptr, "linked node КЛЮЧИК is NULL" );
		//CMP_TEXT(Node(builder.root_)["КЛЮЧИК"].get<std::string>(), "значение", "element КЛЮЧИК linked value");
		//CMP_TEXT(Node(builder.root_)["ключик0"].get<std::string>(), "two", "element ключик0 linked value");
		//CMP_TEXT(Node(builder.root_)["ключик1"].get<std::string>(), "kok", "element ключик1 linked value");
		//CMP_TEXT(Node(builder.root_)["ключик2"].get<std::string>(), "kok", "element ключик2 linked value");
		//CMP_TEXT(Node(builder.root_)["ключик3"].get<std::string>(), "lol", "element ключик3 linked value");
		//CMP_TEXT(Node(builder.root_)["ключик4"].get<std::string>(), "lol", "element ключик4 linked value");
		//CMP_TEXT(Node(builder.root_)["ключик5"].get<std::string>(), "lol", "element ключик5 linked value");
		//CMP_TEXT(Node(builder.root_)["ключик6"].get<std::string>(), "lol", "element ключик6 linked value");
	}
}

int main()
{
	pthread_t t1;
	if (pthread_create(&t1, NULL, lscl_routine, NULL))
	{
		std::cout << "Error creating CLI thread" << std::endl;
	}
	
	pthread_t t2;
	if (pthread_create(&t2, NULL, lscl_routine, NULL))
	{
		std::cout << "Error creating CLI thread" << std::endl;
	}
	
	pthread_t t3;
	if (pthread_create(&t3, NULL, lscl_routine, NULL))
	{
		std::cout << "Error creating CLI thread" << std::endl;
	}
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	
	return 0;
}
