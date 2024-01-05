# Parking lot

You're tasked with developing a simulation for an automated parking lot. This lot can accommodate cars, motorcycles, and buses. The parking lot charges different rates based on vehicle type and the duration a vehicle remains parked.

(based on original task - see task.txt)

1. Hom to compile? 
You need C++23 compaler and CMake one of the latest versions. For unit testing this project uses CTest (which is included into CMake).

After successful build via CMake you should get 3 *.exe file in your "build" directory:
1) build/tests/testLibs.exe - unit test (to run them use ctest *.exe)
2) build/parkingDemo.exe - basic demo how to park/release vehicles
3) build/parkingMTDemo.exe - most interesting part - multithreaded hi-load simulation: 4 worker threads represent "parker" (someone who wants to park a vehicle(s) for some time) + 1 thread (timer) for time simulation for each 2 seconds it fast forward for 1 minute

2. Assumption: 
1) parking capacity does not change during its work
2) parking can work concurrently: can park/release vehicle simultaneously from several parking user 
3) vehicles are unique and each vehicle can be parked/released many times consecutively, every time generating new parking ticket on park and generating some money on release
4) there are some basic "defence" generating exceptions: park same vehicle twice (same time), release vehicle that is not in the parking (now), trying to park while parking is full

3. Hi-level design choices:
1) according to task was design vehicle classes structure: common part of the code in base class "Vehicle", charging system - base on concret class(Bus, Car, Motocycle). You cannot create instance of abstract "Vehicle".
2) was add utility class Money to represent money if requested to calculate charge or vehicle release. It is convinient to use it as Proxy and after getting it from Parking you can perform additional actions
3) class Parking have functionality to fast forward time. It is better to move it somewhere, but for this task it is ok as it need to be tested faster rather than waiting in real time :)
4) you can create vehicle with specific plate number, but for simplification there is getUid() just to generate some unique number + letters related to its type
5) once vehicle parked it can be released/charged base on generated unique id or its plate number. It is requirment as part of the task (it was specified later). So, it was decide to use one data structure - map (with more complex key) to reduce memory usage and potential problems with maintaining 2 containers or any synchronisation.