/** @file
  DisableInterrupts function.

  Copyright (c) 2020 Baoqi Zhang (zhangbaoqi@loongson.cn).
  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.
  Copyright (c) 2006 - 2008, Intel Corporation<BR>

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

/**
  Disables CPU interrupts.

**/
VOID
EFIAPI
DisableInterrupts (
  VOID
  )
{
	UINT64 val;
	__asm__ __volatile__(
		"li      %0, 0x4\n\t"
		"csrxchg $r0, %0, 0x0\n\t"
		: "=r"(val)
		:
		:
		);
}
