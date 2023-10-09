#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <Arduino.h>

/*@brief Enum for types of places where a command can
* originate from. Used in cmd_struct_t
*/
typedef enum origin_t{
    e_origin_undefined,
    e_origin_interrupt,
    e_origin_task,
    e_origin_core
}origin_t;

/*@brief Enum for command content types aka jobs to do.
* Used in cmd_struct_t
*/
typedef enum cmd_t{
    e_cmd_undefined,
    e_cmd_generic
}cmd_t;

/*@brief Struct for sendable commands
* @param origin (origin_t): Place of command creation
* @param content (cmd_t): The job to do
* @param o_value (uint16_t): Optional 16 bit value
* @note Although not certain, it is assumed that both origin and
* content are 8 bit wide, which makes the entire struct 32 bit
* wide, small enough for atomic access and able to be passed along
* wirh FreeRTOS task notifications
*/
typedef struct cmd_struct_t{
    origin_t origin = e_origin_undefined;
    cmd_t content = e_cmd_undefined;
    uint16_t o_value = 0;
}cmd_struct_t;

#endif