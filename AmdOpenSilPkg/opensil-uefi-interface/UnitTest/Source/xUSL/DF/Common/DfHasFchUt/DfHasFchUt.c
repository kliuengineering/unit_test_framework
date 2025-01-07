/* Copyright (C) 2021 - 2024 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file DfHasFchUt.c
 * @brief Unit tests for DfHasFchUt
 *
 * Iterations: WIP
 *
 * 
 * 
 * 
 */

#include "DfHasFchUt.h"

// mock implementation for DfFindDeviceTypeEntryInMap
const 
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
DfFindDeviceTypeEntryInMapUt(
    FABRIC_DEVICE_TYPE Type
  ) 
{
  check_expected (Type);
  return mock_ptr_type(const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *);
}

// mock definition for DfFindDeviceTypeEntryInMap
void 
MockDfFindDeviceTypeEntryInMapOnce(
    const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *DeviceMap,
    void  *ExpectedReturn
  ) 
{
  expect_value(DfFindDeviceTypeEntryInMapUt, Type, Ios);
  will_return(DfFindDeviceTypeEntryInMapUt, DeviceMap);
}

// mock implementation for DfGetDeviceMapOnDie
const 
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
DfGetDeviceMapOnDieUt (
  void
  )
{
  return mock_ptr_type (const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *);
}

// mock definition for DfGetDeviceMapOnDie
void 
MockDfGetDeviceMapOnDieOnce (
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *DeviceMap
  )
{
  will_return(DfGetDeviceMapOnDieUt, DeviceMap);
}

// mock implementation for DfFindComponentLocationMap
const 
COMPONENT_LOCATION *
DfFindComponentLocationMapUt (
  uint32_t *Count, 
  uint32_t *PhysIos0FabricId
  )
{
  *Count = mock_type (uint32_t);
  *PhysIos0FabricId = mock_type (uint32_t);
  return mock_ptr_type (const COMPONENT_LOCATION *);
}

// mock definition for DfFindComponentLocationMap
void
MockDfFindComponentLocationMapOnce (
  const COMPONENT_LOCATION *Location,
  uint32_t Count,
  uint32_t PhysIos0FabricId
  )
{
  will_return (DfFindComponentLocationMapUt, Location);
  will_return (DfFindComponentLocationMapUt, Count);
  will_return (DfFindComponentLocationMapUt, PhysIos0FabricId);
}

// instantiates the transfer table globally
static DF_COMMON_2_REV_XFER_BLOCK MockDfXfer = {
  .DfFabricRegisterAccRead = NULL,
  .DfFabricRegisterAccWrite = NULL,
  .DfGetNumberOfProcessorsPresent = NULL,
  .DfGetNumberOfSystemDies = NULL,
  .DfGetNumberOfSystemRootBridges = NULL,
  .DfGetNumberOfRootBridgesOnSocket = NULL,
  .DfGetNumberOfRootBridgesOnDie = NULL,
  .DfGetDieSystemOffset = NULL,
  .DfGetDeviceMapOnDie = DfGetDeviceMapOnDieUt,
  .DfGetHostBridgeBusBase = NULL,
  .DfGetHostBridgeBusLimit = NULL,
  .DfGetPhysRootBridgeNumber = NULL,
  .DfFindComponentLocationMap = DfFindComponentLocationMapUt,
  .DfBaseFabricTopologyConstructor = NULL,
  .DfBuildDomainInfo = NULL
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

  // variable declaration for global use to all test cases
  // FABRIC_DEVICE_TYPE IosEntry = Ios;
  uint32_t Socket = 0, Die = 0, Index = 0; 
  uint32_t Count = 1, Ios0FabricId = 0;

  if (strcmp(IterationName, "GetCommon2RevXferTablePass") == 0) {

    // Arrange
    static const DEVICE_IDS MockDeviceIDs[] = {
      {
        .FabricID = 1,
        .InstanceID = 0
      }
    };

    static const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP MockDeviceMap[] = {
      { .Type = Ios, .Count = 1, .IDs = MockDeviceIDs},
      { .Type = FabricDeviceTypeMax } // terminates the array
    };

    static const COMPONENT_LOCATION MockLocations[] = {
      { 
        .Socket = 0, 
        .Die = 0, 
        .Type = FchTypeMax - 1,
        .IomsFabricId = 1
      }
    };

    // Act
    MockSilGetCommon2RevXferTableOnce( (void *)&MockDfXfer, SilPass );
    Ut->Log(AMD_UNIT_TEST_LOG_DEBUG, __FUNCTION__, __LINE__, "SilGetCommon2RevXferTableOnce has done acting.");

    expect_value(DfFindDeviceTypeEntryInMapUt, Type, Ios);
    MockDfFindDeviceTypeEntryInMapOnce( MockDeviceMap, (void *)&MockDeviceMap );

    Ut->Log(AMD_UNIT_TEST_LOG_DEBUG, __FUNCTION__, __LINE__, "DfFindDeviceTypeEntryInMap has done acting.");

    MockDfGetDeviceMapOnDieOnce( MockDeviceMap );
    Ut->Log(AMD_UNIT_TEST_LOG_DEBUG, __FUNCTION__, __LINE__, "DfGetDeviceMapOnDieUtOnce has done acting.");

    MockDfFindComponentLocationMapOnce( MockLocations, Count, Ios0FabricId );
    Ut->Log(AMD_UNIT_TEST_LOG_DEBUG, __FUNCTION__, __LINE__, "DfFindComponentLocationMapOnce has done acting.");

    bool result = DfHasFch( Socket, Die, Index );
    Ut->Log(AMD_UNIT_TEST_LOG_DEBUG, __FUNCTION__, __LINE__, "DfHasFch has done acting.");

    // Assert
    if ( result )
      // declares passing 
      UtSetTestStatus(Ut, AMD_UNIT_TEST_PASSED);
    else
      // declares failing
      UtSetTestStatus (Ut, AMD_UNIT_TEST_FAILED);

  } /* else if(strcmp(IterationName, "iteration name goes here") == 0) {

    // Arrange

    // Act
    Ut->Log(AMD_UNIT_TEST_LOG_DEBUG, __FUNCTION__, __LINE__, "WIP");

    // Assert

      // declares passing 
      UtSetTestStatus(Ut, AMD_UNIT_TEST_PASSED);

      // declares failing
      UtSetTestStatus (Ut, AMD_UNIT_TEST_FAILED);

  } */ else {
    Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__, "Iteration: %s is not implemented.", IterationName);
    UtSetTestStatus (Ut, AMD_UNIT_TEST_FAILED);
  }

  Ut->Log(AMD_UNIT_TEST_LOG_DEBUG, __FUNCTION__, __LINE__, "%s (Iteration: %s) test ended.", TestName, IterationName);
  UtSetTestStatus(Ut, AMD_UNIT_TEST_PASSED);
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
