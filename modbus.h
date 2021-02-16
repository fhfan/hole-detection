//上位机和下位机通信
//modbus.h头文件

#ifndef MODBUSPP_MODBUS_H
#define MODBUSPP_MODBUS_H

#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#include "stdafx.h"

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <string>
#include <iostream>
#include<vector>
#include <WinSock2.h>
#include<WS2tcpip.h>
#include <stdio.h>
#include<stdlib.h>
#include "modbus_exception.h"


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "502"


using namespace std;


#define MAX_MSG_LENGTH 260

//Function Code
enum {
	READ_COILS = 0x01,			// -1-  Read coil status
	READ_INPUT_BITS = 0x02,		// -2-  Read input status
	READ_REGS = 0x03,			// -3-  Read holding register
	READ_INPUT_REGS = 0x04,		// -4-  Read input register
	WRITE_COIL = 0x05,			// -5-  Write single coil status
	WRITE_REG = 0x06,			// -6-  Write single register
	WRITE_COILS = 0x0F,			// -15- Multiple coil write
	WRITE_REGS = 0x10,			// -16- Multiple register write
};

//Exception Codes
enum {
	EX_ILLEGAL_FUNCTION = 0x01,		// Function Code not Supported
	EX_ILLEGAL_ADDRESS = 0x02,		// Output Address not exists
	EX_ILLEGAL_VALUE = 0x03,		// Output Value not in Range
	EX_SERVER_FAILURE = 0x04,		// Slave Deive Fails to process request
	EX_ACKNOWLEDGE = 0x05,			// Service Need Long Time to Execute
	EX_SERVER_BUSY = 0x06,			// Server Was Unable to Accept MB Request PDU
	EX_GATEWAY_PROBLEMP = 0x0A,		// Gateway Path not Available
	EX_GATEWYA_PROBLEMF = 0x0B,		// Target Device Failed to Response
};
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;

class modbus {

private:
	bool _connected;
	uint16_t PORT;
	int _socket;
	int _msg_id;
	int _slaveid;
	string HOST;
	struct sockaddr_in _server;
	WSADATA wsaData;
	int iResult;


	void modbus_build_request(uint8_t* to_send, int address, int func);
	void modbus_read(int address, int amount, int func);
	void modbus_write(int address, int amount, int func, uint16_t *value);

	ssize_t modbus_send(uint8_t* to_send, int length);
	ssize_t modbus_receive(uint8_t *buffer);

	void modbus_error_handle(uint8_t *msg, int func);


public:
	modbus(string host, uint16_t port);
	modbus(string host);
	~modbus();

	bool modbus_connect();
	void modbus_close();

	void modbus_set_slave_id(int id);

	void modbus_read_coils(int address, int amount, bool* buffer);
	void modbus_read_input_bits(int address, int amount, bool* buffer);
	void modbus_read_holding_registers(int address, int amount, uint16_t *buffer);
	void modbus_read_input_registers(int address, int amount, uint16_t *buffer);

	void modbus_write_coil(int address, bool to_write);
	void modbus_write_register(int address, uint16_t value);

	void modbus_write_coils(int address, int amount, bool* value);
	void modbus_write_registers(int address, int amount, uint16_t *value);
};


#endif //MODBUSPP_MODBUS_H
