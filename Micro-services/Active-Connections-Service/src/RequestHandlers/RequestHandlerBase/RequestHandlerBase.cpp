/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 15.10.2019
*/

#include "RequestHandlerBase.h"

using namespace RequestHandler;

RequestHandlerBase::RequestHandlerBase(ConnectionManager& connection_manager)
	: _connection_manager(connection_manager) {}

RequestHandlerBase::~RequestHandlerBase() {}
