/*
 * AT25F1024 SPI Flash Memory.c
 *
 * Created: 10/15/2026 4:04:52 PM
 * Author : 91989
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define WREN 0X06
#define WRDI 0X04
#define RDSR 0X05
#define WRSR 0X01
#define READ 0X03
#define PROGRAM 0X02
#define SECTOR_ERASE 0X52
#define CHIP_ERASE 0X62
#define RDID 0X15
#define SPI_EN() (PORTB &= 0XEF)
#define SPI_DI() (PORTB |= 0X10)

#define K1_DOWN() ((PINC & _BV(PC0)) == 0X00)
#define K2_DOWN() ((PINC & _BV(PC1)) == 0X00)
#define K3_DOWN() ((PINC & _BV(PC2)) == 0X00)
#define K4_DOWN() ((PINC & _BV(PC3)) == 0X00)

uint8_t segCC[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void enable_LED_Segment(void);
void show_Data_IN_Segment(uint8_t data);
void enable_Buttons(void);
void SPI_MasterInit(void);
uint8_t SPI_Transmit(uint8_t data);
void _3bytes_spi_transmit(uint32_t add);
void Busy_wait(void);
void transmit_Data(uint32_t address, uint8_t data);
uint8_t read_Data(uint32_t address);
void erase_Data(uint32_t address);
void read_Flash_ID(uint8_t *manufacturer,uint8_t *device,uint8_t *extra);
void main(void)
{
   enable_LED_Segment();
   enable_Buttons();
   SPI_MasterInit();
   uint8_t read_data;
   uint32_t address = 0x00000;
    while (1) 
    {
		if(K1_DOWN()){
			PORTA |= (1<<PA0)|(1<<PA1);
			uint8_t data = 70;
			
			transmit_Data(address, data);
			_delay_ms(5);
			
		}
		
		if(K2_DOWN()){
			read_data = read_Data(address);
			_delay_ms(5);
			if(read_data == 255){
				continue;	
			}
			show_Data_IN_Segment(read_data);
			_delay_ms(2);
			while(K2_DOWN()){
				show_Data_IN_Segment(read_data);
			}
			PORTA |= (1<<PA0)|(1<<PA1);
		}
		if(K3_DOWN()){
			PORTA |= (1<<PA0)|(1<<PA1);
			erase_Data(address);
			_delay_ms(5);
		}
		if (K4_DOWN())
		{
			uint8_t manufacturer, device, extra;

			read_Flash_ID(&manufacturer, &device, &extra);

			while (K4_DOWN())
			{
				show_Data_IN_Segment(manufacturer);
				show_Data_IN_Segment(device);
				show_Data_IN_Segment(extra);
			}
			PORTA |= (1<<PA0) | (1<<PA1);
		}
    }
}

void enable_LED_Segment(void){
	DDRD = 0XFF;
	DDRA |= (1<<PA1) | (1<<PA0);
}
void show_Data_IN_Segment(uint8_t data){
	uint8_t ones, tens;
	ones = data%10;
	tens = data/10;
	for(int i = 0;i<40;i++){
	PORTA |= ((1<<PA1)|(1<<PA0));
	PORTA &= ~(1<<PA0);
	PORTD = segCC[ones];	
	_delay_ms(5);

	PORTA |= ((1<<PA1)|(1<<PA0));
	PORTA &= ~(1<<PA1);	
	PORTD = segCC[tens];
	_delay_ms(5);
	}
}


void enable_Buttons(void){
	DDRC = 0X00;
	PORTC = 0X0F;
}

void SPI_MasterInit(void)
{
	DDRB |= (1 << PB4) | (1 << PB5) | (1 << PB7);
	DDRB &= ~(1 << PB6);

	PORTB |= (1 << PB4);  // Keep Flash CS HIGH

	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t SPI_Transmit(uint8_t dat){
	SPDR = dat;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void _3bytes_spi_transmit(uint32_t add){
	SPI_Transmit((uint8_t)(add>>16)&0XFF);
	SPI_Transmit((uint8_t)(add>>8)&0XFF);
	SPI_Transmit((uint8_t)(add&0XFF));
}
void Busy_wait(void)
{
	uint8_t status;

	do
	{
		SPI_EN();
		SPI_Transmit(RDSR);
		status = SPI_Transmit(0xFF);
		SPI_DI();
	}
	while (status & 0x01);
}
void transmit_Data(uint32_t address, uint8_t data){
	SPI_EN();
	SPI_Transmit(WREN);
	SPI_DI();
	SPI_EN();
	SPI_Transmit(PROGRAM);
	_3bytes_spi_transmit(address);
	SPI_Transmit(data);
	SPI_DI();
	Busy_wait();
}

uint8_t read_Data(uint32_t address){
	uint8_t data;
	SPI_EN();
	SPI_Transmit(READ);
	_3bytes_spi_transmit(address);
	data = SPI_Transmit(0xFF);
	SPI_DI();
	return data;
}

void erase_Data(uint32_t address){
	SPI_EN();
	SPI_Transmit(WREN);
	SPI_DI();
	_delay_ms(2);
	SPI_EN();
	SPI_Transmit(SECTOR_ERASE);
	_3bytes_spi_transmit(address);
	SPI_DI();
	Busy_wait();
}
void read_Flash_ID(uint8_t *manufacturer,
uint8_t *device,
uint8_t *extra)
{
	SPI_EN();

	SPI_Transmit(RDID);

	*manufacturer = SPI_Transmit(0xFF);
	*device       = SPI_Transmit(0xFF);
	*extra        = SPI_Transmit(0xFF);

	SPI_DI();
}