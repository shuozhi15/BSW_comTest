/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
#ifndef NOT_READY_FOR_TESTING_OR_DEPLOYMENT
#error The content of this file is a template which provides empty stubs. The content of this file must be completed by the integrator accordingly to project specific requirements
#else
#warning The content of this file is a template which provides empty stubs. The content of this file must be completed by the integrator accordingly to project specific requirements
#endif /* NOT_READY_FOR_TESTING_OR_DEPLOYMENT */



#ifndef RBA_TRCV_TIME_H_
# define RBA_TRCV_TIME_H_

#include "ComStack_Types.h"
/* additional files can be includes which contains timing functions*/
//#error "Check if inittimer.h is required and available  and delete this line"
//#include "inittimer.h"
// if os-timers are used, include os.h. Otherwise it is not used
#include "os.h"


// for AR4.2 the RH850 MCAL-SPI is not according agreement to provide Spi_RbSetupEB to link to an AR4.2 compatible Spi_SetupEB function
// Especially for this SPI next line should be used if an SPI-Trcv is configured
// #define Spi_RbSetupEB Spi_SetupEB


// For ASR3 SPI
//#define RBA_TRCV_SPI_TRANSMIT(seq) Spi_SyncTransmit(seq)
// For ASR4 ESA-SPI
#define RBA_TRCV_SPI_TRANSMIT(seq) Spi_RbTransmit(seq)
// default value
//#define RBA_TRCV_SPI_TRANSMIT(seq) Spi_AsyncTransmit(seq)


// Typedef of time-variable of type rba_Trcv_Cfg_Tick_to inside of the rba_Trcv component.
// This type is used as return value of function RBA_TRCV_GETTIME(p) and is used to compare and store time.
// Currently only uint32 is tested. Timings of 1us to 1000us are needed in component
// Usage of uint32 is recommended as the time of a Trcv-statechange is stored internally. Before new statechange is
// called a minimum state-change time is waited. At 16-Bit time a wrap around could lead to unneccessary wait times
// possible interrupts should also be considered
//#error "Check if typedef fints to your project.  And delete this line"
//typedef TickType  rba_Trcv_Cfg_Tick_to;
typedef uint32  rba_Trcv_Cfg_Tick_to;


// Value to calculate from tick time to micro seconds to adjusted timer-ticks
// To reduce rounding jitter a smaller hardware ticktime than 1us can be adjusted in hardware-timer-ticks and this
// factor can be used to have micro seconds again.   (E.g. a HW-timer of 100ns needs a RBA_TRCV_CFG_TIME_FACTOR = 10)
//#error "Check time-factor which provides the RBA_TRCV_GETTIME macro, adjust this value and delete this line"
#define RBA_TRCV_CFG_TIME_FACTOR 40u

// Additonal Timeoutvalue. It is a loop-counter how often the result of RBA_TRCV_GETTIME can be same until the loop is left and a DET is called.
// This define is used in function rba_Trcv_WaitUs.
// The intention of this value is to have a DET call and an exit of the loop inside of rba_Trcv_WaitUs if timer does not work (e.g. if not configured correctly at integration time)
// This value is not used to measure a time. No exact value can be suggested.
// The given value (1000) in this template is based on previous experiences in projects.
// The Timeoutvalue depends on the processor speed, the execution time of rba_Trcv_GetTime, compiler options, ....
// The value can be evaluated by calling the function rba_Trcv_WaitUs with a huge value of TimePeriod_o. If so, timeout
// will expire and the time until the return of function can be measured.
#define RBA_TRCV_TIMEMEASUREMENT_TIMEOUT 10uL

// The integrator can add a function where Timer are initialized
// this function is be called in rba_Trcv_Init
//#error "Check if rba_Trcv_TimeInit is required. If yes, update next line. If no, leave next line. Afterwards delete this line"
#define rba_Trcv_TimeInit() do{}while(0)

// The integrator should add a function to get time value (from a free running counter).
// !!! The counter shall be a freerunning counter which shall not be stoppt at any time !!!
// Some Timings of trcv needs a resolution of 1micro seconds.
// !!! Please be sure, that at this timer no timing for FlexRay Snychron OS is derived which manipulates its timer !!!
// Resolution see hints to rba_Trcv_Cfg_Tick_to and  RBA_TRCV_CFG_TIME_FACTOR
// The value is used to store some timeout times and some long-value times as state-change times
// The returned value is used in function rba_Trcv_TimeElapsedUs to evaluate micro second timer values
// Possible other implementations could be:
//#error "Check and implement RBA_TRCV_GETTIME macro. Next lines are only examples. Integrator is responible for correctes and warningfree of code. Afterwards delete this line"
/*
#define RBA_TRCV_GETTIME(p)  \
do{ \
    rba_Trcv_Cfg_Tick_to retVal; \
    retVal = MCU_RB_TIM0TICKS_TO_US(Mcu_Rb_GetSysTimePart(TIM0)); \
    *(p) = retVal; \
}while(0)
*/
#define RBA_TRCV_GETTIME(p) do{((void)GetCounterValue((CounterType)&(Os_const_counters[0]),(TickRefType)(p)));}while(0)
#endif //R BA_TRCV_TIME_H_
