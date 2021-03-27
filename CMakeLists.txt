cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 11)

project(cryptography)

set(SOURCE_EXE main.cpp crypto.cpp elliptic_curve.cpp ElGamal.cpp mapping.cpp rng.cpp test.cpp)
include_directories(BigInt)

add_executable(cryptography ${SOURCE_EXE})
add_subdirectory(BigInt)

target_link_libraries(cryptography bigint)

include_directories(SHA256)

add_executable(prime_rng.cpp rng.cpp crypto.cpp )
add_subdirectory(SHA256)

add_subdirectory(googleTestLib)

target_link_libraries(bigint SHA256) #nlohmann_json::nlohmann_json, blockchain)

#add_executable(blockchain blockchain.cpp blockchain_client.cpp merkle_tree.cpp prime_rng.cpp rng.cpp cryptography.cpp rsa.cpp blockchain_main.cpp)
#add_subdirectory(SHA256)

#target_link_libraries(blockchain bigint SHA256 nlohmann_json::nlohmann_json)

#add_executable(group_rsa group_rsa.cpp prime_rng.cpp cryptography.cpp rng.cpp rsa.cpp group_rsa_main.cpp)
#target_link_libraries(group_rsa bigint)

#add_executable(prime_rng.cpp crypto.cpp rng.cpp)
#target_link_libraries(group_rsa bigint)