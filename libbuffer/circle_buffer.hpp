/*
 * circle_buffer.hpp
 *
 *  Created on: Sep 24, 2017
 *      Author: mihail rylov
 */

#ifndef CIRCLE_BUFFER_HPP_
#define CIRCLE_BUFFER_HPP_


#include <ostream>
#include <array>
#include <mutex>
#include <condition_variable>
#include <string.h>

#include "message.hpp"

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#include <iostream>
#endif

/**
 * @note
 * start: in=0, out=0
 *        o,i
 *         v
 * buffer [ | | | | | | | | | | ]
 *
 * first char contain size of stored message, maximum length of message 255
 *
 * after 2  push messages with size 1 and 5 bytes:
 *
 *         o   i
 *         v   v
 * buffer [1|@|5|@|@|@|@|@| | | ]
 *
 *then after pop first message:
 *
 *             o           i
 *             v           v
 * buffer [#|#|5|@|@|@|@|@| | | ]
 *
 */

extern "C"
{
    Message pop();
    void push(Message);
}

template <size_t N>
class CircleBuffer
{
private:
  std::array<char, N> buffer;
  volatile size_t in, out, capacity;
  std::mutex lkm;
  std::condition_variable cv;

public:
  CircleBuffer() : in(75), out(75), capacity(N) {}

  void push(char* val, unsigned char mesSize)
  {
    {
      std::unique_lock<std::mutex> lk(lkm);
#ifdef DEBUG_PRINT
      std::cerr << "\n==PUSH LOCK ENTERED\n"
    		    << " capacity=" << capacity;
#endif
      cv.wait(lk, [&]{ return capacity > mesSize; });
#ifdef DEBUG_PRINT
      std::cerr << "\n==PUSH COND WAIT FINISHED\n";
#endif

      buffer[in] = mesSize;

      if (N - in > mesSize)
      {
          std::move(val, val + mesSize, &buffer[in + 1]);
      }
      else
      {
    	  std::cerr << "\ncircle memorry calculation...\n";
    	  size_t ring_delta_end = N - in - 1;
    	  std::cerr << " ring_delta_end = " << ring_delta_end << "\n";
          std::move(val, val + ring_delta_end, &buffer[in+1]);
          std::move(val + ring_delta_end,val + mesSize, &buffer[0]);
      }

      in = (in + mesSize + 1) % N;
      capacity = capacity - mesSize - 1;

#ifdef DEBUG_PRINT
      std::cerr << "==PUSHED " << val << "\n"
                << " (i=" << in
                << ",o=" << out
                << ",c=" << capacity << ")"
                << "\n" << *this << "\n";
#endif
    }
    cv.notify_all();
  }

  char* pop()
  {

    unsigned char messageSize = buffer[out];
    char ret[messageSize+1];

    if(messageSize > 0 && messageSize < 256)
    {
      std::unique_lock<std::mutex> lk(lkm);
#ifdef DEBUG_PRINT
      std::cerr << "\n==POP LOCK ENTERED\n"
                << " capacity=" << capacity
                << " messageSize for POP=" << (int)messageSize << '\n';
#endif
      cv.wait(lk, [&]{ return (capacity > 2) && (in != out); });
#ifdef DEBUG_PRINT
      std::cerr << "==POP COND WAIT FINISHED\n";
#endif


     for(int n = 0; n < messageSize; ++n)
     {
    	 ret[n] = buffer[(out+n+1)% N]; // this is ring memory model
    	 buffer[(out+n+1)% N] = 0;
     }
     buffer[out] = 0;//there was unsigned char with length of poped message

     out = (out + messageSize + 1) % N; // HERE I MADE ERROR BEFORE

     capacity = (capacity + messageSize + 1);

#ifdef DEBUG_PRINT
      std::cerr << "==POPPED " << ret << "\n"
                << " (i=" << in
                << ",o=" << out
                << ",c=" << capacity << ")"
                << "\n" << *this << "\n";
#endif
    cv.notify_all();

    }
    return ret;
  }

  friend std::ostream & operator<<(std::ostream & ost,
                                   const CircleBuffer<N> & buffer)
  {
    ost << '[';
    for(int i=0; i < N; ++i)
    {
      if (i > 0 && i < N - 1)
    	  ost << ',';
      if(i == buffer.in)
          ost << '>';
      if(i == buffer.out)
    	  ost << '<';
      if(buffer.buffer[i])
          ost << char(buffer.buffer[i]);
      else
    	  ost << ' ';


    }
    ost << ']';
    ost.flush();

    return ost;
  }
};


#endif /* CIRCLE_BUFFER_HPP_ */
