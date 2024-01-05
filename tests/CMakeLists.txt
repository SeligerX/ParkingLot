project(testLibs)

link_directories(utils vehicles parking)

add_executable(testLibs testLibs.cpp)

target_link_libraries(testLibs PRIVATE utils vehicles parking)

add_test(NAME money::testDefaultConstruct
        COMMAND testLibs money::testDefaultConstruct)
        
add_test(NAME money::testConstruct
        COMMAND testLibs money::testConstruct)

add_test(NAME money::testConstructWithDouble
        COMMAND testLibs money::testConstructWithDouble)
        
add_test(NAME money::testConstructNegativePapers
        COMMAND testLibs money::testConstructNegativePapers)
        
add_test(NAME money::testConstructNegativeCoins
        COMMAND testLibs money::testConstructNegativeCoins)

add_test(NAME money::testPlusAssign
        COMMAND testLibs money::testPlusAssign)
        
add_test(NAME money::testPlus
        COMMAND testLibs money::testPlus)

add_test(NAME money::testMulScalarAssign
        COMMAND testLibs money::testMulScalarAssign)
        
add_test(NAME money::testMulScalar
        COMMAND testLibs money::testMulScalar)

add_test(NAME money::testArithmetics
        COMMAND testLibs money::testArithmetics)
   
add_test(NAME money::testGetPapers
        COMMAND testLibs money::testGetPapers)

add_test(NAME money::testGetCoins
        COMMAND testLibs money::testGetCoins)

add_test(NAME parking::testAddVehicle
        COMMAND testLibs parking::testAddVehicle)

add_test(NAME parking::testAddVehicleTwice
        COMMAND testLibs parking::testAddVehicleTwice)

add_test(NAME parking::testRemoveVehicleById
        COMMAND testLibs parking::testRemoveVehicleById)

add_test(NAME parking::testRemoveVehicleByIdTwice
        COMMAND testLibs parking::testRemoveVehicleByIdTwice)

add_test(NAME parking::testRemoveVehicleByPlate
        COMMAND testLibs parking::testRemoveVehicleByPlate)

add_test(NAME parking::testRemoveVehicleByPlatedTwice
        COMMAND testLibs parking::testRemoveVehicleByPlatedTwice)

add_test(NAME parking::testEmptyCapacity
        COMMAND testLibs parking::testEmptyCapacity)

add_test(NAME parking::testCapacityOverflow
        COMMAND testLibs parking::testCapacityOverflow)

add_test(NAME parking::testSize
        COMMAND testLibs parking::testSize)

add_test(NAME parking::testUniqueTicketIdForSameVehicle
        COMMAND testLibs parking::testUniqueTicketIdForSameVehicle)

add_test(NAME parking::testAddRemoveVehicleByPlate
        COMMAND testLibs parking::testAddRemoveVehicleByPlate)

add_test(NAME parking::testFastForwardTime
        COMMAND testLibs parking::testFastForwardTime)

add_test(NAME vehicle::car::testChargeById
        COMMAND testLibs vehicle::car::testChargeById)
        
add_test(NAME vehicle::car::testChargeByPlate
        COMMAND testLibs vehicle::car::testChargeByPlate)

add_test(NAME vehicle::car::testReleaseVehicleById
        COMMAND testLibs vehicle::car::testReleaseVehicleById)
        
add_test(NAME vehicle::car::testReleaseVehicleByPlate
        COMMAND testLibs vehicle::car::testReleaseVehicleByPlate)

add_test(NAME vehicle::motorcycle::testChargeById
        COMMAND testLibs vehicle::motorcycle::testChargeById)

add_test(NAME vehicle::motorcycle::testChargeByPlate
        COMMAND testLibs vehicle::motorcycle::testChargeByPlate)

add_test(NAME vehicle::motorcycle::testReleaseVehicleById
        COMMAND testLibs vehicle::motorcycle::testReleaseVehicleById)

add_test(NAME vehicle::motorcycle::testReleaseVehicleByPlate
        COMMAND testLibs vehicle::motorcycle::testReleaseVehicleByPlate)

add_test(NAME vehicle::bus::testChargeById
        COMMAND testLibs vehicle::bus::testChargeById)

add_test(NAME vehicle::bus::testChargeByPlate
        COMMAND testLibs vehicle::bus::testChargeByPlate)

add_test(NAME vehicle::bus::testReleaseVehicleById
        COMMAND testLibs vehicle::bus::testReleaseVehicleById)

add_test(NAME vehicle::bus::testReleaseVehicleByPlate
        COMMAND testLibs vehicle::bus::testReleaseVehicleByPlate)
        