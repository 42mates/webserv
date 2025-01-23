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

Config::Config() 
	: _tokens(), _path("DEFAULT")
{
	_std_blocks.push_back("server");
}

Config::~Config()
{
	for (vector<ServerConfig *>::iterator server = _servers.begin(); server != _servers.end(); server++)
		delete *server;
}


vector<ServerConfig *> Config::getServers()
{
	return _servers;
}


/**
 * @brief Opens the file specified by the _path member variable, reads its contents line by line,
 *        stores each line in a vector of strings, and returns the vector.
 * 
 * @return A vector containing the lines of the file as strings.
 * 
 * @throws If the path is invalid, if the path is a directory, or if the file cannot be opened.
 */
vector<string> Config::getFileVector()
{
	ifstream ifs;
	string tmp;
	vector<string> file;

	struct stat path_stat;
    if (stat(_path.c_str(), &path_stat) != 0) 
      throw std::runtime_error("Could not stat path: " + _path + ": " + strerror(errno));

    if (S_ISDIR(path_stat.st_mode)) 
      throw std::runtime_error("Could not open path: " + _path + ": " + strerror(errno));

	ifs.open(_path.c_str());

	if (!ifs.is_open() || ifs.bad())// || ifs.fail())
	{
		if (_path == DEFAULT_CONFIG_FILE)
			throw runtime_error("could not open default config file: " + _path + ": " + strerror(errno));
		else
			throw runtime_error("could not open file: " + _path + ": " + strerror(errno));
	}

	while (getline(ifs, tmp))
		file.push_back(tmp);
	
	ifs.close();
	
	return file;
}

/**
 * @brief Finds the end of a block starting from the given iterator.
 *
 * @param begin An iterator pointing to the start of the block.
 * @return An iterator pointing to the end of the block.
 * @throws runtime_error If the end of the file is reached before finding the end of the block.
 */
vector<Token>::iterator Config::findBlockEnd(vector<Token>::iterator begin)
{
	vector<Token>::iterator end = begin;
	int block_count = 0;

	while (end != _tokens.end())
	{
		if (end->type == TKN_BLOCK_START)
			block_count++;
		else if (end->type == TKN_BLOCK_END)
		{
			block_count--;
			if (block_count == 0)
				return end;
		}
		end++;
	}
	if (block_count != 0)
		throw runtime_error("unexpected end of file, expecting \"" + string(1, BLOCK_END) + "\" in " + _path + ":" + itostr(begin->line));
	return end;
}

void Config::parse(string &config_file)
{
	_path = config_file;

	vector<string> file = getFileVector(); 
	Tokenizer tokenizer(file);

	_tokens = tokenizer.tokenize();

	for (vector<Token>::iterator it = _tokens.begin(); it != _tokens.end(); it++)
	{
		// if not a block known in _std_blocks (like server), throw error
		if (find(_std_blocks.begin(), _std_blocks.end(), it->token) == _std_blocks.end())
			throw runtime_error("unknown directive \"" + it->token + "\" in " + _path + ":" + itostr(it->line));

		it++;

		vector<Token>::iterator end = findBlockEnd(it);
		if (end == it)
			throw runtime_error("empty block in " + _path + ":" + itostr((it - 1)->line));
		vector<Token> block_tokens(it + 1, end);

		ServerConfig *sconfig = new ServerConfig();
		ServerBlock block(sconfig, _path);
		block.process(block_tokens);
	
		_servers.push_back(sconfig);

		it = end;
	}

	printConfig(_servers);
}

