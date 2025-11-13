# Ramble's CloD Server/Session Activity Statistics

Statistics for "IL-2 Sturmovik: Cliffs of Dover BLITZ".

## Contents
* [About This Programme](#about-this-programme)
* [Installation](#installation)
	--> [C++ Compiler for Linux](#linux)
	--> [C++ Compiler for Windows](#windows)
* [Programme Launch Script File](#creating-a-script-file-to-easily-run-the-programme)
	--> [.BAT Script for Windows](#windows-bat-file)
	--> [.SH Script for Linux](#linux-sh-file)
* [Notes](#notes)
* [Contact Information](#contact-info)
* [About the Code](#about-the-code)
* [Known Limitations and Bugs](#known-limitationsbugs)
* [About "config.txt"](#configtxt)
* [How the programme works](#how-this-programme-works)
* [Ackknowledgements](#acknowledgements)

## About This Programme
Running CloD on Linux presents the problem that some of the text fails to render.
This includes the post-mission summary screen.
This project was designed as an alternative to the CloD post-mission summary screen.
This programme can summarise the basic information of the "log.txt" file that CloD generates.
Some information includes vehicle destruction information, bail out information and landing information.

I have successfully run the programme on the following Systems:
* Windows 10 Professional
* Linux Mint 22.2 Cinnamon (Ubuntu/Debian based)
* Commodore OS Vision 3 (MX Linux/Debian based)
* CachyOS Cinnamon (Arch based)
* Fedora 42 KDE Plasma
* Debian 13 Cinnamon

## Installation
1. Clone/download the repository

2. Navigate to the directory you downloaded the project to.
You should see this directory tree:
```
|__ Main Project Directory/
|  |_ main.cpp
|  |_ config/
|  | |_ config.txt
```

3. Navigate to "config/" and edit the "config.txt".
For now, you can leave most of the variables as their defaults,
but change the Log Location value to the location of your log file,
e.g.: `C:\Users\YourUser\Documents\1C SoftClub\il-2 sturmovik cliffs of dover\log.txt`
Also change the Steam Username to your Steam username. You can optionally leave this as "NULL".

4. Navigate back to the main project directory of the programme (where the .cpp file is).
Compile `main.cpp": "g++ --std=c++17 main.cpp -o sas`

5. After compiling you should see an executable file.
Open your terminal and navigate to the root directory of the programme if you are not there already.
Enter the following:
Linux: `./sas`
Windows: `sas.exe`

This should successfully run the programme, allowing you to summarise your previous CloD session.

## C++ Compiler Installation
If you don't already have a C++ compiler installed on your system, follow these instructions:
### Linux
Installation on different distros may vary. These commands should work:
* Debian-based Distros: `sudo apt install g++`
* Fedora-based Distros: `sudo dnf install gcc-c++`
* Arch-based Distros: `sudo pacman -S gcc`
### Windows
This [article](code.visualstudio.com/docs/cpp/config-mingw) by Microsoft should cover you.
Watch the video or follow the instructions under "Installing the MinGW-w64 toolchain".
You do not need to install Visual Studio Code.


If you have installed everything correctly, typing `g++ --version` into the terminal should return a result.

## Creating a script file to easily run the programme
You will not be able to open the programme by clicking on it. You will need to go into the terminal and run it from there.
Creating the following .bat/.sh script file will allow you to run the programme by clicking on it instead.

### Windows: .bat File
1. In the folder that contains the executable file, create a new file called "run_sas.bat". It is important that you create this file INSIDE of the SAME folder that the executable is in!

2. Open "run_sas.bat" in notepad and enter in the following lines:
```
	@echo off
	start cmd /k "C:\Users\Ramble\Desktop\SAS\sas.exe"
```
You should replace the above location with the location of the programme on your computer!

3. Going back to File Explorer, clicking the "run_sas.bat" file should open the terminal window and run the programme.

4. Right-click the "run_sas.bat" file and press "Create shortcut".
It will create a shortcut file, clicking on this should have the same effect as running the .bat file.

5. Move the shortcut to the folder of your choice, the Desktop would be a good idea. Clicking this should still open the programme.
If you rename it, make sure ".lnk" is still the extension at the end, this is the file-type for shortcuts.

### Linux: .sh File
1. (Any location) Create a new document called "run_sas.sh".

2. Open the terminal and navigate to the directory that the script file is located in.

3. Run the following command: `chmod +x run_sas.sh`

4. Open the script file in the text editor of your choice and enter in the following:
```
	#! /bin/bash
	gnome-terminal -- bash -c "/home/ramble/Documents/SAS/sas; exec bash"
```
* You can replace "gnome-terminal" with the terminal that is installed on your system.
This may not work on all terminals or desktop environments.
I have tested this on the GNOME terminal and it works fine,
but on Konsole it does not seem to work for me.
Installing the `gnome-terminal` package on Fedora and running it through that works,
but I have encountered some weird visual glitches, it might just be my system though.
* You should replace "/home/ramble/Documents/SAS/sas" with the location of the programme on your system.
Make sure the executable file is on the end. In this case it is "sas".

ALL DONE! You can now use the programme! Happy flying!

## Notes
* Please note that this programme may not work on all terminals, operating systems, distros or desktop environments.

## Contact Info
If you need any support or have found bugs you can reach me on [Github](https://github.com/DoctorRamble/clod-sas), the Cliffs of Dover ATAG forums (To Be Posted) or the [IL-2 Sturmovik Forums](https://forum.il2sturmovik.com/profile/433135-doctor_ramble/). I will do my best to get back to you as soon as possible.
If it is bug-related, please include the contents of your "log.txt" file (or at least the line that is broken).

### About the code
1. The code for this programme is contained in a single file.
The reason for keeping it in a single file is that this programme has to be as easy to compile/install as possible.
Many people seem to be afraid of using the terminal, so having a shorter command to compile the programme is beneficial.
"g++ --std=c++17 main.cpp -o sas" versus "g++ --std=c++17 main.cpp destruction.hpp bail.hpp landing.hpp etc -o sas"
For people who do not like to use the terminal, the longer command (despite just adding more files) may seem scarier.
I do not want this to be the user's experience. So even though it is not proper, this is how it will be.

2. I am only a hobbyist programmer, so the code contained in the programme will not be up to professional standards and there are probably some much more efficient ways to do what I have done.
I might come back to this project in a couple of years time when I am better and fix it up.

3. This programme was not created with the assistance of AI coding tools.

4. There are A LOT of comments in this project. And many people may feel like I am over-commenting.
I have purposely "over-commented" the code in case anyone who is not a programmer wants to read through it.
(To check for malware, are curious, etc.,.)
Reading the programme down you can see that it can sort of be read like a scroll.

5. Version naming scheme.
"v<x>-<y>". Where <x> is the major version number (decimal) and <y> is a minor version number (hexadecimal... because it looks cool).
A MAJOR version would be a new feature of some sort being added. E.g. Pilot/crew-member deaths.
Or some other recognition for an existing feature. E.g. Under crash info recognition there needs to be "<player> took off badly and crashed.".
A MINOR version would be a small fix or tweak. Examples would be a non-player entity getting its own profile or jumbled up text (means that an existing feature needs to be tweaked).
An example of a version number would be "v0.2-1E5".

Note:
v1.0 has not been released. The version number will reach v1.0 whenever I feel like the programme is at least 95% bug-free and has most of the known messages in the categories I am aiming to process.

## Known Limitations/Bugs
* If your username is, for example "Spitfire", then the programme may think any line with "Spitfire" in it is related to your username.
* If your username contains, "(AI)" then the programme will think that you are an AI and will not add your player profile.
* Some things work in Linux, but don't in Windows and vice versa. This is due to the way this programme was developed: some features were programmed in while I was using Linux, and some when I was using Windows. I cannot bug-test Windows and Linux equally, so if you find any bugs where information isn't displayed correctly, it may be due to this. Be sure to [contact me](#contact-info) about the error and include the broken line and your OS info.

## "config.txt"
This programme loads information from "./config/config.txt".
It is a very primative config file system, but it does the job.
I am not satisfied with this so I might eventually implement a better config loader system in the future.

Otherwise this is how to use it:
The config.txt file consists of "variable_name=value"
The "variable_name" part is just there to make it easier to modify. The programme
can function with "=value" alone.

Boolean values (will be noted below) accept a "true" or "false" value.

### Lines:
1. The Log Location :: String.
Enter in the location of CloD's "log.txt" file.
For Windows, it should be "C:\Users\username\Documents\1C SoftClub\il-2 sturmovik cliffs of dover\"
Append "log.txt" on the end. Example: "C:\Users\username\Documents\1C SoftClub\il-2 sturmovik cliffs of dover\log.txt".
2. Your Steam Username :: String.
Enter in your Steam username. Optional.
This just allows the programme to see if a line is relevant to you.
3. Debug Info :: Boolean value.
If true some additional programme debug information will be shown.
4. Show All Information :: Boolean value.
If true the programme will not process the log file and will just print it out as is.
5. Show Player Connection Info :: Boolean value.
If true information about player connections and disconnections will be shown.
6. Other Player Connection Info :: Boolean value.
If false, only player connection information with your username in it will be shown.
7. Show Player Side Switch Info :: Boolean value.
If true, then information related to players joining/changing teams will be shown.
8. Other Player Side Switch Info :: Boolean value.
If false, only player side switch information related to you will be shown.
9. New Mission Loading Info :: Boolean value.
If true, information about new missions being loaded will be shown.
10. New Mission Info :: Boolean value.
If true, information about new missions that have been loaded will be shown.
11. Show Battle Start Info :: Boolean value.
If true, shows information about new battles that have been started.
12. Show Battle End Info :: Boolean value.
If true, shows information about battles ending.
13. Show Destruction Info :: Boolean value.
If true, shows information about vehicles being destroyed (shot down, sunk, etc.,)
14. Show Unrelated Destruction Info :: Boolean value.
If true, shows information about vehicles being destroyed even if it does not contain the player's username.
15. Show Landing Info :: Boolean value.
If true, shows information related to aircraft landing or crash landing.
16. Show Unrelated Landing Info :: Boolean value.
If true, shows information related to aircraft landing or crash landing even if it does not contain the player's username.
17. Show Bail Info :: Boolean value.
If true, shows information related to entities bailing out of an aircraft.
16. Show Unrealted Bail Info :: Boolean value.
If true, shows information related to entities bailing out of an aircraft even if it does not contain the player's username.
18. Show Crash Info :: Boolean value.
If true, shows information related to aircraft crashes.
Does not include crash landing, that is classified as a "good" landing.
19. Show Unrelated Crash Info :: Boolean value.
If ture, shows information related to aircraft crashes even if it does not contain the player's username.
20. Show Other Player Profiles :: Boolean value.
If true, other player's statistics will be shown. Useful for multiplayer rounds.
If you are not the server admin, you may have joined after the round was started.
In this case some information may be missing from before you joined.
21. Show Chat :: Boolean value.
If true, chat will be shown.
22. Colour Enabled :: Boolean value.
Some terminals support ANSI escape codes. This allows the results to be printed in colour.
This allows for a better experience as you can differentiate the different categories from each other.
Not all terminals support this feature though, including the Windows 10 CMD. In that case you will have to disable this feature.
23. Colour FG Enabled :: Boolean value
If Colour FG (Foreground) is true, the text will show the colour with a black background.
If false, the text will be black with a coloured background.

## How this programme works
This programme does not connect to CloD in any way.
It instead reads the "log.txt" file that CloD makes and uses that information to make a summary of the previous session.
The categories this programme aims to process:
* Player connection/disconnection information
* Player side-switching information
* Mission loading information
* New mission information
* Battle start/end information
* Destruction information
* Landing information
* Bail-out information
* Crash information
* Chat

### Side Switch/Enlist Information
* (username) enlists in the (side) air force.
* (username) will fly for the (side) forces.
* (username) joins the (side) army.
* (username) has changed to the (side) side.
* (username) is now with the (side) army.
* (username) has switched to (side).
--> Becomes: "At (time) (username) enlisted in the (side) forces."

### Connection Information
* (username) enters the battle.
* (username) is now connected.
* (username) joins the server.
--> Becomes: "At (time) (username) connected to the server."
### Disconnection Information
* Goodbye, (username)!
* (username) leaves the server.
--> Becomes: "At (time) (username) disconnected from the server."

### Landing Information
* (username) returned to base.
* (username) is safe on the ground.
* (username) crash landed.
* (username) landed.
--> Becomes: "At (time) (username) landed."
Or if a crash landing: "At (time) (username) crash landed."
### Crash Information
* (entity) collided with terrain.
* (entity) crashed.
--> Becomes: "At (time) (name) in a (aircraft) crashed."

### Bail-Out Information
* (entity) had to jump.
* (entity) hit the silk.
--> Becomes: "At (time) (name) of a (aircraft) bailed out."

### Successfully Parachuted to the Ground Information
* (username) parachuted to the ground.
--> Becomes: "At (time) (username) parachuted to the ground.

### Unsuccessfully Parachute to the Ground Information
* (username)'s parachute failed to deploy.

### Destruction Information
==> Note: "At (time) " is at the beginning of the following outputs.
* (entity) was shot down by (entity).
--> Becomes: "(entity) destroyed (entity)."
* (entity) was shot down by (entity), (entity), (etc).
--> Becomes: "(entity), (entity), (etc), destroyed (entity).
* (entity) shot down (entity).
--> Becomes: "(entity) destroyed (entity)."
* (entity), (entity), (etc) destroyed (entity).
--> Becomes: "(entity), (entity), (etc), destroyed (entity)."
* (entity) is no more.
--> Becomes: "(name) in a (aircraft) is no more."
* (entity) sank (entity).
--> Becomes: "(entity) sank (entity)."
* (entity), (entity), (etc) sank (entity).
--> Becomes: "(entity), (entity), (etc), sank (entity)."
* (entity) goes down thanks to (entity).
--> Becomes: "(entity) sank (entity)."
* (entity) goes down thanks to (entity), (entity), (etc).
--> Becomes: "(entity), (entity), (etc), sank (entity).
* (entity) was destroyed.
--> Becomes: "(entity) was destroyed."
* (entity) destroyed (entity).
--> Becomes: "(entity) destroyed (entity)."
* (entity), (entity), (etc) destroyed (entity).
--> Becomes: "(entity), (entity), (etc), destroyed (entity)."
* (entity) was destroyed by (entity).
--> Becomes: "(entity) destroyed (entity)."
* (entity) was destroyed by (entity), (entity), (etc).
--> Becomes: "(entity), (entity), (etc), destroyed (entity)."
* (entity) claimed (entity).
--> Becomes: "(entity) destroyed (entity)."
* (entity), (entity), (etc) claimed (entity).
--> Becomes: "(entity), (entity), (etc), destroyed (entity).
* (entity) was blown up by (entity) before he could take off.
--> Becomes: "(entity) blew up (entity)."
* (entity) was blown up by (entity), (entity), (etc) before he could take off.
--> Becomes: "(entity), (entity), (etc), blew up (entity)."
* (entity) claimed a friendly kill: (entity).
--> Becomes: (entity) destroyed a friendly (entity).
* (entity), (entity), (etc) claimed a friendly kill: (entity)
--> Becomes: "(entity), (entity), (etc), destroyed a friendly (entity)."

### Chat Information
* Chat: (username):    (message)
--> Becomes: "At (time) (username) sent: (message) in chat."

### Known Still Need:
* "(username) defected to the (side) team." <-- Still Around?

[Contact me](#contact-info) if you have found more messages that need adding.
If you can replicate it and provide multiple examples (maybe screenshots?) and/or instructions, please do so.

## Acknowledgements
* The folks at Team Fusion for maintaining the greatest WWII flight simulator! Thanks guys! :-)
