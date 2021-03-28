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
