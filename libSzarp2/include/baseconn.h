#ifndef __BASECONN_H_
#define __BASECONN_H_

#include "serialportconf.h"
#include "szarp_config.h"
#include "exception.h"
#include <string>
#include <vector>

/** Virtual connection using observer pattern
 */

class BaseConnection;

/** Listener receiving notifications from Connection */
class ConnectionListener
{
public:
	/** Callback for informing listener that the connection is open */
	virtual void OpenFinished(const BaseConnection *conn) = 0;
	/** Callback for notifications from SerialPort */
	virtual void ReadData(const BaseConnection *conn, const std::vector<unsigned char>& data) = 0;
	/** Callback for error notifications from SerialPort.
	 * After an error is received, the listener responsible for port
	 * should try to close and open it once again */
	virtual void ReadError(const BaseConnection *conn, short int event) = 0;
	/** Callback for informing listener that his call to
	 * SetConfiguration() has been processed and connection is ready
	 * for writing
	 */
	virtual void SetConfigurationFinished(const BaseConnection *conn) = 0;
};

class ConnectionException : public SzException {
	SZ_INHERIT_CONSTR(ConnectionException, SzException)
};

/** Provides interface to a bufferevent-driven generalized connection.
 * User of this class is responsible for handling exceptions, errors
 * and restarting connection if neccessary.
 * All errors are reported to ConnectionListener via ReadError callback
 */
class BaseConnection
{
public:
	BaseConnection() = default;
	virtual ~BaseConnection() {}

	virtual void Init(UnitInfo* unit) = 0;

	/** Open connection (previously initialized).
	  * Open must result either in OpenFinished or ReadError callback or throw */
	virtual void Open() = 0;
	/** Close connection */
	virtual void Close() = 0;
	/** Write data to connection */
	virtual void WriteData(const void* data, size_t size) = 0;
	/** Returns true if connection is ready for communication */
	virtual bool Ready() const = 0;
	/** Set line configuration for an already open port */
	virtual void SetConfiguration(const SerialPortConfiguration& serial_conf) = 0;

	/** Adds listener for port */
	void AddListener(ConnectionListener* listener)
	{
		m_listeners.push_back(listener);
	}
	/** Removes all listeners */
	void ClearListeners()
	{
		m_listeners.clear();
	}

protected:
	/** Callbacks, notify listeners */
	virtual void OpenFinished();
	void ReadData(const std::vector<unsigned char>& data);
	void SetConfigurationFinished();
	virtual void Error(short int event);

protected:
	std::vector<ConnectionListener*> m_listeners;

public:
	/** Avoid copying */
	BaseConnection(const BaseConnection&) = delete;
	BaseConnection& operator=(const BaseConnection&) = delete;
};

#endif // __BASECONN_H__
