#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// For compiling:
// g++ --std=c++17 main.cpp -o main

// Gets OS
#ifdef __linux__
#define LINUX
#endif
#ifdef _WIN32
#define WINDOWS
#endif

// Stores the user's OS 
std::string user_system;
// Stores the executables location
std::string exec_location;
// Stores the location of "config.txt"
std::string config_location;
// Location of "log.txt"
std::string log_location;
// Your Steam Name
std::string username;
// Version
std::string version = "v0.1-5";
// Last Updated
std::string last_updated = "2025-11-13";
// CloD Version
std::string game_ver = "v5.046";
// Used for aborting if OS is not supported
bool _OSUnsupported = false;
// Should the programme show debug info?
bool _debugInfo;
// Should ALL info be kept?
bool _allInfo;
// Should players connecting/disconnecting be shown?
bool _playerConnectionInfo;
bool _otherPlayerConnectionInfo;
// Should player side-switching be shown?
bool _sideSwitchInfo;
bool _otherPlayerSideSwitchInfo;
// Should new missions loading be shown?
bool _newMissionLoadInfo;
// Should new missions loaded be shown?
bool _newMissionInfo;
// Should new battles started be shown?
bool _battleStartedInfo;
// Should battle ending info be shown?
bool _battleEndInfo;
// Should aircraft shot-down info be shown?
bool _destructionInfo;
// Should aircraft shot-down info unrelated to you be shown?
bool _destructionUnrelatedInfo;
// Should landing info be shown?
bool _landingInfo;
bool _otherLandingInfo;
// Should bail info be shown?
bool _bailInfo;
bool _otherBailInfo;
// Should crash info be shown?
bool _crashInfo;
bool _otherCrashInfo;
// Should other player stats be shown?
bool _otherPlayerStats;
// Should chat be shown
bool _showChat;
// Should there be colour
bool _colour;
// Background/Font colour
bool _colourFG;

// Used for getting each line of "log.txt"
std::string log_lines;
// Used for getting each line of "config.txt"
std::string config_lines;
// Stores each line of "log.txt"
std::vector<std::string> log_contents;
// Stores each line of "config.txt"
std::vector<std::string> config_contents;
// Contains processed "log.txt"
std::vector<std::string> log_processed;
// Stores the list of players in the session
// {{ name, side, score, landings, reserve, deaths, crashes, bailouts, connections, disconnections, messages_sent }, ...}
std::vector<std::vector<std::string>> session_players;

// Clears the screen (Not currently used)
void clr();
// Gets value of a config line and returns it
std::string getConfVal(std::string conf_line);
// Loads "config.txt"
void load_config();
// Opens & Loads "log.txt" into a vector
void load_log();
// Processes "log.txt" and gets statistics
void process_log();
// Gets the date and/or time from a string
std::string getDateTime(std::string string);
// Checks if the player is in the "session_players" list
void checkPlayerInList(std::string player_name);
// Modify Player Profiles
void changePlayerSide(std::string player_name, std::string side);
void addPlayerScore(std::string player_name);
void addPlayerLanding(std::string player_name);
void addPlayerDeath(std::string player_name);
void addPlayerCrash(std::string player_name);
void addPlayerBail(std::string player_name);
void addPlayerConnection(std::string player_name);
void addPlayerDisconnection(std::string player_name);
void addPlayerMessageSent(std::string player_name);
// Gets player connecting/disconnecting info
void playerConnectionInfo(std::string string);
// Gets player side switching-info
void playerSideSwitchInfo(std::string string);
// Gets the landing info
void landingInfo(std::string string);
// Gets crash info
void crashInfo(std::string string);
// Gets bail-out info
void bailInfo(std::string string);
// Gets aircraft shot down info
void destructionInfo(std::string string);
// Gets a message sent in chat
void messageInfo(std::string string);
// Prints the processed log
void printProcLog();

int main(int argc, char* argv[]) {
	// Sets "user_system"
#ifdef LINUX
	user_system = "Linux";
#endif
#ifdef WINDOWS
	user_system = "Windows";
#endif
	if (user_system == "Windows" || user_system == "Linux") {
		std::cout << "--! " << user_system << " was detected.\n";
		if (user_system == "Linux") {
			std::cout << "    (Sorry... \"GNU/Linux\".)\n";
		}
	}
	else {
		std::cout << "--! Your OS is unsupported. Please try again on Windows or Linu... GNU/Linux.\n";
		_OSUnsupported = true;
	}

	// Sets the location for "config.txt"
	// Should be: $MainExecDir/config/config.txt
	std::string argv_str(argv[0]);
	if (user_system == "Windows") {
		exec_location = argv_str.substr(0, argv_str.find_last_of("\\"));
		config_location = ".\\config\\config.txt";
	}
	else if (user_system == "Linux") {
		exec_location = argv_str.substr(0, argv_str.find_last_of("/"));
		config_location = exec_location + "/config/config.txt";
	}

	// Main programme
	if (!_OSUnsupported) {
		load_config();
		if (_colour) {
		std::cout << "\n\033[32;40mRamble's CloD \033[31;40mServer \033[37;40mActivity \033[34;40mStatistics\033[32;40m - " << version << "\033[39m\033[49m\n";
		}
		else {
			std::cout << "\nRamble's CloD Server Activity Statistics - " << version << "\n";
		}
		std::cout << "Last Updated: "<< last_updated << "; CloD Version: " << game_ver << ";\n\n";
		load_log();
		process_log();
		printProcLog();
	}
}

void clr() {
	if (user_system == "Linux") {
		std::cout << "\033[2J";
	}
	else if (user_system == "Windows") {
		system("cls");
	}
}

std::string getConfVal(std::string conf_line) {
	std::string line = conf_line;
	int x = line.find_last_of("=");
	line = line.substr(x + 1);
	return line;
}

void load_config() {
	std::fstream config(config_location, std::ios::in);
	// Checks if "log.txt" is found or not
	if (!config.is_open()) { std::cout << "--! \"config.txt\" was not found.\n--> Please enter the correct location for \"config.txt\"!\n"; }
	else { std::cout << "--! \"config.txt\" was opened successfully!\n"; }

	// Write "log" to "log_contents"
	while (std::getline(config, config_lines)) { config_contents.push_back(config_lines); }
	// Checks for end of file
	if (config.eof()) { std::cout << "--! Finished loading \"config.txt\"!\n\n"; }
	else { std::cout << "--! Error reading \"config.txt\"; Failed to load it into the programme!\n\n"; }
	// Closes "log.txt"
	config.close();

	// Sets vars
	/*
	On Linux, when getting the string values, such as the user's Steam
	username, it adds a mysterious extra character on the end which messes
	the programme up.
	*/
	if (user_system == "Linux") {
		log_location = getConfVal(config_contents[0]);
		log_location = log_location.substr(0, log_location.length() - 1);
		username = getConfVal(config_contents[1]);
		username = username.substr(0, username.length() - 1);
	}
	else {
		log_location = getConfVal(config_contents[0]);
		username = getConfVal(config_contents[1]);
	}
	std::istringstream(getConfVal(config_contents[2])) >> std::boolalpha >> _debugInfo;
	std::istringstream(getConfVal(config_contents[3])) >> std::boolalpha >> _allInfo;
	std::istringstream(getConfVal(config_contents[4])) >> std::boolalpha >> _playerConnectionInfo;
	std::istringstream(getConfVal(config_contents[5])) >> std::boolalpha >> _otherPlayerConnectionInfo;
	std::istringstream(getConfVal(config_contents[6])) >> std::boolalpha >> _sideSwitchInfo;
	std::istringstream(getConfVal(config_contents[7])) >> std::boolalpha >> _otherPlayerSideSwitchInfo;
	std::istringstream(getConfVal(config_contents[8])) >> std::boolalpha >> _newMissionLoadInfo;
	std::istringstream(getConfVal(config_contents[9])) >> std::boolalpha >> _newMissionInfo;
	std::istringstream(getConfVal(config_contents[10])) >> std::boolalpha >> _battleStartedInfo;
	std::istringstream(getConfVal(config_contents[11])) >> std::boolalpha >> _battleEndInfo;
	std::istringstream(getConfVal(config_contents[12])) >> std::boolalpha >> _destructionInfo;
	std::istringstream(getConfVal(config_contents[13])) >> std::boolalpha >> _destructionUnrelatedInfo;
	std::istringstream(getConfVal(config_contents[14])) >> std::boolalpha >> _landingInfo;
	std::istringstream(getConfVal(config_contents[15])) >> std::boolalpha >> _otherLandingInfo;
	std::istringstream(getConfVal(config_contents[16])) >> std::boolalpha >> _bailInfo;
	std::istringstream(getConfVal(config_contents[17])) >> std::boolalpha >> _otherBailInfo;
	std::istringstream(getConfVal(config_contents[18])) >> std::boolalpha >> _crashInfo;
	std::istringstream(getConfVal(config_contents[19])) >> std::boolalpha >> _otherCrashInfo;
	std::istringstream(getConfVal(config_contents[20])) >> std::boolalpha >> _otherPlayerStats;
	std::istringstream(getConfVal(config_contents[21])) >> std::boolalpha >> _showChat;
	std::istringstream(getConfVal(config_contents[22])) >> std::boolalpha >> _colour;
	std::istringstream(getConfVal(config_contents[23])) >> std::boolalpha >> _colourFG;

	// Print vars if _debugInfo is true
	if (_debugInfo) {
		std::cout << "--! Log Location: \"" << log_location << "\".\n";
		std::cout << "--! Steam Username: \"" << username << "\".\n";
		std::cout << "--! Show Debug Info: \"" << _debugInfo << "\".\n";
		std::cout << "--! Show All Info: \"" << _allInfo << "\".\n";
		std::cout << "--! Show Player Connection Info: \"" << _playerConnectionInfo << "\".\n";
		std::cout << "--! Show Other Player Connection Info: \"" << _otherPlayerConnectionInfo << "\".\n";
		std::cout << "--! Show New Mission Loading Info: \"" << _newMissionLoadInfo << "\".\n";
		std::cout << "--! Show New Mission Info: \"" << _newMissionInfo << "\".\n";
		std::cout << "--! Show Battle Start Info: \"" << _battleStartedInfo << "\".\n";
		std::cout << "--! Show Battle End Info: \"" << _battleEndInfo << "\".\n";
		std::cout << "--! Show Destruction Info: \"" << _destructionInfo << "\".\n";
		std::cout << "--! Show Unrelated Destruction Info: \"" << _destructionUnrelatedInfo << "\".\n";
		std::cout << "--! Show Landing Info: \"" << _landingInfo << "\".\n";
		std::cout << "--! Show Unrelated Landing Info: \"" << _otherLandingInfo << "\".\n";
		std::cout << "--! Show Bail-Out Info: \"" << _bailInfo << "\".\n";
		std::cout << "--! Show Unrelated Bail-Out Info: \"" << _otherBailInfo << "\".\n";
		std::cout << "--! Show Crash Info: \"" << _crashInfo << "\".\n";
		std::cout << "--! Show Unrelated Crash Info: \"" << _otherCrashInfo << "\".\n";
		std::cout << "--! Show Other Player Profiles: \"" << _otherPlayerStats << "\".\n";
		std::cout << "--! Show Chat: \"" << _showChat << "\".\n";
		std::cout << "--! Colour Enabled: \"" << _colour << "\"\n";
		std::cout << "--! Colour Mode: \"" << _colourFG << "\"\n";
		std::cout << "\n";
	}
}

void load_log() {
	std::fstream log(log_location, std::ios::in);
	// Checks if "log.txt" is found or not
	if (!log.is_open() && _debugInfo) { std::cout << "--! \"log.txt\" was not found.\n--> Please enter the correct location for \"log.txt\"!\n"; }
	else { if (_debugInfo) { std::cout << "--! \"log.txt\" was opened successfully!\n"; } }

	// Write "log" to "log_contents"
	while (std::getline(log, log_lines)) { log_contents.push_back(log_lines); }
	// Checks for end of file
	if (log.eof() && _debugInfo) { std::cout << "--! Finished loading \"log.txt\"!\n\n"; }
	else { if (_debugInfo) { std::cout << "--! Error reading \"log.txt\"; Failed to load it into the programme!\n\n"; } }
	// Closes "log.txt"
	log.close();
}

void process_log() {
	// If ALL info was kept, then print it out:
	if (_allInfo) {
		std::cout << "\n";
		for (int i = 0; i < log_contents.size(); i++) { std::cout << log_contents[i] << "\n"; }
	}
	// Else, process the log:
	else {
		// Loops through "log_contents" and processes information
		for (int i = 0; i < log_contents.size(); i++) {
			// If chat message
			if (log_contents[i].find("Chat: ") != std::string::npos) {
				// Gets the message sender, and then the message
				messageInfo(log_contents[i]);
			}
			// If not a chat message
			else {
				// If element is the "BEGIN log" header AND "_sessionInfo" is true
				if ((log_contents[i].find("BEGIN log session") != std::string::npos)) {
					std::string message = "--> You started playing Cliffs of Dover at: ";
					message.append(getDateTime(log_contents[i]));
					log_processed.push_back(message);
				}
				// If element is the "END log" header AND "_sessionInfo" is true
				if ((log_contents[i].find("END log session") != std::string::npos)) {
					std::string message = "--> You stopped playing Cliffs of Dover at: ";
					message.append(getDateTime(log_contents[i]));
					log_processed.push_back(message);
				}
				// Checks for player connection/disconnection info
				playerConnectionInfo(log_contents[i]);
				// Checks for player side-switching info
				playerSideSwitchInfo(log_contents[i]);
				// Checks for landing info
				landingInfo(log_contents[i]);
				// Checks for crash info
				crashInfo(log_contents[i]);
				// Checks for bail-out info
				bailInfo(log_contents[i]);
				// Checks for mission loading info
				if (_newMissionLoadInfo && (log_contents[i].find("Loading mission ") != std::string::npos)) {
					// Gets battle name info
					std::string mission_name = log_contents[i];
					int x = mission_name.find_first_of("m");
					mission_name = mission_name.substr(x + 8);
					x = mission_name.find_last_of(".");
					mission_name = mission_name.substr(0, x - 3);
					// Puts together message and sends it
					std::string message = "! \"";
					message.append(mission_name);
					message.append("\" started loading at ");
					message.append(getDateTime(log_contents[i]));
					message.append(".");
					log_processed.push_back(message);
				}
				// Checks for new mission loaded info
				if (_newMissionInfo && (log_contents[i].find("Mission loaded. time =") != std::string::npos)) {
					std::string message = "! New mission was loaded at ";
					message.append(getDateTime(log_contents[i]));
					message.append(".");
					log_processed.push_back(message);
				}
				// Checks for battle starting info
				if (_battleStartedInfo && (log_contents[i].find("Server: Battle begins!") != std::string::npos)) {
					std::string message = "! A New battle was started at ";
					message.append(getDateTime(log_contents[i]));
					message.append(".");
					log_processed.push_back(message);
				}
				// Checks for battle ending info
				if (_battleEndInfo && (log_contents[i].find("Server: The battle ends.") != std::string::npos)) {
					std::string message = "! The battle ended at ";
					message.append(getDateTime(log_contents[i]));
					message.append(".");
					log_processed.push_back(message);
				}
				// Checks for aircraft shot down
				if (_destructionInfo) {
					destructionInfo(log_contents[i]);
				}
			}
		}
	}
}

void printProcLog() {
	// Print Processed Log
	// * = Kill
	// _ = Crash/Failed Bail
	// = = Landing/Landed Bail
	// ^ = Bail Out
	// # = Connection/Side Switch
	// >> = Chat
	// --> =
	for (auto i : log_processed) {
		if (_colour) {
			if (_colourFG) {
				// Font
				if (i.find("*") != std::string::npos) {
					// Red
					std::cout << "\033[31;40m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("^") !=std::string::npos) {
					// Bright Blue
					std::cout << "\033[94;40m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("#") != std::string::npos) {
					// Bright Cyan
					std::cout << "\033[96;40m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find(">>") != std::string::npos) {
					// No Colour: Chat
					std::cout << i << "\n";
				}
				else if (i.find("-->") != std::string::npos) {
					// Bright Red
					std::cout << "\033[91;40m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("!") != std::string::npos) {
					// Bright Green
					std::cout << "\033[92;40m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("=") != std::string::npos) {
					// Green
					std::cout << "\033[32;40m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("_") != std::string::npos) {
					// Bright Yellow
					std::cout << "\033[93;40m" << i << "\033[39m\033[49m\n";
				}
				else {
					// No Colour
					std::cout << i << "\n";
				}
			}
			else {
				// Background
				if (i.find("*") != std::string::npos) {
					// Red
				std::cout << "\033[30;41m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("^") !=std::string::npos) {
					// Bright Blue
					std::cout << "\033[30;104m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("#") != std::string::npos) {
					// Bright Cyan
					std::cout << "\033[30;106m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find(">>") != std::string::npos) {
					// No Colour: Chat
					std::cout << i << "\n";
				}
				else if (i.find("-->") != std::string::npos) {
					// Bright Red
					std::cout << "\033[30;101m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("!") != std::string::npos) {	
					// Bright Green
					std::cout << "\033[30;102m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("=") != std::string::npos) {
					// Green
					std::cout << "\033[30;42m" << i << "\033[39m\033[49m\n";
				}
				else if (i.find("_") != std::string::npos) {
					// Bright Yellow
					std::cout << "\033[30;103m" << i << "\033[39m\033[49m\n";
				}
				else {
					// No Colour
					std::cout << i << "\n";
				}
			}
		}
		else {
			std::cout << i << "\n";
		}
	}

	// Prints player list
	std::cout << "\n";
	for (int i = 0; i < session_players.size(); i++) {
		if (_otherPlayerStats) {
			std::cout << "-> User: \"" << session_players[i][0] << "\""
				<< "\n#  Last recorded on the \"" << session_players[i][1] << "\" team."
				<< "\n*  With a recorded score of \"" << session_players[i][2] << "\"."
				<< "\n-  With a recorded landing count of : \"" << session_players[i][3] << "\""
				<< "\n!  With a recorded \"" << session_players[i][5] << "\" death(s)."
				<< "\n!  With a recorded \"" << session_players[i][6] << "\" crash(es)."
				<< "\n!  With a recorded \"" << session_players[i][7] << "\" bail-out(s)."
				<< "\n%  With a recorded \"" << session_players[i][8] << "\" connection(s)."
				<< "\n%  With a recorded \"" << session_players[i][9] << "\" disconnection(s)."
				<< "\n%  With a recorded \"" << session_players[i][10] << "\" message(s) sent.\n";
		}
		else {
			if (session_players[i][0] == username) {
				std::cout << "-> User: \"" << session_players[i][0] << "\""
					<< "\n#  Last recorded on the \"" << session_players[i][1] << "\" team."
					<< "\n*  With a recorded score of \"" << session_players[i][2] << "\"."
					<< "\n-  With a recorded landing count of: \"" << session_players[i][3] << "\""
					<< "\n!  With a recorded \"" << session_players[i][5] << "\" death(s)."
					<< "\n!  With a recorded \"" << session_players[i][6] << "\" crash(es)."
					<< "\n!  With a recorded \"" << session_players[i][7] << "\" bail-out(s)."
					<< "\n%  With a recorded \"" << session_players[i][8] << "\" connection(s)."
					<< "\n%  With a recorded \"" << session_players[i][9] << "\" disconnection(s)."
					<< "\n%  With a recorded \"" << session_players[i][10] << "\" message(s) sent.\n";
			}
		}
	}
}

std::string getDateTime(std::string string) {
	std::string element = string;
	std::string dateTime;
	int start = element.find_first_of('[');
	int end;
	dateTime = element.substr(start);
	end = dateTime.find_first_of(']');
	dateTime = dateTime.substr(0, end + 1);

	return dateTime;
}

void playerSideSwitchInfo(std::string string) {
	if (string.find("enlists in the") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " enlists in the ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 16);
		// Gets the side that the player switched to
		char end_side[] = " air force.";
		res = -1;
		y = 0;
		while ((res = element_mod.find(end_side, res + 1)) != std::string::npos) { y = res; }
		std::string get_side = element_mod.substr(0, y);
		// Sends message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else {
			if (element.find(username) != std::string::npos && _sideSwitchInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("will fly for the") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " will fly for the ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 18);
		// Gets the side that the player switched to
		char end_side[] = " forces.";
		res = -1;
		y = 0;
		while ((res = element_mod.find(end_side, res + 1)) != std::string::npos) { y = res; }
		std::string get_side = element_mod.substr(0, y);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else {
			if (element.find(username) != std::string::npos && _sideSwitchInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("joins the") != std::string::npos && string.find(" army.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " joins the ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 11);
		// Gets the side that the player switched to
		char end_side[] = " army.";
		res = -1;
		y = 0;
		while ((res = element_mod.find(end_side, res + 1)) != std::string::npos) { y = res; }
		std::string get_side = element_mod.substr(0, y);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else {
			if (element.find(username) != std::string::npos && _sideSwitchInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("has changed to the") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " has changed to the ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 20);
		// Gets the side that the player switched to
		char end_side[] = " side.";
		res = -1;
		y = 0;
		while ((res = element_mod.find(end_side, res + 1)) != std::string::npos) { y = res; }
		std::string get_side = element_mod.substr(0, y);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else {
			if (element.find(username) != std::string::npos && _sideSwitchInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("is now with the") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " is now with the ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 17);
		// Gets the side that the player switched to
		char end_side[] = " army.";
		res = -1;
		y = 0;
		while ((res = element_mod.find(end_side, res + 1)) != std::string::npos) { y = res; }
		std::string get_side = element_mod.substr(0, y);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else {
			if (element.find(username) != std::string::npos && _sideSwitchInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("has switched to") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " has switched to ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 17);
		// Gets the side that the player switched to
		x = element_mod.find_last_of(".");
		std::string get_side = element_mod.substr(0, x);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else {
			if (element.find(username) != std::string::npos && _sideSwitchInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
}

void playerConnectionInfo(std::string string) {
	// Player connection
	if (string.find("enters the battle.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " enters the battle";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_playerConnectionInfo && _otherPlayerConnectionInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
		}
		else {
			if (element.find(username) != std::string::npos && _playerConnectionInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerConnection(get_name);
	}
	else if (string.find("is now connected.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " is now connected.";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_playerConnectionInfo && _otherPlayerConnectionInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
		}
		else {
			if (element.find(username) != std::string::npos && _playerConnectionInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerConnection(get_name);
	}
	else if (string.find(" joins the server.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " joins the server.";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_playerConnectionInfo && _otherPlayerConnectionInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
		}
		else {
			if (element.find(username) != std::string::npos && _playerConnectionInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerConnection(get_name);
	}
	// Player disconnection
	if (string.find("Server: Goodbye, ") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_first_of(",");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		x = element_mod.find_last_of("!");
		std::string get_name = element_mod.substr(0, x);
		// Sends the message
		// Checks if it should
		if (_playerConnectionInfo && _otherPlayerConnectionInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " disconnected from the server.");
		}
		else {
			if (element.find(username) != std::string::npos && _playerConnectionInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " disconnected from the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerDisconnection(get_name);
	}
	else if (string.find(" leaves the server.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		char end_name[] = " leaves the server.";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_playerConnectionInfo && _otherPlayerConnectionInfo) {
			log_processed.push_back("# At " + time + " " + get_name + " disconnected from the server.");
		}
		else {
			if (element.find(username) != std::string::npos && _playerConnectionInfo) {
				log_processed.push_back("# At " + time + " " + get_name + " disconnected from the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerDisconnection(get_name);
	}
}

void landingInfo(std::string string) {
	if (string.find("returned to base") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username
		char end_name[] = " returned to base.";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_landingInfo && _otherLandingInfo) {
			log_processed.push_back("= At " + time + " " + get_name + " landed.");
		}
		else {
			if (element.find(username) != std::string::npos && _landingInfo) {
				log_processed.push_back("= At " + time + " " + get_name + " landed.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
			checkPlayerInList(get_name);
			addPlayerLanding(get_name);
		}
	}
	else if (string.find("is safe on the ground") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username
		char end_name[] = " is safe on the ground.";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_landingInfo && _otherLandingInfo) {
			log_processed.push_back("= At " + time + " " + get_name + " landed.");
		}
		else {
			if (element.find(username) != std::string::npos && _landingInfo) {
				log_processed.push_back("= At " + time + " " + get_name + " landed.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
			checkPlayerInList(get_name);
			addPlayerLanding(get_name);
		}
	}
	else if (string.find("crash landed") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username
		char end_name[] = " crash landed.";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_landingInfo && _otherLandingInfo) {
			log_processed.push_back("= At " + time + " " + get_name + " crash landed.");
		}
		else {
			if (element.find(username) != std::string::npos && _landingInfo) {
				log_processed.push_back("= At " + time + " " + get_name + " crash landed.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
			checkPlayerInList(get_name);
			addPlayerLanding(get_name);
		}
	}
	else if (string.find("landed") != std::string::npos && string.find("crash landed") == std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username
		char end_name[] = " landed.";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_landingInfo && _otherLandingInfo) {
			log_processed.push_back("= At " + time + " " + get_name + " landed.");
		}
		else {
			if (element.find(username) != std::string::npos && _landingInfo) {
				log_processed.push_back("= At " + time + " " + get_name + " landed.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
			checkPlayerInList(get_name);
			addPlayerLanding(get_name);
		}
	}
}

void crashInfo(std::string string) {
	if (string.find(" collided with terrain.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the player/AI and aircraft information substring
		char end_info[] = " collided with terrain";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { y = res; }
		std::string aircraft_pilot = element_mod.substr(0, y);
		// Gets the information
		// "<aircraft> flown by <name>"
		// "<name> in a <aircraft>"
		// "<player>'s <aircraft>"
		if (aircraft_pilot.find(" flown by ") != std::string::npos) {
			// Gets index of beginning
			char end_name[] = " flown by ";
			res = -1;
			y = 0;
			while ((res = aircraft_pilot.find(end_name, res + 1)) != std::string::npos) { y = res; }
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(0, y);
			// Gets the player info
			std::string get_name = aircraft_pilot.substr(y + 10);
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo) {
				log_processed.push_back(message);
			}
			else {
				if (element.find(username) != std::string::npos && _crashInfo) {
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else if (aircraft_pilot.find(" in a ") != std::string::npos) {
			// Gets index of beginning
			char end_name[] = " in a ";
			res = -1;
			y = 0;
			while ((res = aircraft_pilot.find(end_name, res + 1)) != std::string::npos) { y = res; }
			// Gets the name info
			std::string get_name = aircraft_pilot.substr(0, y);
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(y + 6);
			// Message
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo) {
				log_processed.push_back(message);
			}
			else {
				if (element.find(username) != std::string::npos && _crashInfo) {
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else if (aircraft_pilot.find("’") != std::string::npos) {
			// Gets index of beginning
			int x = aircraft_pilot.find_first_of("’");
			// Gets the name info
			std::string get_name = aircraft_pilot.substr(0, x);
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(x + 5);
			// Message
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo) {
				log_processed.push_back(message);
			}
			else {
				if (element.find(username) != std::string::npos && _crashInfo) {
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else {
			// AI
			std::string message = "_ At " + time + " " + aircraft_pilot + " crashed.";
			if (_crashInfo && _otherCrashInfo) {
				log_processed.push_back(message);
			}
			else {
				if (element.find(username) != std::string::npos && _crashInfo) {
					log_processed.push_back(message);
				}
			}
		}
	}
	else if (string.find(" crashed.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the player/AI and aircraft information substring
		char end_info[] = " crashed.";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { y = res; }
		std::string aircraft_pilot = element_mod.substr(0, y);
		// Gets the information
		// "<aircraft> flown by <name>"
		// "<name> in a <aircraft>"
		// "<player>'s <aircraft>"
		if (aircraft_pilot.find(" flown by ") != std::string::npos) {
			// Gets index of beginning
			char end_name[] = " flown by ";
			res = -1;
			y = 0;
			while ((res = aircraft_pilot.find(end_name, res + 1)) != std::string::npos) { y = res; }
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(0, y);
			// Gets the player info
			std::string get_name = aircraft_pilot.substr(y + 10);
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo) {
				log_processed.push_back(message);
			}
			else {
				if (element.find(username) != std::string::npos && _crashInfo) {
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else if (aircraft_pilot.find(" in a ") != std::string::npos) {
			// Gets index of beginning
			char end_name[] = " in a ";
			res = -1;
			y = 0;
			while ((res = aircraft_pilot.find(end_name, res + 1)) != std::string::npos) { y = res; }
			// Gets the name info
			std::string get_name = aircraft_pilot.substr(0, y);
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(y + 6);
			// Message
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo) {
				log_processed.push_back(message);
			}
			else {
				if (element.find(username) != std::string::npos && _crashInfo) {
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else if (aircraft_pilot.find("’") != std::string::npos) {
			// Gets index of beginning
			int x = aircraft_pilot.find_first_of("’");
			// Gets the name info
			std::string get_name = aircraft_pilot.substr(0, x);
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(x + 5);
			// Message
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo) {
				log_processed.push_back(message);
			}
			else {
				if (element.find(username) != std::string::npos && _crashInfo) {
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else {
			// AI
			std::string message = "_ At " + time + " " + aircraft_pilot + " crashed.";
			if (_crashInfo && _otherCrashInfo) {
				log_processed.push_back(message);
			}
			else {
				if (element.find(username) != std::string::npos && _crashInfo) {
					log_processed.push_back(message);
				}
			}
		}
	}
}

void bailInfo(std::string string) {
	// Bailing Out
	if (string.find(" had to jump.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (element_mod.find(" of an ") != std::string::npos) {
			// <crew name> of an <aircraft> had to jump.
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos) {
				// Get beginning of " had to jump."
				char end_info[] = " had to jump.";
				int y = 0;
				int res = -1;
				while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { y = res; }
				std::string info = element_mod.substr(0, y);
				// Separates the crew and aircraft
				char end_name[] = " of an AI ";
				y = 0;
				res = -1;
				while ((res = info.find(end_name, res + 1)) != std::string::npos) { y = res; }
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 10);
				// Sends message
				if (_bailInfo && _otherBailInfo) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else if (element_mod.find(" of a ") != std::string::npos) {
			// <crew name> of a <aircraft> had to jump.
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos) {
				// Get beginning of " had to jump."
				char end_info[] = " had to jump.";
				int y = 0;
				int res = -1;
				while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { y = res; }
				std::string info = element_mod.substr(0, y);
				// Separate the crew and aircraft
				char end_name[] = " of a ";
				y = 0;
				res = -1;
				while ((res = info.find(end_name, res + 1)) != std::string::npos) { y = res; }
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 6);
				// Gets the end of "AI" in <get_name>
				char end_ai[] = "AI ";
				y = 0;
				res = -1;
				while ((res = get_name.find(end_ai, res + 1)) != std::string::npos) { y = res; }
				get_name = get_name.substr(y + 3);
				// Sends message
				if (_bailInfo && _otherBailInfo) {
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos) {
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else {
			// <aircraft> <username> had to jump.
			// Checks if does not contain "AI"
			if (element_mod.find("AI") == std::string::npos) {
				x = element_mod.find_last_of(")");
				std::string get_aircraft = element_mod.substr(0, x + 1);
				std::string get_name = element_mod.substr(x + 2);
				char end_name[] = " had to jump.";
				int y = 0;
				int res = -1;
				while ((res = get_name.find(end_name, res + 1)) != std::string::npos) { y = res; }
				get_name = get_name.substr(0, y);
				// Sends the message
				if (_bailInfo && _otherBailInfo) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				// Adds bail-out info to user profile
				// If get_name contains "and crew", separate it first
				if (get_name.find(" and crew") != std::string::npos) {
					char end_username[] = " and crew";
					y = 0;
					res = -1;
					while ((res = get_name.find(end_username, res + 1)) != std::string::npos) { y = res; }
					get_name = get_name.substr(0, y);
				}
				checkPlayerInList(get_name);
				addPlayerBail(get_name);
			}
		}
	}
	else if (string.find(" bailed out.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (element_mod.find(" of an ") != std::string::npos) {
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos) {
				char end_info[] = " bailed out.";
				int y = 0;
				int res = -1;
				while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { y = res; }
				std::string info = element_mod.substr(0, y);
				// Separates the crew and aircraft
				char end_name[] = " of an AI ";
				y = 0;
				res = -1;
				while ((res = info.find(end_name, res + 1)) != std::string::npos) { y = res; }
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 10);
				// Sends message
				if (_bailInfo && _otherBailInfo) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else if (element_mod.find(" of a ") != std::string::npos) {
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos) {
				char end_info[] = " bailed out.";
				int y = 0;
				int res = -1;
				while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { y = res; }
				std::string info = element_mod.substr(0, y);
				// Separate the crew and aircraft
				char end_name[] = " of a ";
				y = 0;
				res = -1;
				while ((res = info.find(end_name, res + 1)) != std::string::npos) { y = res; }
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 6);
				// Gets the end of "AI" in <get_name>
				char end_ai[] = "AI ";
				y = 0;
				res = -1;
				while ((res = get_name.find(end_ai, res + 1)) != std::string::npos) { y = res; }
				get_name = get_name.substr(y + 3);
				// Sends message
				if (_bailInfo && _otherBailInfo) {
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos) {
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else {
			// Checks if does not contain "AI"
			if (element_mod.find("AI") == std::string::npos) {
				x = element_mod.find_last_of(")");
				std::string get_aircraft = element_mod.substr(0, x + 1);
				std::string get_name = element_mod.substr(x + 2);
				char end_name[] = " bailed out.";
				int y = 0;
				int res = -1;
				while ((res = get_name.find(end_name, res + 1)) != std::string::npos) { y = res; }
				get_name = get_name.substr(0, y);
				// Sends the message
				if (_bailInfo && _otherBailInfo) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				// Adds bail-out info to user profile
				// If get_name contains "and crew", separate it first
				if (get_name.find(" and crew") != std::string::npos) {
					char end_username[] = " and crew";
					y = 0;
					res = -1;
					while ((res = get_name.find(end_username, res + 1)) != std::string::npos) { y = res; }
					get_name = get_name.substr(0, y);
				}
				checkPlayerInList(get_name);
				addPlayerBail(get_name);
			}
		}
	}
	else if (string.find(" hit the silk.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (element_mod.find(" of an ") != std::string::npos) {
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos) {
				char end_info[] = " hit the silk.";
				int y = 0;
				int res = -1;
				while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { y = res; }
				std::string info = element_mod.substr(0, y);
				// Separates the crew and aircraft
				char end_name[] = " of an AI ";
				y = 0;
				res = -1;
				while ((res = info.find(end_name, res + 1)) != std::string::npos) { y = res; }
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 10);
				// Sends message
				if (_bailInfo && _otherBailInfo) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else if (element_mod.find(" of a ") != std::string::npos) {
			if (element_mod.find("AI") != std::string::npos) {
				// Is AI
				char end_info[] = " hit the silk.";
				int y = 0;
				int res = -1;
				while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { y = res; }
				std::string info = element_mod.substr(0, y);
				// Separate the crew and aircraft
				char end_name[] = " of a ";
				y = 0;
				res = -1;
				while ((res = info.find(end_name, res + 1)) != std::string::npos) { y = res; }
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 6);
				// Gets the end of "AI" in <get_name>
				char end_ai[] = "AI ";
				y = 0;
				res = -1;
				while ((res = get_name.find(end_ai, res + 1)) != std::string::npos) { y = res; }
				get_name = get_name.substr(y + 3);
				// Sends message
				if (_bailInfo && _otherBailInfo) {
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos) {
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
			else {
				// Not AI
				char end_info[] = " hit the silk.";
				int y = 0;
				int res = -1;
				while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { y = res; }
				std::string info = element_mod.substr(0, y);
				// Separate the crew and the aircraft
				char end_name[] = " of a ";
				y = 0;
				res = -1;
				while ((res = info.find(end_name, res + 1)) != std::string::npos) { y = res; }
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 6);
				// Gets the username in " The <username>
				char end_ai[] = "The ";
				y = 0;
				res = -1;
				while ((res = get_name.find(end_ai, res + 1)) != std::string::npos) { y = res; }
				get_name = get_name.substr(y + 4);
				// Sends message
				if (_bailInfo && _otherBailInfo) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos) {
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				// Adds bail-out info to user profile
				// If get_name contains "and crew", separate it first
				if (get_name.find(" and crew") != std::string::npos) {
					char end_username[] = " and crew";
					y = 0;
					res = -1;
					while ((res = get_name.find(end_username, res + 1)) != std::string::npos) { y = res; }
					get_name = get_name.substr(0, y);
				}
				checkPlayerInList(get_name);
				addPlayerBail(get_name);
			}
		}

	}
	// Landed on the ground
	else if (string.find(" parachuted to the ground.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (_bailInfo && _otherBailInfo) {
			log_processed.push_back("= At " + get_time + " " + element_mod);
		}
		else if (_bailInfo && element_mod.find(username) != std::string::npos) {
			log_processed.push_back("= At " + get_time + " " + element_mod);
		}
	}
	// Splatted
	// Parachute Failed to Deploy
	else if (string.find("'s parachute failed to deploy.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (_bailInfo && _otherBailInfo) {
			log_processed.push_back("_ At " + get_time + " " + element_mod);
		}
		else if (_bailInfo && element_mod.find(username) != std::string::npos) {
			log_processed.push_back("= At " + get_time + " " + element_mod);
		}
	}
}

void destructionInfo(std::string string) {
	if (string.find("was destroyed.") != std::string::npos) {
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		// Remove first part of string ([xx:xx:xx] Server: )
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Remove end part of string ([...] was destroyed.)
		x = 0;
		char end_info[] = " was destroyed.";
		int res = -1;
		while ((res = element_mod.find(end_info, res + 1)) != std::string::npos) { x = res; }
		element_mod = element_mod.substr(0, x);

		// "’" = <player>’s <aircraft>
		// " in a " = <name> in a <aircraft>
		// " flown by " = <aircraft> flown by <name>
		// Else: AI = <aircraft> was destroyed.
		std::string destroyed;
		if (element_mod.find("’") != std::string::npos) {
			// Get username and aircraft
			x = element_mod.find_first_of("’");
			std::string get_name = element_mod.substr(0, x);
			std::string get_aircraft = element_mod.substr(x + 5);
			// Set destroyed
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if username is in list and adds death
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		else if (element_mod.find(" in a ") != std::string::npos)  {
			// Gets name and aircraft
			x = 0;
			int res = -1;
			char end_name[] = " in a ";
			while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { x = res; }
			//
			std::string get_name = element_mod.substr(0, x);
			std::string get_aircraft = element_mod.substr(x + 6);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if username is in the list and adds death (if not AI)
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (element_mod.find(" flown by ") != std::string::npos) {
			// Gets name and aircraft
			x = 0;
			int res = -1;
			char end_name[] = " flown by ";
			while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { x = res; }
			//
			std::string get_aircraft = element_mod.substr(0, x);
			std::string get_name = element_mod.substr(x + 10);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if username is in the list and adds death (if not AI)
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else {
			// AI
			destroyed = element_mod;
		}
		// Creates and sends message
		// Creates and sends message
		std::string message = "* At " + get_time + " ";
		// Gets aircraft destroyed
		message = message + destroyed + " was destroyed.";
		// Checks if related to player
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("destroyed") != std::string::npos) && (string.find("was destroyed by") == std::string::npos)) {
		// Stores the string entered
		std::string element = string;
		// Modified element
		std::string element_mod = element;
		// Stores the time
		std::string time = getDateTime(element);
		// Stores the details for the destroyed aircraft
		std::string destroyed;
		// Stores the details for the contributor
		std::string contributor;
		// Stores the contributors to the destroyed aircraft
		std::vector<std::string> contributors;
		// Advance to beginning
		int x = element_mod.find_last_of(':');
		element_mod = element_mod.substr(x + 2);
		// Gets contributor info
		bool valid = true;
		while (valid) {
			// Checks if element mod still contains a ","
			// That means that there are more contributors
			if (element_mod.find(",") != std::string::npos) {
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string append = element_mod.substr(0, x);
				// Checks if <append> contains a "’" or "in a" or "flown by"
				// If so, then it is a player
				if (append.find("’") != std::string::npos) {
					// Player
					x = append.find_first_of("’");
					std::string get_name = append.substr(0, x);
					append = append.substr(x + 4);
					std::string get_aircraft = append;
					// Set string
					std::string message = get_name + " in a" + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Player
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// AI Aircraft
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else {
				// At the end of contributors, gets last contributor
				char end_name[] = " destroyed ";
				int res = -1;
				int y = 0;
				while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
					y = res;
				}
				//
				std::string append = element_mod.substr(0, y);
				// Checks if <append> contains a "’" or "in a"
				// If so, then it is a player
				if (append.find("’") != std::string::npos) {
					// Player
					x = append.find_first_of("’");
					std::string get_name = append.substr(0, x);
					append = append.substr(x + 4);
					std::string get_aircraft = append;
					// Set string
					std::string message = get_name + " in a" + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Player
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// AI Aircraft
					contributors.push_back(append);
				}
				valid = false;
			}
		}
		// Gets destroyed aircraft
		// Gets index of "destroyed"
		element_mod = element;
		char sub[] = "destroyed";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(sub, res + 1)) != std::string::npos) {
			y = res;
		}
		y += 10;
		element_mod = element_mod.substr(y);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		// Done by checking if destroy contains "in a" or a "’"
		if (destroyed.find(" in a ") != std::string::npos) {
			// Gets the beginning of " in a "
			char substr[] = " in a ";
			res = -1;
			y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos) {
			// Player
			char end_name[] = " flown by ";
			int res = -1;
			int y = 0;
			while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
				y = res;
			}
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			std::string get_name = destroyed.substr(0, destroyed.length() - 2);
			// Set message
			destroyed = get_name + " in a " + get_aircraft + ".";
			element_mod = element_mod.substr(x + 2);
			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos) {
			// Gets the first instance of "’" and sets username
			x = destroyed.find_first_of("’");
			std::string get_name = destroyed.substr(0, x);
			std::string get_aircraft = destroyed.substr(x + 4);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if the username is in the list and adds a user death count to it
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		// Creates and sends message
		std::string message = "* At " + time + " ";
		// Gets contributors
		for (int i = 0; i < contributors.size(); i++) {
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("was destroyed by") != std::string::npos)) {
		// Stores the string entered
		std::string element = string;
		// Modified element
		std::string element_mod = element;
		// Stores the time
		std::string time = getDateTime(element);
		// Stores the details for the destroyed aircraft
		std::string destroyed;
		// Stores the contributors to the destroyed aircraft
		std::vector<std::string> contributors;

		// Advance
		int x = element_mod.find_last_of(':');
		element_mod = element_mod.substr(x + 2);
		// Gets aircraft shot down info
		// End shot down
		char end_des[] = " was destroyed by ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_des, res + 1)) != std::string::npos) {
			y = res;
		}
		destroyed = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 18);
		// Checks if the aircraft shot down was a player
		// Done by checking if <destroyed> contains either a "’" or "in a"
		if (destroyed.find("’") != std::string::npos) {
			// Gets the first instance of "’" and sets username
			x = destroyed.find_first_of("’");
			std::string get_name = destroyed.substr(0, x);
			// Sets aircraft
			std::string get_aircraft = destroyed.substr(x + 5);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if the username is in the list and adds a user death count to it
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		else if (destroyed.find(" in a ") != std::string::npos) {
			// Gets the beginning of " in a "
			char substr[] = " in a ";
			int res = -1;
			int y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos) {
			// Gets the beginning of flown by
			// Gets the beginning of " in a "
			char substr[] = " flown by ";
			int res = -1;
			int y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username and aircraft
			std::string aircraft = destroyed.substr(0, y);
			std::string get_name = destroyed.substr(y + 10);
			destroyed = get_name + " in a " + aircraft;
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}


		// Gets contributor info
		bool valid = true;
		while (valid == true) {
			// Checks element mod still contains a ","
			// If so, then there are more contributors
			// Else, there are none left
			if (element_mod.find(",") != std::string::npos) {
				// Gets next contributor
				x = element_mod.find_first_of(',');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos) {
					x = element_mod.find_first_of("’");
					std::string get_name = element_mod.substr(0, x);
					element_mod = element_mod.substr(x + 4);
					x = element_mod.find_first_of(",");
					std::string get_aircraft = element_mod.substr(0, x);
					append = get_name + " in a" + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Gets beginning
					char substr[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(substr, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets vars
					std::string get_name = append.substr(0, y);
					std::string get_aircraft = append.substr(y + 6);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// If AI aircraft
					x = element_mod.find_first_of(',');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else {
				// Gets info
				x = element_mod.find_last_of('.');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos) {
					x = element_mod.find_first_of("’");
					std::string get_name = element_mod.substr(0, x);
					element_mod = element_mod.substr(x + 4);
					x = element_mod.find_last_of(".");
					std::string get_aircraft = element_mod.substr(0, x);
					append = get_name + " in a" + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(0, x);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Gets beginning
					char sub[] = "in a";
					int res = -1;
					int y = 0;
					while ((res = append.find(sub, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets vars
					std::string get_name = append.substr(0, y - 1);
					std::string get_aircraft = append.substr(y + 5);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// If AI aircraft
					x = element_mod.find_last_of('.');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(0, x);
				}
				// Sets valid to false
				valid = false;
			}
		}
		// Creates and sends message
		std::string message = "* At " + time + " ";
		// Gets contributros
		for (int i = 0; i < contributors.size(); i++) {
			message = message + contributors[i] + ", ";
			// Checks if user contributed to attack
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed + ".";
		// Checks if related
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("was shot down by") != std::string::npos)) {
		// Stores the string entered
		std::string element = string;
		// Modified element
		std::string element_mod = element;
		// Stores the time
		std::string time = getDateTime(element);
		// Stores the details for the destroyed aircraft
		std::string destroyed;
		// Stores the contributors to the destroyed aircraft
		std::vector<std::string> contributors;

		// Advance
		int x = element_mod.find_last_of(':');
		element_mod = element_mod.substr(x + 2);
		// Gets aircraft shot down info
		char end_des[] = " was shot down by ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_des, res + 1)) != std::string::npos) {
			y = res;
		}
		destroyed = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 18);
		// Checks if the aircraft shot down was a player
		// Done by checking if <destroyed> contains either a "’" or "in a"
		if (destroyed.find("’") != std::string::npos) {
			// Gets the first instance of "’" and sets username
			x = destroyed.find_first_of("’");
			std::string get_name = destroyed.substr(0, x);
			// Sets aircraft
			std::string get_aircraft = destroyed.substr(x + 5);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if the username is in the list and adds a user death count to it
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		else if (destroyed.find(" in a ") != std::string::npos) {
			// Gets the beginning of " in a "
			char substr[] = " in a ";
			int res = -1;
			int y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos) {
			// Gets the beginning of flown by
			// Gets the beginning of " in a "
			char substr[] = " flown by ";
			int res = -1;
			int y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username and aircraft
			std::string aircraft = destroyed.substr(0, y);
			std::string get_name = destroyed.substr(y + 10);
			destroyed = get_name + " in a " + aircraft;
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}


		// Gets contributor info
		bool valid = true;
		while (valid == true) {
			// Checks element mod still contains a ","
			// If so, then there are more contributors
			// Else, there are none left
			if (element_mod.find(",") != std::string::npos) {
				// Gets next contributor
				x = element_mod.find_first_of(',');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos) {
					x = element_mod.find_first_of("’");
					std::string get_name = element_mod.substr(0, x);
					element_mod = element_mod.substr(x + 4);
					x = element_mod.find_first_of(",");
					std::string get_aircraft = element_mod.substr(0, x);
					append = get_name + " in a" + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Gets beginning
					char substr[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(substr, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets vars
					std::string get_name = append.substr(0, y);
					std::string get_aircraft = append.substr(y + 6);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// If AI aircraft
					x = element_mod.find_first_of(',');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else {
				// Gets info
				x = element_mod.find_last_of('.');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos) {
					x = element_mod.find_first_of("’");
					std::string get_name = element_mod.substr(0, x);
					element_mod = element_mod.substr(x + 4);
					x = element_mod.find_last_of(".");
					std::string get_aircraft = element_mod.substr(0, x);
					append = get_name + " in a" + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(0, x);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Gets beginning
					char sub[] = "in a";
					int res = -1;
					int y = 0;
					while ((res = append.find(sub, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets vars
					std::string get_name = append.substr(0, y - 1);
					std::string get_aircraft = append.substr(y + 5);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// If AI aircraft
					x = element_mod.find_last_of('.');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(0, x);
				}
				// Sets valid to false
				valid = false;
			}
		}
		// Creates and sends message
		std::string message = "* At " + time + " ";
		// Gets contributros
		for (int i = 0; i < contributors.size(); i++) {
			message = message + contributors[i] + ", ";
			// Checks if user contributed to attack
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed + ".";
		// Checks if related
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("shot down") != std::string::npos) && (string.find("was shot down by") == std::string::npos)) {
		// Stores the string entered
		std::string element = string;
		// Modified element
		std::string element_mod = element;
		// Stores the time
		std::string time = getDateTime(element);
		// Stores the details for the aircraft destroyed
		std::string destroyed;
		// Stores the contributors to the destroyed aircraft
		std::vector<std::string> contributors;

		// Advance to beginning of used info
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets contributor info
		bool valid = true;
		while (valid) {
			// Checks if there is more than 1 contributor left
			// This is done by checking for a ","
			if (element_mod.find(",") != std::string::npos) {
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string contributor = element_mod.substr(0, x);
				// Checks if the contributor is a player or not
				if (contributor.find("’") != std::string::npos) {
					// Player
					x = contributor.find_first_of("’");
					std::string get_name = contributor.substr(0, x);
					contributor = contributor.substr(x + 4);
					std::string get_aircraft = contributor;
					contributors.push_back(get_name + " in a" + get_aircraft);
					// Advances element_mod
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if the player is in the player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (contributor.find(" in a ") != std::string::npos) {
					// Player
					// Get beginning index of " in a "
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = contributor.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					// Pushes back contributor
					std::string get_name = contributor.substr(0, y);
					contributors.push_back(contributor);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if the player is in the player list and adds score
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (contributor.find(" flown by ") != std::string::npos) {
					// Player
					// Get beginning index of " flown by "
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = contributor.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets the username and aircraft, then pushes back
					std::string get_aircraft = contributor.substr(0, y);
					contributor = contributor.substr(y + 10);
					std::string get_name = contributor;
					contributors.push_back(get_name + " in a " + get_aircraft);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// AI
					contributors.push_back(contributor);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
				}
			}
			else {
				// Last contributor
				// Gets the beginning of " shot down "
				char end_name[] = " shot down ";
				int res = -1;
				int y = 0;
				while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
					y = res;
				}
				//
				std::string contributor = element_mod.substr(0, y);
				// Checks if the contributor is a player
				if (contributor.find("’") != std::string::npos) {
					// Player
					x = contributor.find_first_of("’");
					std::string get_name = contributor.substr(0, x);
					contributor = contributor.substr(x + 4);
					std::string get_aircraft = contributor;
					contributors.push_back(get_name + " in a" + get_aircraft);
					// Checks if the player is in the player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (contributor.find(" in a ") != std::string::npos) {
					// Player
					// Get beginning index of " in a "
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = contributor.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					// Pushes back contributor
					std::string get_name = contributor.substr(0, y);
					contributors.push_back(contributor);
					// Checks if the player is in the player list and adds score
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (contributor.find(" flown by ") != std::string::npos) {
					// Player
					// Get beginning index of " flown by "
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = contributor.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets the username and aircraft, then pushes back
					std::string get_aircraft = contributor.substr(0, y);
					contributor = contributor.substr(y + 10);
					std::string get_name = contributor;
					contributors.push_back(get_name + " in a " + get_aircraft);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// AI
					contributors.push_back(contributor);
				}
				valid = false;
			}
		}
		// Gets the destroyed aircraft
		// Gets index of "shot down"
		element_mod = element;
		char sub[] = " shot down ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(sub, res + 1)) != std::string::npos) {
			y = res;
		}
		y += 11;
		element_mod = element_mod.substr(y, element_mod.length() - 1);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		if (destroyed.find(" in a ") != std::string::npos) {
			// Gets the beginning of " in a "
			char substr[] = " in a ";
			res = -1;
			y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos) {
			// Player
			char end_name[] = " flown by ";
			int res = -1;
			int y = 0;
			while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
				y = res;
			}
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			x = destroyed.find_last_of(".");
			std::string get_name = destroyed.substr(0, x);
			// Set message
			destroyed = get_name + " in a " + get_aircraft;
			element_mod = element_mod.substr(x + 2);
			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos) {
			// Gets the first instance of "’" and sets username
			x = destroyed.find_first_of("’");
			std::string get_name = destroyed.substr(0, x);
			std::string get_aircraft = destroyed.substr(x + 4);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if the username is in the list and adds a user death count to it
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		// Creates and sends message
		std::string message = "* At " + time + " ";
		// Gets contributors
		for (int i = 0; i < contributors.size(); i++) {
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find(" claimed a friendly kill: ") != std::string::npos) && (string.find("was destroyed by") == std::string::npos)) {
		// Stores the string entered
		std::string element = string;
		// Modified element
		std::string element_mod = element;
		// Stores the time
		std::string time = getDateTime(element);
		// Stores the details for the destroyed aircraft
		std::string destroyed;
		// Stores the details for the contributor
		std::string contributor;
		// Stores the contributors to the destroyed aircraft
		std::vector<std::string> contributors;
		// Advance to beginning
		int x = element_mod.find_last_of(']');
		element_mod = element_mod.substr(x);
		x = element_mod.find_first_of(':');
		element_mod = element_mod.substr(x + 2);
		// Gets contributor info
		bool valid = true;
		while (valid) {
			// Checks if element mod still contains a ","
			// That means that there are more contributors
			if (element_mod.find(",") != std::string::npos) {
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string append = element_mod.substr(0, x);
				// Checks if <append> contains a "’" or "in a" or "flown by"
				// If so, then it is a player
				if (append.find("’") != std::string::npos) {
					// Player
					x = append.find_first_of("’");
					std::string get_name = append.substr(0, x);
					append = append.substr(x + 4);
					std::string get_aircraft = append;
					// Set string
					std::string message = get_name + " in a" + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Player
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
					}
				}
				else {
					// AI Aircraft
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else {
				// At the end of contributors, gets last contributor
				char end_name[] = " claimed a friendly kill:";
				int res = -1;
				int y = 0;
				while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
					y = res;
				}
				//
				std::string append = element_mod.substr(0, y);
				// Checks if <append> contains a "’" or "in a"
				// If so, then it is a player
				if (append.find("’") != std::string::npos) {
					// Player
					x = append.find_first_of("’");
					std::string get_name = append.substr(0, x);
					append = append.substr(x + 4);
					std::string get_aircraft = append;
					// Set string
					std::string message = get_name + " in a" + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
					}
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Player
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
					}
				}
				else {
					// AI Aircraft
					contributors.push_back(append);
				}
				valid = false;
			}
		}
		// Gets destroyed aircraft
		// Gets index of "friendly kill: "
		element_mod = element;
		char sub[] = "friendly kill: ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(sub, res + 1)) != std::string::npos) {
			y = res;
		}
		y += 15;
		element_mod = element_mod.substr(y);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		// Done by checking if destroy contains "in a" or a "’"
		if (destroyed.find(" in a ") != std::string::npos) {
			// Gets the beginning of " in a "
			char substr[] = " in a ";
			res = -1;
			y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos) {
			// Player
			char end_name[] = " flown by ";
			int res = -1;
			int y = 0;
			while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
				y = res;
			}
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			std::string get_name = destroyed.substr(0, destroyed.length() - 1);
			// Set message
			destroyed = get_name + " in a " + get_aircraft + ".";
			
			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos) {
			// Gets the first instance of "’" and sets username
			x = destroyed.find_first_of("’");
			std::string get_name = destroyed.substr(0, x);
			std::string get_aircraft = destroyed.substr(x + 4);
			destroyed = get_name + " in a" + get_aircraft;
			// Checks if the username is in the list and adds a user death count to it
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		// Creates and sends message
		std::string message = "* At " + time + " ";
		// Gets contributors
		for (int i = 0; i < contributors.size(); i++) {
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "destroyed a friendly " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("claimed") != std::string::npos)) {
		// Stores the string entered
		std::string element = string;
		// Modified element
		std::string element_mod = element;
		// Stores the time
		std::string time = getDateTime(element);
		// Stores the details for the aircraft destroyed
		std::string destroyed;
		// Stores the contributors to the destroyed aircraft
		std::vector<std::string> contributors;

		// Advance to beginning of used info
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets contributor info
		bool valid = true;
		while (valid) {
			// Checks if there is more than 1 contributor left
			// This is done by checking for a ","
			if (element_mod.find(",") != std::string::npos) {
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string contributor = element_mod.substr(0, x);
				// Checks if the contributor is a player or not
				if (contributor.find("’") != std::string::npos) {
					// Player
					x = contributor.find_first_of("’");
					std::string get_name = contributor.substr(0, x);
					contributor = contributor.substr(x + 4);
					std::string get_aircraft = contributor;
					contributors.push_back(get_name + " in a" + get_aircraft);
					// Advances element_mod
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if the player is in the player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (contributor.find(" in a ") != std::string::npos) {
					// Player
					// Get beginning index of " in a "
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = contributor.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					// Pushes back contributor
					std::string get_name = contributor.substr(0, y);
					contributors.push_back(contributor);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if the player is in the player list and adds score
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (contributor.find(" flown by ") != std::string::npos) {
					// Player
					// Get beginning index of " flown by "
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = contributor.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets the username and aircraft, then pushes back
					std::string get_aircraft = contributor.substr(0, y);
					contributor = contributor.substr(y + 10);
					std::string get_name = contributor;
					contributors.push_back(get_name + " in a " + get_aircraft);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// AI
					contributors.push_back(contributor);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
				}
			}
			else {
				// Last contributor
				// Gets the beginning of " claimed "
				char end_name[] = " claimed ";
				int res = -1;
				int y = 0;
				while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
					y = res;
				}
				//
				std::string contributor = element_mod.substr(0, y);
				// Checks if the contributor is a player
				if (contributor.find("’") != std::string::npos) {
					// Player
					x = contributor.find_first_of("’");
					std::string get_name = contributor.substr(0, x);
					contributor = contributor.substr(x + 4);
					std::string get_aircraft = contributor;
					contributors.push_back(get_name + " in a" + get_aircraft);
					// Checks if the player is in the player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (contributor.find(" in a ") != std::string::npos) {
					// Player
					// Get beginning index of " in a "
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = contributor.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					// Pushes back contributor
					std::string get_name = contributor.substr(0, y);
					contributors.push_back(contributor);
					// Checks if the player is in the player list and adds score
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (contributor.find(" flown by ") != std::string::npos) {
					// Player
					// Get beginning index of " flown by "
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = contributor.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets the username and aircraft, then pushes back
					std::string get_aircraft = contributor.substr(0, y);
					contributor = contributor.substr(y + 10);
					std::string get_name = contributor;
					contributors.push_back(get_name + " in a " + get_aircraft);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// AI
					contributors.push_back(contributor);
				}
				valid = false;
			}
		}
		// Gets the destroyed aircraft
		// Gets index of "shot down"
		element_mod = element;
		char sub[] = " claimed ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(sub, res + 1)) != std::string::npos) {
			y = res;
		}
		y += 9;
		element_mod = element_mod.substr(y, element_mod.length() - 1);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		if (destroyed.find(" in a ") != std::string::npos) {
			// Gets the beginning of " in a "
			char substr[] = " in a ";
			res = -1;
			y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos) {
			// Player
			char end_name[] = " flown by ";
			int res = -1;
			int y = 0;
			while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
				y = res;
			}
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			x = destroyed.find_last_of(".");
			std::string get_name = destroyed.substr(0, x);
			// Set message
			destroyed = get_name + " in a " + get_aircraft;
			element_mod = element_mod.substr(x + 2);
			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos) {
			// Gets the first instance of "’" and sets username
			x = destroyed.find_first_of("’");
			std::string get_name = destroyed.substr(0, x);
			std::string get_aircraft = destroyed.substr(x + 4);
			destroyed = get_name + " in a" + get_aircraft;
			// Checks if the username is in the list and adds a user death count to it
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		// Creates and sends message
		std::string message = "* At " + time + " ";
		// Gets contributors
		for (int i = 0; i < contributors.size(); i++) {
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("was blown up by") != std::string::npos)) {
		// Stores the string entered
		std::string element = string;
		// Modified element
		std::string element_mod = element;
		// Stores the time
		std::string time = getDateTime(element);
		// Stores the details for the destroyed aircraft
		std::string destroyed;
		// Stores the contributors to the destroyed aircraft
		std::vector<std::string> contributors;

		// Advance
		int x = element_mod.find_last_of(':');
		element_mod = element_mod.substr(x + 2);
		// Gets aircraft shot down info
		// End shot down
		char end_des[] = " was blown up by ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_des, res + 1)) != std::string::npos) {
			y = res;
		}
		destroyed = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 17);
		// Checks if the aircraft shot down was a player
		// Done by checking if <destroyed> contains either a "’" or "in a"
		if (destroyed.find("’") != std::string::npos) {
			// Gets the first instance of "’" and sets username
			x = destroyed.find_first_of("’");
			std::string get_name = destroyed.substr(0, x);
			// Sets aircraft
			std::string get_aircraft = destroyed.substr(x + 5);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if the username is in the list and adds a user death count to it
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		else if (destroyed.find(" in a ") != std::string::npos) {
			// Gets the beginning of " in a "
			char substr[] = " in a ";
			int res = -1;
			int y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos) {
			// Gets the beginning of flown by
			// Gets the beginning of " in a "
			char substr[] = " flown by ";
			int res = -1;
			int y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username and aircraft
			std::string aircraft = destroyed.substr(0, y);
			std::string get_name = destroyed.substr(y + 10);
			destroyed = get_name + " in a " + aircraft;
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}


		// Gets contributor info
		bool valid = true;
		while (valid == true) {
			// Checks element mod still contains a ","
			// If so, then there are more contributors
			// Else, there are none left
			if (element_mod.find(",") != std::string::npos) {
				// Gets next contributor
				x = element_mod.find_first_of(',');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos) {
					x = element_mod.find_first_of("’");
					std::string get_name = element_mod.substr(0, x);
					element_mod = element_mod.substr(x + 4);
					x = element_mod.find_first_of(",");
					std::string get_aircraft = element_mod.substr(0, x);
					append = get_name + " in a" + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Gets beginning
					char substr[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(substr, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets vars
					std::string get_name = append.substr(0, y);
					std::string get_aircraft = append.substr(y + 6);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// If AI aircraft
					x = element_mod.find_first_of(',');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else {
				// Gets info
				// Gets the beginning of " before"
				char substr[] = " before ";
				int res = -1;
				int y = 0;
				while ((res = element_mod.find(substr, res + 1)) != std::string::npos) {
					y = res;
				}
				std::string append = element_mod.substr(0, y);
				append = append;
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos) {
					x = element_mod.find_first_of("’");
					std::string get_name = element_mod.substr(0, x);
					element_mod = element_mod.substr(x + 4);
					char sub[] = " before he could take off";
					int res = -1;
					int y = 0;
					while ((res = element_mod.find(sub, res + 1)) != std::string::npos) {
						y = res;
					}
					std::string get_aircraft = element_mod.substr(0, y);
					append = get_name + " in a" + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(0, x);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Gets beginning
					char sub[] = "in a";
					int res = -1;
					int y = 0;
					while ((res = append.find(sub, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets vars
					std::string get_name = append.substr(0, y - 1);
					std::string get_aircraft = append.substr(y + 5);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// If AI aircraft
					contributors.push_back(append);
					element_mod = element_mod;
				}
				// Sets valid to false
				valid = false;
			}
		}
		// Creates and sends message
		std::string message = "* At " + time + " ";
		// Gets contributros
		for (int i = 0; i < contributors.size(); i++) {
			message = message + contributors[i] + ", ";
			// Checks if user contributed to attack
		}
		// Gets aircraft destroyed
		message = message + "blew up " + destroyed + ".";
		// Checks if related
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("sank") != std::string::npos)) {
		// Stores the string entered
		std::string element = string;
		// Modified element
		std::string element_mod = element;
		// Stores the time
		std::string time = getDateTime(element);
		// Stores the details for the destroyed aircraft
		std::string destroyed;
		// Stores the details for the contributor
		std::string contributor;
		// Stores the contributors to the destroyed aircraft
		std::vector<std::string> contributors;
		// Advance to beginning
		int x = element_mod.find_last_of(':');
		element_mod = element_mod.substr(x + 2);
		// Gets contributor info
		bool valid = true;
		while (valid) {
			// Checks if element mod still contains a ","
			// That means that there are more contributors
			if (element_mod.find(",") != std::string::npos) {
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string append = element_mod.substr(0, x);
				// Checks if <append> contains a "’" or "in a" or "flown by"
				// If so, then it is a player
				if (append.find("’") != std::string::npos) {
					// Player
					x = append.find_first_of("’");
					std::string get_name = append.substr(0, x);
					append = append.substr(x + 4);
					std::string get_aircraft = append;
					// Set string
					std::string message = get_name + " in a" + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Player
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// AI Aircraft
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else {
				// At the end of contributors, gets last contributor
				char end_name[] = " sank ";
				int res = -1;
				int y = 0;
				while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
					y = res;
				}
				//
				std::string append = element_mod.substr(0, y);
				// Checks if <append> contains a "’" or "in a"
				// If so, then it is a player
				if (append.find("’") != std::string::npos) {
					// Player
					x = append.find_first_of("’");
					std::string get_name = append.substr(0, x);
					append = append.substr(x + 4);
					std::string get_aircraft = append;
					// Set string
					std::string message = get_name + " in a" + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Player
					char end_name[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// AI Aircraft
					contributors.push_back(append);
				}
				valid = false;
			}
		}
		// Gets destroyed aircraft
		// Gets index of "sank"
		element_mod = element;
		char sub[] = "sank";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(sub, res + 1)) != std::string::npos) {
			y = res;
		}
		y += 5;
		element_mod = element_mod.substr(y);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		// Done by checking if destroy contains "in a" or a "’"
		if (destroyed.find(" in a ") != std::string::npos) {
			// Gets the beginning of " in a "
			char substr[] = " in a ";
			res = -1;
			y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos) {
			// Player
			char end_name[] = " flown by ";
			int res = -1;
			int y = 0;
			while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) {
				y = res;
			}
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			std::string get_name = destroyed.substr(0, destroyed.length() - 1);
			// Set message
			destroyed = get_name + " in a " + get_aircraft;
			element_mod = element_mod.substr(x + 2);
			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos) {
			// Gets the first instance of "’" and sets username
			x = destroyed.find_first_of("’");
			std::string get_name = destroyed.substr(0, x);
			std::string get_aircraft = destroyed.substr(x + 4);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if the username is in the list and adds a user death count to it
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		// Creates and sends message
		std::string message = "* At " + time + " ";
		// Gets contributors
		for (int i = 0; i < contributors.size(); i++) {
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "sank " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("goes down thanks to") != std::string::npos)) {
		// Stores the string entered
		std::string element = string;
		// Modified element
		std::string element_mod = element;
		// Stores the time
		std::string time = getDateTime(element);
		// Stores the details for the destroyed aircraft
		std::string destroyed;
		// Stores the contributors to the destroyed aircraft
		std::vector<std::string> contributors;

		// Advance
		int x = element_mod.find_last_of(':');
		element_mod = element_mod.substr(x + 2);
		// Gets aircraft shot down info
		// End shot down
		char end_des[] = " goes down thanks to ";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(end_des, res + 1)) != std::string::npos) {
			y = res;
		}
		destroyed = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 21);
		// Checks if the aircraft shot down was a player
		// Done by checking if <destroyed> contains either a "’" or "in a"
		if (destroyed.find("’") != std::string::npos) {
			// Gets the first instance of "’" and sets username
			x = destroyed.find_first_of("’");
			std::string get_name = destroyed.substr(0, x);
			// Sets aircraft
			std::string get_aircraft = destroyed.substr(x + 5);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if the username is in the list and adds a user death count to it
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		else if (destroyed.find(" in a ") != std::string::npos) {
			// Gets the beginning of " in a "
			char substr[] = " in a ";
			int res = -1;
			int y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos) {
			// Gets the beginning of flown by
			// Gets the beginning of " in a "
			char substr[] = " flown by ";
			int res = -1;
			int y = 0;
			while ((res = destroyed.find(substr, res + 1)) != std::string::npos) {
				y = res;
			}
			// Sets the username and aircraft
			std::string aircraft = destroyed.substr(0, y);
			std::string get_name = destroyed.substr(y + 10);
			destroyed = get_name + " in a " + aircraft;
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}


		// Gets contributor info
		bool valid = true;
		while (valid == true) {
			// Checks element mod still contains a ","
			// If so, then there are more contributors
			// Else, there are none left
			if (element_mod.find(",") != std::string::npos) {
				// Gets next contributor
				x = element_mod.find_first_of(',');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos) {
					x = element_mod.find_first_of("’");
					std::string get_name = element_mod.substr(0, x);
					element_mod = element_mod.substr(x + 4);
					x = element_mod.find_first_of(",");
					std::string get_aircraft = element_mod.substr(0, x);
					append = get_name + " in a" + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Gets beginning
					char substr[] = " in a ";
					int res = -1;
					int y = 0;
					while ((res = append.find(substr, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets vars
					std::string get_name = append.substr(0, y);
					std::string get_aircraft = append.substr(y + 6);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// If AI aircraft
					x = element_mod.find_first_of(',');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else {
				// Gets info
				x = element_mod.find_last_of('.');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos) {
					x = element_mod.find_first_of("’");
					std::string get_name = element_mod.substr(0, x);
					element_mod = element_mod.substr(x + 4);
					x = element_mod.find_last_of(".");
					std::string get_aircraft = element_mod.substr(0, x);
					append = get_name + " in a" + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(0, x);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos) {
					// Gets beginning
					char sub[] = "in a";
					int res = -1;
					int y = 0;
					while ((res = append.find(sub, res + 1)) != std::string::npos) {
						y = res;
					}
					// Sets vars
					std::string get_name = append.substr(0, y - 1);
					std::string get_aircraft = append.substr(y + 5);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos) {
					// Player
					char end_name[] = " flown by ";
					int res = -1;
					int y = 0;
					while ((res = append.find(end_name, res + 1)) != std::string::npos) {
						y = res;
					}
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else {
					// If AI aircraft
					x = element_mod.find_last_of('.');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(0, x);
				}
				// Sets valid to false
				valid = false;
			}
		}
		// Creates and sends message
		std::string message = "* At " + time + " ";
		// Gets contributros
		for (int i = 0; i < contributors.size(); i++) {
			message = message + contributors[i] + ", ";
			// Checks if user contributed to attack
		}
		// Gets aircraft destroyed
		message = message + "sank " + destroyed + ".";
		// Checks if related
		if (_destructionUnrelatedInfo) {
			log_processed.push_back(message);
		}
		else {
			if (element.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find(" is no more.") != std::string::npos)) {
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets info
		// Gets beginning of " is no more"
		char begin_info[] = " is no more.";
		int res = -1;
		int y = 0;
		while ((res = element_mod.find(begin_info, res + 1)) != std::string::npos) { y = res; }
		element_mod = element_mod.substr(0, y);
		// <aircraft> <name>
		// Get aircraft/name info
		if (element_mod.find(" flown by ") != std::string::npos) {
			char end_name[] = " flown by ";
			res = -1;
			y = 0;
			while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
			std::string get_aircraft = element_mod.substr(0, y);
			std::string get_name = element_mod.substr(y + 10);
			// Sends message
			std::string message = "* At " + get_time + " " + get_name + " in a " + get_aircraft + " is no more.";
			if (_destructionInfo && _destructionUnrelatedInfo) {
				log_processed.push_back(message);
			}
			else if (_destructionInfo && get_name.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
			// Checks if name is in list if not AI
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (element_mod.find(" in a ") != std::string::npos) {
			char end_name[] = " in a ";
			res = -1;
			y = 0;
			while ((res = element_mod.find(end_name, res + 1)) != std::string::npos) { y = res; }
			std::string get_name = element_mod.substr(0, y);
			std::string get_aircraft = element_mod.substr(y + 6);
			// Sends message
			std::string message = "* At " + get_time + " " + get_name + " in a " + get_aircraft + " is no more.";
			if (_destructionInfo && _destructionUnrelatedInfo) {
				log_processed.push_back(message);
			}
			else if (_destructionInfo && get_name.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
			// Checks if name is in list if not AI
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (element_mod.find("’") != std::string::npos) {
			int x = element_mod.find_first_of("’");
			std::string get_name = element_mod.substr(0, x);
			std::string get_aircraft = element_mod.substr(x + 4);
			// Sends message
			std::string message = "* At " + get_time + " " + get_name + " in a" + get_aircraft + " is no more.";
			if (_destructionInfo && _destructionUnrelatedInfo) {
				log_processed.push_back(message);
			}
			else if (_destructionInfo && get_name.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
			// Checks if name is in list
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		else {
			// <aircraft> <name>
			// Aircraft
			int x = element_mod.find_first_of(")");
			std::string get_aircraft = element_mod.substr(0, x + 1);
			std::string get_name = element_mod.substr(x + 2);
			// Sends message
			std::string message = "* At " + get_time + " " + get_name + " in a " + get_aircraft + " is no more.";
			if (_destructionInfo && _destructionUnrelatedInfo) {
				log_processed.push_back(message);
			}
			else if (_destructionInfo && get_name.find(username) != std::string::npos) {
				log_processed.push_back(message);
			}
			// Checks if name is in list
			if (get_name != "AI" && get_name != "(AI)" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos) {
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
	}
}

void messageInfo(std::string string) {
	std::string element = string;
	std::string element_mod = element;
	std::string time = getDateTime(element);
	char end_chat[] = "Chat: ";
	int y = 0;
	int res = -1;
	while ((res = element_mod.find(end_chat, res + 1)) != std::string::npos) { y = res; }
	element_mod = element_mod.substr(y + 6);
	// Gets the sender of the message
	y = element_mod.find_first_of(":");
	std::string get_name = element_mod.substr(0, y);
	element_mod = element_mod.substr(y + 3);
	// Gets the message
	std::string message = element_mod;
	// Pushes to log processed
	if (_showChat) {
		log_processed.push_back(">> At " + time + " " + get_name + " sent: \"" + message + "\" in chat.");
	}
	// Checks if the player is in the player list
	checkPlayerInList(get_name);
	addPlayerMessageSent(get_name);
}

// Player List Related
void checkPlayerInList(std::string player_name) {
	// Loops through "session_players" to see if string is in it
	bool _inList = false;
	for (int i = 0; i < session_players.size(); i++) {
		// Checks if player is in list
		if (session_players[i][0] == player_name) { _inList = true; }
	}
	// If not in list
	if (!_inList) {
		session_players.push_back({ player_name, "not recorded", "0", "0", "0", "0", "0", "0", "0", "0", "0" });
	}
}
void changePlayerSide(std::string player_name, std::string player_side) {
	// Changes the correct players side info
	for (int i = 0; i < session_players.size(); i++) {
		// Checks if player is in list
		if (session_players[i][0] == player_name) {
			session_players[i][1] = player_side;
		}
	}
}
void addPlayerScore(std::string player_name) {
	// Changes the correct players score
	for (int i = 0; i < session_players.size(); i++) {
		if (session_players[i][0] == player_name) {
			session_players[i][2] = std::to_string(std::stoi(session_players[i][2]) + 1);
		}
	}
}
void addPlayerLanding(std::string player_name) {
	// Changes the correct players death info
	for (int i = 0; i < session_players.size(); i++) {
		if (session_players[i][0] == player_name) {
			session_players[i][3] = std::to_string(std::stoi(session_players[i][3]) + 1);
		}
	}
}
void addPlayerDeath(std::string player_name) {
	// Changes the correct players death info
	for (int i = 0; i < session_players.size(); i++) {
		if (session_players[i][0] == player_name) {
			session_players[i][5] = std::to_string(std::stoi(session_players[i][5]) + 1);
		}
	}
}
void addPlayerCrash(std::string player_name) {
	// Changes the correct players crash info
	for (int i = 0; i < session_players.size(); i++) {
		if (session_players[i][0] == player_name) {
			session_players[i][6] = std::to_string(std::stoi(session_players[i][6]) + 1);
		}
	}
}
void addPlayerBail(std::string player_name) {
	// Changes the correct players death info
	for (int i = 0; i < session_players.size(); i++) {
		if (session_players[i][0] == player_name) {
			session_players[i][7] = std::to_string(std::stoi(session_players[i][7]) + 1);
		}
	}
}
void addPlayerConnection(std::string player_name) {
	// Changes the correct players connection info
	for (int i = 0; i < session_players.size(); i++) {
		if (session_players[i][0] == player_name) {
			session_players[i][8] = std::to_string(std::stoi(session_players[i][8]) + 1);
		}
	}
}
void addPlayerDisconnection(std::string player_name) {
	// Changes the correct players connection info
	for (int i = 0; i < session_players.size(); i++) {
		if (session_players[i][0] == player_name) {
			session_players[i][9] = std::to_string(std::stoi(session_players[i][9]) + 1);
		}
	}
}
void addPlayerMessageSent(std::string player_name) {
	// Changes the correct players connection info
	for (int i = 0; i < session_players.size(); i++) {
		if (session_players[i][0] == player_name) {
			session_players[i][10] = std::to_string(std::stoi(session_players[i][10]) + 1);
		}
	}

}


