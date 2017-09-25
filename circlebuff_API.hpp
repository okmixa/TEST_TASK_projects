/*
 * circlebuff_API.hpp
 *
 *  Created on: Sep 24, 2017
 *      Author: mihail rylov
 */

#include "libbuffer/message.hpp"

/**
 * Returns a  Message from FIFO circle memmory buffer, placed on First position
 *
 *
 * @param void
 * @return copy of Message
 */
Message pop();
typedef Message (*pop_f)();

/**
 * Add to FIFO circle buffer new Message element
 * Restriction : max message size 255 bytes
 *
 * @param Message
 * @return void
 */
void push(Message);
typedef char* (*push_f)(Message);
