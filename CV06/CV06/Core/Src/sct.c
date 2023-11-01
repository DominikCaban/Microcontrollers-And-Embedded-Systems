/*
 * sct.c
 *
 *  Created on: Oct 11, 2023
 *      Author: xcaban01
 */
#include "main.h"
#include "sct.h"

void sct_init(void) {
    HAL_GPIO_WritePin(SCT_NOE_GPIO_Port, SCT_NOE_Pin, GPIO_PIN_SET); // Set NOE high (turn off output)
    HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, GPIO_PIN_RESET); // Initialize CLK pin to low
    HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin, GPIO_PIN_RESET); // Initialize SDI pin to low
}

void sct_led(uint32_t value) {
    HAL_GPIO_WritePin(SCT_NOE_GPIO_Port, SCT_NOE_Pin, GPIO_PIN_RESET); // Enable output

    for(uint32_t i = 0; i < 32; i++) {
        HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin, (value & 1)); // Write SDI
        value >>=1 ;

        HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, GPIO_PIN_SET); // Toggle CLK
        HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, GPIO_PIN_RESET);
    }

    HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, GPIO_PIN_SET); // Latch data
    HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, GPIO_PIN_RESET);
}

void sct_value(uint16_t value, uint8_t bargraph){

	static const uint32_t reg_values[4][10] = {
	{
	//PCDE--------GFAB @ DIS1
	0b0111000000000111 << 16,
	0b0100000000000001 << 16,
	0b0011000000001011 << 16,
	0b0110000000001011 << 16,
	0b0100000000001101 << 16,
	0b0110000000001110 << 16,
	0b0111000000001110 << 16,
	0b0100000000000011 << 16,
	0b0111000000001111 << 16,
	0b0110000000001111 << 16,
	},

	{
	//----PCDEGFAB---- @ DIS2
	0b0000111101110000 << 0, //0
	0b0000110000010000 << 0, //1
	0b0000101110110000 << 0, //2
	0b0000111010110000 << 0, //3
	0b0000110011010000 << 0, //4
	0b0000111011100000 << 0, //5
	0b0000111111100000 << 0, //6
	0b0000110000110000 << 0, //7
	0b0000111111110000 << 0, //8
	0b0000111011110000 << 0, //9
	},

	{
	//PCDE--------GFAB @ DIS3
	0b0111000000000111 << 0,
	0b0100000000000001 << 0,
	0b0011000000001011 << 0,
	0b0110000000001011 << 0,
	0b0100000000001101 << 0,
	0b0110000000001110 << 0,
	0b0111000000001110 << 0,
	0b0100000000000011 << 0,
	0b0111000000001111 << 0,
	0b0110000000001111 << 0,
	},

	{
	//----43215678---- @ LED
	0b0000000000000000 << 16,   // Default
	0b0000000100000000 << 16,   // LED 1
	0b0000001100000000 << 16,   // LED 1,2
	0b0000011100000000 << 16,   // LED 1,2,3
	0b0000111100000000 << 16,   // LED 1,2,3,4
	0b0000111110000000 << 16,   // LED 1,2,3,4,5
	0b0000111111000000 << 16,   // LED 1,2,3,4,5,6
	0b0000111111100000 << 16,   // LED 1,2,3,4,5,6,7
	0b0000111111110000 << 16,   // LED 1,2,3,4,5,6,7,8
	}

	};


	uint32_t reg = 0;

	//position of 100
	reg |= reg_values[0][value / 100 % 10];
	//position of 10
	reg |= reg_values[1][value / 10 % 10];
	//position of 1
	reg |= reg_values[2][value / 1 % 10];

	// LED BARGRAPH
	reg |= reg_values[3][bargraph];

	sct_led(reg);




}
