
#include <avr/io.h>
#include <stdbool.h>

uint8_t*	dmaUpdateAutoBuffer				( uint8_t channel , uint16_t blockSize );	

void		dmaSetBuffer					( uint8_t channel );							

void		dmaReset						( void );									

void		dmaSetBlockSize					( uint8_t channel , uint16_t blockSize );	

void		dmaUseChannel					( uint8_t channel , uint8_t use );			

void		dmaSetSource					( uint8_t channel , void* source );			

void		dmaSetSource					( uint8_t channel , void* source , SrcDirection_enum direction , SrcReload_enum mode ); 

void		dmaSetSource					( uint8_t channel , void* source1 , void* source2 , SrcDirection_enum direction , SrcReload_enum mode ); 


void		dmaSetSourceDirection			( uint8_t channel , SrcDirection_enum direction );	

void		dmaSetSourceReload				( uint8_t channel , SrcReload_enum mode );			

void		*dmaSetDestination				( uint8_t channel , void* destination );				

void		*dmaSetDestination				( uint8_t channel , void* destination, DestDirection_enum direction, DestReload_enum mode);						

void		dmaSetDestination				( uint8_t channel , void* destination1, void* destination2, DestDirection_enum direction, DestReload_enum mode); 

void		dmaSetDestDirection				( uint8_t channel , DestDirection_enum direction);		

void		dmaSetDestReload				( uint8_t channel , DestReload_enum mode );				

void		dmaSetBurstLength				( uint8_t channel , BurstLength_enum burstLength );		

void		dmaEnable						( uint8_t channel );			

void		dmaDisable						( uint8_t channel );			

bool		dmaIsTransactionComplete		( uint8_t channel );			

bool		dmaIsBlockTransferBusy			( uint8_t channel );			

void		dmaTrigger						( uint8_t channel );			

bool		dmaGetFreeChannel				( uint8_t channel );		

bool		dmaCopyMemory					( void* source , void* destination , uint16_t size );					

bool		dmaCopyMemory					( uint8_t channel , void* source , void* destination , uint16_t size );	

bool		dmaFillMemory					( void* destination , uint16_t size , uint8_t byte );					

void		dmaSetRepeatCount				( uint8_t channel , uint8_t repeatCount );		

void		dmaUseSingleShot				( uint8_t channel , bool use );					

void		dmaSetTriggerSource				( uint8_t channel , uint8_t triggerSource );		

