# YCCARD - Smart Card Reader Library

## Project Overview

YCCARD is a Windows DLL library for smart card reader communication, designed to interface with HID-type USB devices. The library provides comprehensive APIs for operating Mifare (M1) cards, constructing card data, and communicating with reader hardware via the HID protocol.

## Project Structure

```
YCCARD/
├── YCCARD.CPP          # Main implementation file (~9,750 lines)
├── YCCARD.h            # Header file with data structures and declarations
├── DLL.DEF             # DLL export definitions
├── StdAfx.h            # Precompiled header (MFC-based)
├── HID/                # HID communication module
│   ├── HID.h           # HID IO class definitions
│   ├── hid.lib         # Windows HID library
│   ├── hidpi.h         # HID parsing definitions
│   ├── hidsdi.h        # HID device attributes
│   └── hidusage.h      # HID usage page definitions
├── Release/            # Build output directory
└── Backup/             # Backup files
```

## Hardware Configuration

| Parameter | Value |
|-----------|-------|
| USB Vendor ID (VID) | 0x2011 |
| USB Product ID (PID) | 0x0318 |
| HID Packet Size | 64 bytes |
| Communication Mode | Asynchronous Read/Write |

## Core Data Structures

### System Information
```c
struct _sys_info {
    unsigned char ch[2];
    int mjxt;              // Access control system flag
    int mjxt_sec;          // Access control sector
    unsigned char mjxt_cardtype;
    int xfxt;              // Payment system flag
    int xfxt_sec;          // Payment system sector
    unsigned char syscard_no[5];
    unsigned char sys_keyb[8];
    unsigned char user_password[8];
    unsigned char oprater_password[8];
    unsigned char comm_password[8];
};
```

### User Card Information
```c
typedef struct _UserCardInfo {
    unsigned long Card_Number;        // Card serial number
    unsigned long Rest_Money;         // Balance
    unsigned long Used_Money;         // Amount used
    unsigned long Used_Recharge_Times;// Recharge count
    unsigned long Used_All_Times;     // Total usage count
    unsigned long Used_All_Money;     // Total consumption
    unsigned long Used_Address;       // Address
    unsigned long Card_Type;          // Card type
    unsigned char User_Password[3];   // User password
    unsigned char User_EndTime[3];    // Expiry date
    unsigned char User_Name[6];       // User name
    unsigned char Used_Time[6];       // Usage time
    unsigned char User_CardData[48];  // Card data
} _UserCardInfo;
```

### Consumption Record
```c
typedef struct XFREC {
    unsigned long RecordSnr;      // Record serial number
    unsigned long CardNum;        // Card number
    unsigned long Rest_Money;     // Balance
    unsigned long Used_Money;     // Consumption amount
    unsigned long Used_All_Times; // Total usage count
    unsigned long Terminal_ID;    // Terminal ID
    unsigned long Operater;       // Operator ID
    char UsedTime[12];            // Usage time
    unsigned char Status;         // Transaction status
} XFREC;
```

## Supported System Types

| System | Code | Description |
|--------|------|-------------|
| MJXT | 1 | Access Control System |
| SFXT | 2 | Payment/Billing System |
| JSXT | 3 | Water Billing System |
| MJDT | 4 | Access Control (alternative) |

## Protocol Format

The communication protocol follows this structure:

```
| Header (2B) | Address (2B) | Command (1B) | Length (1B) | Data (NB) | CRC (1B) | Tail (2B) |
|-------------|--------------|--------------|-------------|-----------|----------|-----------|
| 0xAA5A      | 0x0000       | CommandCode  | DataLen     | Data      | CRC8     | 0xBB6B    |
```

### Command Codes

| Command | Code | Description |
|---------|------|-------------|
| OPEN_RF | 0x20 | Open RF module |
| RF_REQUEST | 0x21 | Request card |
| RF_ANTICOLL | 0x22 | Anti-collision |
| RF_SELECT | 0x23 | Select card |
| RF_AUTHENTICATION | 0x24 | Authenticate |
| RF_READ | 0x25 | Read block |
| RF_WRITE | 0x26 | Write block |
| RF_HALT | 0x27 | Halt card |
| RF_LOAD_KEY | 0x28 | Load key |
| SET_TIME | 0x13 | Set device time |
| GET_TIME | 0x14 | Get device time |
| SET_BELL | 0x15 | Buzzer control |
| SET_DISPLAY | 0x16 | Display control |

## Exported API Functions

### Device Communication
| Function | Description |
|----------|-------------|
| `OpenComm(int CommPort)` | Open communication port |
| `CloseComm(HANDLE icdev)` | Close communication |
| `rf_init(__int16 port, long baud)` | Initialize reader |
| `rf_exit(HANDLE icdev)` | Exit reader |
| `rf_beep(HANDLE icdev, unsigned short _Msec)` | Buzzer beep |

### Card Operations
| Function | Description |
|----------|-------------|
| `rf_request(HANDLE icdev, unsigned char _Mode, unsigned __int16 *TagType)` | Request card |
| `rf_anticoll(HANDLE icdev, unsigned char _Bcnt, unsigned long *_Snr)` | Anti-collision |
| `rf_select(HANDLE icdev, unsigned long _Snr, unsigned char *_Size)` | Select card |
| `rf_authentication(HANDLE icdev, unsigned char _Mode, unsigned char _SecNr)` | Authenticate sector |
| `rf_read(HANDLE icdev, unsigned char _Adr, unsigned char *_Data)` | Read 16-byte block |
| `rf_write(HANDLE icdev, unsigned char _Adr, unsigned char *_Data)` | Write 16-byte block |
| `rf_halt(HANDLE icdev)` | Halt card |
| `rf_load_key(HANDLE icdev, unsigned char _Mode, unsigned char _SecNr, unsigned char *_NKey)` | Load key |

### System Card Operations
| Function | Description |
|----------|-------------|
| `Init_SysCard12_NewCreat()` | Create new system card |
| `Init_SysCard12_NewReWrite()` | Rewrite system card |
| `ReadCard_ID()` | Read card ID |
| `Query_Card_Type()` | Query card type |

### User Card Operations
| Function | Description |
|----------|-------------|
| `Init_Pos_UserCard12()` | Initialize POS user card |
| `WRT_Pos_UserCard12()` | Write to POS user card |
| `RST_Pos_UserCard12()` | Reset POS user card |
| `Init_Js_UserCard()` | Initialize billing user card |
| `WRT_Js_UserCard()` | Write to billing user card |
| `QueryJsCard()` | Query billing card |
| `Make_UserCard_New()` | Create new user card |

### Record Operations
| Function | Description |
|----------|-------------|
| `ReadRecNum()` | Read record count |
| `ReadAllRec()` | Read all records |
| `ClearRecNum()` | Clear records |

### Encryption
| Function | Description |
|----------|-------------|
| `Des()` | DES encryption |
| `UnDes()` | DES decryption |

## Error Codes

| Code | Constant | Description |
|------|----------|-------------|
| 0 | MI_OK / DAS_OK | Success |
| -1 | Comm_Err | Communication error |
| -2 | Reader_Err | Reader error |
| -5 | Para_Err | Parameter error |
| -6 | TimesOut_Err | Timeout error |
| -7 | No_Card | No card present |
| -12 | ReadCard_Err | Card read error |
| -13 | WriteCard_Err | Card write error |
| -17 | No_Key_Err | Key file not found |

## Security Features

1. **DES Encryption**: Built-in DES encryption/decryption for secure data handling
2. **Key Management**: Support for Key A and Key B authentication (Mifare standard)
3. **Password Protection**: User, operator, and communication passwords
4. **CRC Validation**: CRC-8 and CRC-16 for data integrity

## HID Communication Layer

The library uses Windows HID API for USB communication:

```c
class CHidIO {
    // Opens device by VID/PID
    BOOL OpenDevice(BOOL bUseTwoHandle, USHORT usVID, USHORT usPID);
    
    // Asynchronous read with timeout
    BOOL ReadFile(char *pcBuffer, size_t szMaxLen, DWORD *pdwLength, DWORD dwMilliseconds);
    
    // Asynchronous write with timeout
    BOOL WriteFile(const char *pcBuffer, size_t szLen, DWORD *pdwLength, DWORD dwMilliseconds);
};
```

## Build Requirements

- Visual Studio (MFC support required)
- Windows SDK
- HID.lib and Setupapi.lib

## Typical Usage Flow

```c
// 1. Initialize
HANDLE icdev = rf_init(port, baud);

// 2. Request card
unsigned __int16 TagType;
rf_request(icdev, 0x52, &TagType);

// 3. Anti-collision
unsigned long Snr;
rf_anticoll(icdev, 0, &Snr);

// 4. Select card
unsigned char Size;
rf_select(icdev, Snr, &Size);

// 5. Authenticate
rf_authentication(icdev, KEYA, 1);

// 6. Read/Write
unsigned char data[16];
rf_read(icdev, 4, data);
rf_write(icdev, 4, newData);

// 7. Halt
rf_halt(icdev);
rf_exit(icdev);
```

## License

License validation via `licence.das` or `licencecard.dat` files.

---

*Document generated on 2026-03-11*