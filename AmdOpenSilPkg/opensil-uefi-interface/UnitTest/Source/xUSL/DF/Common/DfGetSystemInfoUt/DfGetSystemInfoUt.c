/* Copyright (C) 2021 - 2024 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file DfGetSystemInfoUt.c
 * @brief Unit tests for DfGetSystemInfoUt
 *
 * Iterations: WIP
 *
 * 
 * 
 * 
 */

#include "DfGetSystemInfoUt.h"

// stub definition for DfGetNumberOfProcessorPresent
uint32_t DfGetNumberOfProcessorsPresentUt (
  void
  )
{
  return 1;
}

// stub definition for DfGetNumberOfSystemDies
uint32_t DfGetNumberOfSystemDiesUt (
  void
  )
{
  return 1;
}

// stub definition for DfGetNumberOfSystemRootBridges
uint32_t 
DfGetNumberOfSystemRootBridgesUt (
  void
  )
{
  return 1;
}

// mock implementation for DfFindComponentLocationMap
COMPONENT_LOCATION *
DfFindComponentLocationMapUt (
  uint32_t *Count,
  uint32_t *PhysIos0FabricId
  )
{
  static COMPONENT_LOCATION MockLocations[] = {
      { .Type = PrimaryFch, .Socket = 0, .Die = 0, .IomsFabricId = 42 },
      { .Type = PrimarySmu, .Socket = 1, .Die = 1, .IomsFabricId = 43 }
  };

  if (Count) {
      *Count = sizeof(MockLocations) / sizeof(MockLocations[0]);
  }

  return MockLocations;
}

// mock declaration for edge case - empty location
COMPONENT_LOCATION *
EmptyComponentLocationMapMock (
    uint32_t *Count,
    uint32_t *PhysIos0FabricId
  )
{
    if (Count) {
        *Count = 0; // Indicate no component locations
    }
    return NULL; // Return NULL to indicate no data
}

// mock declaration for edge case - valid data but no matched type
COMPONENT_LOCATION *
NoMatchingComponentLocationMapMock (
    uint32_t *Count,
    uint32_t *PhysIos0FabricId
  )
{
    static COMPONENT_LOCATION MockLocations[] = {
        { .Type = 50, .Socket = 0, .Die = 0, .IomsFabricId = 42 } // Type doesn't match PrimaryFch or PrimarySmu
    };

    if (Count) {
        *Count = sizeof(MockLocations) / sizeof(MockLocations[0]);
    }

    return MockLocations;
}

// Helper function to set simulated parameters for DfGetDeviceMapOnDieUt
void 
SetMockDeviceMapParameters (
  uint32_t Die, 
  uint32_t *Count
  )
{
    MockDie = Die;   // Simulate Die parameter
    if (Count) {
        *Count = MockCount; // Set the Count value
    }
}

// mock implementation for DfGetDeviceMapOnDieUt
const 
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
MockDfGetDeviceMapOnDieUt(
  void
  ) 
{
    static DEVICE_IDS MockDeviceIDs[] = {
        { .FabricID = 42, .InstanceID = 0 },
        { .FabricID = 43, .InstanceID = 1 }
    };

    static AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP MockDeviceMap = {
        .Type = Ios,
        .Count = sizeof(MockDeviceIDs) / sizeof(MockDeviceIDs[0]),
        .IDs = MockDeviceIDs
    };

    MockCount = MockDeviceMap.Count; // Simulate Count as a global/static value

    return &MockDeviceMap;
}

// instantiates the transfer table globally
static DF_COMMON_2_REV_XFER_BLOCK MockDfXfer = {
    .DfFabricRegisterAccRead = NULL,                                      // Not needed for this test
    .DfFabricRegisterAccWrite = NULL,                                     // Not needed for this test
    .DfGetNumberOfProcessorsPresent = DfGetNumberOfProcessorsPresentUt,   // Mock for number of processors
    .DfGetNumberOfSystemDies = DfGetNumberOfSystemDiesUt,                 // Mock for number of dies
    .DfGetNumberOfSystemRootBridges = DfGetNumberOfSystemRootBridgesUt,   // Mock for number of root bridges
    .DfGetNumberOfRootBridgesOnSocket = NULL,                             // Not needed for this test
    .DfGetNumberOfRootBridgesOnDie = NULL,                                // Not needed for this test
    .DfGetDieSystemOffset = NULL,                                         // Not needed for this test
    .DfGetDeviceMapOnDie = MockDfGetDeviceMapOnDieUt,                     // Mock function for device map on die
    .DfGetHostBridgeBusBase = NULL,                                       // Not needed for this test
    .DfGetHostBridgeBusLimit = NULL,                                      // Not needed for this test
    .DfGetPhysRootBridgeNumber = NULL,                                    // Not needed for this test
    .DfFindComponentLocationMap = DfFindComponentLocationMapUt,           // Mock for component location map
    .DfBaseFabricTopologyConstructor = NULL,                              // Not needed for this test
    .DfBuildDomainInfo = NULL                                             // Not needed for this test
};

// prerequisuite begins
AMD_UNIT_TEST_STATUS
EFIAPI
TestPrerequisite (
  IN AMD_UNIT_TEST_CONTEXT Context
  )
{
  AMD_UNIT_TEST_STATUS Status = AMD_UNIT_TEST_PASSED;
  AMD_UNIT_TEST_FRAMEWORK *Ut = (AMD_UNIT_TEST_FRAMEWORK*) UtGetActiveFrameworkHandle ();
  const char* TestName        = UtGetTestName (Ut);
  const char* IterationName   = UtGetTestIteration (Ut);
  Ut->Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__,
    "%s (Iteration: %s) Prerequisite started.", TestName, IterationName);
  Status = UtSilInit ();
  if (Status != AMD_UNIT_TEST_PASSED) {
    return AMD_UNIT_TEST_ABORTED;
  }
  Ut->Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__, "Allocating memory for openSIL.");
  return AMD_UNIT_TEST_PASSED; 
}

// body begins
void
EFIAPI
TestBody (
  IN AMD_UNIT_TEST_CONTEXT Context
  )
{
  AMD_UNIT_TEST_FRAMEWORK *Ut = (AMD_UNIT_TEST_FRAMEWORK*) UtGetActiveFrameworkHandle ();
  const char* TestName        = UtGetTestName (Ut);
  const char* IterationName   = UtGetTestIteration (Ut);
  Ut->Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__, "%s (Iteration: %s) Test started.", TestName, IterationName);

  if (strcmp(IterationName, "EmptyComponentLocation") == 0) {
      // Arrange
      ROOT_BRIDGE_LOCATION MockLocation = {0};
      MockSilGetCommon2RevXferTableOnce(&MockDfXfer, SilPass);

      // Assign the empty component location mock
      MockDfXfer.DfFindComponentLocationMap = EmptyComponentLocationMapMock;

      // Act
      bool result = DfGetSystemComponentRootBridgeLocation(PrimaryFch, &MockLocation);

      // Assert
      assert_false(result); // Expect failure due to no component locations
      UtSetTestStatus (Ut, AMD_UNIT_TEST_PASSED);
  }

  else if (strcmp(IterationName, "InvalidComponentType") == 0) {
      // Arrange
      ROOT_BRIDGE_LOCATION MockLocation = {0};
      MockSilGetCommon2RevXferTableOnce(&MockDfXfer, SilPass);

      // Act
      bool result = DfGetSystemComponentRootBridgeLocation(99, &MockLocation); // Unsupported component type

      // Assert
      assert_false(result); // Expect failure due to invalid component type
      UtSetTestStatus (Ut, AMD_UNIT_TEST_PASSED);
  }

  else if (strcmp(IterationName, "NoMatchingComponent") == 0) {
      // Arrange
      ROOT_BRIDGE_LOCATION MockLocation = {0};
      MockSilGetCommon2RevXferTableOnce(&MockDfXfer, SilPass);

      // Assign the mock for no matching component
      MockDfXfer.DfFindComponentLocationMap = NoMatchingComponentLocationMapMock;

      // Act
      bool result = DfGetSystemComponentRootBridgeLocation(PrimaryFch, &MockLocation);

      // Assert
      assert_false(result); // Expect failure due to no matching component type
      UtSetTestStatus (Ut, AMD_UNIT_TEST_PASSED);
  }

  else if (strcmp(IterationName, "ValidSmuScenario") == 0) {
    // Arrange
    ROOT_BRIDGE_LOCATION MockLocation = {0};
    uint32_t Die = 1;
    uint32_t Count = 0;

    // Set parameters for the mock
    SetMockDeviceMapParameters(Die, &Count);

    // Assign the mock to the xfer table
    MockDfXfer.DfGetDeviceMapOnDie = MockDfGetDeviceMapOnDieUt;
    MockSilGetCommon2RevXferTableOnce(&MockDfXfer, SilPass);

    // Act
    bool result = DfGetSystemComponentRootBridgeLocation(PrimarySmu, &MockLocation);

    // Assert
    assert_true(result); // Expect the function to succeed
    assert_int_equal(MockLocation.Socket, 1);
    assert_int_equal(MockLocation.Die, 1);
    assert_int_equal(MockLocation.Index, 1);
    UtSetTestStatus (Ut, AMD_UNIT_TEST_PASSED);
  } 

  else if (strcmp(IterationName, "NoTransferTable") == 0) {
    // Arrange
    ROOT_BRIDGE_LOCATION MockLocation = {0};

    // Simulate SilNotFound return status
    MockSilGetCommon2RevXferTableOnce(NULL, SilNotFound);

    // Act
    bool result = DfGetSystemComponentRootBridgeLocation(PrimaryFch, &MockLocation);

    // Assert
    assert_false(result); // Expect failure due to no transfer table
    UtSetTestStatus (Ut, AMD_UNIT_TEST_PASSED);
  }

  else {
    Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__, "%s (Iteration: %s) Test is not implemented.", TestName, IterationName);
    UtSetTestStatus (Ut, AMD_UNIT_TEST_ABORTED);
  }

  Ut->Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__, "%s (Iteration: %s) Test ended.", TestName, IterationName);
  UtSetTestStatus (Ut, AMD_UNIT_TEST_PASSED);
}

// cleanup begins
AMD_UNIT_TEST_STATUS
EFIAPI
TestCleanUp (
  IN AMD_UNIT_TEST_CONTEXT Context
  )
{
  AMD_UNIT_TEST_FRAMEWORK *Ut = (AMD_UNIT_TEST_FRAMEWORK*) UtGetActiveFrameworkHandle ();
  Ut->Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__, "Freeing memory allocated to openSIL.");
  UtSilDeinit ();
  return AMD_UNIT_TEST_PASSED;  

}

/**
 * main
 * @brief      Starting point for Execution
 *
 * @details    This routine:
 *              - Handles the command line arguments.
 *              - Declares the unit test framework.
 *              - Run the tests.
 *              - Deallocate the Unit test framework.
 *
 * @param      argc                     Argument count
 * @param      *argv[]                  Argument vector
 *
 * @retval     AMD_UNIT_TEST_PASSED     Function succeeded
 * @retval     NON-ZERO                 Error occurs
 */
int
main (
  int   argc,
  char  *argv[]
  )
{
  AMD_UNIT_TEST_STATUS Status;              
  AMD_UNIT_TEST_FRAMEWORK Ut;               

  // Initializing the UnitTest framework    
  Status = UtInitFromArgs (
    &Ut,
    argc,
    argv
  );
  if (Status != AMD_UNIT_TEST_PASSED) {
    return Status;
  }

  // Logging the start of the test.         
  Ut.Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__,
    "Test %s started. TestStatus is %s.", UtGetTestName (&Ut), UtGetTestStatusString (&Ut));

  // Running test.                          
  Ut.Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__, "Running test.");
  UtRunTest (&Ut);

  // Freeing up all framework related allocated memories
  Ut.Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__, "Test %s ended.", UtGetTestName (&Ut));
  UtDeinit (&Ut);

  return AMD_UNIT_TEST_PASSED;
}
