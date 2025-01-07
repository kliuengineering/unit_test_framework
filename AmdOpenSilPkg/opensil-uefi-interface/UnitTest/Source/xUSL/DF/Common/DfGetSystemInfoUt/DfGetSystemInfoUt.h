/**
 * @file  DfGetSystemInfoUt.h
 * @brief Common declarations of DfGetSystemInfoUt functions
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#ifndef __DFGETSYSTEMINFOUT_H__
#define __DFGETSYSTEMINFOUT_H__

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <xSIM.h>
#include <UtBaseLib.h>
#include <UtLogLib.h>
#include <UtSilInitLib.h>

#include <Include/MsrReg.h>
#include <Include/Library/UtSilServicesMockLib.h>

#include <DF/Common/DfCmn2Rev.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>

HOST_DEBUG_SERVICE mHostDebugService = NULL;

// Global/static variables to simulate parameters
static uint32_t MockDie = 0;
static uint32_t MockCount = 0;

// stub decalaration for DfGetNumberOfProcessorPresent
uint32_t 
DfGetNumberOfProcessorsPresentUt (
  void
  );

// stub decalaration for DfGetNumberOfSystemDies
uint32_t 
DfGetNumberOfSystemDiesUt (
  void
);

// stub decalaration for DfGetNumberOfSystemRootBridges
uint32_t 
DfGetNumberOfSystemRootBridgesUt (
  void
);

// mock declaration for DfFindComponentLocationMap
COMPONENT_LOCATION *
DfFindComponentLocationMapUt (
  uint32_t *Count,
  uint32_t *PhysIos0FabricId
  );

// mock declaration for edge case - empty location
COMPONENT_LOCATION *
EmptyComponentLocationMapMock (
    uint32_t *Count,
    uint32_t *PhysIos0FabricId
  );

// mock declaration for edge case - valid data but no matched type
COMPONENT_LOCATION *
NoMatchingComponentLocationMapMock (
    uint32_t *Count,
    uint32_t *PhysIos0FabricId
  );

// Helper function to set simulated parameters for DfGetDeviceMapOnDieUt
void 
SetMockDeviceMapParameters (
  uint32_t Die, 
  uint32_t *Count
  );

// mock declaration for DfGetDeviceMapOnDieUt
const 
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
MockDfGetDeviceMapOnDieUt (
  void
  );

// misc; added here for reference; may not be used
uint64_t
xUslRdMsr (
  uint32_t MsrAddress
  )
{
  return 0;
}

void
xUslWrMsr (
  uint32_t MsrAddress,
  uint64_t MsrValue
  )
{
  return;
}

void
xUslMsrAnd (
  uint32_t Index,
  uint64_t AndData
  )
{
  return;
}

void xUslMsrOr (
  uint32_t Index,
  uint64_t OrData
  )
{
  return;
}

void
xUslMsrAndThenOr (
  uint32_t Index,
  uint64_t AndData,
  uint64_t OrData
  )
{
  return;
}

bool xUslIsComputeUnitPrimary (
  void
  )
{
  return TRUE;
}

#endif