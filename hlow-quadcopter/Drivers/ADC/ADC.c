#include <Drivers/ADC/ADC.h>

#include <Interfaces/Actuators/Actuators.h>
#include <General/util.h>
#include <lpc17xx_pinsel.h>
Bool ADC_init()
{
	PINSEL_CFG_Type PinCfg1;
	PinCfg1.Funcnum = 1;
	PinCfg1.OpenDrain = 0;
	PinCfg1.Pinmode = 0;
	PinCfg1.Pinnum = 25;
	PinCfg1.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg1);

	PINSEL_CFG_Type PinCfg2;
	PinCfg2.Funcnum = 1;
	PinCfg2.OpenDrain = 0;
	PinCfg2.Pinmode = 0;
	PinCfg2.Pinnum = 24;
	PinCfg2.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg2);

	PINSEL_CFG_Type PinCfg3;
	PinCfg3.Funcnum = 1;
	PinCfg3.OpenDrain = 0;
	PinCfg3.Pinmode = 0;
	PinCfg3.Pinnum = 23;
	PinCfg3.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg3);

	PINSEL_CFG_Type PinCfg4;
	PinCfg4.Funcnum = 1;
	PinCfg4.OpenDrain = 0;
	PinCfg4.Pinmode = 0;
	PinCfg4.Pinnum = 26;
	PinCfg4.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg4);

	PINSEL_CFG_Type PinCfg5;
	PinCfg5.Funcnum = 1;
	PinCfg5.OpenDrain = 0;
	PinCfg5.Pinmode = 0;
	PinCfg5.Pinnum = 30;
	PinCfg5.Portnum = 1;
	PINSEL_ConfigPin(&PinCfg5);

	ADC_Init(LPC_ADC, 10000);

	return TRUE;
}

uint16_t getADC(int channel)
{
	uint16_t adc_value;
	uint16_t adc_value1;
	uint16_t adc_value2;
	uint16_t adc_value3;

	// Start conversion
	ADC_StartCmd(LPC_ADC,ADC_START_NOW);
	//Wait conversion complete
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN2,RESET);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_2,ENABLE);
	while (!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_2,ADC_DATA_DONE)));
	adc_value = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_2);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_2,DISABLE);

	ADC_IntConfig(LPC_ADC,ADC_ADINTEN1,RESET);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,ENABLE);
	while (!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_1,ADC_DATA_DONE)));
	adc_value1 = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_1);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,DISABLE);

	ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,RESET);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);
	while (!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_0,ADC_DATA_DONE)));
	adc_value2 = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,DISABLE);

	ADC_IntConfig(LPC_ADC,ADC_ADINTEN3,RESET);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,ENABLE);
	while (!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_3,ADC_DATA_DONE)));
	adc_value3 = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_3);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,DISABLE);

	if (channel == 0)
	{
		return adc_value;
	}
	if (channel == 1)
	{
		return adc_value1;
	}
	if (channel == 2)
	{
		return adc_value2;
	}
	if (channel == 3)
	{
		return adc_value3;
	}
	else
	{
		return 0;
	}
}
