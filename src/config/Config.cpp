/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:46:37 by mbecker           #+#    #+#             */
/*   Updated: 2025/01/10 11:42:40 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Blocks.hpp"
#include "Tokenizer.hpp"


Config::Config() 
	: _path("")
{}

Config::~Config()
{
	for (vector<ServerConfig *>::iterator it = _servers.begin(); it != _servers.end(); it++)
		delete *it;
}

void Config::parse(string &config_file)
{
	ifstream ifs;
	string tmp;
	vector<string> file;
	
	_path = config_file;
	openFile(ifs);

	while (getline(ifs, tmp))
		file.push_back(tmp);


	Tokenizer tokenizer(file, 0);

	tokenizer.tokenize();

	////while there are servers directives
	//{
	//	// Skip comments and ws
	//	// meet server block: initialise a new ServerConfig node
	//	ServerConfig *new_serv = new ServerConfig();
		
	//	// parse the server block
	//	ServerBlock	serv_block(new_serv, 0);
	//	serv_block.parse(file);
	//	_servers.push_back(new_serv);

	//	//repeat
	//}



	ifs.close();
}

void Config::openFile(ifstream& file)
{
	struct stat path_stat;
    if (stat(_path.c_str(), &path_stat) != 0) 
      throw std::runtime_error("Could not stat path: " + _path + ": " + strerror(errno));

    if (S_ISDIR(path_stat.st_mode)) 
      throw std::runtime_error("Could not open path: " + _path + ": " + strerror(errno));

	file.open(_path.c_str());

	if (!file.is_open() || file.bad() || file.fail())
	{
		if (_path == DEFAULT_CONFIG_FILE)
			throw runtime_error("Could not open default config file: " + _path + ": " + strerror(errno));
		else
			throw runtime_error("Could not open file: " + _path + ": " + strerror(errno));
	}
	else
		cout << "Config file opened successfully." << endl;
}


