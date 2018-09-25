/**
 @file lcd.h
 @brief LCD module header
 @author: Dr. Klaus Schaefer
 */

#ifndef LCD_H_
#define LCD_H_

void LCD_init( void);
void lcd_write_line( unsigned line, char * text);

#endif /* LCD_H_ */
