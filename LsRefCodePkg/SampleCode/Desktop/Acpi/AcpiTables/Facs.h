/** @file
  This file describes the contents of the ACPI Fixed ACPI Description Table (FADT).
  Some additional ACPI values are defined in Acpi10.h, Acpi20.h, and Acpi30.h
  All changes to the FADT contents should be done in this file.

  Copyright (c) 2013-2015 Intel Corporation.
  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.

  Any violations of copyright or other intellectual property rights of the Loongson
  Technology Corporation Limited will be held accountable in accordance with the law,
  if you (or any of your subsidiaries, corporate affiliates or agents) initiate directly
  or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
  in whole or in part from any software, technology, product or service of Loongson Technology Corporation Limited
  or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

**/

#ifndef _FACS_H_
#define _FACS_H_

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>

//
// FACS Definitions
//
#define EFI_ACPI_FIRMWARE_WAKING_VECTOR 0x00000000
#define EFI_ACPI_GLOBAL_LOCK 0x00000000

#define EFI_ACPI_FIRMWARE_CONTROL_STRUCTURE_FLAGS 0x00000000
#define EFI_ACPI_X_FIRMWARE_WAKING_VECTOR 0x0000000000000000

#endif
