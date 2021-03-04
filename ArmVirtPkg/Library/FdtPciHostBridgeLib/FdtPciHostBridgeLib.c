/** @file
  PCI Host Bridge Library instance for pci-ecam-generic DT nodes

  Copyright (c) 2016, Linaro Ltd. All rights reserved.<BR>

  This program and the accompanying materials are licensed and made available
  under the terms and conditions of the BSD License which accompanies this
  distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
  IMPLIED.

**/
#include <PiDxe.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/FdtClient.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>

#pragma pack(1)
typedef struct {
  ACPI_HID_DEVICE_PATH     AcpiDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_PCI_ROOT_BRIDGE_DEVICE_PATH;
#pragma pack ()

STATIC EFI_PCI_ROOT_BRIDGE_DEVICE_PATH mEfiPciRootBridgeDevicePath = {
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof(ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof(ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID(0x0A03),
    0
  },

  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED
CHAR16 *mPciHostBridgeLibAcpiAddressSpaceTypeStr[] = {
  L"Mem", L"I/O", L"Bus"
};

//
// We expect the "ranges" property of "pci-host-ecam-generic" to consist of
// records like this.
//
#pragma pack (1)
typedef struct {
  UINT32 Type;
  UINT64 ChildBase;
  UINT64 CpuBase;
  UINT64 Size;
} DTB_PCI_HOST_RANGE_RECORD;
#pragma pack ()

#define DTB_PCI_HOST_RANGE_RELOCATABLE  BIT31
#define DTB_PCI_HOST_RANGE_PREFETCHABLE BIT30
#define DTB_PCI_HOST_RANGE_ALIASED      BIT29
#define DTB_PCI_HOST_RANGE_MMIO32       BIT25
#define DTB_PCI_HOST_RANGE_MMIO64       (BIT25 | BIT24)
#define DTB_PCI_HOST_RANGE_IO           BIT24
#define DTB_PCI_HOST_RANGE_TYPEMASK     (BIT31 | BIT30 | BIT29 | BIT25 | BIT24)

STATIC
EFI_STATUS
ProcessPciHost (
  OUT  UINT64    *IoBase,
  OUT  UINT64    *IoSize,
  OUT  UINT64    *Mmio32Base,
  OUT  UINT64    *Mmio32Size,
  OUT  UINT64    *Mmio64Base,
  OUT  UINT64    *Mmio64Size,
  OUT  UINT32    *BusMin,
  OUT  UINT32    *BusMax
  )
{
  FDT_CLIENT_PROTOCOL         *FdtClient;
  INT32                       Node;
  UINT64                      ConfigBase, ConfigSize;
  CONST VOID                  *Prop;
  UINT32                      Len;
  UINT32                      RecordIdx;
  EFI_STATUS                  Status;
  UINT64                      IoTranslation;
  UINT64                      Mmio32Translation;
  UINT64                      Mmio64Translation;

  //
  // The following output arguments are initialized only in
  // order to suppress '-Werror=maybe-uninitialized' warnings
  // *incorrectly* emitted by some gcc versions.
  //
  *IoBase = 0;
  *Mmio32Base = 0;
  *Mmio64Base = MAX_UINT64;
  *BusMin = 0;
  *BusMax = 0;

  //
  // *IoSize, *Mmio##Size and IoTranslation are initialized to zero because the
  // logic below requires it. However, since they are also affected by the issue
  // reported above, they are initialized early.
  //
  *IoSize = 0;
  *Mmio32Size = 0;
  *Mmio64Size = 0;
  IoTranslation = 0;

  Status = gBS->LocateProtocol (&gFdtClientProtocolGuid, NULL,
                  (VOID **)&FdtClient);
  ASSERT_EFI_ERROR (Status);

  Status = FdtClient->FindCompatibleNode (FdtClient, "pci-host-ecam-generic",
                        &Node);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO,
      "%a: No 'pci-host-ecam-generic' compatible DT node found\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  DEBUG_CODE (
    INT32 Tmp;

    //
    // A DT can legally describe multiple PCI host bridges, but we are not
    // equipped to deal with that. So assert that there is only one.
    //
    Status = FdtClient->FindNextCompatibleNode (FdtClient,
                          "pci-host-ecam-generic", Node, &Tmp);
    ASSERT (Status == EFI_NOT_FOUND);
  );

  Status = FdtClient->GetNodeProperty (FdtClient, Node, "reg", &Prop, &Len);
  if (EFI_ERROR (Status) || Len != 2 * sizeof (UINT64)) {
    DEBUG ((EFI_D_ERROR, "%a: 'reg' property not found or invalid\n",
      __FUNCTION__));
    return EFI_PROTOCOL_ERROR;
  }

  //
  // Fetch the ECAM window.
  //
  ConfigBase = SwapBytes64 (((CONST UINT64 *)Prop)[0]);
  ConfigSize = SwapBytes64 (((CONST UINT64 *)Prop)[1]);

  //
  // Fetch the bus range (note: inclusive).
  //
  Status = FdtClient->GetNodeProperty (FdtClient, Node, "bus-range", &Prop,
                        &Len);
  if (EFI_ERROR (Status) || Len != 2 * sizeof (UINT32)) {
    DEBUG ((EFI_D_ERROR, "%a: 'bus-range' not found or invalid\n",
      __FUNCTION__));
    return EFI_PROTOCOL_ERROR;
  }
  *BusMin = SwapBytes32 (((CONST UINT32 *)Prop)[0]);
  *BusMax = SwapBytes32 (((CONST UINT32 *)Prop)[1]);

  //
  // Sanity check: the config space must accommodate all 4K register bytes of
  // all 8 functions of all 32 devices of all buses.
  //
  if (*BusMax < *BusMin || *BusMax - *BusMin == MAX_UINT32 ||
      DivU64x32 (ConfigSize, SIZE_4KB * 8 * 32) < *BusMax - *BusMin + 1) {
    DEBUG ((EFI_D_ERROR, "%a: invalid 'bus-range' and/or 'reg'\n",
      __FUNCTION__));
    return EFI_PROTOCOL_ERROR;
  }

  //
  // Iterate over "ranges".
  //
  Status = FdtClient->GetNodeProperty (FdtClient, Node, "ranges", &Prop, &Len);
  if (EFI_ERROR (Status) || Len == 0 ||
      Len % sizeof (DTB_PCI_HOST_RANGE_RECORD) != 0) {
    DEBUG ((EFI_D_ERROR, "%a: 'ranges' not found or invalid\n", __FUNCTION__));
    return EFI_PROTOCOL_ERROR;
  }

  for (RecordIdx = 0; RecordIdx < Len / sizeof (DTB_PCI_HOST_RANGE_RECORD);
       ++RecordIdx) {
    CONST DTB_PCI_HOST_RANGE_RECORD *Record;

    Record = (CONST DTB_PCI_HOST_RANGE_RECORD *)Prop + RecordIdx;
    switch (SwapBytes32 (Record->Type) & DTB_PCI_HOST_RANGE_TYPEMASK) {
    case DTB_PCI_HOST_RANGE_IO:
      *IoBase = SwapBytes64 (Record->ChildBase);
      *IoSize = SwapBytes64 (Record->Size);
      IoTranslation = SwapBytes64 (Record->CpuBase) - *IoBase;

      ASSERT (PcdGet64 (PcdPciIoTranslation) == IoTranslation);
      break;

    case DTB_PCI_HOST_RANGE_MMIO32:
      *Mmio32Base = SwapBytes64 (Record->ChildBase);
      *Mmio32Size = SwapBytes64 (Record->Size);
      Mmio32Translation = SwapBytes64 (Record->CpuBase) - *Mmio32Base;

      if (*Mmio32Base > MAX_UINT32 || *Mmio32Size > MAX_UINT32 ||
          *Mmio32Base + *Mmio32Size > SIZE_4GB) {
        DEBUG ((EFI_D_ERROR, "%a: MMIO32 space invalid\n", __FUNCTION__));
        return EFI_PROTOCOL_ERROR;
      }

      ASSERT (PcdGet64 (PcdPciMmio32Translation) == Mmio32Translation);

      if (Mmio32Translation != 0) {
        DEBUG ((EFI_D_ERROR, "%a: unsupported nonzero MMIO32 translation "
          "0x%Lx\n", __FUNCTION__, Mmio32Translation));
        return EFI_UNSUPPORTED;
      }

      break;

    case DTB_PCI_HOST_RANGE_MMIO64:
      *Mmio64Base = SwapBytes64 (Record->ChildBase);
      *Mmio64Size = SwapBytes64 (Record->Size);
      Mmio64Translation = SwapBytes64 (Record->CpuBase) - *Mmio64Base;

      ASSERT (PcdGet64 (PcdPciMmio64Translation) == Mmio64Translation);

      if (Mmio64Translation != 0) {
        DEBUG ((EFI_D_ERROR, "%a: unsupported nonzero MMIO64 translation "
          "0x%Lx\n", __FUNCTION__, Mmio64Translation));
        return EFI_UNSUPPORTED;
      }

      break;
    }
  }
  if (*IoSize == 0 || *Mmio32Size == 0) {
    DEBUG ((EFI_D_ERROR, "%a: %a space empty\n", __FUNCTION__,
      (*IoSize == 0) ? "IO" : "MMIO32"));
    return EFI_PROTOCOL_ERROR;
  }

  //
  // The dynamic PCD PcdPciExpressBaseAddress should have already been set,
  // and should match the value we found in the DT node.
  //
  ASSERT (PcdGet64 (PcdPciExpressBaseAddress) == ConfigBase);

  DEBUG ((EFI_D_INFO, "%a: Config[0x%Lx+0x%Lx) Bus[0x%x..0x%x] "
    "Io[0x%Lx+0x%Lx)@0x%Lx Mem32[0x%Lx+0x%Lx)@0x0 Mem64[0x%Lx+0x%Lx)@0x0\n",
    __FUNCTION__, ConfigBase, ConfigSize, *BusMin, *BusMax, *IoBase, *IoSize,
    IoTranslation, *Mmio32Base, *Mmio32Size, *Mmio64Base, *Mmio64Size));
  return EFI_SUCCESS;
}

STATIC PCI_ROOT_BRIDGE mRootBridge;

/**
  Return all the root bridge instances in an array.

  @param Count  Return the count of root bridge instances.

  @return All the root bridge instances in an array.
          The array should be passed into PciHostBridgeFreeRootBridges()
          when it's not used.
**/
PCI_ROOT_BRIDGE *
EFIAPI
PciHostBridgeGetRootBridges (
  UINTN *Count
  )
{
  UINT64              IoBase, IoSize;
  UINT64              Mmio32Base, Mmio32Size;
  UINT64              Mmio64Base, Mmio64Size;
  UINT32              BusMin, BusMax;
  EFI_STATUS          Status;

  if (PcdGet64 (PcdPciExpressBaseAddress) == 0) {
    DEBUG ((EFI_D_INFO, "%a: PCI host bridge not present\n", __FUNCTION__));

    *Count = 0;
    return NULL;
  }

  Status = ProcessPciHost (&IoBase, &IoSize, &Mmio32Base, &Mmio32Size,
             &Mmio64Base, &Mmio64Size, &BusMin, &BusMax);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: failed to discover PCI host bridge: %r\n",
      __FUNCTION__, Status));
    *Count = 0;
    return NULL;
  }

  *Count = 1;

  mRootBridge.Segment               = 0;
  mRootBridge.Supports              = EFI_PCI_ATTRIBUTE_ISA_IO_16 |
                                      EFI_PCI_ATTRIBUTE_ISA_MOTHERBOARD_IO |
                                      EFI_PCI_ATTRIBUTE_VGA_IO_16  |
                                      EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO_16;
  mRootBridge.Attributes            = mRootBridge.Supports;

  mRootBridge.DmaAbove4G            = TRUE;
  mRootBridge.NoExtendedConfigSpace = FALSE;
  mRootBridge.ResourceAssigned      = FALSE;

  mRootBridge.AllocationAttributes  = EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM;

  mRootBridge.Bus.Base              = BusMin;
  mRootBridge.Bus.Limit             = BusMax;
  mRootBridge.Io.Base               = IoBase;
  mRootBridge.Io.Limit              = IoBase + IoSize - 1;
  mRootBridge.Mem.Base              = Mmio32Base;
  mRootBridge.Mem.Limit             = Mmio32Base + Mmio32Size - 1;

  if (sizeof (UINTN) == sizeof (UINT64)) {
    mRootBridge.MemAbove4G.Base       = Mmio64Base;
    mRootBridge.MemAbove4G.Limit      = Mmio64Base + Mmio64Size - 1;
    if (Mmio64Size > 0) {
      mRootBridge.AllocationAttributes |= EFI_PCI_HOST_BRIDGE_MEM64_DECODE;
    }
  } else {
    //
    // UEFI mandates a 1:1 virtual-to-physical mapping, so on a 32-bit
    // architecture such as ARM, we will not be able to access 64-bit MMIO
    // BARs unless they are allocated below 4 GB. So ignore the range above
    // 4 GB in this case.
    //
    mRootBridge.MemAbove4G.Base       = MAX_UINT64;
    mRootBridge.MemAbove4G.Limit      = 0;
  }

  //
  // No separate ranges for prefetchable and non-prefetchable BARs
  //
  mRootBridge.PMem.Base             = MAX_UINT64;
  mRootBridge.PMem.Limit            = 0;
  mRootBridge.PMemAbove4G.Base      = MAX_UINT64;
  mRootBridge.PMemAbove4G.Limit     = 0;

  mRootBridge.DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath;

  return &mRootBridge;
}

/**
  Free the root bridge instances array returned from
  PciHostBridgeGetRootBridges().

  @param Bridges The root bridge instances array.
  @param Count   The count of the array.
**/
VOID
EFIAPI
PciHostBridgeFreeRootBridges (
  PCI_ROOT_BRIDGE *Bridges,
  UINTN           Count
  )
{
  ASSERT (Count == 1);
}

/**
  Inform the platform that the resource conflict happens.

  @param HostBridgeHandle Handle of the Host Bridge.
  @param Configuration    Pointer to PCI I/O and PCI memory resource
                          descriptors. The Configuration contains the resources
                          for all the root bridges. The resource for each root
                          bridge is terminated with END descriptor and an
                          additional END is appended indicating the end of the
                          entire resources. The resource descriptor field
                          values follow the description in
                          EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
                          .SubmitResources().
**/
VOID
EFIAPI
PciHostBridgeResourceConflict (
  EFI_HANDLE                        HostBridgeHandle,
  VOID                              *Configuration
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *Descriptor;
  UINTN                             RootBridgeIndex;
  DEBUG ((EFI_D_ERROR, "PciHostBridge: Resource conflict happens!\n"));

  RootBridgeIndex = 0;
  Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Configuration;
  while (Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    DEBUG ((EFI_D_ERROR, "RootBridge[%d]:\n", RootBridgeIndex++));
    for (; Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR; Descriptor++) {
      ASSERT (Descriptor->ResType <
              (sizeof (mPciHostBridgeLibAcpiAddressSpaceTypeStr) /
               sizeof (mPciHostBridgeLibAcpiAddressSpaceTypeStr[0])
               )
              );
      DEBUG ((EFI_D_ERROR, " %s: Length/Alignment = 0x%lx / 0x%lx\n",
              mPciHostBridgeLibAcpiAddressSpaceTypeStr[Descriptor->ResType],
              Descriptor->AddrLen, Descriptor->AddrRangeMax
              ));
      if (Descriptor->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        DEBUG ((EFI_D_ERROR, "     Granularity/SpecificFlag = %ld / %02x%s\n",
                Descriptor->AddrSpaceGranularity, Descriptor->SpecificFlag,
                ((Descriptor->SpecificFlag &
                  EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_CACHEABLE_PREFETCHABLE
                  ) != 0) ? L" (Prefetchable)" : L""
                ));
      }
    }
    //
    // Skip the END descriptor for root bridge
    //
    ASSERT (Descriptor->Desc == ACPI_END_TAG_DESCRIPTOR);
    Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)(
                   (EFI_ACPI_END_TAG_DESCRIPTOR *)Descriptor + 1
                   );
  }
}
