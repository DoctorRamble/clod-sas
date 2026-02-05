#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <format>

// For compiling:
// g++ --std=c++20 main.cpp -o main

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
// Stores the directory to save the output *.txt file
std::string output_dir;
// File name to save if not null
std::string output_name;
// Location of "log.txt"
std::string log_location;
// Your Steam Name
std::string username;
// Version
std::string version = "v0.2-0";
// Last Updated
std::string last_updated = "2026-02-05";
// CloD Version
std::string game_ver = "v5.046";
// Used for aborting if OS is not supported
bool _OSUnsupported = false;
// Should the programme save an output text file?
bool _outputFile;
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
// Should SAS be run headless (just saves to a text file, no text output)
bool _headless;

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
// Stores lines for outputting to a file
std::vector<std::string> output_lines;
// Stores the list of players in the session
// {{ name, side, score, landings, reserve, deaths, crashes, bailouts, connections, disconnections, messages_sent }, ...}
std::vector<std::vector<std::string>> session_players;

// Clears the screen (Not currently used)
void clr();
// Gets the index of a sub-string in a string
int findStrIndex(const std::string& str, const std::string& sub_str);
// Returns the current Date & Time
std::string getDateTime(bool timeMode);
// Converts a string number to a bool
bool strToBool(const std::string& str);
// Opens & Loads "log.txt" into a vector
void load_log();
// Processes "log.txt" and gets statistics
void process_log();
// Saves the contents of output_lines to a file
void saveFile();
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

int main(int argc, char* argv[])
{
	// Sets "user_system"
#ifdef LINUX
	user_system = "Linux";
#endif
#ifdef WINDOWS
	user_system = "Windows";
#endif
	
    if (user_system == "Windows" || user_system == "Linux")
	{
		std::cout << "--! " << user_system << " was detected.\n";
		if (user_system == "Linux")
		{
			std::cout << "    (Sorry... \"GNU/Linux\".)\n";
		}
	}
	else
	{ 
		std::cout << "--!  Bail Out! Bail Out!\n     Your OS is unsupported! Please try again on Windows or GNU/Linux.\n";
		output_lines.push_back("--!  Bail Out! Bail Out!\n     Your OS is unsupported! Please try again on Windows or GNU/Linux.");
		_OSUnsupported = true;
	}

	// Load Arguments
    std::string initial_arg = argv[1];
	if (initial_arg == "help")
	{
		std::cout << "Help info for SAS\n"
			<< "SAS Version: " << version << "\n"
			<< "Last Updated: " << last_updated << "\n"
			<< "\nGitHub Page: https://www.github.com/DoctorRamble/clod-sas\n"
			<< "ATAG Post: https://theairtacticalassaultgroup.com/forum/showthread.php?t=39090\n";
		std::cout << "\nHow to call the programme in the command line:\n"
			<< "exec_name \"username\" \"log_location\" \"output_dir\" _outputFile _debugInfo _allInfo _playerConnectionInfo _otherPlayerConnectionInfo _sideSwitchInfo _otherPlayerSideSwitchInfo _newMissionLoadInfo _newMissionInfo _battleStartedInfo _battleEndInfo _destructionInfo _destructionUnrelatedInfo _landingInfo _otherLandingInfo _bailInfo _otherBailInfo _crashInfo _otherCrashInfo _otherPlayerStats _showChat _colour _colourFG _headless\n\n"
			<< "\tusername                   :: String :: Your Steam Username\n"
			<< "\tlog_location               :: String :: Location of the CloD Log File\n"
			<< "\toutput_dir                 :: String :: Directory to save the output file\n"
			<< "\toutput_name                :: String :: Output file name. Useful for GUIs. Best left at \"null\" for terminal useage\n"
			<< "\t_outputFile                :: Bool   :: Should the programme save an output file?\n"
			<< "\t_debugInfo                 :: Bool   :: Show Additional Debug Information\n"
			<< "\t_allInfo                   :: Bool   :: Prints out the log file as is. No reason to use this programme if this is enabled, but each to his own\n"
			<< "\t_playerConnectionInfo      :: Bool   :: Shows related player connection information\n"
			<< "\t_otherPlayerConnectionInfo :: Bool   :: Shows unrelated player connection information\n"
			<< "\t_sideSwitchInfo            :: Bool   :: Shows related player side-switch information\n"
			<< "\t_otherPlayerSideSwitchInfo :: Bool   :: Shows unrelated player side-switch information\n"
			<< "\t_newMissionLoadInfo        :: Bool   :: Shows new missions being loaded information\n"
			<< "\t_newMissionInfo            :: Bool   :: Shows new mission information\n"
			<< "\t_battleStartedInfo         :: Bool   :: Shows a battle being started\n"
			<< "\t_battleEndInfo             :: Bool   :: Shows a battle ending\n"
			<< "\t_destructionInfo           :: Bool   :: Shows related vehicle destruction information\n"
			<< "\t_destructionUnrelatedInfo  :: Bool   :: Shows unrelated vehicle destruction information\n"
			<< "\t_landingInfo               :: Bool   :: Shows related landing information\n"
			<< "\t_otherLandingInfo          :: Bool   :: Shows unrelated landing information\n"
			<< "\t_bailInfo                  :: Bool   :: Shows related bail-out information\n"
			<< "\t_otherBailInfo             :: Bool   :: Shows unrelated bail-out information\n"
			<< "\t_crashInfo                 :: Bool   :: Shows related crash information\n"
			<< "\t_otherCrashInfo            :: Bool   :: Shows unrelated crash information\n"
			<< "\t_otherPlayerStats          :: Bool   :: Shows other player statistics; Useful for multiplayer missions\n"
			<< "\t_showChat                  :: Bool   :: Shows game chat; On 'social' multiplayer missions there may be unnecessary clutter\n"
			<< "\t_colour                    :: Bool   :: Enable colour. If SAS is used with a GUI this is not required.\n"
			<< "\t_colourFG                  :: Bool   :: If true the colour will be applied to the foreground/font.\n"
			<< "\t_headless                  :: Bool   :: If true then _outputFile will be forced to true.\n";
		return 0;
	}
    else
    {
        // Checks if the right amount of arguments are there
        if (argc == 29)
        {
			username = argv[1];
            log_location = argv[2];
            output_dir = argv[3];
			output_name = argv[4];
            _outputFile = strToBool(argv[5]);
            _debugInfo = strToBool(argv[6]);
            _allInfo = strToBool(argv[7]);
            _playerConnectionInfo = strToBool(argv[8]);
            _otherPlayerConnectionInfo = strToBool(argv[9]);
            _sideSwitchInfo = strToBool(argv[10]);
            _otherPlayerSideSwitchInfo = strToBool(argv[11]);
            _newMissionLoadInfo = strToBool(argv[12]);
            _newMissionInfo = strToBool(argv[13]);
            _battleStartedInfo = strToBool(argv[14]);
            _battleEndInfo = strToBool(argv[15]);
            _destructionInfo = strToBool(argv[16]);
            _destructionUnrelatedInfo = strToBool(argv[17]);
            _landingInfo = strToBool(argv[18]);
            _otherLandingInfo = strToBool(argv[19]);
            _bailInfo = strToBool(argv[20]);
            _otherBailInfo = strToBool(argv[21]);
            _crashInfo = strToBool(argv[22]);
            _otherCrashInfo = strToBool(argv[23]);
            _otherPlayerStats = strToBool(argv[24]);
            _showChat = strToBool(argv[25]);
            _colour = strToBool(argv[26]);
            _colourFG = strToBool(argv[27]);
			_headless = strToBool(argv[28]);
			if (_headless) { _outputFile = true; }
        }
        else
        {
            std::cout << "--!  Boffin Leader from Boffin 3. My engine's dead. Bailing out!\n     Boffin 3 from Boffin Leader! You silly -------! You forgot to enter in the right amount of arguments again!\n";
			output_lines.push_back("--!  Boffin Leader from Boffin 3. My engine's dead. Bailing out!\n     Boffin 3 from Boffin Leader! You silly -------! You forgot to enter in the right amount of arguments again!");
        }
    }

	// Print vars if _debugInfo is true
	if (_debugInfo)
	{
		output_lines.push_back("");
		output_lines.push_back("Argument Values: ");
        std::cout << "\n--!  Username: " << username << "\n";
		output_lines.push_back("--!  Username: " + username);
        std::cout << "--!  Log Location: " << log_location << "\n";
		output_lines.push_back("--!  Log Location: " + log_location);
        std::cout << "--!  Output Directory: " << output_dir << "\n";
		output_lines.push_back("--!  Output Directory: " + output_dir);
		std::cout << "--!  Output Filename: " + output_name << "\n";
		output_lines.push_back("--!  Output Filename: " + output_name);
        std::cout << "--!  Output File?: " << _outputFile << "\n";
		output_lines.push_back("--!  Output File?: " + std::to_string((int)_outputFile));
        std::cout << "--!  Debug Info?: " << _debugInfo << "\n";
		output_lines.push_back("--!  Debug Info?: " + std::to_string((int)_outputFile));
        std::cout << "--!  All Info?: " << _allInfo << "\n";
		output_lines.push_back("--!  All Info?: " + std::to_string((int)_allInfo));
        std::cout << "--!  Player Connection Info?: " << _playerConnectionInfo << "\n";
		output_lines.push_back("--!  Player Connection Info?: " + std::to_string((int)_playerConnectionInfo));
        std::cout << "--!  Other Player Connection Info?: " << _otherPlayerConnectionInfo << "\n";
		output_lines.push_back("--!  Other Player Connection Info?: " + std::to_string((int)_otherPlayerConnectionInfo));
        std::cout << "--!  Side Switch Info?: " << _sideSwitchInfo << "\n";
		output_lines.push_back("--!  Side Switch Info?: " + std::to_string((int)_sideSwitchInfo));
        std::cout << "--!  Other Side Switch Info?: " << _otherPlayerSideSwitchInfo << "\n";
		output_lines.push_back("--!  Other Side Switch Info?: " + std::to_string((int)_otherPlayerSideSwitchInfo));
        std::cout << "--!  New Mission Load Info?: " << _newMissionLoadInfo << "\n";
		output_lines.push_back("--!  New Mission Load Info?: " + std::to_string((int)_newMissionLoadInfo));
        std::cout << "--!  New Mission Info?: " << _newMissionInfo << "\n";
		output_lines.push_back("--!  New Mission Info?: " + std::to_string((int)_newMissionInfo));
        std::cout << "--!  Battle Start Info?: " << _battleStartedInfo << "\n";
		output_lines.push_back("--!  Battle Start Info?: " + std::to_string((int)_battleStartedInfo));
        std::cout << "--!  Battle End Info?: " << _battleEndInfo << "\n";
		output_lines.push_back("--!  Battle End Info?: " + std::to_string((int)_battleEndInfo));
        std::cout << "--!  Destruction Info?: " << _destructionInfo << "\n";
		output_lines.push_back("--!  Destruction Info?: " + std::to_string((int)_destructionInfo));
        std::cout << "--!  Other Destruction Info?: " << _destructionUnrelatedInfo << "\n";
		output_lines.push_back("--!  Other Destruction Info?: " + std::to_string((int)_destructionUnrelatedInfo));
        std::cout << "--!  Landing Info?: " << _landingInfo << "\n";
		output_lines.push_back("--!  Landing Info?: " + std::to_string((int)_landingInfo));
        std::cout << "--!  Other Landing Info?: " << _otherLandingInfo << "\n";
		output_lines.push_back("--!  Other Landing Info?: " + std::to_string((int)_otherLandingInfo));
        std::cout << "--!  Bail Info?: " << _bailInfo << "\n";
		output_lines.push_back("--!  Bail Info?: " + std::to_string((int)_bailInfo));
        std::cout << "--!  Other Bail Info?: " << _otherBailInfo << "\n";
		output_lines.push_back("--!  Other Bail Info?: " + std::to_string((int)_otherBailInfo));
        std::cout << "--!  Crash Info?: " << _crashInfo << "\n";
		output_lines.push_back("--!  Crash Info?: " + std::to_string((int)_crashInfo));
        std::cout << "--!  Other Crash Info?: " << _otherCrashInfo << "\n";
		output_lines.push_back("--!  Other Crash Info?: " + std::to_string((int)_otherCrashInfo));
        std::cout << "--!  Other Player Stats?: " << _otherPlayerStats << "\n";
		output_lines.push_back("--!  Other Player Stats?: " + std::to_string((int)_otherPlayerStats));
        std::cout << "--!  Show Chat?: " << _showChat << "\n";
		output_lines.push_back("--!  Show Chat?: " + std::to_string((int)_showChat));
        std::cout << "--!  Colour?: " << _colour << "\n";
		output_lines.push_back("--!  Colour?: " + std::to_string((int)_colour));
        std::cout << "--!  Colour Foreground?: " << _colourFG << "\n";
		output_lines.push_back("--!  Colour Foreground?: " + std::to_string((int)_colourFG));
		std::cout << "--!  Headless?: " << _headless << "\n";
		output_lines.push_back("--!  Headless?: " + std::to_string((int)_headless));
		std::cout << "\n";
		output_lines.push_back("");
	}

	// Main programme
	if (!_OSUnsupported)
	{
		if (_colour) { std::cout << "\n\033[32;40mRamble's CloD \033[31;40mServer \033[37;40mActivity \033[34;40mStatistics\033[32;40m - " << version << "\033[39m\033[49m\n"; }
		else { std::cout << "\nRamble's CloD Server Activity Statistics - " << version << "\n"; }
		output_lines.push_back("Ramble's CloD Server Activity Statistics - " + version);

		std::cout << "Last Updated: " << last_updated << "; CloD Version: " << game_ver << ";\n\n";
		output_lines.push_back("Last Updated: " + last_updated + "; CloD Version: " + game_ver);

		load_log();
		process_log();
	}
	printProcLog();
	if (_outputFile)
	{
		saveFile();
	}
}

void clr()
{
	if (user_system == "Linux")
	{
		std::cout << "\033[2J";
	}
	else if (user_system == "Windows")
	{
		system("cls");
	}
}

bool strToBool(const std::string& str)
{
    return std::stoi(str);
}

// If "timeMode" is "true" then the ":"s in the string will be replaced with "-"s
std::string getDateTime(bool timeMode)
{
	// Gets the date and time and saves it to a string
	auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
	std::string dateTime = std::format("{:%Y-%m-%d_%X}", time);
	// Checks if the ":"s need to be replaced
	if (timeMode) { for (int i = 0; i < dateTime.length(); i++) { if (dateTime[i] == ':') { dateTime[i] = '-'; } } }
	// Returns the time
	return dateTime;
}

void load_log()
{
	std::fstream log(log_location, std::ios::in);
	// Checks if "log.txt" is found or not
	if (!log.is_open() && _debugInfo)
	{
		std::cout << "--! \"log.txt\" was not found.\n--> Please enter the correct location for \"log.txt\"!\n";
		output_lines.push_back("--! \"log.txt\" was not found.\n--> Please enter the correct location for \"log.txt\"!");
	}
	else
	{
		if (_debugInfo)
		{
			std::cout << "--! \"log.txt\" was opened successfully!\n";
			output_lines.push_back("--! \"log.txt\" was opened successfully!");
		}
	}

	// Write "log" to "log_contents"
	while (std::getline(log, log_lines)) { log_contents.push_back(log_lines); }
	// Checks for end of file
	if (log.eof() && _debugInfo)
	{
		std::cout << "--! Finished loading \"log.txt\"!\n\n";
		output_lines.push_back("--! Finished loading \"log.txt\"!");
	}
	else
	{
		if (_debugInfo)
		{
			std::cout << "--! Error reading \"log.txt\"; Failed to load it into the programme!\n\n";
			output_lines.push_back("--! Error reading \"log.txt\"; Failed to load it into the programme!");
		}
	}
	// Closes "log.txt"
	log.close();
}

void process_log()
{
	// If ALL info was kept, then print it out:
	if (_allInfo)
	{
		std::cout << "\n";
		for (int i = 0; i < log_contents.size(); i++) { std::cout << log_contents[i] << "\n"; }
	}
	// Else, process the log:
	else
	{
		// Loops through "log_contents" and processes information
		for (int i = 0; i < log_contents.size(); i++)
		{
			// If chat message
			if (log_contents[i].find("Chat: ") != std::string::npos)
			{
				// Gets the message sender, and then the message
				messageInfo(log_contents[i]);
			}
			// If not a chat message
			else
			{
				// If element is the "BEGIN log" header AND "_sessionInfo" is true
				if ((log_contents[i].find("BEGIN log session") != std::string::npos))
				{
					std::string message = "--> You started playing Cliffs of Dover at: ";
					message.append(getDateTime(log_contents[i]));
					log_processed.push_back(message);
				}
				// If element is the "END log" header AND "_sessionInfo" is true
				if ((log_contents[i].find("END log session") != std::string::npos))
				{
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
				if (_newMissionLoadInfo && (log_contents[i].find("Loading mission ") != std::string::npos))
				{
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
				if (_newMissionInfo && (log_contents[i].find("Mission loaded. time =") != std::string::npos))
				{
					std::string message = "! New mission was loaded at ";
					message.append(getDateTime(log_contents[i]));
					message.append(".");
					log_processed.push_back(message);
				}
				// Checks for battle starting info
				if (_battleStartedInfo && (log_contents[i].find("Server: Battle begins!") != std::string::npos))
				{
					std::string message = "! A New battle was started at ";
					message.append(getDateTime(log_contents[i]));
					message.append(".");
					log_processed.push_back(message);
				}
				// Checks for battle ending info
				if (_battleEndInfo && (log_contents[i].find("Server: The battle ends.") != std::string::npos))
				{
					std::string message = "! The battle ended at ";
					message.append(getDateTime(log_contents[i]));
					message.append(".");
					log_processed.push_back(message);
				}
				// Checks for aircraft shot down
				if (_destructionInfo) { destructionInfo(log_contents[i]); }
			}
		}
	}
}

void printProcLog()
{
	// Print Processed Log
	// * = Kill
	// _ = Crash/Failed Bail
	// = = Landing/Landed Bail
	// ^ = Bail Out
	// # = Connection/Side Switch
	// >> = Chat
	// --> =
	output_lines.push_back("");
	for (auto& i : log_processed)
	{
		if (_colour)
		{
			if (_colourFG)
			{
				// Font
				if (i.find("*") != std::string::npos) { std::cout << "\033[31;40m" << i << "\033[39m\033[49m\n"; } // Red
				else if (i.find("^") != std::string::npos) { std::cout << "\033[94;40m" << i << "\033[39m\033[49m\n"; } // Bright Blue
				else if (i.find("#") != std::string::npos) { std::cout << "\033[96;40m" << i << "\033[39m\033[49m\n"; } // Bright Cyan
				else if (i.find(">>") != std::string::npos) { std::cout << i << "\n"; } // No Colour : Chat
				else if (i.find("-->") != std::string::npos) { std::cout << "\033[91;40m" << i << "\033[39m\033[49m\n"; } // Bright Red
				else if (i.find("!") != std::string::npos) { std::cout << "\033[92;40m" << i << "\033[39m\033[49m\n"; } // Bright Green
				else if (i.find("=") != std::string::npos) { std::cout << "\033[32;40m" << i << "\033[39m\033[49m\n"; } // Green
				else if (i.find("_") != std::string::npos) { std::cout << "\033[93;40m" << i << "\033[39m\033[49m\n"; } // Bright Yellow
				else { std::cout << i << "\n"; } // No Colour
			}
			else
			{
				// Background
				if (i.find("*") != std::string::npos) { std::cout << "\033[30;41m" << i << "\033[39m\033[49m\n"; } // Red
				else if (i.find("^") != std::string::npos) { std::cout << "\033[30;104m" << i << "\033[39m\033[49m\n"; } // Bright Blue
				else if (i.find("#") != std::string::npos) { std::cout << "\033[30;106m" << i << "\033[39m\033[49m\n"; } // Bright Cyan
				else if (i.find(">>") != std::string::npos) { std::cout << i << "\n"; } // No Colour : Chat
				else if (i.find("-->") != std::string::npos) { std::cout << "\033[30;101m" << i << "\033[39m\033[49m\n"; } // Bright Red
				else if (i.find("!") != std::string::npos) { std::cout << "\033[30;102m" << i << "\033[39m\033[49m\n"; } // Bright Green
				else if (i.find("=") != std::string::npos) { std::cout << "\033[30;42m" << i << "\033[39m\033[49m\n"; } // Green
				else if (i.find("_") != std::string::npos) { std::cout << "\033[30;103m" << i << "\033[39m\033[49m\n"; } // Bright Yellow
				else { std::cout << i << "\n"; } // No Colour
			}
		}
		else
		{
			std::cout << i << "\n";
		}
		output_lines.push_back(i);
	}
	output_lines.push_back("\nPlayer Statistics:");

	// Prints player list
	std::cout << "\n";
	for (int i = 0; i < session_players.size(); i++)
	{
		if (_otherPlayerStats)
		{
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
			//
			output_lines.push_back("-> User: \"" + session_players[i][0] + "\"");
			output_lines.push_back("#  Last recorded on the \"" + session_players[i][1] + "\" team.");
			output_lines.push_back("*  With a recorded score of \"" + session_players[i][2] + "\".");
			output_lines.push_back("-  With a recorded landing count of: \"" + session_players[i][3] + "\".");
			output_lines.push_back("!  With a recorded \"" + session_players[i][5] + "\" death(s).");
			output_lines.push_back("!  With a recorded \"" + session_players[i][6] + "\" crash(es).");
			output_lines.push_back("!  With a recorded \"" + session_players[i][7] + "\" bail-out(s).");
			output_lines.push_back("%  With a recorded \"" + session_players[i][8] + "\" connection(s).");
			output_lines.push_back("%  With a recorded \"" + session_players[i][9] + "\" disconnection(s).");
			output_lines.push_back("%  With a recorded \"" + session_players[i][10] + "\" message(s) sent.");
			output_lines.push_back("");
		}
		else
		{
			if (session_players[i][0] == username)
			{
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
					//
				output_lines.push_back("-> User: \"" + session_players[i][0] + "\"");
				output_lines.push_back("#  Last recorded on the \"" + session_players[i][1] + "\" team.");
				output_lines.push_back("*  With a recorded score of \"" + session_players[i][2] + "\".");
				output_lines.push_back("-  With a recorded landing count of: \"" + session_players[i][3] + "\".");
				output_lines.push_back("!  With a recorded \"" + session_players[i][5] + "\" death(s).");
				output_lines.push_back("!  With a recorded \"" + session_players[i][6] + "\" crash(es).");
				output_lines.push_back("!  With a recorded \"" + session_players[i][7] + "\" bail-out(s).");
				output_lines.push_back("%  With a recorded \"" + session_players[i][8] + "\" connection(s).");
				output_lines.push_back("%  With a recorded \"" + session_players[i][9] + "\" disconnection(s).");
				output_lines.push_back("%  With a recorded \"" + session_players[i][10] + "\" message(s) sent.");
				output_lines.push_back("");
			}
		}
	}
}

void saveFile()
	{
		std::ofstream file;
		// Combines the save location with the name that the file should be and the extension
		std::string file_name;
		if (output_name == "null")
		{
			// Sets unique filename
			if (user_system == "Linux") { file_name = output_dir + "/" + getDateTime(true) + ".txt"; }
			else if (user_system == "Windows") { file_name = output_dir + "\\" + getDateTime(true) + ".txt";}
		}
		else
		{
			// Sets file name to user set one
			file_name = output_dir + "\\" + output_name + ".txt";
		}
		// Attempts to open/create file
		file.open(file_name);

		// Checks if the file was opened successfully
		if (_debugInfo)
		{
    		if (file.is_open())
    		{
        		std::cout << "--! Mission File Successfully Created: \"" << file_name << "\"\n";
    		}
    		else
			{
        		std::cout << "--! Mission File Failed To Create!\n";
    		}
		}

		// Writes the lines from output_lines to the file
		for (auto& i : output_lines)
		{
    		file << i;
    		file << std::endl;
		}

		// Closes the file
		file.close();
	}

std::string getDateTime(std::string string)
{
	std::string element = string;
	std::string dateTime;
	int start = element.find_first_of('[');
	int end;
	dateTime = element.substr(start);
	end = dateTime.find_first_of(']');
	dateTime = dateTime.substr(0, end + 1);

	return dateTime;
}

void playerSideSwitchInfo(std::string string)
{
	if (string.find("enlists in the") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " enlists in the ");
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 16);
		// Gets the side that the player switched to
		y = findStrIndex(element_mod, " air force.");
		std::string get_side = element_mod.substr(0, y);
		// Sends message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _sideSwitchInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("will fly for the") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " will fly for the ");
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 18);
		// Gets the side that the player switched to
		y = findStrIndex(element_mod, " forces.");
		std::string get_side = element_mod.substr(0, y);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _sideSwitchInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("joins the") != std::string::npos && string.find(" army.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " joins the ");
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 11);
		// Gets the side that the player switched to
		y = findStrIndex(element_mod, " army.");
		std::string get_side = element_mod.substr(0, y);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _sideSwitchInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("has changed to the") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " has changed to the ");
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 20);
		// Gets the side that the player switched to
		y = findStrIndex(element_mod, " side.");
		std::string get_side = element_mod.substr(0, y);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _sideSwitchInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("is now with the") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " is now with the ");
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 17);
		// Gets the side that the player switched to
		y = findStrIndex(element_mod, " army.");
		std::string get_side = element_mod.substr(0, y);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _sideSwitchInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
	else if (string.find("has switched to") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " has switched to ");
		std::string get_name = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 17);
		// Gets the side that the player switched to
		x = element_mod.find_last_of(".");
		std::string get_side = element_mod.substr(0, x);
		// Sends the message
		// Checks if non-related info should be shown
		if (_sideSwitchInfo && _otherPlayerSideSwitchInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _sideSwitchInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " enlisted in the " + get_side + " forces.");
			}
		}
		// Checks if the player is in the list and changes side
		checkPlayerInList(get_name);
		changePlayerSide(get_name, get_side);
	}
}

void playerConnectionInfo(std::string string)
{
	// Player connection
	if (string.find("enters the battle.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " enters the battle");
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_playerConnectionInfo && _otherPlayerConnectionInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _playerConnectionInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerConnection(get_name);
	}
	else if (string.find("is now connected.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " is now connected.");
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_playerConnectionInfo && _otherPlayerConnectionInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _playerConnectionInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerConnection(get_name);
	}
	else if (string.find(" joins the server.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " joins the server.");
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_playerConnectionInfo && _otherPlayerConnectionInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _playerConnectionInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " connected to the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerConnection(get_name);
	}
	// Player disconnection
	if (string.find("Server: Goodbye, ") != std::string::npos)
	{
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
		if (_playerConnectionInfo && _otherPlayerConnectionInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " disconnected from the server.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _playerConnectionInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " disconnected from the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerDisconnection(get_name);
	}
	else if (string.find(" leaves the server.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element_mod);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username of the player
		int y = findStrIndex(element_mod, " leaves the server.");
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_playerConnectionInfo && _otherPlayerConnectionInfo)
		{
			log_processed.push_back("# At " + time + " " + get_name + " disconnected from the server.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _playerConnectionInfo)
			{
				log_processed.push_back("# At " + time + " " + get_name + " disconnected from the server.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		checkPlayerInList(get_name);
		addPlayerDisconnection(get_name);
	}
}

void landingInfo(std::string string)
{
	if (string.find("returned to base") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username
		int y = findStrIndex(element_mod, " returned to base.");
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_landingInfo && _otherLandingInfo)
		{
			log_processed.push_back("= At " + time + " " + get_name + " landed.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _landingInfo)
			{
				log_processed.push_back("= At " + time + " " + get_name + " landed.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
		{
			checkPlayerInList(get_name);
			addPlayerLanding(get_name);
		}
	}
	else if (string.find("is safe on the ground") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username
		int y = findStrIndex(element_mod, " is safe on the ground.");
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_landingInfo && _otherLandingInfo)
		{
			log_processed.push_back("= At " + time + " " + get_name + " landed.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _landingInfo)
			{
				log_processed.push_back("= At " + time + " " + get_name + " landed.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
		{
			checkPlayerInList(get_name);
			addPlayerLanding(get_name);
		}
	}
	else if (string.find("crash landed") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username
		int y = findStrIndex(element_mod, " crash landed.");
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_landingInfo && _otherLandingInfo)
		{
			log_processed.push_back("= At " + time + " " + get_name + " crash landed.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _landingInfo)
			{
				log_processed.push_back("= At " + time + " " + get_name + " crash landed.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
		{
			checkPlayerInList(get_name);
			addPlayerLanding(get_name);
		}
	}
	else if (string.find("landed") != std::string::npos && string.find("crash landed") == std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the username
		int y = findStrIndex(element_mod, " landed.");
		std::string get_name = element_mod.substr(0, y);
		// Sends the message
		// Checks if it should
		if (_landingInfo && _otherLandingInfo)
		{
			log_processed.push_back("= At " + time + " " + get_name + " landed.");
		}
		else
		{
			if (element.find(username) != std::string::npos && _landingInfo)
			{
				log_processed.push_back("= At " + time + " " + get_name + " landed.");
			}
		}
		// Adds the player to the list if not already and adds to connection count
		if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
		{
			checkPlayerInList(get_name);
			addPlayerLanding(get_name);
		}
	}
}

void crashInfo(std::string string)
{
	if (string.find(" collided with terrain.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the player/AI and aircraft information substring
		int y = findStrIndex(element_mod, " collided with terrain");
		std::string aircraft_pilot = element_mod.substr(0, y);
		// Gets the information
		// "<aircraft> flown by <name>"
		// "<name> in a <aircraft>"
		// "<player>'s <aircraft>"
		if (aircraft_pilot.find(" flown by ") != std::string::npos)
		{
			// Gets index of beginning
			y = findStrIndex(aircraft_pilot, " flown by ");
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(0, y);
			// Gets the player info
			std::string get_name = aircraft_pilot.substr(y + 10);
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo)
			{
				log_processed.push_back(message);
			}
			else
			{
				if (element.find(username) != std::string::npos && _crashInfo)
				{
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else if (aircraft_pilot.find(" in a ") != std::string::npos)
		{
			// Gets index of beginning
			y = findStrIndex(aircraft_pilot, " in a ");
			// Gets the name info
			std::string get_name = aircraft_pilot.substr(0, y);
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(y + 6);
			// Message
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo)
			{
				log_processed.push_back(message);
			}
			else
			{
				if (element.find(username) != std::string::npos && _crashInfo)
				{
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else if (aircraft_pilot.find("’") != std::string::npos)
		{
			// Gets index of beginning
			int x = aircraft_pilot.find_first_of("’");
			// Gets the name info
			std::string get_name = aircraft_pilot.substr(0, x);
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(x + 5);
			// Message
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo)
			{
				log_processed.push_back(message);
			}
			else
			{
				if (element.find(username) != std::string::npos && _crashInfo)
				{
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else
		{
			// AI
			std::string message = "_ At " + time + " " + aircraft_pilot + " crashed.";
			if (_crashInfo && _otherCrashInfo)
			{
				log_processed.push_back(message);
			}
			else
			{
				if (element.find(username) != std::string::npos && _crashInfo)
				{
					log_processed.push_back(message);
				}
			}
		}
	}
	else if (string.find(" crashed.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		// Gets the time
		std::string time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets the player/AI and aircraft information substring
		int y = findStrIndex(element_mod, " crashed.");
		std::string aircraft_pilot = element_mod.substr(0, y);
		// Gets the information
		// "<aircraft> flown by <name>"
		// "<name> in a <aircraft>"
		// "<player>'s <aircraft>"
		if (aircraft_pilot.find(" flown by ") != std::string::npos)
		{
			// Gets index of beginning
			y = findStrIndex(aircraft_pilot, " flown by ");
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(0, y);
			// Gets the player info
			std::string get_name = aircraft_pilot.substr(y + 10);
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo)
			{
				log_processed.push_back(message);
			}
			else
			{
				if (element.find(username) != std::string::npos && _crashInfo)
				{
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else if (aircraft_pilot.find(" in a ") != std::string::npos)
		{
			// Gets index of beginning
			y = findStrIndex(aircraft_pilot, " in a ");
			// Gets the name info
			std::string get_name = aircraft_pilot.substr(0, y);
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(y + 6);
			// Message
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo)
			{
				log_processed.push_back(message);
			}
			else
			{
				if (element.find(username) != std::string::npos && _crashInfo)
				{
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else if (aircraft_pilot.find("’") != std::string::npos)
		{
			// Gets index of beginning
			int x = aircraft_pilot.find_first_of("’");
			// Gets the name info
			std::string get_name = aircraft_pilot.substr(0, x);
			// Gets the aircraft info
			std::string get_aircraft = aircraft_pilot.substr(x + 5);
			// Message
			// Message
			std::string message = "_ At " + time + " " + get_name + " in a " + get_aircraft + " crashed.";
			if (_crashInfo && _otherCrashInfo)
			{
				log_processed.push_back(message);
			}
			else
			{
				if (element.find(username) != std::string::npos && _crashInfo)
				{
					log_processed.push_back(message);
				}
			}
			// Adds to player statistics if no AI
			if (get_name.find("AI") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerCrash(get_name);
			}
		}
		else
		{
			// AI
			std::string message = "_ At " + time + " " + aircraft_pilot + " crashed.";
			if (_crashInfo && _otherCrashInfo)
			{
				log_processed.push_back(message);
			}
			else
			{
				if (element.find(username) != std::string::npos && _crashInfo)
				{
					log_processed.push_back(message);
				}
			}
		}
	}
}

void bailInfo(std::string string)
{
	// Bailing Out
	if (string.find(" had to jump.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (element_mod.find(" of an ") != std::string::npos)
		{
			// <crew name> of an <aircraft> had to jump.
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos)
			{
				// Get beginning of " had to jump."
				int y = findStrIndex(element_mod, " had to jump.");
				std::string info = element_mod.substr(0, y);
				// Separates the crew and aircraft
				y = findStrIndex(info, " on an AI ");
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 10);
				// Sends message
				if (_bailInfo && _otherBailInfo)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else if (element_mod.find(" of a ") != std::string::npos)
		{
			// <crew name> of a <aircraft> had to jump.
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos)
			{
				// Get beginning of " had to jump."
				int y = findStrIndex(element_mod, " had to jump.");
				std::string info = element_mod.substr(0, y);
				// Separate the crew and aircraft
				y = findStrIndex(info, " of a ");
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 6);
				// Gets the end of "AI" in <get_name>
				char end_ai[] = "AI ";
				y = findStrIndex(get_name, "AI ");
				get_name = get_name.substr(y + 3);
				// Sends message
				if (_bailInfo && _otherBailInfo)
				{
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos)
				{
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else
		{
			// <aircraft> <username> had to jump.
			// Checks if does not contain "AI"
			if (element_mod.find("AI") == std::string::npos)
			{
				x = element_mod.find_last_of(")");
				std::string get_aircraft = element_mod.substr(0, x + 1);
				std::string get_name = element_mod.substr(x + 2);
				int y = findStrIndex(get_name, " had to jump.");
				get_name = get_name.substr(0, y);
				// Sends the message
				if (_bailInfo && _otherBailInfo)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				// Adds bail-out info to user profile
				// If get_name contains "and crew", separate it first
				if (get_name.find(" and crew") != std::string::npos)
				{
					y = findStrIndex(get_name, " and crew");
					get_name = get_name.substr(0, y);
				}
				checkPlayerInList(get_name);
				addPlayerBail(get_name);
			}
		}
	}
	else if (string.find(" bailed out.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (element_mod.find(" of an ") != std::string::npos)
		{
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos)
			{
				int y = findStrIndex(element_mod, " bailed out.");
				std::string info = element_mod.substr(0, y);
				// Separates the crew and aircraft
				y = findStrIndex(info, " of an AI ");
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 10);
				// Sends message
				if (_bailInfo && _otherBailInfo)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else if (element_mod.find(" of a ") != std::string::npos)
		{
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos)
			{
				int y = findStrIndex(element_mod, " bailed out.");
				std::string info = element_mod.substr(0, y);
				// Separate the crew and aircraft
				y = findStrIndex(info, " of a ");
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 6);
				// Gets the end of "AI" in <get_name>
				y = findStrIndex(get_name, "AI ");
				get_name = get_name.substr(y + 3);
				// Sends message
				if (_bailInfo && _otherBailInfo)
				{
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos)
				{
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else
		{
			// Checks if does not contain "AI"
			if (element_mod.find("AI") == std::string::npos)
			{
				x = element_mod.find_last_of(")");
				std::string get_aircraft = element_mod.substr(0, x + 1);
				std::string get_name = element_mod.substr(x + 2);
				int y = findStrIndex(get_name, " bailed out.");
				get_name = get_name.substr(0, y);
				// Sends the message
				if (_bailInfo && _otherBailInfo)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				// Adds bail-out info to user profile
				// If get_name contains "and crew", separate it first
				if (get_name.find(" and crew") != std::string::npos)
				{
					y = findStrIndex(get_name, " and crew");
					get_name = get_name.substr(0, y);
				}
				checkPlayerInList(get_name);
				addPlayerBail(get_name);
			}
		}
	}
	else if (string.find(" hit the silk.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (element_mod.find(" of an ") != std::string::npos)
		{
			// Checks if AI
			if (element_mod.find("AI") != std::string::npos)
			{
				int y = findStrIndex(element_mod, " hit the silk.");
				std::string info = element_mod.substr(0, y);
				// Separates the crew and aircraft
				y = findStrIndex(info, " of an AI ");
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 10);
				// Sends message
				if (_bailInfo && _otherBailInfo)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
		}
		else if (element_mod.find(" of a ") != std::string::npos)
		{
			if (element_mod.find("AI") != std::string::npos)
			{
				// Is AI
				int y = findStrIndex(element_mod, " hit the silk.");
				std::string info = element_mod.substr(0, y);
				// Separate the crew and aircraft
				y = findStrIndex(info, " of a ");
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 6);
				// Gets the end of "AI" in <get_name>
				y = findStrIndex(get_name, "AI ");
				get_name = get_name.substr(y + 3);
				// Sends message
				if (_bailInfo && _otherBailInfo)
				{
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos)
				{
					log_processed.push_back("^ At " + get_time + " The " + get_name + " of a " + get_aircraft + " (AI) bailed out.");
				}
			}
			else
			{
				// Not AI
				int y = findStrIndex(element_mod, " hit the silk.");
				std::string info = element_mod.substr(0, y);
				// Separate the crew and the aircraft
				y = findStrIndex(info, " of a ");
				std::string get_name = info.substr(0, y);
				std::string get_aircraft = info.substr(y + 6);
				// Gets the username in " The <username>
				y = findStrIndex(get_name, "The ");
				get_name = get_name.substr(y + 4);
				// Sends message
				if (_bailInfo && _otherBailInfo)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				else if (_bailInfo && get_name.find(username) != std::string::npos)
				{
					log_processed.push_back("^ At " + get_time + " " + get_name + " of a " + get_aircraft + " bailed out.");
				}
				// Adds bail-out info to user profile
				// If get_name contains "and crew", separate it first
				if (get_name.find(" and crew") != std::string::npos)
				{
					y = findStrIndex(get_name, " and crew");
					get_name = get_name.substr(0, y);
				}
				checkPlayerInList(get_name);
				addPlayerBail(get_name);
			}
		}

	}
	// Landed on the ground
	else if (string.find(" parachuted to the ground.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (_bailInfo && _otherBailInfo)
		{
			log_processed.push_back("= At " + get_time + " " + element_mod);
		}
		else if (_bailInfo && element_mod.find(username) != std::string::npos)
		{
			log_processed.push_back("= At " + get_time + " " + element_mod);
		}
	}
	// Splatted
	// Parachute Failed to Deploy
	else if (string.find("'s parachute failed to deploy.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		if (_bailInfo && _otherBailInfo)
		{
			log_processed.push_back("_ At " + get_time + " " + element_mod);
		}
		else if (_bailInfo && element_mod.find(username) != std::string::npos)
		{
			log_processed.push_back("= At " + get_time + " " + element_mod);
		}
		// Gets username and adds to death count.
		std::string get_username = element_mod.substr(0, element_mod.find_first_of("'"));
		checkPlayerInList(get_username);
		addPlayerDeath(get_username);
	}
}

void destructionInfo(std::string string)
{
	if (string.find("was destroyed.") != std::string::npos)
	{
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		// Remove first part of string ([xx:xx:xx] Server: )
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Remove end part of string ([...] was destroyed.)
		x = 0;
		int y = findStrIndex(element_mod, " was destroyed.");
		element_mod = element_mod.substr(0, x);

		// "’" = <player>’s <aircraft>
		// " in a " = <name> in a <aircraft>
		// " flown by " = <aircraft> flown by <name>
		// Else: AI = <aircraft> was destroyed.
		std::string destroyed;
		if (element_mod.find("’") != std::string::npos)
		{
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
		else if (element_mod.find(" in a ") != std::string::npos)
		{
			// Gets name and aircraft
			x = 0;
			int y = findStrIndex(element_mod, " in a ");
			//
			std::string get_name = element_mod.substr(0, x);
			std::string get_aircraft = element_mod.substr(x + 6);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if username is in the list and adds death (if not AI)
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (element_mod.find(" flown by ") != std::string::npos)
		{
			// Gets name and aircraft
			x = 0;
			int y = findStrIndex(element_mod, " flown by ");
			//
			std::string get_aircraft = element_mod.substr(0, x);
			std::string get_name = element_mod.substr(x + 10);
			destroyed = get_name + " in a " + get_aircraft;
			// Checks if username is in the list and adds death (if not AI)
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else
		{
			// AI
			destroyed = element_mod;
		}
		// Creates and sends message
		// Creates and sends message
		std::string message = "* At " + get_time + " ";
		// Gets aircraft destroyed
		message = message + destroyed + " was destroyed.";
		// Checks if related to player
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("destroyed") != std::string::npos) && (string.find("was destroyed by") == std::string::npos))
	{
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
		while (valid)
		{
			// Checks if element mod still contains a ","
			// That means that there are more contributors
			if (element_mod.find(",") != std::string::npos)
			{
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string append = element_mod.substr(0, x);
				// Checks if <append> contains a "’" or "in a" or "flown by"
				// If so, then it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " in a ");
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// AI Aircraft
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else
			{
				// At the end of contributors, gets last contributor
				int y = findStrIndex(element_mod, " destroyed ");
				//
				std::string append = element_mod.substr(0, y);
				// Checks if <append> contains a "’" or "in a"
				// If so, then it is a player
				if (append.find("’") != std::string::npos)
				{
					// Player
					x = append.find_first_of("’");
					std::string get_name = append.substr(0, x);
					append = append.substr(x + 4);
					std::string get_aircraft = append;
					// Set string
					std::string message = get_name + " in a" + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" in a ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " in a ");
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// AI Aircraft
					contributors.push_back(append);
				}
				valid = false;
			}
		}
		// Gets destroyed aircraft
		// Gets index of "destroyed"
		int y = findStrIndex(element_mod, "destroyed") + 10;
		element_mod = element_mod.substr(y);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		// Done by checking if destroy contains "in a" or a "’"
		if (destroyed.find(" in a ") != std::string::npos)
		{
			// Gets the beginning of " in a "
			y = findStrIndex(destroyed, " in a ");
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos)
		{
			// Player
			int y = findStrIndex(element_mod, " flown by ");
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			std::string get_name;
			if (user_system == "Windows")
			{
				get_name = destroyed.substr(0, destroyed.length() - 1);
				destroyed = get_name + " in a " + get_aircraft;
			}
			else if (user_system == "Linux")
			{
				get_name = destroyed.substr(0, destroyed.length() - 2);
			}
			// Create Message
			destroyed = get_name + " in a " + get_aircraft + ".";
			element_mod = element_mod.substr(x + 2);
			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos)
		{
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
		for (int i = 0; i < contributors.size(); i++)
		{
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("was destroyed by") != std::string::npos))
	{
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
		int y = findStrIndex(element_mod, " was destroyed by ");
		destroyed = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 18);
		// Checks if the aircraft shot down was a player
		// Done by checking if <destroyed> contains either a "’" or "in a"
		if (destroyed.find("’") != std::string::npos)
		{
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
		else if (destroyed.find(" in a ") != std::string::npos)
		{
			// Gets the beginning of " in a "
			int y = findStrIndex(destroyed, " in a ");
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos)
		{
			// Gets the beginning of flown by
			// Gets the beginning of " in a "
			int y = findStrIndex(destroyed, " flown by ");
			// Sets the username and aircraft
			std::string aircraft = destroyed.substr(0, y);
			std::string get_name = destroyed.substr(y + 10);
			destroyed = get_name + " in a " + aircraft;
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}


		// Gets contributor info
		bool valid = true;
		while (valid)
		{
			// Checks element mod still contains a ","
			// If so, then there are more contributors
			// Else, there are none left
			if (element_mod.find(",") != std::string::npos)
			{
				// Gets next contributor
				x = element_mod.find_first_of(',');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Gets beginning
					int y = findStrIndex(append, " in a ");
					// Sets vars
					std::string get_name = append.substr(0, y);
					std::string get_aircraft = append.substr(y + 6);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// If AI aircraft
					x = element_mod.find_first_of(',');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else
			{
				// Gets info
				x = element_mod.find_last_of('.');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Gets beginning
					int y = findStrIndex(append, "in a");
					// Sets vars
					std::string get_name = append.substr(0, y - 1);
					std::string get_aircraft = append.substr(y + 5);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
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
		for (int i = 0; i < contributors.size(); i++)
		{
			message = message + contributors[i] + ", ";
			// Checks if user contributed to attack
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed + ".";
		// Checks if related
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("was shot down by") != std::string::npos))
	{
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
		int y = findStrIndex(element_mod, " was shot down by ");
		destroyed = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 18);
		// Checks if the aircraft shot down was a player
		// Done by checking if <destroyed> contains either a "’" or "in a"
		if (destroyed.find("’") != std::string::npos)
		{
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
		else if (destroyed.find(" in a ") != std::string::npos)
		{
			// Gets the beginning of " in a "
			int y = findStrIndex(destroyed, " in a ");
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos)
		{
			// Gets the beginning of flown by
			int y = findStrIndex(destroyed, " flown by ");
			// Sets the username and aircraft
			std::string aircraft = destroyed.substr(0, y);
			std::string get_name = destroyed.substr(y + 10);
			destroyed = get_name + " in a " + aircraft;
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}


		// Gets contributor info
		bool valid = true;
		while (valid)
		{
			// Checks element mod still contains a ","
			// If so, then there are more contributors
			// Else, there are none left
			if (element_mod.find(",") != std::string::npos)
			{
				// Gets next contributor
				x = element_mod.find_first_of(',');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Gets beginning
					int y = findStrIndex(append, " in a ");
					// Sets vars
					std::string get_name = append.substr(0, y);
					std::string get_aircraft = append.substr(y + 6);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// If AI aircraft
					x = element_mod.find_first_of(',');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else
			{
				// Gets info
				x = element_mod.find_last_of('.');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Gets beginning
					int y = findStrIndex(append, "in a");
					// Sets vars
					std::string get_name = append.substr(0, y - 1);
					std::string get_aircraft = append.substr(y + 5);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
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
		for (int i = 0; i < contributors.size(); i++)
		{
			message = message + contributors[i] + ", ";
			// Checks if user contributed to attack
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed + ".";
		// Checks if related
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("shot down") != std::string::npos) && (string.find("was shot down by") == std::string::npos))
	{
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
		while (valid)
		{
			// Checks if there is more than 1 contributor left
			// This is done by checking for a ","
			if (element_mod.find(",") != std::string::npos)
			{
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string contributor = element_mod.substr(0, x);
				// Checks if the contributor is a player or not
				if (contributor.find("’") != std::string::npos)
				{
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
				else if (contributor.find(" in a ") != std::string::npos)
				{
					// Player
					// Get beginning index of " in a "
					int y = findStrIndex(contributor, " in a ");
					// Pushes back contributor
					std::string get_name = contributor.substr(0, y);
					contributors.push_back(contributor);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if the player is in the player list and adds score
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (contributor.find(" flown by ") != std::string::npos)
				{
					// Player
					// Get beginning index of " flown by "
					int y = findStrIndex(contributor, " flown by ");
					// Sets the username and aircraft, then pushes back
					std::string get_aircraft = contributor.substr(0, y);
					contributor = contributor.substr(y + 10);
					std::string get_name = contributor;
					contributors.push_back(get_name + " in a " + get_aircraft);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// AI
					contributors.push_back(contributor);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
				}
			}
			else
			{
				// Last contributor
				// Gets the beginning of " shot down "
				int y = findStrIndex(element_mod, " shot down ");
				//
				std::string contributor = element_mod.substr(0, y);
				// Checks if the contributor is a player
				if (contributor.find("’") != std::string::npos)
				{
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
				else if (contributor.find(" in a ") != std::string::npos)
				{
					// Player
					// Get beginning index of " in a "
					int y = findStrIndex(contributor, " in a ");
					// Pushes back contributor
					std::string get_name = contributor.substr(0, y);
					contributors.push_back(contributor);
					// Checks if the player is in the player list and adds score
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (contributor.find(" flown by ") != std::string::npos)
				{
					// Player
					// Get beginning index of " flown by "
					int y = findStrIndex(contributor, " flown by ");
					// Sets the username and aircraft, then pushes back
					std::string get_aircraft = contributor.substr(0, y);
					contributor = contributor.substr(y + 10);
					std::string get_name = contributor;
					contributors.push_back(get_name + " in a " + get_aircraft);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// AI
					contributors.push_back(contributor);
				}
				valid = false;
			}
		}
		// Gets the destroyed aircraft
		// Gets index of "shot down"
		element_mod = element;
		int y = findStrIndex(element_mod, " shot down ") + 11;
		element_mod = element_mod.substr(y, element_mod.length() - 1);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		if (destroyed.find(" in a ") != std::string::npos)
		{
			// Gets the beginning of " in a "
			y = findStrIndex(destroyed, " in a ");
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos)
		{
			// Player
			int y = findStrIndex(element_mod, " flown by ");
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			x = destroyed.find_last_of(".");
			std::string get_name = destroyed.substr(0, x);
			// Set message
			destroyed = get_name + " in a " + get_aircraft;
			element_mod = element_mod.substr(x + 2);
			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos)
		{
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
		for (int i = 0; i < contributors.size(); i++)
		{
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find(" claimed a friendly kill: ") != std::string::npos) && (string.find("was destroyed by") == std::string::npos))
	{
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
		while (valid)
		{
			// Checks if element mod still contains a ","
			// That means that there are more contributors
			if (element_mod.find(",") != std::string::npos)
			{
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string append = element_mod.substr(0, x);
				// Checks if <append> contains a "’" or "in a" or "flown by"
				// If so, then it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " in a ");
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
					}
				}
				else
				{
					// AI Aircraft
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else
			{
				// At the end of contributors, gets last contributor
				int y = findStrIndex(element_mod, " claimed a friendly kill:");
				//
				std::string append = element_mod.substr(0, y);
				// Checks if <append> contains a "’" or "in a"
				// If so, then it is a player
				if (append.find("’") != std::string::npos)
				{
					// Player
					x = append.find_first_of("’");
					std::string get_name = append.substr(0, x);
					append = append.substr(x + 4);
					std::string get_aircraft = append;
					// Set string
					std::string message = get_name + " in a" + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
					}
				}
				else if (append.find(" in a ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " in a ");
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
					}
				}
				else
				{
					// AI Aircraft
					contributors.push_back(append);
				}
				valid = false;
			}
		}
		// Gets destroyed aircraft
		// Gets index of "friendly kill: "
		element_mod = element;
		int y = findStrIndex(element_mod, "friendly kill: ") + 15;
		element_mod = element_mod.substr(y);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		// Done by checking if destroy contains "in a" or a "’"
		if (destroyed.find(" in a ") != std::string::npos)
		{
			// Gets the beginning of " in a "
			y = findStrIndex(destroyed, " in a ");
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos)
		{
			// Player
			int y = findStrIndex(element_mod, " flown by ");
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			std::string get_name = destroyed.substr(0, destroyed.length() - 1);
			// Set message
			destroyed = get_name + " in a " + get_aircraft + ".";

			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos)
		{
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
		for (int i = 0; i < contributors.size(); i++)
		{
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "destroyed a friendly " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("claimed") != std::string::npos))
	{
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
		while (valid)
		{
			// Checks if there is more than 1 contributor left
			// This is done by checking for a ","
			if (element_mod.find(",") != std::string::npos)
			{
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string contributor = element_mod.substr(0, x);
				// Checks if the contributor is a player or not
				if (contributor.find("’") != std::string::npos)
				{
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
				else if (contributor.find(" in a ") != std::string::npos)
				{
					// Player
					// Get beginning index of " in a "
					int y = findStrIndex(contributor, " in a ");
					// Pushes back contributor
					std::string get_name = contributor.substr(0, y);
					contributors.push_back(contributor);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if the player is in the player list and adds score
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (contributor.find(" flown by ") != std::string::npos)
				{
					// Player
					// Get beginning index of " flown by "
					int y = findStrIndex(contributor, " flown by ");
					// Sets the username and aircraft, then pushes back
					std::string get_aircraft = contributor.substr(0, y);
					contributor = contributor.substr(y + 10);
					std::string get_name = contributor;
					contributors.push_back(get_name + " in a " + get_aircraft);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// AI
					contributors.push_back(contributor);
					element_mod = element_mod.substr(element_mod.find_first_of(",") + 2);
				}
			}
			else
			{
				// Last contributor
				// Gets the beginning of " claimed "
				int y = findStrIndex(element_mod, " claimed ");
				//
				std::string contributor = element_mod.substr(0, y);
				// Checks if the contributor is a player
				if (contributor.find("’") != std::string::npos)
				{
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
				else if (contributor.find(" in a ") != std::string::npos)
				{
					// Player
					// Get beginning index of " in a "
					int y = findStrIndex(contributor, " in a ");
					// Pushes back contributor
					std::string get_name = contributor.substr(0, y);
					contributors.push_back(contributor);
					// Checks if the player is in the player list and adds score
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (contributor.find(" flown by ") != std::string::npos)
				{
					// Player
					// Get beginning index of " flown by "
					int y = findStrIndex(contributor, " flown by ");
					// Sets the username and aircraft, then pushes back
					std::string get_aircraft = contributor.substr(0, y);
					contributor = contributor.substr(y + 10);
					std::string get_name = contributor;
					contributors.push_back(get_name + " in a " + get_aircraft);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// AI
					contributors.push_back(contributor);
				}
				valid = false;
			}
		}
		// Gets the destroyed aircraft
		// Gets index of " claimed "
		element_mod = element;
		int y = findStrIndex(element_mod, " claimed ") + 9;
		element_mod = element_mod.substr(y, element_mod.length() - 1);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		if (destroyed.find(" in a ") != std::string::npos)
		{
			// Gets the beginning of " in a "
			y = findStrIndex(destroyed, " in a ");
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos)
		{
			// Player
			int y = findStrIndex(element_mod, " flown by ");
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			x = destroyed.find_last_of(".");
			std::string get_name = destroyed.substr(0, x);
			// Set message
			destroyed = get_name + " in a " + get_aircraft;
			element_mod = element_mod.substr(x + 2);
			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos)
		{
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
		for (int i = 0; i < contributors.size(); i++)
		{
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "destroyed " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("was blown up by") != std::string::npos))
	{
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
		int y = findStrIndex(element_mod, " was blown up by ");
		destroyed = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 17);
		// Checks if the aircraft shot down was a player
		// Done by checking if <destroyed> contains either a "’" or "in a"
		if (destroyed.find("’") != std::string::npos)
		{
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
		else if (destroyed.find(" in a ") != std::string::npos)
		{
			// Gets the beginning of " in a "
			int y = findStrIndex(destroyed, " in a ");
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos)
		{
			// Gets the beginning of flown by
			// Gets the beginning of " in a "
			int y = findStrIndex(destroyed, " flown by ");
			// Sets the username and aircraft
			std::string aircraft = destroyed.substr(0, y);
			std::string get_name = destroyed.substr(y + 10);
			destroyed = get_name + " in a " + aircraft;
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}


		// Gets contributor info
		bool valid = true;
		while (valid)
		{
			// Checks element mod still contains a ","
			// If so, then there are more contributors
			// Else, there are none left
			if (element_mod.find(",") != std::string::npos)
			{
				// Gets next contributor
				x = element_mod.find_first_of(',');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Gets beginning
					int y = findStrIndex(append, " in a ");
					// Sets vars
					std::string get_name = append.substr(0, y);
					std::string get_aircraft = append.substr(y + 6);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// If AI aircraft
					x = element_mod.find_first_of(',');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else
			{
				// Gets info
				// Gets the beginning of " before"
				int y = findStrIndex(element_mod, " before ");
				std::string append = element_mod.substr(0, y);
				append = append;
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos)
				{
					x = element_mod.find_first_of("’");
					std::string get_name = element_mod.substr(0, x);
					element_mod = element_mod.substr(x + 4);
					int y = findStrIndex(element_mod, " before he could take off");
					std::string get_aircraft = element_mod.substr(0, y);
					append = get_name + " in a" + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(0, x);
					// Checks if player is in player list and adds score
					checkPlayerInList(get_name);
					addPlayerScore(get_name);
				}
				else if (append.find(" in a ") != std::string::npos)
				{
					// Gets beginning
					int y = findStrIndex(append, "in a");
					// Sets vars
					std::string get_name = append.substr(0, y - 1);
					std::string get_aircraft = append.substr(y + 5);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
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
		for (int i = 0; i < contributors.size(); i++)
		{
			message = message + contributors[i] + ", ";
			// Checks if user contributed to attack
		}
		// Gets aircraft destroyed
		message = message + "blew up " + destroyed + ".";
		// Checks if related
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("sank") != std::string::npos))
	{
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
		while (valid)
		{
			// Checks if element mod still contains a ","
			// That means that there are more contributors
			if (element_mod.find(",") != std::string::npos)
			{
				// Gets the next contributor
				x = element_mod.find_first_of(",");
				std::string append = element_mod.substr(0, x);
				// Checks if <append> contains a "’" or "in a" or "flown by"
				// If so, then it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " in a ");
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// AI Aircraft
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else
			{
				// At the end of contributors, gets last contributor
				int y = findStrIndex(element_mod, " sank ");
				//
				std::string append = element_mod.substr(0, y);
				// Checks if <append> contains a "’" or "in a"
				// If so, then it is a player
				if (append.find("’") != std::string::npos)
				{
					// Player
					x = append.find_first_of("’");
					std::string get_name = append.substr(0, x);
					append = append.substr(x + 4);
					std::string get_aircraft = append;
					// Set string
					std::string message = get_name + " in a" + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)\
					{
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" in a ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " in a ");
					//
					std::string get_name = append.substr(0, y);
					append = append.substr(y + 6);
					std::string get_aircraft = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds to score
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// AI Aircraft
					contributors.push_back(append);
				}
				valid = false;
			}
		}
		// Gets destroyed aircraft
		// Gets index of "sank"
		element_mod = element;
		int y = findStrIndex(element_mod, "sank") + 5;
		element_mod = element_mod.substr(y);
		destroyed = element_mod;
		// Checks if the destroyed aircraft is a player
		// Done by checking if destroy contains "in a" or a "’"
		if (destroyed.find(" in a ") != std::string::npos)
		{
			// Gets the beginning of " in a "
			y = findStrIndex(destroyed, " in a ");
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos)
		{
			// Player
			int y = findStrIndex(element_mod, " flown by ");
			//
			std::string get_aircraft = destroyed.substr(0, y);
			destroyed = destroyed.substr(y + 10);
			std::string get_name = destroyed.substr(0, destroyed.length() - 1);
			// Set message
			destroyed = get_name + " in a " + get_aircraft;
			element_mod = element_mod.substr(x + 2);
			// Checks if player is in player list and adds score
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find("’") != std::string::npos)
		{
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
		for (int i = 0; i < contributors.size(); i++)
		{
			message = message + contributors[i] + ", ";
		}
		// Gets aircraft destroyed
		message = message + "sank " + destroyed;
		// Checks if related to player
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find("goes down thanks to") != std::string::npos))
	{
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
		int y = findStrIndex(element_mod, " goes down thanks to ");
		destroyed = element_mod.substr(0, y);
		element_mod = element_mod.substr(y + 21);
		// Checks if the aircraft shot down was a player
		// Done by checking if <destroyed> contains either a "’" or "in a"
		if (destroyed.find("’") != std::string::npos)
		{
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
		else if (destroyed.find(" in a ") != std::string::npos)
		{
			// Gets the beginning of " in a "
			int y = findStrIndex(destroyed, " in a ");
			// Sets the username
			std::string get_name = destroyed.substr(0, y);
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (destroyed.find(" flown by ") != std::string::npos)
		{
			// Gets the beginning of flown by
			int y = findStrIndex(destroyed, " flown by ");
			// Sets the username and aircraft
			std::string aircraft = destroyed.substr(0, y);
			std::string get_name = destroyed.substr(y + 10);
			destroyed = get_name + " in a " + aircraft;
			// If not "AI"
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				// Checks if the username is already in the list and adds a user death count to it
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}


		// Gets contributor info
		bool valid = true;
		while (valid)
		{
			// Checks element mod still contains a ","
			// If so, then there are more contributors
			// Else, there are none left
			if (element_mod.find(",") != std::string::npos)
			{
				// Gets next contributor
				x = element_mod.find_first_of(',');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Gets beginning
					int y = findStrIndex(append, " in a ");
					// Sets vars
					std::string get_name = append.substr(0, y);
					std::string get_aircraft = append.substr(y + 6);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					element_mod = element_mod.substr(x + 2);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
					// If AI aircraft
					x = element_mod.find_first_of(',');
					append = element_mod.substr(0, x);
					contributors.push_back(append);
					element_mod = element_mod.substr(x + 2);
				}
			}
			else
			{
				// Gets info
				x = element_mod.find_last_of('.');
				std::string append = element_mod.substr(0, x);
				// Checks if append contains a "’"; This means that it is a player
				if (append.find("’") != std::string::npos)
				{
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
				else if (append.find(" in a ") != std::string::npos)
				{
					// Gets beginning
					int y = findStrIndex(append, "in a");
					// Sets vars
					std::string get_name = append.substr(0, y - 1);
					std::string get_aircraft = append.substr(y + 5);
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(append);
					// If not "AI"
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						// Checks if the username is already in the list and adds a user death count to it
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else if (append.find(" flown by ") != std::string::npos)
				{
					// Player
					int y = findStrIndex(append, " flown by ");
					//
					std::string get_aircraft = append.substr(0, y);
					append = append.substr(y + 10);
					std::string get_name = append;
					// Set message
					std::string message = get_name + " in a " + get_aircraft;
					contributors.push_back(message);
					// Checks if player is in player list and adds score
					if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
					{
						checkPlayerInList(get_name);
						addPlayerScore(get_name);
					}
				}
				else
				{
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
		for (int i = 0; i < contributors.size(); i++)
		{
			message = message + contributors[i] + ", ";
			// Checks if user contributed to attack
		}
		// Gets aircraft destroyed
		message = message + "sank " + destroyed + ".";
		// Checks if related
		if (_destructionUnrelatedInfo)
		{
			log_processed.push_back(message);
		}
		else
		{
			if (element.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
		}
	}
	else if ((string.find(" is no more.") != std::string::npos))
	{
		std::string element = string;
		std::string element_mod = element;
		std::string get_time = getDateTime(element);
		int x = element_mod.find_last_of(":");
		element_mod = element_mod.substr(x + 2);
		// Gets info
		// Gets beginning of " is no more"
		int y = findStrIndex(element_mod, " is no more.");
		element_mod = element_mod.substr(0, y);
		// <aircraft> <name>
		// Get aircraft/name info
		if (element_mod.find(" flown by ") != std::string::npos)
		{
			y = findStrIndex(element_mod, " flown by ");
			std::string get_aircraft = element_mod.substr(0, y);
			std::string get_name = element_mod.substr(y + 10);
			// Sends message
			std::string message = "* At " + get_time + " " + get_name + " in a " + get_aircraft + " is no more.";
			if (_destructionInfo && _destructionUnrelatedInfo)
			{
				log_processed.push_back(message);
			}
			else if (_destructionInfo && get_name.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
			// Checks if name is in list if not AI
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (element_mod.find(" in a ") != std::string::npos)
		{
			y = findStrIndex(element_mod, " in a ");
			std::string get_name = element_mod.substr(0, y);
			std::string get_aircraft = element_mod.substr(y + 6);
			// Sends message
			std::string message = "* At " + get_time + " " + get_name + " in a " + get_aircraft + " is no more.";
			if (_destructionInfo && _destructionUnrelatedInfo)
			{
				log_processed.push_back(message);
			}
			else if (_destructionInfo && get_name.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
			// Checks if name is in list if not AI
			if (get_name != "AI" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
		else if (element_mod.find("’") != std::string::npos)
		{
			int x = element_mod.find_first_of("’");
			std::string get_name = element_mod.substr(0, x);
			std::string get_aircraft = element_mod.substr(x + 4);
			// Sends message
			std::string message = "* At " + get_time + " " + get_name + " in a" + get_aircraft + " is no more.";
			if (_destructionInfo && _destructionUnrelatedInfo)
			{
				log_processed.push_back(message);
			}
			else if (_destructionInfo && get_name.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
			// Checks if name is in list
			checkPlayerInList(get_name);
			addPlayerDeath(get_name);
		}
		else
		{
			// <aircraft> <name>
			// Aircraft
			int x = element_mod.find_first_of(")");
			std::string get_aircraft = element_mod.substr(0, x + 1);
			std::string get_name = element_mod.substr(x + 2);
			// Sends message
			std::string message = "* At " + get_time + " " + get_name + " in a " + get_aircraft + " is no more.";
			if (_destructionInfo && _destructionUnrelatedInfo)
			{
				log_processed.push_back(message);
			}
			else if (_destructionInfo && get_name.find(username) != std::string::npos)
			{
				log_processed.push_back(message);
			}
			// Checks if name is in list
			if (get_name != "AI" && get_name != "(AI)" && get_name != "a friendly AI" && get_name.find("(AI)") == std::string::npos)
			{
				checkPlayerInList(get_name);
				addPlayerDeath(get_name);
			}
		}
	}
}

void messageInfo(std::string string)
{
	std::string element = string;
	std::string element_mod = element;
	std::string time = getDateTime(element);
	int y = findStrIndex(element_mod, "Chat: ");
	element_mod = element_mod.substr(y + 6);
	// Gets the sender of the message
	y = element_mod.find_first_of(":");
	std::string get_name = element_mod.substr(0, y);
	element_mod = element_mod.substr(y + 3);
	// Gets the message
	std::string message = element_mod;
	// Pushes to log processed
	if (_showChat)
	{
		log_processed.push_back(">> At " + time + " " + get_name + " sent: \"" + message + "\" in chat.");
	}
	// Checks if the player is in the player list
	checkPlayerInList(get_name);
	addPlayerMessageSent(get_name);
}

// Player List Related
void checkPlayerInList(std::string player_name)
{
	// Loops through "session_players" to see if string is in it
	bool _inList = false;
	for (int i = 0; i < session_players.size(); i++)
	{
		// Checks if player is in list
		if (session_players[i][0] == player_name)
		{
			_inList = true;
		}
	}
	// If not in list
	if (!_inList)
	{
		session_players.push_back({ player_name, "not recorded", "0", "0", "0", "0", "0", "0", "0", "0", "0" });
	}
}
void changePlayerSide(std::string player_name, std::string player_side)
{
	// Changes the correct players side info
	for (int i = 0; i < session_players.size(); i++)
	{
		// Checks if player is in list
		if (session_players[i][0] == player_name)
		{
			session_players[i][1] = player_side;
		}
	}
}
void addPlayerScore(std::string player_name)
{
	// Changes the correct players score
	for (int i = 0; i < session_players.size(); i++)
	{
		if (session_players[i][0] == player_name)
		{
			session_players[i][2] = std::to_string(std::stoi(session_players[i][2]) + 1);
		}
	}
}
void addPlayerLanding(std::string player_name)
{
	// Changes the correct players death info
	for (int i = 0; i < session_players.size(); i++)
	{
		if (session_players[i][0] == player_name)
		{
			session_players[i][3] = std::to_string(std::stoi(session_players[i][3]) + 1);
		}
	}
}
void addPlayerDeath(std::string player_name)
{
	// Changes the correct players death info
	for (int i = 0; i < session_players.size(); i++)
	{
		if (session_players[i][0] == player_name)
		{
			session_players[i][5] = std::to_string(std::stoi(session_players[i][5]) + 1);
		}
	}
}
void addPlayerCrash(std::string player_name)
{
	// Changes the correct players crash info
	for (int i = 0; i < session_players.size(); i++)
	{
		if (session_players[i][0] == player_name)
		{
			session_players[i][6] = std::to_string(std::stoi(session_players[i][6]) + 1);
		}
	}
}
void addPlayerBail(std::string player_name)
{
	// Changes the correct players death info
	for (int i = 0; i < session_players.size(); i++)
	{
		if (session_players[i][0] == player_name)
		{
			session_players[i][7] = std::to_string(std::stoi(session_players[i][7]) + 1);
		}
	}
}
void addPlayerConnection(std::string player_name)
{
	// Changes the correct players connection info
	for (int i = 0; i < session_players.size(); i++)
	{
		if (session_players[i][0] == player_name)
		{
			session_players[i][8] = std::to_string(std::stoi(session_players[i][8]) + 1);
		}
	}
}
void addPlayerDisconnection(std::string player_name)
{
	// Changes the correct players connection info
	for (int i = 0; i < session_players.size(); i++)
	{
		if (session_players[i][0] == player_name)
		{
			session_players[i][9] = std::to_string(std::stoi(session_players[i][9]) + 1);
		}
	}
}
void addPlayerMessageSent(std::string player_name)
{
	// Changes the correct players connection info
	for (int i = 0; i < session_players.size(); i++)
	{
		if (session_players[i][0] == player_name)
		{
			session_players[i][10] = std::to_string(std::stoi(session_players[i][10]) + 1);
		}
	}

}

int findStrIndex(const std::string& str, const std::string& sub_str)
{
	std::string::size_type index = str.find(sub_str, 0);
	if (index != std::string::npos)
	{
		return index;
	}
	else
	{
		return -1;
	}
}
