#include "stdafx.h"
#include "modbus.h"

using namespace std;
//#include "LogInstance.h"


/**
* Main Constructor of Modbus Object
* @param host IP Address of Host
* @param port Port for Connection
* @return     A Modbus Object
*/
modbus::modbus(string host, uint16_t port) {
	HOST = host;
	PORT = port;
	_slaveid = 1;
	_msg_id = 1;
	_connected = false;

}


/*
* Overloading Modbus Constructor with Default Port at 502
* @param host  IP Address of Host
* @return      A Modbus Object
*/
modbus::modbus(string host) {
	modbus(host, 502);
}


/**
* Destructor of Modbus Object
*/
modbus::~modbus(void) {
	/// No need to deallocate heap memory
	/// these are not pointers hence, their 
	/// allocations are deallocted when they
	/// are out of scope.

	//delete HOST;
	//delete &PORT;
	//delete &_socket;
	//delete _server;
}

/**
* Set Slave ID
* @param id  Id of Slave in Server to Set
*/
void modbus::modbus_set_slave_id(int id) {
	_slaveid = id;
}

/**
* Build up Connection for windows platform only.
* Unix or Linux platform users need to refer to version 1 by Fanzhe
* @return   If A Connection Is Successfully Built
*/
bool modbus::modbus_connect() {
	if (HOST == "" || PORT == 0) {
		std::cout << "Missing Host and Port" << std::endl;
		return false;
	}
	else {
		std::cout << "Found Proper Host " << HOST << " and Port " << PORT << std::endl;
	}
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return true;
	}
	//----------------------
	// Create a SOCKET for connecting to server
	//SOCKET ConnectSocket;
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == -1) {
		std::cout << "Error Opening Socket" << std::endl;
		WSACleanup();
		return true;
	}
	else {
		std::cout << "Socket Opened Successfully" << std::endl;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	_server.sin_family = AF_INET;
	_server.sin_addr.s_addr = inet_addr(HOST.c_str());
	//_server.sin_addr.s_addr = inet_pton(AF_INET, HOST.c_str(), PVOID(PORT));//_WINSOCK_DEPRECATED_NO_WARNINGS
	_server.sin_port = htons(PORT);
	//----------------------
	// Connect to server.
	iResult = connect(_socket, (SOCKADDR *)& _server, sizeof(_server));
	if (iResult < 0) {
		std::cout << "Connection Error" << std::endl;
		WSACleanup();
		return false;
	}
	std::cout << "Connected" << std::endl;
	_connected = true;
	return true;
}




/**
* Close the Connection
*/
void modbus::modbus_close() {
	// close(_socket);
	closesocket(_socket);
	std::cout << "Socket Closed" << std::endl;
}


/**
* Modbus Request Builder
* @param to_send   Message Buffer to Send
* @param address   Reference Address
* @param func      Functional Code
*/
void modbus::modbus_build_request(uint8_t* to_send, int address, int func) {
	to_send[0] = (uint8_t)(_msg_id >> 8);
	to_send[1] = (uint8_t)(_msg_id & 0x00FF);
	to_send[2] = 0;
	to_send[3] = 0;
	to_send[4] = 0;
	to_send[6] = (uint8_t)_slaveid;
	to_send[7] = (uint8_t)func;
	to_send[8] = (uint8_t)(address >> 8);
	to_send[9] = (uint8_t)(address & 0x00FF);
}


/**
* Write Request Builder and Sender
* @param address   Reference Address
* @param amount    Amount to Write
* @param func      Functional Code
* @param value     Value to Write
*/
void modbus::modbus_write(int address, int amount, int func, uint16_t *value) {
	if (func == WRITE_COIL || func == WRITE_REG) {
		uint8_t to_send[12];
		//array<uint8_t> to_send;
		modbus_build_request(to_send, address, func);
		to_send[5] = 6;
		to_send[10] = (uint8_t)(value[0] >> 8);
		to_send[11] = (uint8_t)(value[0] & 0x00FF);
		modbus_send(to_send, 12);
	}
	else if (func == WRITE_REGS) {
		uint8_t* to_send;
		int x = 13 + 2 * amount;
		to_send = new uint8_t[x];
		// uint8_t to_send[13 + 2 * amount];
		modbus_build_request(to_send, address, func);
		to_send[5] = (uint8_t)(7 + 2 * amount);
		to_send[10] = (uint8_t)(amount >> 8);
		to_send[11] = (uint8_t)(amount & 0x00FF);
		to_send[12] = (uint8_t)(2 * amount);
		for (int i = 0; i < amount; i++) {
			to_send[13 + 2 * i] = (uint8_t)(value[i] >> 8);
			to_send[14 + 2 * i] = (uint8_t)(value[i] & 0x00FF);
		}
		modbus_send(to_send, 13 + 2 * amount);
	}
	else if (func == WRITE_COILS) {
		uint8_t* to_send;
		int x = 14 + (amount - 1) / 8;
		to_send = new uint8_t[x];
		//uint8_t to_send[14 + (amount -1) / 8 ];
		modbus_build_request(to_send, address, func);
		to_send[5] = (uint8_t)(7 + (amount - 1) / 8);
		to_send[10] = (uint8_t)(amount >> 8);
		to_send[11] = (uint8_t)(amount >> 8);
		to_send[12] = (uint8_t)((amount + 7) / 8);
		for (int i = 0; i < amount; i++) {
			to_send[13 + (i - 1) / 8] += (uint8_t)(value[i] << (i % 8));
		}
		modbus_send(to_send, 14 + (amount - 1) / 8);
	}
}


/**
* Read Requeset Builder and Sender
* @param address   Reference Address
* @param amount    Amount to Read
* @param func      Functional Code
*/
void modbus::modbus_read(int address, int amount, int func) {
	uint8_t to_send[12];
	modbus_build_request(to_send, address, func);
	to_send[5] = 6;
	to_send[10] = (uint8_t)(amount >> 8);
	to_send[11] = (uint8_t)(amount & 0x00FF);
	modbus_send(to_send, 12);
}


/**
* Read Holding Registers           MODBUS FUNCTION 0x03
* @param address    Reference Address
* @param amount     Amount of Registers to Read
* @param buffer     Buffer to Store Data
*/
void modbus::modbus_read_holding_registers(int address, int amount, uint16_t *buffer) {
	if (_connected) {
		if (amount > 65535 || address > 65535) {
			throw modbus_amount_exception();
		}
		modbus_read(address, amount, READ_REGS);
		uint8_t to_rec[MAX_MSG_LENGTH];
		ssize_t k = modbus_receive(to_rec);
		try {
			modbus_error_handle(to_rec, READ_REGS);
			for (int i = 0; i < amount; i++) {
				buffer[i] = ((uint16_t)to_rec[9 + 2 * i]) << 8;
				buffer[i] += (uint16_t)to_rec[10 + 2 * i];
			}
		}
		/*catch (exception &e) {
		cout << e.what() << endl;
		delete(&to_rec);
		delete(&k);
		throw e;
		}*/
		catch (exception &e) {
			cout << e.what() << endl;
			delete(&to_rec);
			delete(&k);
			throw e;
		}

	}
	else {
		throw modbus_connect_exception();
	}
}


/**
* Read Input Registers             MODBUS FUNCTION 0x04
* @param address     Reference Address
* @param amount      Amount of Registers to Read
* @param buffer      Buffer to Store Data
*/
void modbus::modbus_read_input_registers(int address, int amount, uint16_t *buffer) {
	if (_connected) {
		if (amount > 65535 || address > 65535) {
			throw modbus_amount_exception();
		}
		modbus_read(address, amount, READ_INPUT_REGS);
		uint8_t to_rec[MAX_MSG_LENGTH];
		ssize_t k = modbus_receive(to_rec);
		try {
			modbus_error_handle(to_rec, READ_INPUT_REGS);
			for (int i = 0; i < amount; i++) {
				buffer[i] = ((uint16_t)to_rec[9 + 2 * i]) << 8;
				buffer[i] += (uint16_t)to_rec[10 + 2 * i];
			}
		}
		catch (exception &e) {
			cout << e.what() << endl;
			delete(&to_rec);
			delete(&k);
			throw e;
		}
	}
	else {
		throw modbus_connect_exception();
	}
}


/**
* Read Coils           MODBUS FUNCTION 0x01
* @param address     Reference Address
* @param amount      Amount of Coils to Read
* @param buffer      Buffers to Store Data
*/
void modbus::modbus_read_coils(int address, int amount, bool *buffer) {
	if (_connected) {
		if (amount > 2040 || address > 65535) {
			throw modbus_amount_exception();
		}
		modbus_read(address, amount, READ_COILS);
		uint8_t to_rec[MAX_MSG_LENGTH];
		ssize_t k = modbus_receive(to_rec);
		try {
			modbus_error_handle(to_rec, READ_COILS);
			for (int i = 0; i < amount; i++) {
				buffer[i] = (bool)((to_rec[9 + i / 8] >> (i % 8)) & 1);
			}
		}
		catch (exception &e) {
			cout << e.what() << endl;
			delete(&to_rec);
			delete(&k);
			throw e;
		}
	}
	else {
		throw modbus_connect_exception();
	}
}


/**
* Read Input Bits(Discrete Data)      MODBUS FUNCITON 0x02
* @param address   Reference Address
* @param amount    Amount of Bits to Read
* @param buffer    Buffer to store Data
*/
void modbus::modbus_read_input_bits(int address, int amount, bool* buffer) {
	if (_connected) {
		if (amount > 2040 || address > 65535) {
			throw modbus_amount_exception();
		}
		modbus_read(address, amount, READ_INPUT_BITS);
		uint8_t to_rec[MAX_MSG_LENGTH];
		ssize_t k = modbus_receive(to_rec);
		try {
			modbus_error_handle(to_rec, READ_INPUT_BITS);
			for (int i = 0; i < amount; i++) {
				buffer[i] = (bool)((to_rec[9 + i / 8] >> (i % 8)) & 1);
			}
		}
		catch (exception &e) {

			cout << e.what() << endl;
			delete(&to_rec);
			delete(&k);
			throw e;
		}
	}
	else {
		throw modbus_connect_exception();
	}
}


/**
* Write Single Coils         MODBUS FUNCTION 0x05
* @param address    Reference Address
* @param to_write   Value to Write to Coil
*/
void modbus::modbus_write_coil(int address, bool to_write) {
	if (_connected) {
		if (address > 65535) {
			throw modbus_amount_exception();
		}
		int value = to_write * 0xFF00;
		modbus_write(address, 1, WRITE_COIL, (uint16_t *)&value);
		uint8_t to_rec[MAX_MSG_LENGTH];
		ssize_t k = modbus_receive(to_rec);
		try {
			modbus_error_handle(to_rec, WRITE_COIL);
		}
		catch (exception &e) {
			cout << e.what() << endl;
			delete(&to_rec);
			delete(&k);
			throw e;
		}
	}
	else {
		throw modbus_connect_exception();
	}
}


/**
* Write Single Register        FUCTION 0x06
* @param address   Reference Address
* @param value     Value to Write to Register
*/
void modbus::modbus_write_register(int address, uint16_t value) {
	if (_connected) {
		if (address > 65535) {
			throw modbus_amount_exception();
		}
		modbus_write(address, 1, WRITE_REG, &value);
		uint8_t to_rec[MAX_MSG_LENGTH];
		ssize_t k = modbus_receive(to_rec);
		try {
			modbus_error_handle(to_rec, WRITE_COIL);
		}
		catch (exception &e) {
			cout << e.what() << endl;
			delete (&to_rec);
			delete (&k);
			throw e;
		}
	}
	else {
		throw modbus_connect_exception();
	}
}


/**
* Write Multiple Coils        MODBUS FUNCTION 0x0F
* @param address  Reference Address
* @param amount   Amount of Coils to Write
* @param value    Values to Write
*/
void modbus::modbus_write_coils(int address, int amount, bool *value) {
	if (_connected) {
		if (address > 65535 || amount > 65535) {
			throw modbus_amount_exception();
		}
		uint16_t* temp;
		int x = amount;
		temp = new uint16_t[x];
		//uint16_t temp[amount];
		for (int i = 0; i < x; i++) {
			temp[i] = (uint16_t)value[i];
		}
		modbus_write(address, amount, WRITE_COILS, temp);

		uint8_t to_rec[MAX_MSG_LENGTH];
		ssize_t k = modbus_receive(to_rec);

		try {
			modbus_error_handle(to_rec, WRITE_COILS);
		}
		catch (exception &e) {
			cout << e.what() << endl;
			delete (&to_rec);
			delete (&k);
			throw e;
		}
	}
	else {
		throw modbus_connect_exception();
	}
}


/**
* Write Multiple Registers    MODBUS FUNCION 0x10
* @param address Reference Address
* @param amount  Amount of Value to Write
* @param value   Values to Write
*/
void modbus::modbus_write_registers(int address, int amount, uint16_t *value) {
	if (_connected) {
		if (address > 65535 || amount > 65535) {
			throw modbus_amount_exception();
		}
		modbus_write(address, amount, WRITE_REGS, value);
		uint8_t to_rec[MAX_MSG_LENGTH];
		ssize_t k = modbus_receive(to_rec);
		try {
			modbus_error_handle(to_rec, WRITE_REGS);
		}
		catch (exception &e) {
			cout << e.what() << endl;
			delete (&to_rec);
			delete (&k);
			throw e;
		}
	}
	else {
		throw modbus_connect_exception();
	}
}


/**
* Data Sender
* @param to_send Requeset to Send to Server
* @param length  Length of the Request
* @return        Size of the request
*/
ssize_t modbus::modbus_send(uint8_t* to_send, int length) {

	_msg_id++;
	return send(_socket, (char*)to_send, (size_t)length, 0);
}


/**
* Data Receiver
* @param buffer Buffer to Store the Data
* @return       Size of the Incoming Data
*/
ssize_t modbus::modbus_receive(uint8_t *buffer) {
	return recv(_socket, (char *)buffer, 1024, 0);
}


/**
* Error Code Handler
* @param msg   Message Received from Server
* @param func  Modbus Functional Code
*/
void modbus::modbus_error_handle(uint8_t *msg, int func) {
	if (msg[7] == func + 0x80) {
		switch (msg[8]) {
		case EX_ILLEGAL_FUNCTION:
			throw modbus_illegal_function_exception();
		case EX_ILLEGAL_ADDRESS:
			throw modbus_illegal_address_exception();
		case EX_ILLEGAL_VALUE:
			throw modbus_illegal_data_value_exception();
		case EX_SERVER_FAILURE:
			throw modbus_server_failure_exception();
		case EX_ACKNOWLEDGE:
			throw modbus_acknowledge_exception();
		case EX_SERVER_BUSY:
			throw modbus_server_busy_exception();
		case EX_GATEWAY_PROBLEMP:
		case EX_GATEWYA_PROBLEMF:
			throw modbus_gateway_exception();
		default:
			break;
		}
	}
}
