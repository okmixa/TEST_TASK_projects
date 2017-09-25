/*
 *  circle_buffer.hpp
 *
 *  Created on: Sep 24, 2017
 *      Author: mihail rylov
 */


#include "circle_buffer.hpp"
#include <string.h>


CircleBuffer<80> myBuff;


Message pop()
{
	Message msg(myBuff.pop(), 255);

	return msg;
}


void push(Message _msg)
{

	myBuff.push(_msg.msg_data, _msg.size);
}

