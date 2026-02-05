# Ramble's CloD Server/Session Activity Statistics

Statistics for "IL-2 Sturmovik: Cliffs of Dover BLITZ".

Be sure to check out ["Ramble's SAS GUI" for Windows](https://github.com/DoctorRamble/clod-sas-gui)!

## Contents
* [About This Programme](#about-this-programme)
* [Installation](#installation)
	--> [C++ Compiler for Linux](#linux)
	--> [C++ Compiler for Windows](#windows)
* [Programme Launch Script File](#creating-a-script-file-to-easily-run-the-programme)
	--> [.BAT Script for Windows](#windows-bat-file)
	--> [.SH Script for Linux](#linux-sh-file)
* [Explaining the Arguments](#explaining-the-arguments)
* [Notes](#notes)
* [Contact Information](#contact-info)
* [About the Code](#about-the-code)
* [Known Limitations and Bugs](#known-limitationsbugs)
* [How the programme works](#how-this-programme-works)
* [Ackknowledgements](#acknowledgements)

## About This Programme
Running CloD on Linux presents the problem that some of the text fails to render.
This includes the post-mission summary screen.
This project was designed as an alternative to the CloD post-mission summary screen.
This programme can summarise the basic information of the "log.txt" file that CloD generates.
Some information includes vehicle destruction information, bail out information and landing information.

[ATAG Forums Post](https://theairtacticalassaultgroup.com/forum/showthread.php?t=39090&p=402785#post402785)

## Installation
1. Clone/download the repository

2. Navigate to the directory you downloaded the project to.

3. Compile main.cpp: `g++ --std=c++20 main.cpp -o sas`

4. After compiling you should see an executable file.

5. Go to [programme launch script file](#creating-a-script-file-to-easily-run-the-programme) to see how to run the programme.

## C++ Compiler Installation
If you don't already have a C++ compiler installed on your system, follow these instructions:
### Linux
Installation on different distros may vary. These commands should work:
* Debian-based Distros: `sudo apt install g++`
* Fedora-based Distros: `sudo dnf install gcc-c++`
* Arch-based Distros: `sudo pacman -S gcc`
### Windows
This [article](https://code.visualstudio.com/docs/cpp/config-mingw) by Microsoft should cover you.
Watch the video or follow the instructions under "Installing the MinGW-w64 toolchain".
You do not need to install Visual Studio Code.


If you have installed everything correctly, typing `g++ --version` into the terminal should return a result.

## Creating a script file to easily run the programme
You will not be able to open the programme by clicking on it. You will need to go into the terminal and run it from there using command-line arguments.
Creating the following .bat/.sh script file will allow you to run the programme by clicking on it instead.

### Windows: .bat File
1. In the folder that contains the executable file, create a new file called "run_sas.bat". It is important that you create this file INSIDE of the SAME folder that the executable is in!

2. Open "run_sas.bat" in notepad and enter in the following lines:
```
@echo off
start cmd /k "sas "your_steam_username" "C:\Users\Ramble\Documents\1C SoftClub\il-2 sturmovik cliffs of dover\log.txt" "C:\Users\Ramble\Desktop\SAS Output\\" "null" "1" "0" "0" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "0" "0" "0""
#To End a string with a \, put two like so: \\
```
You should replace the above locations with the locations on your computer!
To see what the arguments do go here: [Explaining the Arguments](#explaining-the-arguments)

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
gnome-terminal -- bash -c "/home/ramble/Desktop/Dev/clod-sas-arg/clod-sas-main/sas Doctor_Ramble /home/ramble/Desktop/log.txt /home/ramble/Desktop/SASOutput/ null 1 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0; exec bash"
```
or the following for colour:
```
#! /bin/bash
gnome-terminal -- bash -c "/home/ramble/Desktop/Dev/clod-sas-arg/clod-sas-main/sas Doctor_Ramble /home/ramble/Desktop/log.txt /home/ramble/Desktop/SASOutput/ null 1 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0; exec bash"
```

Replace the directories with your own.
With this command you have to make sure NOT to have any spaces in it. Such as: "/home/ramble/Desktop/SAS Output/", instead, "/home/ramble/Desktop/SASOutput/"

For the log.txt you will have to create a symbolic link to get around the "no spaces" rule. Take note of the directory your log.txt sits in and create a symlink to a directory (that has no spaces in it). Example:
```
ln -s "/home/ramble/.steam/debian-installation/steamapps/compatdata/754530/pfx/drive_c/users/steamuser/Documents/1C SoftClub/il-2 sturmovik cliffs of dover/log.txt" log.txt
```
This will create a 'link' to log.txt in the directory you run that command.

To see what the arguments do go here: [Explaining the Arguments](#explaining-the-arguments)
or type './sas help'

* You can replace "gnome-terminal" with the terminal that is installed on your system.
This may not work on all terminals or desktop environments.
I have tested this on the GNOME terminal and it works fine,
but on Konsole it does not seem to work for me.
Installing the `gnome-terminal` package on Fedora and running it through that works,
but I have encountered some weird visual glitches, it might just be my system though.
* You should replace the above locations with the locations on your computer!

ALL DONE! You can now use the programme! Happy flying!

## Explaining the Arguments
Important arguments:
1. Your steam username :: E.g. "Linux_1234"
2. Path to your log file :: E.g. "C:\Users\Ramble\Documents\1C SoftClub\il-2 sturmovik cliffs of dover\log.txt"
3. Directory to save the outputted log files :: E.g. "C:\Users\Ramble\Desktop\SAS Output\\" :: Make sure to put two slashes on the end!
4. Name of the outputted txt :: Best kept at null, this means the programme will generate a unique name

You can find more information by running the command 'sas help'.

An example run-command would be:
```
sas "Doctor_Ramble" "C:\Users\Ramble\Documents\1C SoftClub\il-2 sturmovik cliffs of dover\log.txt" "C:\Users\Ramble\Desktop\SAS Output\\" "null" "1" "0" "0" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "1" "0" "0" "0"
```

## Notes
* Please note that this programme may not work on all terminals, operating systems, distros or desktop environments.

## Contact Info
If you need any support or have found bugs you can reach me on the Cliffs of Dover ATAG Forums [Profile](https://theairtacticalassaultgroup.com/forum/member.php?u=19096) [Thread](https://theairtacticalassaultgroup.com/forum/showthread.php?t=39090&p=402785#post402785) or the IL-2 Sturmovik Forums [Profile](https://forum.il2sturmovik.com/profile/433135-doctor_ramble/). I also have an email: "DRamble46@outlook.com", but please only use this if you have to (and no spam/junk please!).
I will do my best to get back to you as soon as possible.
If it is bug-related, please include the contents of your "log.txt" file (or at least the line that is broken) and your OS and Debug information (can be enabled by setting "Debug_Info" in the config to "true").

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

4. Version naming scheme.
"v<x>-<y>". Where <x> is the major version number (decimal) and <y> is a minor version number (hexadecimal... because it looks cool).
A MAJOR version would be a new feature of some sort being added. E.g. Pilot/crew-member deaths.
Or some other recognition for an existing feature. E.g. Under crash info recognition there needs to be "<player> took off badly and crashed.".
A MINOR version would be a small fix or tweak. Examples would be a non-player entity getting its own profile or jumbled up text (means that an existing feature needs to be tweaked).
An example of a version number would be "v0.2-1E5".
Very minor updates might not get their own version name if they are deemed too small. E.g. Changing the "last_updated" date in the case that I had forgotten.

Note:
v1.0 has not been released. The version number will reach v1.0 whenever I feel like the programme is at least 95% bug-free and has most of the known messages in the categories I am aiming to process.

## Known Limitations/Bugs
* If your username is, for example "Spitfire", then the programme may think any line with "Spitfire" in it is related to your username.
* If your username contains, "(AI)" then the programme will think that you are an AI and will not add your player profile.
* Some things work in Linux, but don't in Windows and vice versa. This is due to the way this programme was developed: some features were programmed in while I was using Linux, and some when I was using Windows. I cannot bug-test Windows and Linux equally, so if you find any bugs where information isn't displayed correctly, it may be due to this. Be sure to [contact me](#contact-info) about the error and include the broken line and your OS info.

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
*  Becomes: "At (time) (username) enlisted in the (side) forces."

### Connection Information
* (username) enters the battle.
* (username) is now connected.
* (username) joins the server.
*  Becomes: "At (time) (username) connected to the server."
### Disconnection Information
* Goodbye, (username)!
* (username) leaves the server.
*  Becomes: "At (time) (username) disconnected from the server."

### Landing Information
* (username) returned to base.
* (username) is safe on the ground.
* (username) crash landed.
* (username) landed.
*  Becomes: "At (time) (username) landed."
*  Or if a crash landing: "At (time) (username) crash landed."
### Crash Information
* (entity) collided with terrain.
* (entity) crashed.
*  Becomes: "At (time) (name) in a (aircraft) crashed."

### Bail-Out Information
* (entity) had to jump.
* (entity) hit the silk.
*  Becomes: "At (time) (name) of a (aircraft) bailed out."

### Successfully Parachuted to the Ground Information
* (username) parachuted to the ground.
*  Becomes: "At (time) (username) parachuted to the ground.

### Unsuccessfully Parachute to the Ground Information
* (username)'s parachute failed to deploy.
*  Becomes: "At (time) (username)'s parachute failed to deploy."

### Destruction Information
==> Note: "At (time) " is at the beginning of the following outputs.
* (entity) was shot down by (entity).
*  Becomes: "(entity) destroyed (entity)."
* (entity) was shot down by (entity), (entity), (etc).
*  Becomes: "(entity), (entity), (etc), destroyed (entity).
* (entity) shot down (entity).
*  Becomes: "(entity) destroyed (entity)."
* (entity), (entity), (etc) destroyed (entity).
*  Becomes: "(entity), (entity), (etc), destroyed (entity)."
* (entity) is no more.
*  Becomes: "(name) in a (aircraft) is no more."
* (entity) sank (entity).
*  Becomes: "(entity) sank (entity)."
* (entity), (entity), (etc) sank (entity).
*  Becomes: "(entity), (entity), (etc), sank (entity)."
* (entity) goes down thanks to (entity).
*  Becomes: "(entity) sank (entity)."
* (entity) goes down thanks to (entity), (entity), (etc).
*  Becomes: "(entity), (entity), (etc), sank (entity).
* (entity) was destroyed.
*  Becomes: "(entity) was destroyed."
* (entity) destroyed (entity).
*  Becomes: "(entity) destroyed (entity)."
* (entity), (entity), (etc) destroyed (entity).
*  Becomes: "(entity), (entity), (etc), destroyed (entity)."
* (entity) was destroyed by (entity).
*  Becomes: "(entity) destroyed (entity)."
* (entity) was destroyed by (entity), (entity), (etc).
*  Becomes: "(entity), (entity), (etc), destroyed (entity)."
* (entity) claimed (entity).
*  Becomes: "(entity) destroyed (entity)."
* (entity), (entity), (etc) claimed (entity).
*  Becomes: "(entity), (entity), (etc), destroyed (entity).
* (entity) was blown up by (entity) before he could take off.
*  Becomes: "(entity) blew up (entity)."
* (entity) was blown up by (entity), (entity), (etc) before he could take off.
*  Becomes: "(entity), (entity), (etc), blew up (entity)."
* (entity) claimed a friendly kill: (entity).
*  Becomes: (entity) destroyed a friendly (entity).
* (entity), (entity), (etc) claimed a friendly kill: (entity)
*  Becomes: "(entity), (entity), (etc), destroyed a friendly (entity)."

### Chat Information
* Chat: (username):    (message)
*  Becomes: "At (time) (username) sent: (message) in chat."

### Known Still Need:
* "(username) defected to the (side) team." <-- Still Around?

### To Do:
* Add Message: "Friendly fire! (entity) destroyed (entity)."
* Add command-line arguments.
* Add output text file.

[Contact me](#contact-info) if you have found more messages that need adding.
If you can replicate it and provide multiple examples (maybe screenshots?) and/or instructions, please do so.

## Acknowledgements
* The folks at Team Fusion for maintaining the greatest WWII flight simulator! Thanks guys! :-)
* Thanks to [ATAG_Snapper](https://theairtacticalassaultgroup.com/forum/member.php?u=287) for helping me create an account on the ATAG forums!
