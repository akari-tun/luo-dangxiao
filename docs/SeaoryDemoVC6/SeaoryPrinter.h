// -*- mode:c++ -*-

/**
 * @file	  SeaoryPrinter.h
 * @author	  Shenzhen Seaory Technology Co., Ltd.
 * @date	  2025/10/30
 * @brief	  Card printer function declaration file.
 * @revision  1.7.0.0
 */

////////////////////////////////////////////////////////////////////////////////
#ifndef __SOYPRAPI_H__
#define __SOYPRAPI_H__

#if defined(_MSC_VER)&&(_MSC_VER < 1600) //VS2010

typedef signed char		int8_t;
typedef short			int16_t;
typedef int				int32_t;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;

#else
	#include <stdint.h>
#endif

#if !defined(WIN32)&&!defined(WIN64)
#ifndef _BITMAP_
#define _BITMAP_

typedef struct tagBITMAP {

	long				bmType;
	long				bmWidth;
	long				bmHeight;
	long				bmWidthBytes;
	short				bmPlanes;
	short				bmBitsPixel;
	void*				bmBits;

} BITMAP;

#endif//#define _BITMAP_

#if defined(UNICODE)||defined(_UNICODE)
#define _T(x)			L##x
typedef wchar_t			TCHAR;
#else
#define _T(x)			x
typedef char			TCHAR;
#endif

#define WINAPI

typedef wchar_t			WCHAR;
typedef bool			BOOL;
typedef void*			HDC;

#ifndef NULL
#define NULL			0
#endif

//#ifndef FALSE
#define FALSE			0
#define TRUE			1
//#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif


//Command used for SOY_PR_ExecCommand()
#define CMD_MOVE_CARD_TO_HOPPER					1			//!<Move Card to Output Hopper
#define CMD_MOVE_CARD_TO_CONTACT				2			//!<Move Card to Contact Encoder Station
#define CMD_MOVE_CARD_TO_CONTACTLESS			3			//!<Move Card to Contactless Encoder Station
#define CMD_MOVE_CARD_TO_STANDBY_BACK			4			//!<Move Card to Standby (back)
#define CMD_MOVE_CARD_TO_FLIPPER				5			//!<Move Card to Flipper
#define CMD_MOVE_CARD_TO_REJECT_BOX_FRONT		6			//!<Move Card to Reject Box (front)
#define CMD_MOVE_CARD_TO_REJECT_BOX_DOWN		7			//!<Move Card to Reject Box (down)
#define CMD_MOVE_CARD_TO_STANDBY_FRONT			8			//!<Move Card to Standby (front)
#define CMD_MOVE_CARD_TO_FRONT					9			//!<Move Card to Front position
#define CMD_MOVE_CARD_TO_PREPARE				10			//!<Move Card to Prepare position
#define CMD_MOVE_CARD_TO_INDENT_PRINTER			11			//!<Move Card to Indent-Printing module
#define CMD_MOVE_CARD_TO_STANDBY_DOWN			12			//!<Move Card to Standby (down)
#define CMD_MOVE_CARD_TO_STORAGE				13			//!<Move card to storage card position
#define CMD_MOVE_CARD_FROM_STORAGE_TO_PREPARE	14			//!<storage card position to prepare position

#define CMD_RESET_PRINTER_HARD					21			//!<Hard reset printer, printer status and temporary setting will be lost.
#define CMD_RESET_PRINTER_JAM					22			//!<Card jam reset, the card will move to prepare position and will finish initialize flow.
#define CMD_CLEAN_CARD_PATH						23			//!<Clean card path
#define CMD_FLIP_CARD							24			//!<Flip card then move card to prepare position
#define CMD_CLEAN_CARD_PATH_NO_EJECT			25			//!<Clean card path, after finished, do not eject card.
#define CMD_INITIALIZE_FLIPPER					27			//!<Rotate flipper to initial position.
#define CMD_CALIBRATE_RIBBON_LED				28			//!<Calibrate ribbon LED
#define CMD_CLEAN_CARD_PATH_ADJ_RBN_MTR			29			//!<Clean card path, adjust ribbon motor when cleaning.

#define CMD_ERASE_FULL_CARD						31			//!<Erase rewritable card whole card

#define CMD_CANCEL_SUSPENDED_TASK				50			//!<Cancel suspended task

//Info type used for SOY_PR_GetPrinterInfo()
#define INFO_MODEL_NAME							1			//!<Printer model name
#define INFO_FW_VERSION							2			//!<Printer firmware version
#define INFO_SERIAL_NUMBER						3			//!<Printer serial number
#define INFO_CARD_POSITION						4			//!<Current card position in printer
#define INFO_RIBBON_TYPE						5			//!<Current installed ribbon type
#define INFO_RIBBON_COUNT						6			//!<Current installed ribbon remain count
#define INFO_PRINT_COUNT						7			//!<Printed count by frames
#define INFO_REGION_CODE						8			//!<Region code
#define INFO_UNCLEAN_COUNT						9			//!<Unclean count
#define INFO_INSTALLED_MODULE					10			//!<Current installed modules
#define INFO_REJECT_BOX_STATUS					11			//!<Current status of front reject box
#define INFO_CARD_OUT_STATUS					12			//!<Current status of card out status
#define INFO_FILM_COUNT							13			//!<Current installed film remain count
#define INFO_MAC_ADDRESS						14			//!<Ethernet MAC address
#define INFO_RIBBON_CHIP_UID					15			//!<Current ribbon chip UID
#define INFO_FILM_CHIP_UID						16			//!<Current film chip UID
#define INFO_STORAGE_STATUS						17			//!<Current status of storage position
#define INFO_RIBBON_COUNT_ORG					18			//!<Current installed ribbon original count
#define INFO_SECURITY_MODE						19			//!<Current security mode
#define INFO_RIBBON_LED_VALUE					20			//!<Ribbon LED value
#define INFO_MAGNETIC_COUNT						21			//!<Magnetic count
#define INFO_CONTACT_COUNT						22			//!<Contact count
#define INFO_CONTACTLESS_COUNT					23			//!<Contactless count
#define INFO_DRIVER_VERSION						24			//!<Printer driver version


#define INFO_INDENT_PRINTER_FW_VERSION			30			//!<Indent-printing module firmware version
#define INFO_INDENT_PRINTER_SERIAL				31			//!<Indent-printing module serial number
#define INFO_INDENT_PRINTER_RIBBON_REMAIN		32			//!<Indent-printing module remain ribbon count


//possible return values for INFO_CARD_POSITION
#define CARD_POS_OUT_OF_PRINTER					0
#define CARD_POS_FRONT_STANDBY					3
#define CARD_POS_FLIPPER						4
#define CARD_POS_MAG_IN							5
#define CARD_POS_MAG_OUT						6
#define CARD_POS_START_PRINTING					7
#define CARD_POS_PRINT_END						8
#define CARD_POS_CONTACT						9
#define CARD_POS_CONTACTLESS					10
#define CARD_POS_BACK_STANDBY					11
#define CARD_POS_CARD_JAM_POS					12
#define CARD_POS_PREPARE_POS					13
#define CARD_POS_START_PRINTING2				15
#define CARD_POS_DOWN_STANDBY					17
#define CARD_POS_WAIT_EMBOSS					18

//Flags of installed module for return value of INFO_INSTALLED_MODULE
#define MODULE_FLIPPER							0x00000001
#define MODULE_MAG_STRIPE						0x00000002
#define MODULE_CONTACT_IC						0x00000004
#define MODULE_RFID								0x00000008
#define MODULE_ETHERNET							0x00000010
#define MODULE_INDENT							0x00000040
#define MODULE_600DPI							0x00010000
#define MODULE_RIBBON_SECURITY					0x00020000
#define MODULE_PRINTER_SECURITY					0x00040000

//Configuration type used for SOY_PR_SetPrinterConfig() and SOY_PR_GetPrinterConfig()
#define CONFIG_MANUAL_FEED_FRONT				1			//!<Front end manual feed slot
#define CONFIG_MANUAL_FEED_BACK					2			//!<Back end manual feed slot
#define CONFIG_INPUT_BIN						3			//!<Input bin to be used
#define CONFIG_ADF_HOOK_MODE					4			//!<ADF to use hook mode or not
#define CONFIG_OUTPUT_BIN_MODE					5			//!<Output bin to be used and its mode
#define CONFIG_INPUT_OUTPUT_BINDING				6			//!<Target output bin when use the selected input bin
#define CONFIG_CARD_IN_SIGNAL					7			//!<Smart card reader card-in signal level
#define CONFIG_REJECT_BOX_SENSOR				8			//!<Reject box sensor detection mode
#define CONFIG_CARD_OUT_SENSOR					9			//!<Card out sensor detection mode
#define CONFIG_REJECT_BIN						10			//!<Reject bin to be used
#define CONFIG_INITIAL_RIBBON_MODE				11			//!<Initial ribbon mode
#define CONFIG_IC_CARD_POS						12			//!<Position of moving card to contact encoder
#define CONFIG_RF_CARD_POS						13			//!<Position of moving card to contactless encoder

#define CONFIG_PRINT_POS_L						14			//!<Offset of printing card leading edge
#define CONFIG_PRINT_POS_T						15			//!<Offset of printing card tailing edge
#define CONFIG_PRINT_POS_S						16			//!<Offset of printing card side edge

#define CONFIG_STANDBY_FRONT_END				17			//!<Standby parameters of front end
#define CONFIG_STANDBY_BACK_END					18			//!<Standby parameters of back end

#define CONFIG_AUTO_EJECT_AT_INIT				19			//!<Auto eject card when power on or hard reset printer
#define CONFIG_AUTO_EJECT_OUTPUT_BIN			20			//!<Output bin to be used for auto eject card

#define CONFIG_MANUAL_FEED_DOWN					21			//!<Down end manual feed slot
#define CONFIG_STANDBY_DOWN_END					22			//!<Standby parameters of down end

#define CONFIG_INPUT_BIN_FOR_CLEAN				23			//!<Input bin to be used for cleaning tool
#define CONFIG_OUTPUT_BIN_FOR_CLEAN				24			//!<Output bin to be used for cleaning tool


//retransfer printer only
#define CONFIG_RTR_CARD_TYPE					101			//!<Retransfer parameters group to be used.                   Default is 0. Range is   0~3
#define CONFIG_RTR_USER1_SPEED					102			//!<Retransfer speed of user defined 1.                       Default is 0. Range is -10~+10
#define CONFIG_RTR_USER1_TEMPERATURE			103			//!<Retransfer temperature of user defined 1.                 Default is 0. Range is -11~+7
#define CONFIG_RTR_USER1_OFFSET					104			//!<Retransfer offset of user defined 1.                      Default is 0. Range is   0~10
#define CONFIG_RTR_USER2_SPEED					105			//!<Retransfer speed of user defined 2.                       Default is 0. Range is -10~+10
#define CONFIG_RTR_USER2_TEMPERATURE			106			//!<Retransfer temperature of user defined 2.                 Default is 0. Range is -11~+7
#define CONFIG_RTR_USER2_OFFSET					107			//!<Retransfer offset of user defined 2.                      Default is 0. Range is   0~10
#define CONFIG_RTR_USER3_SPEED					108			//!<Retransfer speed of user defined 3.                       Default is 0. Range is -10~+10
#define CONFIG_RTR_USER3_TEMPERATURE			109			//!<Retransfer temperature of user defined 3.                 Default is 0. Range is -11~+7
#define CONFIG_RTR_USER3_OFFSET					110			//!<Retransfer offset of user defined 3.                      Default is 0. Range is   0~10
#define CONFIG_RTR_FLAT_SINGLE_SIDE				111			//!<Flat card option when printing single side.               Default is 0. Range is   0~5
#define CONFIG_RTR_FLAT_FRONT_SIDE				112			//!<Flat card option when printing front side of double side. Default is 0. Range is   0~5
#define CONFIG_RTR_FLAT_BACK_SIDE				113			//!<Flat card option when printing back side of double side.  Default is 0. Range is   0~5
#define CONFIG_RTR_FLAT_SPEED					114			//!<Flat card speed.                                          Default is 0. Range is   0~200
#define CONFIG_RTR_EJECT_CARD_SIDE				115			//!<Eject card side option.                                   Default is 0. Range is   0~2

#define CONFIG_RTR_RIBBON_ERASE_MODE			150			//!<Ribbon erase mode
#define CONFIG_RTR_PRINT_ACTION_SEQ				151			//!<Print action sequence

#define CONFIG_INDENT_HAMMER_PARAM				201			//!<Indent-printing module hammer parameters
#define CONFIG_INDENT_DISK_COMP_PARAM			202			//!<Indent-printing module disk digit compensation parameters


//possible dwStatus value returnd by SOY_PR_GetPrinterStatus()
typedef struct tagERROR_DESC {
	uint32_t			dwErrCode;
	const TCHAR*		pDesc;
} ERROR_DESC;

static const ERROR_DESC	ErrorMap[] = {

	{ 0x00010001, _T("Firmware error")								},
	{ 0x00010002, _T("TPH thermistor error")						},
	{ 0x00010003, _T("Encoder error(take)")							},
	{ 0x00010004, _T("Encoder error(supply)")						},
	{ 0x00010005, _T("ADC error")									},
	{ 0x00010006, _T("Encoder error (film) (06)")					},
	{ 0x00010007, _T("Heater thermistor error (07)")				},
	{ 0x00010008, _T("FPGA error (08)")								},
	{ 0x00010009, _T("Burn FPGA error (09)")						},
	{ 0x0001000A, _T("Burn LCD error (0A)")							},

	{ 0x00010010, _T("Card jam (10)")								},
	{ 0x00010011, _T("Card jam (11)")								},
	{ 0x00010012, _T("Card jam (12)")								},
	{ 0x00010013, _T("Card jam (13)")								},
	{ 0x00010014, _T("Card jam (14)")								},
	{ 0x00010015, _T("Card jam (15)")								},
	{ 0x00010016, _T("Card jam (16)")								},
	{ 0x00010017, _T("Card jam (17)")								},
	{ 0x00010018, _T("Card jam (18)")								},
	{ 0x00010019, _T("Card jam (19)")								},
	{ 0x0001001A, _T("Card jam (1A)")								},
	{ 0x0001001B, _T("Card jam (1B)")								},
	{ 0x0001001C, _T("Card jam (1C)")								},
	{ 0x0001001D, _T("Card jam (1D)")								},
	{ 0x0001001E, _T("Card jam (1E)")								},
	{ 0x0001001F, _T("Card jam (1F)")								},

	{ 0x00010021, _T("Cover open")									},
	{ 0x00010022, _T("Rejectbox open")								},
	{ 0x00010023, _T("Rejectbox full")								},
	{ 0x00010024, _T("Flat cover open (24)")						},

	{ 0x00010031, _T("TPH cam error")								},
	{ 0x00010032, _T("ADF cam error")								},
	{ 0x00010033, _T("Flipper error")								},
	{ 0x00010034, _T("Heater cam error (34)")						},
	{ 0x00010035, _T("Flat cam error (35)")							},

	{ 0x00010041, _T("Ribbon out (41)")								},
	{ 0x00010042, _T("Ribbon error (42)")							},
	{ 0x00010043, _T("Ribbon missing (43)")							},
	{ 0x00010044, _T("Ribbon unsupport (44)")						},
	{ 0x00010045, _T("Ribbon missing (45)")							},
	{ 0x00010046, _T("Ribbon out (46)")								},
	{ 0x00010047, _T("Ribbon mismatch (47)")						},
	{ 0x00010048, _T("Ribbon error (48)")							},
	{ 0x00010049, _T("Ribbon error (49)")							},
	{ 0x0001004A, _T("Ribbon installation error (4A)")				},

	{ 0x00010051, _T("Card feed error (51)")						},
	{ 0x00010052, _T("Card feed error (52)")						},
	{ 0x00010053, _T("Card feed error (53)")						},
	{ 0x00010054, _T("Card feed error (54)")						},
	{ 0x0001005E, _T("Card eject error (5E)")						},
	{ 0x0001005F, _T("Card out (5F)")								},

	{ 0x00010061, _T("Auth. error (61)")							},
	{ 0x00010062, _T("Auth. error (62)")							},
	{ 0x00010063, _T("Auth. error (63)")							},

	{ 0x00010071, _T("Film out (71)")								},
	{ 0x00010072, _T("Film error (72)")								},
	{ 0x00010073, _T("Film missing (73)")							},
	{ 0x00010074, _T("Film unsupport (74)")							},
	{ 0x00010075, _T("Film missing (75)")							},
	{ 0x00010076, _T("Film out (76)")								},
	{ 0x00010077, _T("Film error (77)")								},
	{ 0x00010078, _T("Film error (78)")								},
	{ 0x00010079, _T("Film unsupport (79)")							},
	{ 0x0001007A, _T("Film installation error (7A)")				},

	//third party module error
	{ 0x00010081, _T("ADF module error (81)")						},
	//concave module error
	{ 0x00010082, _T("Indent-printing error (82)")					},
	{ 0x00012001, _T("System busy. (8201)")									},
	{ 0x00012002, _T("Invalid command. (8202)")								},
	{ 0x00012003, _T("Parameter error. (8203)")								},
	{ 0x00012004, _T("Validation failed. (8204)")							},
	{ 0x00012005, _T("Read-only data. (8205)")								},
	{ 0x00012006, _T("Card inside. (8206)")									},
	{ 0x00012007, _T("No card inside. (8207)")								},
	{ 0x00012008, _T("The pressing mechanism is not at the origin. (8208)")	},
	{ 0x00012009, _T("Card input or card output timeout. (8209)")			},
	{ 0x0001200A, _T("Ribbon cassette not locked. (820A)")					},
	{ 0x0001200B, _T("Ribbon cassette not detected. (820B)")				},
	{ 0x0001200C, _T("RFID detection failed. (820C)")						},
	{ 0x0001200D, _T("Ribbon tension timeout. (820D)")						},
	{ 0x0001200E, _T("Dial positioning error. (820E)")						},
	{ 0x0001200F, _T("Press down timeout. (820F)")							},
	{ 0x00012010, _T("RFID cardless. (8210)")								},
	{ 0x00012011, _T("RFID key error. (8211)")								},
	{ 0x00012012, _T("Failed to read and write RFID data. (8212)")			},
	{ 0x00012013, _T("Abnormal RFID card. (8213)")							},
	{ 0x00012014, _T("Abnormal down pressure. (8214)")						},
	{ 0x00012015, _T("Abnormal ribbon. (8215)")								},
	{ 0x00012016, _T("Abnormal digital disk. (8216)")						},
	{ 0x00012017, _T("Abnormal card entry. (8217)")							},
	{ 0x00012018, _T("Insufficient Ribbon. (8218)")							},
	{ 0x000120FD, _T("UART error. (82FD)")									},
	{ 0x000120FE, _T("Bootloader mode. (82FE)")								},
	{ 0x000120FF, _T("Not connected. (82FF)")								},

	{ 0x00010091, _T("Card jam (91)")								},
	{ 0x00010092, _T("Card jam (92)")								},

	{ 0x000100A1, _T("The firmware command is not supported.")		},
	{ 0x000100A2, _T("The firmware command parameter is wrong.")	},
	{ 0x000100A3, _T("The firmware task is not created.")			},
	{ 0x000100A4, _T("The firmware command is rejected.")			},
	{ 0x000100A5, _T("The printer memory is full.")					},
	{ 0x000100A6, _T("Need authentication before processing command.")	},
	{ 0x000100A7, _T("Fail to update firmware.")					},

	{ 0x00011001, _T("Track data is empty.")						},
	{ 0x00011002, _T("There is wrong character in track 1.")		},
	{ 0x00011003, _T("There is wrong character in track 2.")		},
	{ 0x00011004, _T("There is wrong character in track 3.")		},
	{ 0x00011005, _T("Magnetic encoding module is not attached.") 	},
	{ 0x00011006, _T("Encoding magnetic stripe fail!") 				},
	{ 0x00011007, _T("The flipper module is not attached.")			},
	{ 0x00011008, _T("The 600DPI feature is not enabled.")			},
	{ 0x00011009, _T("Track 1 is empty.")							},
	{ 0x0001100A, _T("Track 2 is empty.")							},
	{ 0x0001100B, _T("Track 3 is empty.")							},
	{ 0x0001100C, _T("Track 1,2 are empty.")						},
	{ 0x0001100D, _T("Track 1,3 are empty.")						},
	{ 0x0001100E, _T("Track 2,3 are empty.")						},
	{ 0x0001100F, _T("Track 1,2,3 are empty.")						},

	{ 0x00011011, _T("Ribbon is not matched setting.")				},
	{ 0x00011012, _T("The feature of printing non-standard length card is not enabled.")	},
	{ 0x00011013, _T("The drawing text or image is outside the scope of the card.")			},
	{ 0x00011014, _T("There is no card in printer.")				},
	{ 0x00011015, _T("Please take out ribbon.")						},

	{ 0x00011020, _T("Magstripe is blank.")							},
	{ 0x00011021, _T("Track 1 data is blank or has errors.")		},
	{ 0x00011022, _T("Track 2 data is blank or has errors.")		},
	{ 0x00011023, _T("Track 3 data is blank or has errors.")		},
	{ 0x00011024, _T("Track 1,2 data is blank or has errors.")		},
	{ 0x00011025, _T("Track 1,3 data is blank or has errors.")		},
	{ 0x00011026, _T("Track 2,3 data is blank or has errors.")		},
	{ 0x00011027, _T("Track 1,2,3 data is blank or has errors.")	},

	{ 0x00011030, _T("The smart card module is not attached.")		},
	{ 0x00011031, _T("Calibrate smart card position fail.")			},

	{ 0x00011040, _T("The firmware file does not match the printer model.")					},
	{ 0x00011041, _T("The current firmware version is the same as the firmware file.")		},
	{ 0x00011042, _T("The current firmware version is newer than firmware file.")			},

	{ 0x00011043, _T("Indent-printing module is not attached.")		},

	{ 0x00013001, _T("Printer is doing ribbon security erase.")		},
	{ 0x00013002, _T("Printing job is suspended.")					},
	{ 0x00013003, _T("There is one card in storage position.")		},
	{ 0x00013004, _T("There is no card in storage position.")		},
	{ 0x00013005, _T("Printer is locked.")							},

	{ 0xFFFFFFFF, _T("")			}
};

static const ERROR_DESC	ErrorMap2[] = {

	{	1,		_T("Incorrect function.")														},
	{	2,		_T("The system cannot find the file specified.")								},
	{	3,		_T("The system cannot find the path specified.")								},
	{	4,		_T("The system cannot open the file.")											},
	{	5,		_T("Access is denied.")															},
	{	6,		_T("The handle is invalid.")													},
	{	8,		_T("Not enough storage is available to process this command.")					},
	{   12,		_T("The access code is invalid.")												},
	{   13,		_T("The data is invalid.")														},
	{   14,		_T("Not enough memory resources are available to complete this operation.")		},
	{   20,		_T("The system cannot find the device specified.")								},
	{   21,		_T("The device is not ready.")													},
	{   22,		_T("The device does not recognize the command.")								},
	{   23,		_T("Data error (cyclic redundancy check).")										},
	{   24,		_T("The program issued a command but the command length is incorrect.")			},
	{   29,		_T("The system cannot write to the specified device.")							},
	{   30,		_T("The system cannot read from the specified device.")							},
	{   31,		_T("A device attached to the system is not functioning.")						},
	{   50,		_T("The request is not supported.")												},
	{   61,		_T("The printer queue is full.")												},
	{   63,		_T("Your file waiting to be printed was deleted.")								},
	{   87,		_T("The parameter is incorrect.")												},
	{  110,		_T("The system cannot open the device or file specified.")						},
	{  120,		_T("This function is not supported on this system.")							},
	{  122,		_T("The data area passed to a system call is too small.")						},
	{  123,		_T("The filename, directory name, or volume label syntax is incorrect.")		},
	{  124,		_T("The system call level is not correct.")										},
	{  126,		_T("The specified module could not be found.")									},
	{  127,		_T("The specified procedure could not be found.")								},
	{  170,		_T("The requested resource is in use.")											},
	{  183,		_T("Cannot create a file when that file already exists.")						},

	{  232,		_T("The pipe is being closed.")													},
	{  234,		_T("More data is available.")													},
	{  258,		_T("The wait operation timed out.")												},

	{  316,		_T("The device does not support the command feature.")								},
	{  321,		_T("The device is unreachable.")													},
	{  322,		_T("The target device has insufficient resources to complete the operation.")		},
	{  323,		_T("A data integrity checksum error occurred. Data in the file stream is corrupt.")	},
	{  329,		_T("An operation is currently in progress with the device.")						},
	{  359,		_T("The device is in maintenance mode.")											},

	{  483,		_T("The request failed due to a fatal device hardware error.")						},
	{  487,		_T("Attempt to access invalid address.")											},

	{  995,		_T("The I/O operation has been aborted because of either a thread exit or an application request.")	},
	{  996,		_T("Overlapped I/O event is not in a signaled state.")											},
	{  997,		_T("Overlapped I/O operation is in progress.")													},

	{ 1003,		_T("Cannot complete this function.")															},
	{ 1010,		_T("The configuration registry key is invalid.")												},
	{ 1111,		_T("The I/O bus was reset.")																	},
	{ 1117,		_T("The request could not be performed because of an I/O device error.")						},
	{ 1157,		_T("One of the library files needed to run this application cannot be found.")					},
	{ 1164,		_T("The indicated device requires reinitialization due to hardware errors.")					},
	{ 1165,		_T("The device has indicated that cleaning is required before further operations are attempted.")	},
	{ 1166,		_T("The device has indicated that its door is open.")											},
	{ 1167,		_T("The device is not connected.")																},

	{ 1200,		_T("The specified device name is invalid.")												},
	{ 1222,		_T("The network is not present or not started.")										},
	{ 1223,		_T("The operation was canceled by the user.")											},
	{ 1225,		_T("The remote computer refused the network connection.")								},
	{ 1231,		_T("The network location cannot be reached.")											},
	{ 1232,		_T("The network location cannot be reached.")											},
	{ 1234,		_T("No service is operating at the destination network endpoint on the remote system.")	},
	{ 1235,		_T("The request was aborted.")															},
	{ 1236,		_T("The network connection was aborted by the local system.")							},
	{ 1237,		_T("The operation could not be completed. A retry should be performed.")				},
	{ 1241,		_T("The network address could not be used for the operation requested.")				},
	{ 1246,		_T("Continue with work in progress.")													},
	{ 1248,		_T("No more local devices.")															},
	{ 1271,		_T("The machine is locked and cannot be shut down without the force option.")			},
	{ 1287,		_T("Insufficient information exists to identify the cause of failure.")					},

	{ 1425,		_T("Invalid device context (DC) handle.")												},
	{ 1450,		_T("Insufficient system resources exist to complete the requested service.")			},
	{ 1460,		_T("This operation returned because the timeout period expired.")						},
	{ 1462,		_T("Incorrect size argument.")															},

	{ 1627,		_T("Function failed during execution.")													},
	{ 1628,		_T("Invalid or unknown table specified.")												},
	{ 1629,		_T("Data supplied is of wrong type.")													},
	{ 1630,		_T("Data of this type is not supported.")												},

	{ 1708,		_T("No endpoint was found.")															},
	{ 1722,		_T("The RPC server is unavailable.")													},
	{ 1743,		_T("The string is too long.")															},
	{ 1784,		_T("The supplied user buffer is not valid for the requested operation.")				},
	{ 1796,		_T("The specified port is unknown.")													},
	{ 1801,		_T("The printer name is invalid.")														},
	{ 1803,		_T("The printer command is invalid.")													},
	{ 1804,		_T("The specified datatype is invalid.")												},
	{ 1903,		_T("The specified form size is invalid.")												},
	{ 1911,		_T("The object specified was not found.")												},
	{ 2000,		_T("The pixel format is invalid.")														},

	{ 2250,		_T("This network connection does not exist.")											},

	{ 3009,		_T("The requested operation is not allowed when there are jobs queued to the printer.")	},
	{ 3012,		_T("No printers were found.")															},

	{ 10061,	_T("No connection could be made because the target machine actively refused it.")                  },

	{ 0xFFFFFFFF, _T("")			}
};

static const ERROR_DESC	WarningMap[] = {

	{ 0x00020001, _T("Ribbon low")						},
	{ 0x00020002, _T("Card low 3")						},
	{ 0x00020003, _T("Waiting for card insertion")		},
	{ 0x00020004, _T("Waiting for card removal")		},
	{ 0x00020005, _T("Card low 2")						},
	{ 0x00020006, _T("Card low 1")						},

	{ 0x00020009, _T("Cleaning procedure recommended")	},
	{ 0x0002000A, _T("Data undentified")				},

	{ 0x00020011, _T("Magstripe is blank")				},
	{ 0x00020014, _T("Magstripe data error")			},

	{ 0xFFFFFFFF, _T("")			}
};



//used for SOY_PR_ModifyDocumentProperties()
typedef struct tagSEAORY_DOC_PROP
{
	uint8_t		byOrientation;			//!<1=portrait, 2=landscape.
	uint8_t		byRibbonType;			//!<For S series: 0=YMCKO, 1=K, 2=1/2ymcKO, 3=YMCKOK, 4=KO, 5=Gold, 6=Silver, 7=White, 8=1/2ymcKOKO, 9=1/2ymcKO-n, 10=RYMCK, 11=UV, 12=Red, 13=Blue.
										//!<For R series: 100=YMCK, 101=YMCKK, 1=K.

	uint8_t		byPrintSide;			//!<0x00=no printing, 0x01=front side, 0x10=back side, 0x11=both sides.
	uint8_t		byPrintPanelFront;		//!<0=YMCKO, 1=YMCO, 2=K, 3=KO, 4=YMCK, 5=YMC. Print panels for front side when ribbon has YMCKO.
										//!<For RYMCK ribbon: 0=RYMCK, 1=RYMC, 2=K, 3=RK, 4=RYMCK+R, 5=YMCK+R.
										//!<For YMCK ribbon: 0=YMCK, 1=YMC, 2=K.
	uint8_t		byPrintPanelBack;		//!<0=YMCKO, 1=YMCO, 2=K, 3=KO, 4=YMCK, 5=YMC. Print panels for back side when ribbon has YMCKO.
										//!<For RYMCK ribbon: 0=RYMCK, 1=RYMC, 2=K, 3=RK, 4=RYMCK+R, 5=YMCK+R.
										//!<For YMCK ribbon: 0=YMCK, 1=YMC, 2=K.

	uint8_t		byResinKfront;			//!<When byPrintPanelFront is K or KO => 0=Dither, 1=Black/White.
										//!<Otherwise => 0=Black text, images and graphics, 1=Black text, 2=All black pixels
	uint8_t		byResinKback;			//!<When byPrintPanelBack is K or KO => 0=Dither, 1=Black/White.
										//!<Otherwise => 0=Black text, images and graphics, 1=Black text, 2=All black pixels
	uint8_t		byRotate180;			//!<0x00=no rotation, 0x01=rotate front side, 0x10=rotate back side, 0x11=rotate both sides.

	uint8_t		byInputBin;				//!<0=card feeder, 1=front end manual slot, 2=back end manual slot.
	uint8_t		byFeedCardMode;			//!<0x00=no hook mode, 0x01=hook mode, 0x02=retry by hook mode, 0x03=hook mode+retry by hook mode.
	uint8_t		byOutputBin;			//!<0=output hopper, 1=front end manual slot, 2=back end manual slot, 3=reject box, 4=Do not eject card.
	uint8_t		byEjectCardMode;		//!<0=Output directly, 1=Wait for removal.
	uint8_t		byCardInOutByDev;		//!<0=Use card input and output settings by above 4 fields. 1=Use card input and output settings saved in device. The above 4 fields will be ignored.

	uint8_t		byResolution;			//!<For S series: 0=300x300dpi, 1=300x600dpi.
										//!<For R series: This field is ignored.
	uint8_t		byEraseCard;			//!<For S20R/S22R only. 0x00=no erase action, 0x01=erase before print, 0x80=erase only.

	uint8_t		byAutoDetectRibbon;		//!<0=Use ribbon as byRibbonType setting, 1=auto detect ribbon.
	uint8_t		byMirror;				//!<0x00=no mirror, 0x01=mirror front side, 0x10=mirror back side, 0x11=mirror both sides.
	uint8_t		byMonoSpeedMode;		//!<0=Normal speed,1=Print mono ribbon by speed mode.
										//!<For R series: This field is ignored.

	uint8_t		byGrayYMC;				//!<0=Normal color printing by YMC, 1=Print color pixels by gray mode.
	uint8_t		byPaperSize;			//!<For S series: 0=CR-80, 1=54mm X 114mm(need mono ribbon), 2=A4(need special driver).
										//!<For R series: This field is ignored.

	uint8_t		byReserved0[5];
	uint8_t		by300x1200Mode;
	uint8_t		bySecurityErase;		//!<For R600 series only, do security erase on K panel: 0x00=no,0x01=front side,0x10=back side,0x11=both sides.
	uint8_t		byReserved[229];

}SEAORY_DOC_PROP;


typedef struct tagPRINT_CARD_PARAM
{
	uint32_t	dwSize;				//!<size of PRINT_CARD_PARAM
	uint32_t	dwReserve2;
	uint32_t	dwReserve3;
	uint32_t	dwReserve4;

	uint8_t		byErasePass;		//!<For S20R/S22R only. Set number of pass for erasing card.
	uint8_t		byReserve2;
	uint8_t		byReserve3;
	uint8_t		byReserve4;
	uint8_t		byReserve5;
	uint8_t		byReserve6;
	uint8_t		byReserve7;
	uint8_t		byReserve8;

	//!<For S series:	 648x1012 pixels, each pixel is 24-bits
	//!<For R300:		 648x1030 pixels, each pixel is 24-bits
	//!<For R600/R600M: 1296x2060 pixels, each pixel is 24-bits

	BITMAP		*lpFrontBGR;		//!<Specifies the color data to be printed on front side.
	BITMAP		*lpFrontK;			//!<Specifies the K data to be printed on front side.
									//!<Each pixel with value 0x00 will be transfer to card by K panel.
	BITMAP		*lpFrontO;			//!<Specifies the O data to be printed on front side.
									//!<Each pixel with value 0x00 will be transfer to card by O panel.
									//!<If lpFrontO is null, then O will be printed on full card by default.

	BITMAP		*lpBackBGR;			//!<Specifies the color data to be printed on back side.
	BITMAP		*lpBackK;			//!<Specifies the K data to be printed on back side.
									//!<Each pixel with value 0x00 will be transfer to card by K panel.
	BITMAP		*lpBackO;			//!<Specifies the O data to be printed on back side.
									//!<Each pixel with value 0x00 will be transfer to card by O panel.
									//!<If lpBackO is null, then O will be printed on full card by default.

	BITMAP		*lpFrontErase;		//!<Specifies the front side erase data for rewrite card, or security erase data R600 series.
	BITMAP		*lpBackErase;		//!<Specifies the back side security erase data for R600 series.

	short		ColorAdj[8];		//!<[0] Brightness		-100 ~ 100
									//!<[1] Contrast		-100 ~ 100
									//!<[2] Sharpness		0 ~ 100
									//!<[3] Yellow Balance	-100 ~ 100
									//!<[4] Magenta Balance	-100 ~ 100
									//!<[5] Cyan Balance	-100 ~ 100
									//!<[6] Saturation		-100 ~ 100
									//!<[7] Gamma	(x0.01)	10 ~ 999


	//Heating Energy, value range of following fields are -127 ~ 127
	//Front side
	char		chFrontHeatYMC;
	char		chFrontHeatK;
	char		chFrontHeatO;
	char		chFrontHeatResinK;

	//Back side
	char		chBackHeatYMC;
	char		chBackHeatK;
	char		chBackHeatO;
	char		chBackHeatResinK;

	//rewrite card
	char		chFrontHeatWrite;
	char		chFrontHeatErase;

	//R of RYMCK ribbon
	char		chFrontHeatRcv;
	char		chBackHeatRcv;

	uint8_t		byReserved[140];

	//
	uint8_t		byPrintConcaveDigits;
	uint8_t		bySpace;
	uint8_t		byReserveC1;
	uint8_t		byReserveC2;
	uint16_t	wDistance;
	uint16_t	wReserveC1;
	char		szDigitsA[24];

}PRINT_CARD_PARAM;


typedef struct tagINDENT_HAMMER_PARAM
{
	int16_t		shDigits[10];

	int16_t		shReserved[6];

} INDENT_HAMMER_PARAM;


typedef struct tagINDENT_DISK_COMP_PARAM
{
	int16_t		shDigits[10];

	int16_t		shReserved[6];

} INDENT_DISK_COMP_PARAM;



typedef struct tagPRINT_AREA_PARAM
{
	//Front side
	BITMAP		frontYMCO;
	BITMAP		frontK;
	BITMAP		frontRewrite;
	BITMAP		frontUnused1;

	//Back side
	BITMAP		backYMCO;
	BITMAP		backK;
	BITMAP		backUnused1;
	BITMAP		backUnused2;

}PRINT_AREA_PARAM;


void	 WINAPI SOY_PR_SetLogLevel				(uint32_t dwLogLevel);
uint32_t WINAPI SOY_PR_SdkVersionA				(char* szVersionOutA);
uint32_t WINAPI SOY_PR_SdkVersionW				(WCHAR* szVersionOutW);

uint32_t WINAPI SOY_PR_ExecCommandA				(const char* szPrinterNameA, uint32_t dwCommand);
uint32_t WINAPI SOY_PR_ExecCommandW				(const WCHAR* szPrinterNameW, uint32_t dwCommand);
uint32_t WINAPI SOY_PR_GetPrinterConfigA		(const char* szPrinterNameA, uint32_t dwConfigType, int32_t* lpnConfigValue);
uint32_t WINAPI SOY_PR_GetPrinterConfigW		(const WCHAR* szPrinterNameW, uint32_t dwConfigType, int32_t* lpnConfigValue);
uint32_t WINAPI SOY_PR_GetPrinterInfoA			(const char* szPrinterNameA, int32_t nInfoType, char* szInfoOutA);
uint32_t WINAPI SOY_PR_GetPrinterInfoW			(const WCHAR* szPrinterNameW, int32_t nInfoType, WCHAR* szInfoOutW);
uint32_t WINAPI SOY_PR_GetPrinterStatusA		(const char* szPrinterNameA, uint32_t *lpdwStatus);
uint32_t WINAPI SOY_PR_GetPrinterStatusW		(const WCHAR* szPrinterNameW, uint32_t *lpdwStatus);
uint32_t WINAPI SOY_PR_GetPrinterWarningA		(const char* szPrinterNameA, uint32_t *lpdwStatus);
uint32_t WINAPI SOY_PR_GetPrinterWarningW		(const WCHAR* szPrinterNameW, uint32_t *lpdwStatus);
uint32_t WINAPI SOY_PR_SetPrinterConfigA		(const char* szPrinterNameA, uint32_t dwConfigType, int32_t nConfigValue);
uint32_t WINAPI SOY_PR_SetPrinterConfigW		(const WCHAR* szPrinterNameW, uint32_t dwConfigType, int32_t nConfigValue);
uint32_t WINAPI SOY_PR_SetStandbyParametersA	(const char* szPrinterNameA, uint8_t byOutputBin, uint8_t byStandbyPos, uint32_t dwStandbyTime);
uint32_t WINAPI SOY_PR_SetStandbyParametersW	(const WCHAR* szPrinterNameW, uint8_t byOutputBin, uint8_t byStandbyPos, uint32_t dwStandbyTime);
uint32_t WINAPI SOY_PR_WaitPrinterBusyA			(const char* szPrinterNameA, uint32_t *lpdwStatus);
uint32_t WINAPI SOY_PR_WaitPrinterBusyW			(const WCHAR* szPrinterNameW, uint32_t *lpdwStatus);

uint32_t WINAPI SOY_PR_ReadTrackA				(const char* szPrinterNameA, uint32_t dwMode, char* szTrack1A, char* szTrack2A, char* szTrack3A);
uint32_t WINAPI SOY_PR_ReadTrackW				(const WCHAR* szPrinterNameW, uint32_t dwMode, WCHAR* szTrack1W, WCHAR* szTrack2W, WCHAR* szTrack3W);
uint32_t WINAPI SOY_PR_WriteTrackA				(const char* szPrinterNameA, uint32_t dwMode, const char* szTrack1A, const char* szTrack2A, const char* szTrack3A);
uint32_t WINAPI SOY_PR_WriteTrackW				(const WCHAR* szPrinterNameW, uint32_t dwMode, const WCHAR* szTrack1W, const WCHAR* szTrack2W, const WCHAR* szTrack3W);
uint32_t WINAPI SOY_PR_EncodeTrackA				(const char* szPrinterNameA, uint32_t dwMode, const char* szTrack1A, const char* szTrack2A, const char* szTrack3A, uint32_t dwRetry);
uint32_t WINAPI SOY_PR_EncodeTrackW				(const WCHAR* szPrinterNameW, uint32_t dwMode, const WCHAR* szTrack1W, const WCHAR* szTrack2W, const WCHAR* szTrack3W, uint32_t dwRetry);

uint32_t WINAPI SOY_PR_PrintOneCardA			(const char* szPrinterNameA, PRINT_CARD_PARAM *lpJobPara);
uint32_t WINAPI SOY_PR_PrintOneCardW			(const WCHAR* szPrinterNameW, PRINT_CARD_PARAM *lpJobPara);

uint32_t WINAPI SOY_PR_StartPrinting2W			(const WCHAR* szPrinterNameW, uint8_t* lpDocPropIn, HDC* lphPrinterDC);
uint32_t WINAPI SOY_PR_StartPrinting2A			(const char* szPrinterNameA, uint8_t* lpDocPropIn, HDC* lphPrinterDC);
uint32_t WINAPI SOY_PR_StartPage2				(HDC hPrinterDC);
uint32_t WINAPI SOY_PR_EndPage2					(HDC hPrinterDC);
uint32_t WINAPI SOY_PR_EndPrinting2				(HDC hPrinterDC, BOOL bCancelJob);
uint32_t WINAPI SOY_PR_PrintImage2W				(HDC hPrinterDC, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, const WCHAR* szImageUrlW);
uint32_t WINAPI SOY_PR_PrintImage2A				(HDC hPrinterDC, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, const char* szImageUrlA);
uint32_t WINAPI SOY_PR_PrintText2W				(HDC hPrinterDC, int32_t nX, int32_t nY, const WCHAR* szTextW, const WCHAR* szFontNameW, int32_t nFontSize, int32_t nFontWeight, uint8_t byFontAttribute, uint8_t byCharSet, uint32_t dwTextColor, BOOL bTransparentBack);
uint32_t WINAPI SOY_PR_PrintText2A				(HDC hPrinterDC, int32_t nX, int32_t nY, const char* szTextA, const char* szFontNameA, int32_t nFontSize, int32_t nFontWeight, uint8_t byFontAttribute, uint8_t byCharSet, uint32_t dwTextColor, BOOL bTransparentBack);
uint32_t WINAPI SOY_PR_PrintText3W				(HDC hPrinterDC, int32_t nX, int32_t nY, const WCHAR* szTextW, const WCHAR* szFontNameW, int32_t nFontSize, int32_t nFontWeight, uint8_t byFontAttribute, uint8_t byCharSet, uint32_t dwTextColor, BOOL bTransparentBack, int32_t nExtraSpace);
uint32_t WINAPI SOY_PR_PrintText3A				(HDC hPrinterDC, int32_t nX, int32_t nY, const char* szTextA, const char* szFontNameA, int32_t nFontSize, int32_t nFontWeight, uint8_t byFontAttribute, uint8_t byCharSet, uint32_t dwTextColor, BOOL bTransparentBack, int32_t nExtraSpace);

uint32_t WINAPI SOY_PR_GetBinFirmwareVersionA	(const char* szBinUrlA, char* szBinVerOutA);
uint32_t WINAPI SOY_PR_GetBinFirmwareVersionW	(const WCHAR* szBinUrlW, WCHAR* szBinVerOutW);
uint32_t WINAPI SOY_PR_UpdateFirmwareA			(const char* szPrinterNameA, const char* szBinUrlA);
uint32_t WINAPI SOY_PR_UpdateFirmwareW			(const WCHAR* szPrinterNameW, const WCHAR* szBinUrlW);

uint32_t WINAPI SOY_PR_PrintConcaveDigitsA		(const char* szPrinterNameA, uint32_t dwReserved, uint32_t dwDistance, uint32_t dwSpace, const char* szDigitsA);
uint32_t WINAPI SOY_PR_PrintConcaveDigitsW		(const WCHAR* szPrinterNameW, uint32_t dwReserved, uint32_t dwDistance, uint32_t dwSpace, const WCHAR* szDigitsW);

uint32_t WINAPI SOY_PR_GetIndentConfigA			(const char* szPrinterNameA, uint32_t dwConfigType, void* lpConfigStruct);
uint32_t WINAPI SOY_PR_GetIndentConfigW			(const WCHAR* szPrinterNameW, uint32_t dwConfigType, void* lpConfigStruct);
uint32_t WINAPI SOY_PR_SetIndentConfigA			(const char* szPrinterNameA, uint32_t dwConfigType, void* lpConfigStruct);
uint32_t WINAPI SOY_PR_SetIndentConfigW			(const WCHAR* szPrinterNameW, uint32_t dwConfigType, void* lpConfigStruct);
uint32_t WINAPI SOY_PR_UpdateIndentFirmwareA	(const char* szPrinterNameA, const char* szBinUrlA);
uint32_t WINAPI SOY_PR_UpdateIndentFirmwareW	(const WCHAR* szPrinterNameW, const WCHAR* szBinUrlW);

uint32_t WINAPI SOY_PR_SetSecurityModeA			(const char* szPrinterNameA, const char* szCurrentPasswdA, int32_t nSecurityMode);
uint32_t WINAPI SOY_PR_SetSecurityModeW			(const WCHAR* szPrinterNameW, const WCHAR* szCurrentPasswdW, int32_t nSecurityMode);
uint32_t WINAPI SOY_PR_SetSecurityPasswordA		(const char* szPrinterNameA, const char* szCurrentPasswdA, const char* szNewPasswdA);
uint32_t WINAPI SOY_PR_SetSecurityPasswordW		(const WCHAR* szPrinterNameW, const WCHAR* szCurrentPasswdW, const WCHAR* szNewPasswdW);


#if defined(UNICODE)||defined(_UNICODE)

#define SOY_PR_SdkVersion					SOY_PR_SdkVersionW
#define SOY_PR_ExecCommand					SOY_PR_ExecCommandW
#define SOY_PR_GetPrinterConfig				SOY_PR_GetPrinterConfigW
#define SOY_PR_GetPrinterInfo				SOY_PR_GetPrinterInfoW
#define SOY_PR_GetPrinterStatus				SOY_PR_GetPrinterStatusW
#define SOY_PR_GetPrinterWarning			SOY_PR_GetPrinterWarningW
#define SOY_PR_SetPrinterConfig				SOY_PR_SetPrinterConfigW
#define SOY_PR_SetStandbyParameters			SOY_PR_SetStandbyParametersW
#define SOY_PR_WaitPrinterBusy				SOY_PR_WaitPrinterBusyW

#define SOY_PR_ReadTrack					SOY_PR_ReadTrackW
#define SOY_PR_WriteTrack					SOY_PR_WriteTrackW
#define SOY_PR_EncodeTrack					SOY_PR_EncodeTrackW

#define SOY_PR_StartPrinting2				SOY_PR_StartPrinting2W
#define SOY_PR_PrintImage2					SOY_PR_PrintImage2W
#define SOY_PR_PrintText2					SOY_PR_PrintText2W
#define SOY_PR_PrintText3					SOY_PR_PrintText3W
#define SOY_PR_PrintOneCard					SOY_PR_PrintOneCardW

#define SOY_PR_GetBinFirmwareVersion		SOY_PR_GetBinFirmwareVersionW
#define SOY_PR_UpdateFirmware				SOY_PR_UpdateFirmwareW

#define SOY_PR_PrintConcaveDigits			SOY_PR_PrintConcaveDigitsW
#define SOY_PR_GetIndentConfig				SOY_PR_GetIndentConfigW
#define SOY_PR_SetIndentConfig				SOY_PR_SetIndentConfigW
#define SOY_PR_UpdateIndentFirmware			SOY_PR_UpdateIndentFirmwareW

#define SOY_PR_SetSecurityMode				SOY_PR_SetSecurityModeW
#define SOY_PR_SetSecurityPassword			SOY_PR_SetSecurityPasswordW

#else

#define SOY_PR_SdkVersion					SOY_PR_SdkVersionA
#define SOY_PR_ExecCommand					SOY_PR_ExecCommandA
#define SOY_PR_GetPrinterConfig				SOY_PR_GetPrinterConfigA
#define SOY_PR_GetPrinterInfo				SOY_PR_GetPrinterInfoA
#define SOY_PR_GetPrinterStatus				SOY_PR_GetPrinterStatusA
#define SOY_PR_GetPrinterWarning			SOY_PR_GetPrinterWarningA
#define SOY_PR_SetPrinterConfig				SOY_PR_SetPrinterConfigA
#define SOY_PR_SetStandbyParameters			SOY_PR_SetStandbyParametersA
#define SOY_PR_WaitPrinterBusy				SOY_PR_WaitPrinterBusyA

#define SOY_PR_ReadTrack					SOY_PR_ReadTrackA
#define SOY_PR_WriteTrack					SOY_PR_WriteTrackA
#define SOY_PR_EncodeTrack					SOY_PR_EncodeTrackA

#define SOY_PR_StartPrinting2				SOY_PR_StartPrinting2A
#define SOY_PR_PrintImage2					SOY_PR_PrintImage2A
#define SOY_PR_PrintText2					SOY_PR_PrintText2A
#define SOY_PR_PrintText3					SOY_PR_PrintText3A
#define SOY_PR_PrintOneCard					SOY_PR_PrintOneCardA

#define SOY_PR_GetBinFirmwareVersion		SOY_PR_GetBinFirmwareVersionA
#define SOY_PR_UpdateFirmware				SOY_PR_UpdateFirmwareA

#define SOY_PR_PrintConcaveDigits			SOY_PR_PrintConcaveDigitsA
#define SOY_PR_GetIndentConfig				SOY_PR_GetIndentConfigA
#define SOY_PR_SetIndentConfig				SOY_PR_SetIndentConfigA
#define SOY_PR_UpdateIndentFirmware			SOY_PR_UpdateIndentFirmwareA

#define SOY_PR_SetSecurityMode				SOY_PR_SetSecurityModeA
#define SOY_PR_SetSecurityPassword			SOY_PR_SetSecurityPasswordA

#endif // !UNICODE




typedef void	 (WINAPI *pfnSOY_PR_SetLogLevel)				(uint32_t dwLogLevel);
typedef uint32_t (WINAPI *pfnSOY_PR_SdkVersionA)				(char* szVersionOutA);
typedef uint32_t (WINAPI *pfnSOY_PR_SdkVersionW)				(WCHAR* szVersionOutW);

typedef uint32_t (WINAPI *pfnSOY_PR_ExecCommandA)				(const char* szPrinterNameA, uint32_t dwCommand);
typedef uint32_t (WINAPI *pfnSOY_PR_ExecCommandW)				(const WCHAR* szPrinterNameW, uint32_t dwCommand);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterConfigA)			(const char* szPrinterNameA, uint32_t dwConfigType, int32_t* lpnConfigValue);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterConfigW)			(const WCHAR* szPrinterNameW, uint32_t dwConfigType, int32_t* lpnConfigValue);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterInfoA)			(const char* szPrinterNameA, int32_t nInfoType, char* szInfoOutA);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterInfoW)			(const WCHAR* szPrinterNameW, int32_t nInfoType, WCHAR* szInfoOutW);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterStatusA)			(const char* szPrinterNameA, uint32_t *lpdwStatus);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterStatusW)			(const WCHAR* szPrinterNameW, uint32_t *lpdwStatus);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterWarningA)			(const char* szPrinterNameA, uint32_t *lpdwStatus);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterWarningW)			(const WCHAR* szPrinterNameW, uint32_t *lpdwStatus);
typedef uint32_t (WINAPI *pfnSOY_PR_SetPrinterConfigA)			(const char* szPrinterNameA, uint32_t dwConfigType, int32_t nConfigValue);
typedef uint32_t (WINAPI *pfnSOY_PR_SetPrinterConfigW)			(const WCHAR* szPrinterNameW, uint32_t dwConfigType, int32_t nConfigValue);
typedef uint32_t (WINAPI *pfnSOY_PR_SetStandbyParametersA)		(const char* szPrinterNameA, uint8_t byOutputBin, uint8_t byStandbyPos, uint32_t dwStandbyTime);
typedef uint32_t (WINAPI *pfnSOY_PR_SetStandbyParametersW)		(const WCHAR* szPrinterNameW, uint8_t byOutputBin, uint8_t byStandbyPos, uint32_t dwStandbyTime);
typedef uint32_t (WINAPI *pfnSOY_PR_WaitPrinterBusyA)			(const char* szPrinterNameA, uint32_t *lpdwStatus);
typedef uint32_t (WINAPI *pfnSOY_PR_WaitPrinterBusyW)			(const WCHAR* szPrinterNameW, uint32_t *lpdwStatus);

typedef uint32_t (WINAPI *pfnSOY_PR_ReadTrackA)					(const char* szPrinterNameA, uint32_t dwMode, char* szTrack1A, char* szTrack2A, char* szTrack3A);
typedef uint32_t (WINAPI *pfnSOY_PR_ReadTrackW)					(const WCHAR* szPrinterNameW, uint32_t dwMode, WCHAR* szTrack1W, WCHAR* szTrack2W, WCHAR* szTrack3W);
typedef uint32_t (WINAPI *pfnSOY_PR_WriteTrackA)				(const char* szPrinterNameA, uint32_t dwMode, const char* szTrack1A, const char* szTrack2A, const char* szTrack3A);
typedef uint32_t (WINAPI *pfnSOY_PR_WriteTrackW)				(const WCHAR* szPrinterNameW, uint32_t dwMode, const WCHAR* szTrack1W, const WCHAR* szTrack2W, const WCHAR* szTrack3W);
typedef uint32_t (WINAPI *pfnSOY_PR_EncodeTrackA)				(const char* szPrinterNameA, uint32_t dwMode, const char* szTrack1A, const char* szTrack2A, const char* szTrack3A, uint32_t dwRetry);
typedef uint32_t (WINAPI *pfnSOY_PR_EncodeTrackW)				(const WCHAR* szPrinterNameW, uint32_t dwMode, const WCHAR* szTrack1W, const WCHAR* szTrack2W, const WCHAR* szTrack3W, uint32_t dwRetry);

typedef uint32_t (WINAPI *pfnSOY_PR_PrintOneCardA)				(const char* szPrinterNameA, PRINT_CARD_PARAM *lpJobPara);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintOneCardW)				(const WCHAR* szPrinterNameW, PRINT_CARD_PARAM *lpJobPara);

typedef uint32_t (WINAPI *pfnSOY_PR_StartPrinting2W)			(const WCHAR* szPrinterNameW, uint8_t* lpDocPropIn, HDC* lphPrinterDC);
typedef uint32_t (WINAPI *pfnSOY_PR_StartPrinting2A)			(const char* szPrinterNameA, uint8_t* lpDocPropIn, HDC* lphPrinterDC);
typedef uint32_t (WINAPI *pfnSOY_PR_StartPage2)					(HDC hPrinterDC);
typedef uint32_t (WINAPI *pfnSOY_PR_EndPage2)					(HDC hPrinterDC);
typedef uint32_t (WINAPI *pfnSOY_PR_EndPrinting2)				(HDC hPrinterDC, BOOL bCancelJob);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintImage2W)				(HDC hPrinterDC, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, const WCHAR* szImageUrlW);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintImage2A)				(HDC hPrinterDC, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, const char* szImageUrlA);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintText2W)				(HDC hPrinterDC, int32_t nX, int32_t nY, const WCHAR* szTextW, const WCHAR* szFontNameW, int32_t nFontSize, int32_t nFontWeight, uint8_t byFontAttribute, uint8_t byCharSet, uint32_t dwTextColor, BOOL bTransparentBack);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintText2A)				(HDC hPrinterDC, int32_t nX, int32_t nY, const char* szTextA, const char* szFontNameA, int32_t nFontSize, int32_t nFontWeight, uint8_t byFontAttribute, uint8_t byCharSet, uint32_t dwTextColor, BOOL bTransparentBack);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintText3W)				(HDC hPrinterDC, int32_t nX, int32_t nY, const WCHAR* szTextW, const WCHAR* szFontNameW, int32_t nFontSize, int32_t nFontWeight, uint8_t byFontAttribute, uint8_t byCharSet, uint32_t dwTextColor, BOOL bTransparentBack, int32_t nExtraSpace);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintText3A)				(HDC hPrinterDC, int32_t nX, int32_t nY, const char* szTextA, const char* szFontNameA, int32_t nFontSize, int32_t nFontWeight, uint8_t byFontAttribute, uint8_t byCharSet, uint32_t dwTextColor, BOOL bTransparentBack, int32_t nExtraSpace);

typedef uint32_t (WINAPI *pfnSOY_PR_SetSecurityModeA)			(const char* szPrinterNameA, const char* szCurrentPasswdA, int32_t nSecurityMode);
typedef uint32_t (WINAPI *pfnSOY_PR_SetSecurityModeW)			(const WCHAR* szPrinterNameW, const WCHAR* szCurrentPasswdW, int32_t nSecurityMode);
typedef uint32_t (WINAPI *pfnSOY_PR_SetSecurityPasswordA)		(const char* szPrinterNameA, const char* szCurrentPasswdA, const char* szNewPasswdA);
typedef uint32_t (WINAPI *pfnSOY_PR_SetSecurityPasswordW)		(const WCHAR* szPrinterNameW, const WCHAR* szCurrentPasswdW, const WCHAR* szNewPasswdW);

typedef uint32_t (WINAPI *pfnSOY_PR_GetBinFirmwareVersionA)		(const char* szBinUrlA, char* szBinVerOutA);
typedef uint32_t (WINAPI *pfnSOY_PR_GetBinFirmwareVersionW)		(const WCHAR* szBinUrlW, WCHAR* szBinVerOutW);
typedef uint32_t (WINAPI *pfnSOY_PR_UpdateFirmwareA)			(const char* szPrinterNameA, const char* szBinUrlA);
typedef uint32_t (WINAPI *pfnSOY_PR_UpdateFirmwareW)			(const WCHAR* szPrinterNameW, const WCHAR* szBinUrlW);

typedef uint32_t (WINAPI *pfnSOY_PR_PrintConcaveDigitsA)		(const char* szPrinterNameA, uint32_t dwReserved, uint32_t dwDistance, uint32_t dwSpace, const char* szDigitsA);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintConcaveDigitsW)		(const WCHAR* szPrinterNameW, uint32_t dwReserved, uint32_t dwDistance, uint32_t dwSpace, const WCHAR* szDigitsW);

typedef uint32_t (WINAPI *pfnSOY_PR_GetIndentConfigA)			(const char* szPrinterNameA, uint32_t dwConfigType, void* lpConfigStruct);
typedef uint32_t (WINAPI *pfnSOY_PR_GetIndentConfigW)			(const WCHAR* szPrinterNameW, uint32_t dwConfigType, void* lpConfigStruct);
typedef uint32_t (WINAPI *pfnSOY_PR_SetIndentConfigA)			(const char* szPrinterNameA, uint32_t dwConfigType, void* lpConfigStruct);
typedef uint32_t (WINAPI *pfnSOY_PR_SetIndentConfigW)			(const WCHAR* szPrinterNameW, uint32_t dwConfigType, void* lpConfigStruct);
typedef uint32_t (WINAPI *pfnSOY_PR_UpdateIndentFirmwareA)		(const char* szPrinterNameA, const char* szBinUrlA);
typedef uint32_t (WINAPI *pfnSOY_PR_UpdateIndentFirmwareW)		(const WCHAR* szPrinterNameW, const WCHAR* szBinUrlW);

//-----------------------------------------------
#if defined(_WIN32)||defined(_WIN64)
uint32_t WINAPI SOY_PR_ModifyDocumentPropertiesA(const char* szPrinterNameA, HDC hPrinterDC, uint8_t* lpDocPropIn, uint8_t* lpDevModeIn);
uint32_t WINAPI SOY_PR_ModifyDocumentPropertiesW(const WCHAR* szPrinterNameW, HDC hPrinterDC, uint8_t* lpDocPropIn, uint8_t* lpDevModeIn);

uint32_t WINAPI SOY_PR_GetPrinterSettingA		(const char* szPrinterNameA, uint8_t* lpDocPropOut);
uint32_t WINAPI SOY_PR_GetPrinterSettingW		(const WCHAR* szPrinterNameW, uint8_t* lpDocPropOut);
uint32_t WINAPI SOY_PR_SetPrinterSettingA		(const char* szPrinterNameA, uint8_t* lpDocPropIn);
uint32_t WINAPI SOY_PR_SetPrinterSettingW		(const WCHAR* szPrinterNameW, uint8_t* lpDocPropIn);

uint32_t WINAPI SOY_PR_GetPrintAreaA			(const char* szPrinterNameA, int32_t nOrientation, int32_t nSide, int32_t nPanel, int32_t* lpnAreaNum, int32_t* lpcbBytes, char* lpAreaNameAll, char* lpAreaNameDefault);
uint32_t WINAPI SOY_PR_GetPrintAreaW			(const WCHAR* szPrinterNameW, int32_t nOrientation, int32_t nSide, int32_t nPanel, int32_t* lpnAreaNum, int32_t* lpcbBytes, WCHAR* lpAreaNameAll, WCHAR* lpAreaNameDefault);
uint32_t WINAPI SOY_PR_SetPrintAreaA			(const char* szPrinterNameA, int32_t nOrientation, int32_t nSide, int32_t nPanel, char* szAreaNameDefault);
uint32_t WINAPI SOY_PR_SetPrintAreaW			(const WCHAR* szPrinterNameW, int32_t nOrientation, int32_t nSide, int32_t nPanel, WCHAR* szAreaNameDefault);
uint32_t WINAPI SOY_PR_DeleteAllJobsA			(const char* szPrinterNameA);
uint32_t WINAPI SOY_PR_DeleteAllJobsW			(const WCHAR* szPrinterNameW);

uint32_t WINAPI SOY_PR_PrintBlackImage2W		(HDC hPrinterDC, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, const WCHAR* szImageUrlW, float fThreshold);
uint32_t WINAPI SOY_PR_PrintBlackImage2A		(HDC hPrinterDC, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, const char* szImageUrlA, float fThreshold);

#if defined(UNICODE)||defined(_UNICODE)

#define SOY_PR_ModifyDocumentProperties		SOY_PR_ModifyDocumentPropertiesW
#define SOY_PR_GetPrinterSetting			SOY_PR_GetPrinterSettingW
#define SOY_PR_SetPrinterSetting			SOY_PR_SetPrinterSettingW

#define SOY_PR_GetPrintArea					SOY_PR_GetPrintAreaW
#define SOY_PR_SetPrintArea					SOY_PR_SetPrintAreaW
#define SOY_PR_DeleteAllJobs				SOY_PR_DeleteAllJobsW

#define SOY_PR_PrintBlackImage2				SOY_PR_PrintBlackImage2W

#else

#define SOY_PR_ModifyDocumentProperties		SOY_PR_ModifyDocumentPropertiesA
#define SOY_PR_GetPrinterSetting			SOY_PR_GetPrinterSettingA
#define SOY_PR_SetPrinterSetting			SOY_PR_SetPrinterSettingA

#define SOY_PR_GetPrintArea					SOY_PR_GetPrintAreaA
#define SOY_PR_SetPrintArea					SOY_PR_SetPrintAreaA
#define SOY_PR_DeleteAllJobs				SOY_PR_DeleteAllJobsA

#define SOY_PR_PrintBlackImage2				SOY_PR_PrintBlackImage2A

#endif//#if defined(UNICODE)||defined(_UNICODE)

typedef uint32_t (WINAPI *pfnSOY_PR_ModifyDocumentPropertiesA)	(const char* szPrinterNameA, HDC hPrinterDC, uint8_t* lpDocPropIn, uint8_t* lpDevModeIn);
typedef uint32_t (WINAPI *pfnSOY_PR_ModifyDocumentPropertiesW)	(const WCHAR* szPrinterNameW, HDC hPrinterDC, uint8_t* lpDocPropIn, uint8_t* lpDevModeIn);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterSettingA)			(const char* szPrinterNameA, uint8_t* lpDocPropOut);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrinterSettingW)			(const WCHAR* szPrinterNameW, uint8_t* lpDocPropOut);
typedef uint32_t (WINAPI *pfnSOY_PR_SetPrinterSettingA)			(const char* szPrinterNameA, uint8_t* lpDocPropIn);
typedef uint32_t (WINAPI *pfnSOY_PR_SetPrinterSettingW)			(const WCHAR* szPrinterNameW, uint8_t* lpDocPropIn);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrintAreaA)				(const char* szPrinterNameA, int32_t nOrientation, int32_t nSide, int32_t nPanel, int32_t* lpnAreaNum, int32_t* lpcbBytes, char* lpAreaNameAll, char* lpAreaNameDefault);
typedef uint32_t (WINAPI *pfnSOY_PR_GetPrintAreaW)				(const WCHAR* szPrinterNameW, int32_t nOrientation, int32_t nSide, int32_t nPanel, int32_t* lpnAreaNum, int32_t* lpcbBytes, WCHAR* lpAreaNameAll, WCHAR* lpAreaNameDefault);
typedef uint32_t (WINAPI *pfnSOY_PR_SetPrintAreaA)				(const char* szPrinterNameA, int32_t nOrientation, int32_t nSide, int32_t nPanel, char* szAreaNameDefault);
typedef uint32_t (WINAPI *pfnSOY_PR_SetPrintAreaW)				(const WCHAR* szPrinterNameW, int32_t nOrientation, int32_t nSide, int32_t nPanel, WCHAR* szAreaNameDefault);
typedef uint32_t (WINAPI *pfnSOY_PR_DeleteAllJobsA)				(const char* szPrinterNameA);
typedef uint32_t (WINAPI *pfnSOY_PR_DeleteAllJobsW)				(const WCHAR* szPrinterNameW);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintBlackImage2W)			(HDC hPrinterDC, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, const WCHAR* szImageUrlW, float fThreshold);
typedef uint32_t (WINAPI *pfnSOY_PR_PrintBlackImage2A)			(HDC hPrinterDC, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, const char* szImageUrlA, float fThreshold);

#endif//#if defined(_WIN32)||defined(_WIN64)
//-----------------------------------------------

#ifdef __cplusplus
}
#endif



#endif//__SOYPRAPI_H__
