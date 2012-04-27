#include <driver/ADC.h>
int ADC_init()
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

	ADC_Init(LPC_ADC, 10000);

	return 0;
}

uint16_t getADC(int channel)
{
	uint16_t adc_value;
	uint16_t adc_value1;

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
	//Display the result of conversion on the UART0

	if (channel == 0)
	{
		return adc_value;
	}
	if (channel == 1)
	{
		return adc_value1;
	}
	else
	{
		return 0;
	}
}
