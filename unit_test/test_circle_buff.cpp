//============================================================================
// Name        : test_circle_buff.cpp
// Author      : mike rylov
//============================================================================

#include <algorithm>
#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <random>
#include <dlfcn.h>
#include <string.h>
#include "../circlebuff_API.hpp"
#include "../libbuffer/message.hpp"

std::string random_string( size_t length )
{

    auto randchar = []() -> char
    {
        const char charset[] =
        //"0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

void simple_test(push_f push_p, pop_f pop_p)
{
    Message msg("Luxoft", 6);
    push_p(msg);
    msg.msg_data = "Ukraine";
    msg.size = 7;
    push_p(msg);
    std::cout << pop_p().msg_data;
    std::cout << pop_p().msg_data;
}

void multithread_test(push_f push_p, pop_f pop_p)
{
    std::future<void> fconsumer =
       std::async(std::launch::async,
                 [&]{

	                Message msg("", 0);
	                for( ; ; )
	                {
	                   unsigned char length = rand()%(20) + 1;

	                   msg.size = length;
	                   std::string str = random_string(length);
	                   char *cstr = new char[length+1];
	                   strcpy(cstr, str.c_str());
	                   msg.msg_data = cstr;
	                   push_p(msg);
	                   std::chrono::milliseconds duration(900);
	                   std::this_thread::sleep_for( duration );

	                }
                 });

      std::future<void> fproducer =
      std::async(std::launch::async,
                 [&]{

                 	for( ;  ; )
                 	{
    	              std::cout << pop_p().msg_data;
	                   std::chrono::milliseconds duration(1000);
	                   std::this_thread::sleep_for( duration );
                    }
                 });

    fconsumer.wait();
    fproducer.wait();
}


int main()
{


	void* handle = dlopen("./liblibcircle_buffer.so", RTLD_LAZY);

	    if (!handle) {
	        std::cerr << "Cannot open library: " << dlerror() << '\n';
	        return 1;
	    }

	    // load the symbol
	    std::cout << "Loading symbol libringbuffer...\n";

	    push_f push_p = (push_f) dlsym(handle, "push");
	    pop_f pop_p = (pop_f) dlsym(handle, "pop");


        simple_test(push_p, pop_p);

        std::chrono::seconds duration(5);
        std::this_thread::sleep_for( duration );

        multithread_test(push_p, pop_p);


	    dlclose(handle);


  return 0;
}


