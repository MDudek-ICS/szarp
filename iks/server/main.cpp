#include <iostream>
#include <string>
#include <functional>

#include <boost/program_options.hpp>

#include "net/tcp_server.h"

#include "locations/manager.h"

#include "global_service.h"

namespace po = boost::program_options;

using boost::asio::ip::tcp;

using std::bind;
namespace p = std::placeholders;

int main( int argc , char** argv )
{
	po::options_description desc("Options"); 

	desc.add_options() 
		("help,h", "Print this help messages")
		("port,p", po::value<unsigned>()->default_value(9002), "Server port on which we will listen")
		("locs,L", po::value<std::vector<std::string>>(), "List of locations" );

	po::variables_map vm; 
	try 
	{ 
		po::store(po::parse_command_line(argc, argv, desc),  vm);
		po::store(po::parse_config_file<char>("iks.ini", desc, true),  vm);

		if ( vm.count("help")  ) { 
			std::cout << desc << std::endl; 
			return 0; 
		} 

		po::notify(vm);
	} 
	catch(po::error& e) 
	{ 
		std::cerr << "Options parse error: " << e.what() << std::endl << std::endl; 
		std::cerr << desc << std::endl; 
		return 1; 
	} 

    boost::asio::io_service& io_service = GlobalService::get_service();

	tcp::endpoint endpoint(tcp::v4(), vm["port"].as<unsigned>() );
	TcpServer ts(io_service, endpoint);

	LocationsMgr lm;

	ts.on_connected   ( bind(&LocationsMgr::on_new_connection,&lm,p::_1) );
	ts.on_disconnected( bind(&LocationsMgr::on_disconnected  ,&lm,p::_1) );

	io_service.run();

	return 0; 
}

