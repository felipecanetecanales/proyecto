/**
  * @file i2c.h
  * @author Mauricio Solis
  * @date 07/12/2017
  * @brief Define the I2C API
  *
  * This File was created by Mauricio Solis in order to make more easy
  * the use of I2C of the MSP430F1612.
  * Please use it with caution and responsability.
*/

#ifndef __I2C__H
#define __I2C__H

/*--->> Includes <<-----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

/*--->> Definition <<---------------------------------------------------------*/
/**
 * I2C Master Slave Mode
 */
typedef enum
{
	I2C_MODE_MASTER,
	I2C_MODE_SLAVE
} e_i2c_ms_mode;



/**
* I2C Read Write Mode
*/
typedef enum
{
	I2C_RW_MODE_NONE,
	I2C_RW_MODE_READ,
	I2C_RW_MODE_WRITE
} e_i2c_rw_mode;

/**
* I2C Word Mode
*/
typedef enum
{
	I2C_MODE_7BITS,
	I2C_MODE_10BITS
} e_i2c_word_mode;

/**
* I2C State
*/
typedef enum
{
	I2C_NOT_INITED,
	I2C_IDLE,
	I2C_BUSY,
	I2C_ERROR
} e_i2c_state;

typedef struct
{
	e_i2c_state i2c_status;
	
	e_i2c_ms_mode i2c_ms_mode;
	e_i2c_rw_mode i2c_rw_mode;
	
	uint8_t number_of_data_to_read;
	uint8_t number_of_data_to_write;
	
	uint8_t number_of_data_read;
	uint8_t number_of_data_written;
	
	uint8_t register_start_address;
	
	uint8_t *rx_buffer;
	uint8_t *tx_buffer;
} t_i2c_usm;



/*--->> API <<----------------------------------------------------------------*/


/**
 * @brief This function initialize the i2c.
 * @param slave_address			The address of the slave
 * @param e_i2c_ms_mode			Mode Master or Slave
 * @retval						True if all works good, false otherwise
*/
bool i2c_init(uint8_t slave_address, e_i2c_ms_mode i2c_ms_mode);

/**
 * @brief This function write/send data through the i2c bus in blocking mode
 * @param register_address		This is the register of the slave to start write
 * @param data					Ptr to data to write or send
 * @param data_len				The number of data to send
 * @retval						True if all works good, false otherwise
*/
bool i2c_master_write(uint8_t register_address, uint8_t* data, uint8_t data_len);


/**
 * @brief This function read data through the i2c bus in blocking mode
 * @param register_address		This is the register of the slave to start read
 * @param data					Ptr to data to store the data read
 * @param data_len				The number of data to read
 * @retval						True if all works good, false otherwise
*/
bool i2c_master_read(uint8_t register_address, uint8_t* data, uint8_t data_len);

/**
 * @brief This function is to know if the i2c is busy
 * @retval						True if is busy, false otherwise
*/
bool i2c_is_busy();



/**
 * @brief This function initialize the i2c and enable the interruptions.
 * @param slave_address			The address of the slave
 * @param e_i2c_ms_mode			Mode Master or Slave
 * @retval						True if all works good, false otherwise
*/
bool i2c_init_it(uint8_t slave_address, e_i2c_ms_mode i2c_ms_mode);

/**
 * @brief This function write/send data through the i2c bus in interruption mode
 * @param register_address		This is the register of the slave to start write
 * @param data					Ptr to data to write or send
 * @param data_len				The number of data to send
 * @retval						True if all works good, false otherwise
 * @note						When the writing process ends, the function i2c_write_finished_it is called.
*/
bool i2c_master_write_it(uint8_t register_address, uint8_t* data, uint8_t data_len);

/**
 * @brief This function read data through the i2c bus in interruption mode
 * @param register_address		This is the register of the slave to start read
 * @param data					Ptr to data to store the data read
 * @param data_len				The number of data to read
 * @retval						True if all works good, false otherwise
 * @note						When the reading process ends, the function i2c_read_finished_it is called.
*/
bool i2c_master_read_it(uint8_t register_address, uint8_t* data, uint8_t data_len);


/**
 * @brief This function is called when the read_it process ends.
 * @note The user must call a function inside this function.
*/
void i2c_read_finished_it();


/**
 * @brief This function is called when the write_it process ends.
 * @note The user must call a function inside this function.
*/
void i2c_write_finished_it();

#endif