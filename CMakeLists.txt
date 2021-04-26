cmake_minimum_required(VERSION 3.14)

#set(CMAKE_CXX_STANDARD 11)
#
#project(cryptography)
#
#set(SOURCE_EXE main.cpp crypto.cpp elliptic_curve.cpp ElGamal.cpp mapping.cpp rng.cpp test.cpp)
#include_directories(BigInt)
#
#add_executable(cryptography ${SOURCE_EXE})
#add_subdirectory(BigInt)
#
#target_link_libraries(cryptography bigint)
#
#include_directories(SHA256)
#
#add_executable(prime_rng.cpp rng.cpp crypto.cpp )
#add_subdirectory(SHA256)
#
#
##add_subdirectory(nlohmann)
##add_executable(blockchain advanced/blockchain.cpp)
##target_link_libraries(blockchain nlohmann_json::nlohmann_json)
#
#
#add_executable(blockchain advanced/blockchain.cpp advanced/blockchain.h advanced/blockchainClient advanced/RSA.cpp advanced/blockchainMain.cpp)
#add_executable(merkleTree advanced/merkleTree.cpp advanced/merkleTree.h prime_rng.cpp rng.cpp crypto.cpp advanced/RSA.cpp)
#
#target_link_libraries(bigint SHA256)
#
#add_executable(RSAConnect advanced/RSAConnect.cpp advanced/RSA.cpp advanced/ConnectRSAMain.cpp crypto.cpp rng.cpp)
#target_link_libraries(RSAConnect bigint)


set(CMAKE_CXX_STANDARD 11)

project(cryptography)

set(SOURCE_EXE main.cpp crypto.cpp elliptic_curve.cpp ElGamal.cpp mapping.cpp rng.cpp)
include_directories(BigInt)

add_executable(cryptography ${SOURCE_EXE})
add_subdirectory(BigInt)

target_link_libraries(cryptography bigint)

include_directories(SHA256)

#find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(blockchain advanced/blockchain.cpp advanced/blockchainClient.cpp advanced/merkleTree.cpp prime_rng.cpp rng.cpp crypto.cpp advanced/RSA.cpp advanced/blockchainMain.cpp)
add_subdirectory(SHA256)

target_link_libraries(blockchain bigint SHA256)

add_executable(groupRsa advanced/RSAConnect.cpp prime_rng.cpp crypto.cpp rng.cpp advanced/RSA.cpp advanced/RSAConnect.cpp)
target_link_libraries(groupRsa bigint)