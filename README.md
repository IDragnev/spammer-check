# spammer-check

A CLI application which checks IPv4 addresses for being spammers using DNSBL.

## Build
 - Windows only
 - c++17 compliant compiler
 - CMake 3.10 or newer
 
 $ git clone --recursive https://github.com/IDragnev/spammer-check.git   
 $ cd spammer-check  
 $ mkdir src/build && cd src/build  
 $ cmake ../..  
 $ cmake --build .  
 
 ## Usage
  spammer_check.exe { \<hostname\> }<sup>+<sup/>
