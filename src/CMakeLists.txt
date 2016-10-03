cmake_minimum_required (VERSION 3.0)

project (PokerServer)

ADD_DEFINITIONS(-std=c++11)

set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)


SET (core_SRCS

    poker_utils/Logger.h
    poker_utils/Logger.cpp

    poker_utils/CommonFunctions.h    

    core/GameEngine.h
    core/GameEngine.cpp

    core/GamePlayer.h
    core/GamePlayer.cpp

    core/Deck.h
    core/Deck.cpp

    core/Hand.h
    core/Hand.cpp

    core/AbstractPlayer.h

    core/Card.h
    core/Card.cpp

    core/Money.h

    comms/engines/telnet/TelnetPlayer.h
    comms/engines/telnet/TelnetPlayer.cpp
    
    comms/engines/telnet/TelnetServer.h
    comms/engines/telnet/TelnetServer.cpp
    
    comms/interfaces/CallBotPlayer.h
    comms/interfaces/CallBotPlayer.cpp
    
    comms/interfaces/ConsolePlayer.h
    comms/interfaces/ConsolePlayer.cpp
    
    comms/interfaces/RandomBotPlayer.h
    comms/interfaces/RandomBotPlayer.cpp
    
    main.cpp
    
)

find_package(Boost 1.55.0 REQUIRED COMPONENTS system)

add_executable(poker_server ${core_SRCS})
include_directories(. ${Boost_INCLUDE_DIRS})
target_link_libraries(poker_server ${Boost_LIBRARIES})
