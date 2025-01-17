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


Config::Config() 
	: _path(""), _tokens()
{
	_std_blocks.push_back("server");
}

Config::~Config()
{
	for (vector<ServerConfig *>::iterator it = _servers.begin(); it != _servers.end(); it++)
		delete *it;
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
			throw runtime_error("Could not open default config ifs: " + _path + ": " + strerror(errno));
		else
			throw runtime_error("Could not open ifs: " + _path + ": " + strerror(errno));
	}

	while (getline(ifs, tmp))
		file.push_back(tmp);
	
	ifs.close();
	
	return file;
}

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

	cout << "Tokens length: " << _tokens.size() << endl;
	for (vector<Token>::iterator it = _tokens.begin(); it != _tokens.end(); it++)
	{
		if (find(_std_blocks.begin(), _std_blocks.end(), it->token) == _std_blocks.end()) // not a block known in _std_blocks
			throw runtime_error("unknown directive \"" + it->token + "\" in " + _path + ":" + itostr(it->line));
		it++;

		vector<Token>::iterator end = findBlockEnd(it);
		vector<Token> block_tokens(it, end + 1);

		//if enough tokens to form a block
			//ServerConfig *sconfig = new ServerConfig();

		//Block block(sconfig);
		//block.parse(block_tokens);

		//_servers.push_back(sconfig);

		it = end;
	}


}

