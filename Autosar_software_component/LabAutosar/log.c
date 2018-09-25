/**************************************************************************
 *
 * Logging module
 *
 * Platform: MB467B
 *
 * Author: Usman Ashraf
 *
 **************************************************************************/



//own header
#include "log.h"

//file static variables
#ifdef LOG_ON
static t_usart LOG__port;
static t_ringbuffer LOG__buffer;
#endif

//Open log with assigned COM port
int LOG_init(t_usartPort port, uint8_t rxBuffersize)
{
#ifdef LOG_ON
	RB_init(&LOG__buffer,rxBuffersize,FALSE,0,0); //No end of protocol characters
	return UART_init(&LOG__port, port, 115200, _8, NONE, _1, &LOG__buffer);
#else
	return 1;
#endif

} //LOG_init

//Write log message
void LOG_write(const char *description, const int value, const char format)
{
#ifdef LOG_ON
  	int i,len;
  	char val[8];
   
  	len = strlen(description);
	
	//Write description
  	for (i=0; i<len; i++)
	{
    	UART_write(&LOG__port,description[i]);    // send it out
  	}

	//Deliminator
	UART_write(&LOG__port, ':');
	UART_write(&LOG__port, ' ');

  	
  	//Transform value into string
  	switch (format)
  	{
	  	case 'x' : //fallthrough
  		case 'X' : sprintf(val,"%x",value); break;
	  	case 'd' :
	  	case 'D' : sprintf(val,"%d",value); break; 
	  	case 'c' :
	  	case 'C' : sprintf(val,"%c",value); break; 

	  	default  : sprintf(val,"%d",value); 
	}
	
	//Write the value
	len =strlen(val);
  	for (i=0; i<len; i++)
	{
    	UART_write(&LOG__port,val[i]);    // send it out
  	}

	//Newline
	UART_write(&LOG__port, 10);
	UART_write(&LOG__port, 13);
#endif
} //LOG_write


//Write log message
void LOG_writeArray(const char *description, const char *value, const char format)
{
#ifdef LOG_ON
  	int i,j,len,len2;
  	char val[8];
   
  	len = strlen(description);
	
	//Write description
  	for (i=0; i<len; i++)
	{
    	UART_write(&LOG__port,description[i]);    // send it out
  	}

	//Deliminator
	UART_write(&LOG__port, ':');
	UART_write(&LOG__port, ' ');

 	len = strlen(value);
	//Write char array
  	for (i=0; i<len; i++)
	{
	
	  	//Transform value into string
  		switch (format)
	  	{
		  	case 'x' : //fallthrough
  			case 'X' : sprintf(val,"%x",value[i]); break;
	  		case 'd' :
		  	case 'D' : sprintf(val,"%d",value[i]); break; 
	  		case 'c' :
		  	case 'C' : sprintf(val,"%c",value[i]); break; 
	  	
		  	default  : sprintf(val,"%d",value); 
		}
	
		//Write the value
		len2 =strlen(val);
  		for (j=0; j<len2; j++)
		{
	    	UART_write(&LOG__port,val[j]);    // send it out
  		}
  		
  			UART_write(&LOG__port,' ');       // space between array elements
	}
	//Newline
	UART_write(&LOG__port, 10);
	UART_write(&LOG__port, 13);
#endif
} //LOG_write


void LOG_comment(const char *description)
{
#ifdef LOG_ON
  	int i,len;
   
  	len = strlen(description);
	
	//Write description
  	for (i=0; i<len; i++)
	{
    	UART_write(&LOG__port,description[i]);    // send it out
  	}

	//Newline
	UART_write(&LOG__port, 10);
	UART_write(&LOG__port, 13);
#endif
} //LOG_comment


int LOG_readCharacter(unsigned char* value)
{
#ifdef LOG_ON
	int dataAvailable;
	
	dataAvailable = UART_read(&LOG__port,value);
	return dataAvailable;

#else
	return 0;
#endif
}

void LOG_isr_read()
{
	UART_isr_read(&LOG__port);
}
