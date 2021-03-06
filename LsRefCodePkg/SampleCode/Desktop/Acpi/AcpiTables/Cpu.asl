/** @file
  Sample ACPI Platform Driver

  Copyright (c) 2008 - 2011, Intel Corporation. All rights reserved.<BR>
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

Scope (\_SB)
{
  Device (C000)
  {
    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
    Name (_UID, 1)  // _UID: Unique ID
    Name (_PXM, Zero)  // _PXM: Device Proximity
    Name (_STA, 0x0F)  // _STA: Status
    Method (_PPC, 0, NotSerialized)  // _PPC: Performance Present Capabilites
    {
      return (0);
    }

    Method (_PCT, 0, NotSerialized)  // _PCT: Performance Control
    {
      Return (Package (0x02)
      {
        ResourceTemplate ()
        {
          Register (FFixedHW,
            0x00,               // Bit Width
            0x00,               // Bit Offset
            0x0000000000000000, // Address
          ,)},

        ResourceTemplate ()
        {
          Register (FFixedHW,
            0x00,               // Bit Width
            0x00,               // Bit Offset
            0x0000000000000000, // Address
        ,)}
      })
    }

    Method (_PSS, 0, NotSerialized)  // _PSS: Performance Supported States
    {
      Return (LPSS)
    }

    Name (LPSS, Package (0x10)
    {
      Package (0x06)
      {
        0x000007D0,
        0x00003A98,
        20000,
        20000,
        0x00000103,
        0x00000003
      },

      Package (0x06)
      {
        0x00000708,
        0x00003A98,
        20000,
        20000,
        0x00000003,
        0x00000003
      },

      Package (0x06)
      {
        0x000006D6,
        0x00003A98,
        20000,
        20000,
        0x00000104,
        0x00000004
      },
      Package (0x06)
      {
        0x00000627,
        0x00003A98,
        20000,
        20000,
        0x00000004,
        0x00000004
      },
      Package (0x06)
      {
        0x000005DC,
        0x00003A98,
        20000,
        20000,
        0x00000105,
        0x00000005
      },
      Package (0x06)
      {
        0x00000546,
        0x00003A98,
        20000,
        20000,
        0x00000005,
        0x00000005
      },
      Package (0x06)
      {
        0x000004E2,
        0x00003A98,
        20000,
        20000,
        0x00000106,
        0x00000006
      },
      Package (0x06)
      {
        0x00000465,
        0x00003A98,
        20000,
        20000,
        0x00000006,
        0x00000006
      },
      Package (0x06)
      {
        0x000003E8,
        0x00003A98,
        20000,
        20000,
        0x00000107,
        0x00000007
      },
      Package (0x06)
      {
        0x00000384,
        0x00003A98,
        20000,
        20000,
        0x00000007,
        0x00000007
      },
      Package (0x06)
      {
        0x000002EE,
        0x00003A98,
        20000,
        20000,
        0x00000108,
        0x00000008
      },
      Package (0x06)
      {
        0x000002A3,
        0x00003A98,
        20000,
        20000,
        0x00000008,
        0x00000008
      },
      Package (0x06)
      {
        0x000001F4,
        0x00003A98,
        20000,
        20000,
        0x00000109,
        0x00000009
      },
      Package (0x06)
      {
        0x000001C2,
        0x00003A98,
        20000,
        20000,
        0x00000009,
        0x00000009
      },
      Package (0x06)
      {
        0x000000FA,
        0x00003A98,
        20000,
        20000,
        0x0000010A,
        0x0000000A
      },
      Package (0x06)
      {
        0x000000E1,
        0x00003A98,
        20000,
        20000,
        0x0000000A,
        0x0000000A
      }
    })
  }

  Device (C001)
  {
    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
    Name (_UID, 2)  // _UID: Unique ID
    Name (_PXM, Zero)  // _PXM: Device Proximity
    Name (_STA, 0x0F)  // _STA: Status
    Method (_PPC, 0, NotSerialized)  // _PPC: Performance Present Capabilites
    {
      return (0);
    }

    Method (_PCT, 0, NotSerialized)  // _PCT: Performance Control
    {
      Return (Package (0x02)
      {
        ResourceTemplate (){
        Register (FFixedHW,
          0x00,               // Bit Width
          0x00,               // Bit Offset
          0x0000000000000000, // Address
        ,)},

        ResourceTemplate (){
        Register (FFixedHW,
          0x00,               // Bit Width
          0x00,               // Bit Offset
          0x0000000000000000, // Address
        ,)}
      })
    }

    Method (_PSS, 0, NotSerialized)  // _PSS: Performance Supported States
    {
      Return (LPSS)
    }

    Name (LPSS, Package (0x10)
    {
      Package (0x06)
      {
        0x000007D0,
        0x00003A98,
        20000,
        20000,
        0x00000103,
        0x00000003
      },
      Package (0x06)
      {
        0x00000708,
        0x00003A98,
        20000,
        20000,
        0x00000003,
        0x00000003
      },
      Package (0x06)
      {
        0x000006D6,
        0x00003A98,
        20000,
        20000,
        0x00000104,
        0x00000004
      },
      Package (0x06)
      {
        0x00000627,
        0x00003A98,
        20000,
        20000,
        0x00000004,
        0x00000004
      },
      Package (0x06)
      {
        0x000005DC,
        0x00003A98,
        20000,
        20000,
        0x00000105,
        0x00000005
      },
      Package (0x06)
      {
        0x00000546,
        0x00003A98,
        20000,
        20000,
        0x00000005,
        0x00000005
      },
      Package (0x06)
      {
        0x000004E2,
        0x00003A98,
        20000,
        20000,
        0x00000106,
        0x00000006
      },
      Package (0x06)
      {
        0x00000465,
        0x00003A98,
        20000,
        20000,
        0x00000006,
        0x00000006
      },
      Package (0x06)
      {
        0x000003E8,
        0x00003A98,
        20000,
        20000,
        0x00000107,
        0x00000007
      },
      Package (0x06)
      {
        0x00000384,
        0x00003A98,
        20000,
        20000,
        0x00000007,
        0x00000007
      },
      Package (0x06)
      {
        0x000002EE,
        0x00003A98,
        20000,
        20000,
        0x00000108,
        0x00000008
      },
      Package (0x06)
      {
        0x000002A3,
        0x00003A98,
        20000,
        20000,
        0x00000008,
        0x00000008
      },
      Package (0x06)
      {
        0x000001F4,
        0x00003A98,
        20000,
        20000,
        0x00000109,
        0x00000009
      },
      Package (0x06)
      {
        0x000001C2,
        0x00003A98,
        20000,
        20000,
        0x00000009,
        0x00000009
      },
      Package (0x06)
      {
        0x000000FA,
        0x00003A98,
        20000,
        20000,
        0x0000010A,
        0x0000000A
      },
      Package (0x06)
      {
        0x000000E1,
        0x00003A98,
        20000,
        20000,
        0x0000000A,
        0x0000000A
      }

    })
  }

  Device (C002)
  {
    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
    Name (_UID, 3)  // _UID: Unique ID
    Name (_PXM, Zero)  // _PXM: Device Proximity
    Name (_STA, 0x0F)  // _STA: Status
    Method (_PPC, 0, NotSerialized)  // _PPC: Performance Present Capabilites
    {
      return (0);
    }

    Method (_PCT, 0, NotSerialized)  // _PCT: Performance Control
    {
      Return (Package (0x02)
      {
        ResourceTemplate (){
        Register (FFixedHW,
          0x00,               // Bit Width
          0x00,               // Bit Offset
          0x0000000000000000, // Address
        ,)},

        ResourceTemplate (){
        Register (FFixedHW,
          0x00,               // Bit Width
          0x00,               // Bit Offset
          0x0000000000000000, // Address
        ,)}
      })
    }
    Method (_PSS, 0, NotSerialized)  // _PSS: Performance Supported States
    {
      Return (LPSS)
    }

    Name (LPSS, Package (0x10)
    {
      Package (0x06)
      {
        0x000007D0,
        0x00003A98,
        20000,
        20000,
        0x00000103,
        0x00000003
      },
      Package (0x06)
      {
        0x00000708,
        0x00003A98,
        20000,
        20000,
        0x00000003,
        0x00000003
      },

      Package (0x06)
      {
        0x000006D6,
        0x00003A98,
        20000,
        20000,
        0x00000104,
        0x00000004
      },
      Package (0x06)
      {
        0x00000627,
        0x00003A98,
        20000,
        20000,
        0x00000004,
        0x00000004
      },
      Package (0x06)
      {
        0x000005DC,
        0x00003A98,
        20000,
        20000,
        0x00000105,
        0x00000005
      },
      Package (0x06)
      {
        0x00000546,
        0x00003A98,
        20000,
        20000,
        0x00000005,
        0x00000005
      },
      Package (0x06)
      {
        0x000004E2,
        0x00003A98,
        20000,
        20000,
        0x00000106,
        0x00000006
      },
      Package (0x06)
      {
        0x00000465,
        0x00003A98,
        20000,
        20000,
        0x00000006,
        0x00000006
      },
      Package (0x06)
      {
        0x000003E8,
        0x00003A98,
        20000,
        20000,
        0x00000107,
        0x00000007
      },
      Package (0x06)
      {
        0x00000384,
        0x00003A98,
        20000,
        20000,
        0x00000007,
        0x00000007
      },
      Package (0x06)
      {
        0x000002EE,
        0x00003A98,
        20000,
        20000,
        0x00000108,
        0x00000008
      },
      Package (0x06)
      {
        0x000002A3,
        0x00003A98,
        20000,
        20000,
        0x00000008,
        0x00000008
      },
      Package (0x06)
      {
        0x000001F4,
        0x00003A98,
        20000,
        20000,
        0x00000109,
        0x00000009
      },
      Package (0x06)
      {
        0x000001C2,
        0x00003A98,
        20000,
        20000,
        0x00000009,
        0x00000009
      },
      Package (0x06)
      {
        0x000000FA,
        0x00003A98,
        20000,
        20000,
        0x0000010A,
        0x0000000A
      },
      Package (0x06)
      {
        0x000000E1,
        0x00003A98,
        20000,
        20000,
        0x0000000A,
        0x0000000A
      }

    })
  }

  Device (C003)
  {
    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
    Name (_UID, 4)  // _UID: Unique ID
    Name (_PXM, Zero)  // _PXM: Device Proximity
    Name (_STA, 0x0F)  // _STA: Status
    Method (_PPC, 0, NotSerialized)  // _PPC: Performance Present Capabilites
    {
      return (0);
    }

    Method (_PCT, 0, NotSerialized)  // _PCT: Performance Control
    {
      Return (Package (0x02)
      {
        ResourceTemplate (){
        Register (FFixedHW,
          0x00,               // Bit Width
          0x00,               // Bit Offset
          0x0000000000000000, // Address
        ,)},

        ResourceTemplate (){
        Register (FFixedHW,
          0x00,               // Bit Width
          0x00,               // Bit Offset
          0x0000000000000000, // Address
       ,)}
      })
    }

    Method (_PSS, 0, NotSerialized)  // _PSS: Performance Supported States
    {
      Return (LPSS)
    }

    Name (LPSS, Package (0x10)
    {
      Package (0x06)
      {
        0x000007D0,
        0x00003A98,
        20000,
        20000,
        0x00000103,
        0x00000003
      },

      Package (0x06)
      {
        0x00000708,
        0x00003A98,
        20000,
        20000,
        0x00000003,
        0x00000003
      },

      Package (0x06)
      {
        0x000006D6,
        0x00003A98,
        20000,
        20000,
        0x00000104,
        0x00000004
      },
      Package (0x06)
      {
        0x00000627,
        0x00003A98,
        20000,
        20000,
        0x00000004,
        0x00000004
      },
      Package (0x06)
      {
        0x000005DC,
        0x00003A98,
        20000,
        20000,
        0x00000105,
        0x00000005
      },
      Package (0x06)
      {
        0x00000546,
        0x00003A98,
        20000,
        20000,
        0x00000005,
        0x00000005
      },
      Package (0x06)
      {
        0x000004E2,
        0x00003A98,
        20000,
        20000,
        0x00000106,
        0x00000006
      },
      Package (0x06)
      {
        0x00000465,
        0x00003A98,
        20000,
        20000,
        0x00000006,
        0x00000006
      },
      Package (0x06)
      {
        0x000003E8,
        0x00003A98,
        20000,
        20000,
        0x00000107,
        0x00000007
      },
      Package (0x06)
      {
        0x00000384,
        0x00003A98,
        20000,
        20000,
        0x00000007,
        0x00000007
      },
      Package (0x06)
      {
        0x000002EE,
        0x00003A98,
        20000,
        20000,
        0x00000108,
        0x00000008
      },
      Package (0x06)
      {
        0x000002A3,
        0x00003A98,
        20000,
        20000,
        0x00000008,
        0x00000008
      },
      Package (0x06)
      {
        0x000001F4,
        0x00003A98,
        20000,
        20000,
        0x00000109,
        0x00000009
      },
      Package (0x06)
      {
        0x000001C2,
        0x00003A98,
        20000,
        20000,
        0x00000009,
        0x00000009
      },
      Package (0x06)
      {
        0x000000FA,
        0x00003A98,
        20000,
        20000,
        0x0000010A,
        0x0000000A
      },
      Package (0x06)
      {
        0x000000E1,
        0x00003A98,
        20000,
        20000,
        0x0000000A,
        0x0000000A
      }
    })
  }
}
