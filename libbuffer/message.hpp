
/*
 *   message.hpp
 *
 *  Created on: Sep 24, 2017
 *      Author: mihail rylov
 */

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_


struct Message
{
	char* msg_data;
	unsigned char size;

	Message(char* _msg_data, unsigned char _size)
	 {
		msg_data = _msg_data;
		size = _size;
	 }
};

#endif /* MESSAGE_HPP_ */
