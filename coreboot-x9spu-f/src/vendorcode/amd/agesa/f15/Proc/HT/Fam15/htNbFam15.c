/* $NoKeywords:$ */
/**
 * @file
 *
 * Initializers for Family 15h northbridge support.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  HyperTransport
 * @e \$Revision: 44846 $   @e \$Date: 2011-01-06 22:21:05 -0700 (Thu, 06 Jan 2011) $
 *
 */
/*
*****************************************************************************
*
* Copyright (C) 2012 Advanced Micro Devices, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Advanced Micro Devices, Inc. nor the names of
*       its contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* ***************************************************************************
*
*/

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */

#include "AGESA.h"
#include "amdlib.h"
#include "OptionsHt.h"
#include "Ids.h"
#include "Topology.h"
#include "htFeat.h"
#include "htNb.h"
#include "CommonReturns.h"
#include "htNbCoherent.h"
#include "htNbCoherentFam15.h"
#include "htNbNonCoherent.h"
#include "htNbNonCoherentFam15.h"
#include "htNbOptimization.h"
#include "htNbOptimizationFam15.h"
#include "htNbSystemFam15.h"
#include "htNbUtilities.h"
#include "htNbUtilitiesFam15.h"
#include "cpuFamRegisters.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)
#include "Filecode.h"

#define FILECODE PROC_HT_FAM15_HTNBFAM15_FILECODE

extern OPTION_HT_CONFIGURATION OptionHtConfiguration;

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/***************************************************************************
 ***               FAMILY/NORTHBRIDGE SPECIFIC FUNCTIONS                 ***
 ***************************************************************************/

/**
 * Map Northbridge links to package links for Family 15h, multi-module.
 *
 * Unfortunately, there is no way to do this except to type the BKDG text into this data structure.
 * Note that there is one entry per package external sublink and each connected internal link.
 */
CONST PACKAGE_HTLINK_MAP_ITEM ROMDATA HtFam15PackageLinkMap[] =
{
  {3, 0, 0},        ///< Module zero, link 3: package link 0
  {7, 0, 4},        ///< Module zero, link 7: package link 4
  {0, 1, 1},        ///< Module one,  link 0: package link 1
  {4, 1, 5},        ///< Module one,  link 4: package link 5
  {1, 0, 2},        ///< Module zero, link 1: package link 2
  {5, 0, 6},        ///< Module zero, link 5: package link 6
  {0, 0, 3},        ///< Module zero, link 0: package link 3
  {3, 1, 7},        ///< Module one,  link 3: package link 7
  {2, 0, HT_LIST_MATCH_INTERNAL_LINK_0},        ///< Internal Link
  {6, 0, HT_LIST_MATCH_INTERNAL_LINK_1},        ///< Internal Link
  {4, 0, HT_LIST_MATCH_INTERNAL_LINK_2},        ///< Internal Link
  {1, 1, HT_LIST_MATCH_INTERNAL_LINK_0},        ///< Internal Link
  {5, 1, HT_LIST_MATCH_INTERNAL_LINK_1},        ///< Internal Link
  {7, 1, HT_LIST_MATCH_INTERNAL_LINK_2},        ///< Internal Link
  {HT_LIST_TERMINAL, HT_LIST_TERMINAL, HT_LIST_TERMINAL}, ///< End
};

/**
 * A default Ignore Link list to power off the 3rd internal sublink.
 */
STATIC CONST IGNORE_LINK ROMDATA Fam15IgnoreLinkList[] = {
  {HT_LIST_MATCH_ANY, HT_LIST_MATCH_INTERNAL_LINK_2, POWERED_OFF},
  {HT_LIST_TERMINAL}
};

/**
 * Initial construction data for Family 15h North Bridge, default, full features.
 */
CONST NORTHBRIDGE ROMDATA HtFam15NbDefault =
{
  8,
  WriteRoutingTable,
  WriteNodeID,
  ReadDefaultLink,
  EnableRoutingTables,
  DisableRoutingTables,
  VerifyLinkIsCoherent,
  ReadToken,
  WriteToken,
  WriteFullRoutingTable,
  IsIllegalTypeMix,
  Fam15IsExceededCapable,
  Fam15StopLink,
  (PF_HANDLE_SPECIAL_LINK_CASE)CommonReturnFalse,
  HandleSpecialNodeCase,
  ReadSouthbridgeLink,
  VerifyLinkIsNonCoherent,
  Fam15SetConfigAddrMap,
  Fam15NorthBridgeFreqMask,
  GatherLinkFeatures,
  SetLinkRegang,
  SetLinkFrequency,
  SetLinkUnitIdClumping,
  Fam15WriteTrafficDistribution,
  Fam15WriteLinkPairDistribution,
  Fam15WriteVictimDistribution,
  Fam15BufferOptimizations,
  Fam15GetNumCoresOnNode,
  SetTotalNodesAndCores,
  GetNodeCount,
  LimitNodes,
  ReadTrueLinkFailStatus,
  Fam15GetNextLink,
  GetPackageLink,
  MakeLinkBase,
  Fam15GetModuleInfo,
  Fam15PostMailbox,
  Fam15RetrieveMailbox,
  Fam15StrappedGetSocket,
  Fam15GetEnabledComputeUnits,
  Fam15GetDualcoreComputeUnits,
  0x00000001,
  0x00000200,
  18,
  TRUE,
  TRUE,
  AMD_FAMILY_15,
  (PACKAGE_HTLINK_MAP) &HtFam15PackageLinkMap,
  0,
  (IGNORE_LINK *)&Fam15IgnoreLinkList,
  MakeKey,
  NULL
};

/**
 * Initial construction data for Family 15h North Bridge, for non-coherent only builds.
 */
CONST NORTHBRIDGE ROMDATA HtFam15NbNonCoherentOnly =
{
  8,
  (PF_WRITE_ROUTING_TABLE)CommonVoid,
  (PF_WRITE_NODEID)CommonVoid,
  (PF_READ_DEFAULT_LINK)CommonReturnZero8,
  (PF_ENABLE_ROUTING_TABLES)CommonVoid,
  (PF_DISABLE_ROUTING_TABLES)CommonVoid,
  (PF_VERIFY_LINK_IS_COHERENT)CommonReturnFalse,
  (PF_READ_TOKEN)CommonReturnZero8,
  (PF_WRITE_TOKEN)CommonVoid,
  (PF_WRITE_FULL_ROUTING_TABLE)CommonVoid,
  (PF_IS_ILLEGAL_TYPE_MIX)CommonReturnFalse,
  (PF_IS_EXCEEDED_CAPABLE)CommonReturnFalse,
  (PF_STOP_LINK)CommonVoid,
  (PF_HANDLE_SPECIAL_LINK_CASE)CommonReturnFalse,
  (PF_HANDLE_SPECIAL_NODE_CASE)CommonReturnFalse,
  ReadSouthbridgeLink,
  VerifyLinkIsNonCoherent,
  Fam15SetConfigAddrMap,
  Fam15NorthBridgeFreqMask,
  GatherLinkFeatures,
  SetLinkRegang,
  SetLinkFrequency,
  SetLinkUnitIdClumping,
  (PF_WRITE_TRAFFIC_DISTRIBUTION)CommonVoid,
  (PF_WRITE_LINK_PAIR_DISTRIBUTION)CommonVoid,
  (PF_WRITE_VICTIM_DISTRIBUTION)CommonVoid,
  Fam15BufferOptimizations,
  Fam15GetNumCoresOnNode,
  SetTotalNodesAndCores,
  GetNodeCount,
  LimitNodes,
  ReadTrueLinkFailStatus,
  Fam15GetNextLink,
  GetPackageLink,
  MakeLinkBase,
  Fam15GetModuleInfo,
  Fam15PostMailbox,
  Fam15RetrieveMailbox,
  Fam15GetSocket,
  Fam15GetEnabledComputeUnits,
  Fam15GetDualcoreComputeUnits,
  0x00000001,
  0x00000200,
  18,
  TRUE,
  TRUE,
  ((AMD_FAMILY_15) & ~(AMD_FAMILY_TN | AMD_FAMILY_KM)),
  (PACKAGE_HTLINK_MAP) &HtFam15PackageLinkMap,
  0,
  NULL,
  MakeKey,
  NULL
};
