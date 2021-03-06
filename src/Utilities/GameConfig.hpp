#ifndef GAMECONFIG_HPP_
#define GAMECONFIG_HPP_

#include <string>
#include <unordered_map>
#include <iostream>

#include "Utilities/INIParser.hpp"

class INIParser;


class GameConfig {
public:



	GameConfig(const std::string& _path);

private:
	INIParser* m_parser;
};

#endif
