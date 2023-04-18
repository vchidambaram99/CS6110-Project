add_executable(OverlayElectionSim OverlayElectionSimMain.cpp BordaNode.cpp MaliciousNode.cpp Network.cpp Node.cpp ThreeMajorityNode.cpp VoteCounter.cpp)
target_include_directories(OverlayElectionSim PUBLIC ${INCLUDE_DIR})
target_link_libraries(OverlayElectionSim Common)
