////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file Watchdog.hpp
///
/// Watchdog class provides a COP watchdog hardware module. 
/// The watchdog serviced (kicked) within the refreshing period. 
/// If the service does not occur, the watchdog issues a system reset. This class contains functions to 
/// configure the registers for the System Integration Module(SIM).
///
/// @if REVISION_HISTORY_INCLUDED
/// @par Edit History
/// - gsb 30-Dec-2015 Original implementation
/// - gsb 08-Jan-2016 Added constant value to magic numbers
/// - zjrybak 04-Feb-2016 Added CpfBsp namespace
/// - zjrybak 29-Feb-2016 Added additional watchdog timeouts and windowed mode option
/// - zjrybak 14-Mar-2016 Removed DisableWatchdog and moved disable into the Init
/// @endif
///
/// @ingroup CpfBsp
///
/// @par Copyright (c) 2016 Rockwell Automation Technologies, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WATCHDOG_HPP__
#define __WATCHDOG_HPP__

// SYSTEM INCLUDES
// (none)

// C PROJECT INCLUDES
#include "Registers.h"

// C++ PROJECT INCLUDES
// (none)

namespace CpfBsp
{

class Watchdog
{
    public:
        
        //**************************************************************************************************************
        // Public definitions
        //**************************************************************************************************************
    
        // Watchdog timeout values
        enum WatchdogTimeout
        {
            TIMEOUT_DISABLED,
            TIMEOUT_128_US,
            TIMEOUT_341_US,
            TIMEOUT_1024_US,
            TIMEOUT_2730_US
        };
        
        //**************************************************************************************************************
        // Public methods
        //**************************************************************************************************************              
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// FUNCTION NAME: Watchdog::Init
        ///
        /// Initialize watchdog HW module.
        ///
        /// @par Full Description
        /// Static function. Initialize the watchdog. The COP register can only be written to once so this function 
        /// can only be called once.
        ///
        /// @param  timeout         the timeout value for the watchdog
        /// @param  windowedMode    configure watchdog for windowed mode. Window mode is only available for timeout
        ///                         values of 341us, 1024us, and 2730us. The function will assert in debug if attempted
        ///                         with 128us.
        ///         
        /// @return none
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void Init(WatchdogTimeout timeout, bool windowedMode);              
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// FUNCTION NAME: Watchdog::KickWatchdog
        ///
        /// Kicks watchdog HW module
        ///
        /// @param  none
        ///         
        /// @return none
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void KickWatchdog(void);              
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// FUNCTION NAME: Watchdog::IsWatchdogReset
        ///
        /// Checks whether there was a watchdog reset last time.
        ///
        /// @param  none
        ///         
        /// @return bool – TRUE:  the system was watchdog reset
        ///                FALSE: the system was not watchdog reset
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static bool IsWatchdogReset(void); 
        
    private:
      
        //**************************************************************************************************************
        // Private definitions
        //**************************************************************************************************************
        
        // Watchdog configuration constants
        // OFS0 Register Mapping
                                                                 // WDT Stop Control bit 30
        static const uint32_t WDTSTPCTL_CONT = 0;                // 0: Counting continues
        static const uint32_t WDTSTPCTL_STOP = 0x40000000U;      // 1 : Counting stops when entering Sleep mode.
 
                                                                 //  WDT Reset Interrupt Request bit 28
                                                                 // Select WDT Behavior Select :
        static const uint32_t WDTRSTIRQS_NMI = 0;                // 0 : NMI.
        static const uint32_t WDTRSTIRQS_RESET = 0x10000000U;    // 1 : Reset

                                                                 // WDT Window Start Position bits 27 and 26
                                                                 // Select
                                                                 // b27 b26
        static const uint32_t WDTRPSS_25  = 0;                   // 0 0 : 25 %
        static const uint32_t WDTRPSS_50  = 0x4000000U;          // 0 1 : 50 %
        static const uint32_t WDTRPSS_75  = 0X8000000U;          // 1 0 : 75 %
        static const uint32_t WDTRPSS_100 = 0XC000000U;          // 1 1 : 100 % (No window start position setting).
                                                                 
                                                                 // WDT Window End Position bits 25 and 24
                                                                 // Select
                                                                 // b25 b24
        static const uint32_t WDTRPES_75 = 0;                    // 0 0 : 75 %
        static const uint32_t WDTRPES_50 = 0x1000000U;           // 0 1 : 50 %
        static const uint32_t WDTRPES_25 = 0X2000000U;           // 1 0 : 25 %
        static const uint32_t WDTRPES_00 = 0X3000000U;           // 1 1 : 0 % (No window end position setting).

                                                                 // WDT Clock Frequency bits 23 through 20
                                                                 // Division Ratio Select b23 b20
        static const uint32_t WDTCKS_DIV_BY_4    = 0x100000U;    // 0 0 0 1: PCLKB divided by 4
        static const uint32_t WDTCKS_DIV_BY_64   = 0x400000U;    // 0 1 0 0 : PCLKB divided by 64
        static const uint32_t WDTCKS_DIV_BY_128  = 0xF00000U;    // 1 1 1 1 : PCLKB divided by 128
        static const uint32_t WDTCKS_DIV_BY_512  = 0x600000U;    // 0 1 1 0 : PCLKB divided by 512
        static const uint32_t WDTCKS_DIV_BY_2048 = 0x700000U;    // 0 1 1 1 : PCLKB divided by 2048
        static const uint32_t WDTCKS_DIV_BY_8192 = 0x800000U;    // 1 0 0 0 : PCLKB divided by 8192
                                                                 // Other settings are prohibited
                                                                 
                                                                 // WDT Timeout Period Select b19 b18
        static const uint32_t WDTTOPS_CYCLES_1024  = 0;          // 0 0 : 1024 cycles(03FFh)
        static const uint32_t WDTTOPS_CYCLES_4096  = 0x40000U;   // 0 1 : 4096 cycles(0FFFh)
        static const uint32_t WDTTOPS_CYCLES_8192  = 0x80000U;   // 1 0 : 8192 cycles(1FFFh)
        static const uint32_t WDTTOPS_CYCLES_16384 = 0xC0000U;   // 1 1 : 16384 cycles(3FFFh
                                                                 
                                                                 // WDT Start Mode Selct
        static const uint32_t WDTSTRT_AUTO = 0;                  // 0 : WDT is automatically activated after a reset(autostart mode)
        static const uint32_t WDTSTRT_STOP = 0x20000U;           // 1 : WDT is stopped after a reset(register - start mode).                                                                
                                                                
                                                                 // IWDT Stop Control bit 14
        static const uint32_t IWDTSTPCTL_CONT = 0;               // 0: Counting continues
        static const uint32_t IWDTSTPCTL_STOP = 0x4000U;         // 1 : Counting stops when entering Sleep mode.

                                                                 // IWDT Reset Interrupt Request bit 12
                                                                 // Select WDT Behavior Select :
        static const uint32_t IWDTRSTIRQS_NMI   = 0;             // 0 : NMI or Interrupt Request.
        static const uint32_t IWDTRSTIRQS_RESET = 0x1000U;       // 1 : Reset

                                                                 // IWDT Window Start Position bits 11 and 10
                                                                 // Select
                                                                 // b11 b10
        static const uint32_t IWDTRPSS_25  = 0;                  // 0 0: 25 %
        static const uint32_t IWDTRPSS_50  = 0x400U;             // 0 1 : 50 %
        static const uint32_t IWDTRPSS_75  = 0X800U;             // 1 0 : 75 %
        static const uint32_t IWDTRPSS_100 = 0XC00U;             // 1 1 : 100 % (No window start position setting).

                                                                 // IWDT Window End Position bits 9 and 8
                                                                 // Select
                                                                 // b09 b08
        static const uint32_t IWDTRPES_25  = 0;                  // 0 0: 25 %
        static const uint32_t IWDTRPES_50  = 0x100U;             // 0 1 : 50 %
        static const uint32_t IWDTRPES_75  = 0X200U;             // 1 0 : 75 %
        static const uint32_t IWDTRPES_100 = 0X300U;             // 1 1 : 100 % (No window end position setting).

                                                                 // IWDT Clock Frequency bits 7 through 4
                                                                 // Division Ratio Select b23 b20
        static const uint32_t IWDTCKS_DIV_BY_1   = 0x0;          // 0 0 0 0 : PCLKB divided by 1
        static const uint32_t IWDTCKS_DIV_BY_16  = 0x20U;        // 0 0 1 0 : PCLKB divided by 16
        static const uint32_t IWDTCKS_DIV_BY_32  = 0x30U;        // 0 0 1 1 : PCLKB divided by 32
        static const uint32_t IWDTCKS_DIV_BY_64  = 0x40U;        // 0 1 0 0 : PCLKB divided by 64
        static const uint32_t IWDTCKS_DIV_BY_128 = 0xF0U;        // 1 1 1 1 : PCLKB divided by 128
        static const uint32_t IWDTCKS_DIV_BY_256 = 0x50U;        // 0 1 0 1 : PCLKB divided by 256

                                                                 // IWDT Timeout Period Select b19 b18
        static const uint32_t IWDTTOPS_CYCLES_128  = 0;          // 0 0 : 128 cycles
        static const uint32_t IWDTTOPS_CYCLES_512  = 0x4U;       // 0 1 : 512 cycles
        static const uint32_t IWDTTOPS_CYCLES_1024 = 0x8U;       // 1 0 : 1024 cycles
        static const uint32_t IWDTTOPS_CYCLES_2048 = 0xCU;       // 1 1 : 2048 cycles

                                                                 // IWDT Start Mode Selct
        static const uint32_t IWDTSTRT_AUTO = 0;                 // 0 : IWDT is automatically activated after a reset(autostart mode)
        static const uint32_t IWDTSTRT_STOP = 0x2U;              // 1 : IWDT is disabled.      

        // Debug configuration for OFS0
        //    IWDTSTRT_STOP |  WDTSTRT_STOP | WDTTOPS_CYCLES_1024 | WDTCKS_DIV_BY_4 | WDTRPES_00 |  WDTRPSS_100 | WDTRSTIRQS_NMI | WDTSTPCTL_STOP
        // Normal Product Operation
        //    IWDTSTRT_AUTO | IWDTTOPS_CYCLES_1024 | WDTCKS_DIV_BY_512 | IWDTRPES_100 | IWDTRPSS_100 | IWDTSTPCTL_CONT  | IWDTRSTIRQS_RESET | WDTSTRT_STOP

        static const uint32_t REG_ADDRESS_WDT  = 0x40044200;
        static const uint32_t REG_ADDRESS_IWDT = 0x40044400;

        
        typedef struct
        {
            volatile uint8_t  ubRefresh;                  // WDT refresh register        0x00
            volatile uint8_t  ubUnusedRefresh;            // WDT unused 8 bits           0x01
            volatile uint16_t uwControl;                  // WDT control register        0x02
            volatile uint16_t uwStatus;                   // WDT status register         0x04
            volatile uint8_t  ubResetControl;             // WDT Reset Control Register  0x06
            volatile uint8_t  ubUnusedResetControl;       // WDT unused 8 bits           0x07
            volatile uint8_t  ubCountStopControl;         // WDT Control Stop Register   0x08
            // For alignment on a 4 byte boundry
            volatile uint8_t  ubCountStopControlUnused_0; // WDT Unused                  0x09
            volatile uint8_t  ubCountStopControlUnused_1; // WDT Unused                  0x0A
            volatile uint8_t  ubcountStopControlUnused_2; // WDT Unused                  0x0B
        }
        RegMapWdt;


        typedef struct
        {
            volatile uint8_t  ubRefresh;                  // IWDT refresh register        0x00
            // For alignment on a 4 byte boundry
            volatile uint8_t  ubUnusedRefresh_0;          // IWDT unused 8 bits           0x01
            volatile uint8_t  ubUnusedRefresh_1;          // IWDT unused 8 bits           0x02
            volatile uint8_t  ubUnusedRefresh_2;          // IWDT unused 8 bits           0x02
            volatile uint16_t uwStatus;                   // IWDT status register         0x04
            // For alignment on a 4 byte boundry
            volatile uint8_t  ubUnusedStatus_0;           // IWDT unused 8 bits           0x06
            volatile uint8_t  ubUnusedStatus_1;           // IWDT unused 8 bits           0x07
        }
        RegMapIWdt;


        // WDT Control Register Mappings
                                                          // WDT Timeout Period Select b1 b0
        static const uint32_t WDTCR_CYCLES_1024  = 0;     // 0 0: 1024 cycles(03FFh)
        static const uint32_t WDTCR_CYCLES_4096  = 0x1;   // 0 1 : 4096 cycles(0FFFh)
        static const uint32_t WDTCR_CYCLES_8192  = 0x2;   // 1 0 : 8192 cycles(1FFFh)
        static const uint32_t WDTCR_CYCLES_16384 = 0x3;   // 1 1 : 16384 cycles(3FFFh

                                                          // Division Ratio Select b23 b20
        static const uint32_t WDTCR_DIV_BY_4    = 0x10;   // 0 0 0 1: PCLKB divided by 4
        static const uint32_t WDTCR_DIV_BY_64   = 0x40;   // 0 1 0 0 : PCLKB divided by 64
        static const uint32_t WDTCR_DIV_BY_128  = 0xF0;   // 1 1 1 1 : PCLKB divided by 128
        static const uint32_t WDTCR_DIV_BY_512  = 0x60;   // 0 1 1 0 : PCLKB divided by 512
        static const uint32_t WDTCR_DIV_BY_2048 = 0x70    // 0 1 1 1 : PCLKB divided by 2048
        static const uint32_t WDTCR_DIV_BY_8192 = 0x80;   // 1 0 0 0 : PCLKB divided by 8192
                                                          // Other settings are prohibited
                                                        
                                                          // WDT Window End Position bits 9 and 8
                                                          // Select
                                                          // b09 b08
        static const uint32_t WDTCR_WDW_END_75 = 0;       // 0 0 : 75 %
        static const uint32_t WDTCR_WDW_END_50 = 0x100;   // 0 1 : 50 %
        static const uint32_t WDTCR_WDW_END_25 = 0X200;   // 1 0 : 25 %
        static const uint32_t WDTCR_WDW_END_00 = 0X300;   // 1 1 : 100 % (No window end position setting).

                                                          // WDT Window Start Position bits 27 and 26
                                                          // Select
                                                          // b27 b26
        static const uint32_t WDTCR_WDW_STR_25  = 0;      // 0 0: 25 %
        static const uint32_t WDTCR_WDW_STR_50  = 0x1000; // 0 1 : 50 %
        static const uint32_t WDTCR_WDW_STR_75  = 0X2000; // 1 0 : 75 %
        static const uint32_t WDTCR_WDW_STR_100 = 0X3000; // 1 1 : 100 % (No window start position setting).

        // Nominal setting should be approximately 10 ms for watchdog time period for a system clock of 32 mega herz with a refresh enabled anytime during the watchdog period.
        static const uint16_t NOMINAL_WDT_SETTING = WDTCR_CYCLES_16384 | WDTCR_DIV_BY_4 | WDTCR_WDW_END_00 | WDTCR_WDW_STR_100;

        // Refreshing the watchdog involves first writing a 0 to refresh register and then an 0xff to the same register
        static const uint8_t  FIRST_REFRESH_BYTE  = 0;
        static const uint8_t  SECOND_REFRESH_BYTE = 0Xff;
        
        // Clear error status info
        static const uint16_t WDT_CLEAR_STATUS = 0;

        // Status bit for having count stopped in sleep mode.
        static const uint16_t WDT_STOP_COUNT_IN_SLEEP_MODES = 0X80;

        // Reset Status bit mask
        static const uint16_t WDT_STATUS_ERR_MASK = 0xC000;

        //**************************************************************************************************************
        // Register pointer
        static RegMapWdt *  const m_pRegMapWdt;    // Pointer to the WDT Registers
        static RegMapIWdt * const m_pRegMapIWdt;   // Pointer to the IWDT Registers 

};


#endif //__WATCHDOG_HPP__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of file.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





