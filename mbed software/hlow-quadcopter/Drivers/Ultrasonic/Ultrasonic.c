#include <Drivers/Ultrasonic/Ultrasonic.h>

/*Global variables*/
COX_EXTI_PI *exti = &pi_exti;
TIM_MATCHCFG_Type TMR0_Match;
Bool startPulse = TRUE;
OS_EventID distanceSem;
int distance = 0;

Bool initializeUltrasonicSensor(void)
{
	distanceSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (distanceSem == E_CREATE_FAIL)
	{
		return FALSE;
	}

	/*Initialize output pin*/
	GPIO_init(ULTRASONICOUTPUTPIN, 1,1);
	/*Initialize input pin*/
	GPIO_init(ULTRASONICINPUTPIN, 0,0);

	EXTINT_eint0();

	/*initialize timer*/
	TIM_TIMERCFG_Type TMR0_Cfg;

	/* On reset, Timer0/1 are enabled (PCTIM0/1 = 1), and Timer2/3 are disabled (PCTIM2/3 = 0).*/
	/* Initialize timer 0, prescale count time of 10uS */
	TMR0_Cfg.PrescaleOption = TIM_PRESCALE_USVAL;
	TMR0_Cfg.PrescaleValue = 1;
	/* Use channel 0, MR0 */
	TMR0_Match.MatchChannel = 0;
	/* Enable interrupt when MR0 matches the value in TC register */
	TMR0_Match.IntOnMatch = ENABLE;
	/* Enable reset on MR0: TIMER will reset if MR0 matches it */
	TMR0_Match.ResetOnMatch = TRUE;
	/* Don't stop on MR0 if MR0 matches it*/
	TMR0_Match.StopOnMatch = FALSE;
	/* Do nothing for external output pin if match (see cmsis help, there are another options) */
	TMR0_Match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	/* Set Match value, count value of 1 (1 * 10uS = 10us) */
	TMR0_Match.MatchValue = 10;
	/* Set configuration for Tim_config and Tim_MatchConfig */
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TMR0_Cfg);
	TIM_ConfigMatch(LPC_TIM0, &TMR0_Match);

	/* Preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER0_IRQn);
	/* Start timer 0 */
	TIM_Cmd(LPC_TIM0, DISABLE);

	return TRUE;
	CoPostSem(distanceSem);
}

int EXTINT_eint0 (void)
{
	exti->Handler(exti_handler);
	exti->Init(ULTRASONICINPUTPIN);
	exti->Event(ULTRASONICINPUTPIN,COX_EXTI_EDGE_RISING_FALLING);
	return 1;
}

/*External interrupt handler*/
void exti_handler (COX_PIN_Dev pin)
{
	/*If there is an interrupt on the ultrasonic input pin*/
	if(pin == ULTRASONICINPUTPIN) {
		/*Begin of measuring time*/
		if (startPulse)
		{
			/*if there isn't an object the time of flight will be 36 ms or longer*/
			TMR0_Match.MatchValue = 36000;
			TIM_ConfigMatch(LPC_TIM0, &TMR0_Match);
			TIM_Cmd(LPC_TIM0, ENABLE);

			startPulse = FALSE;
		}
		/*End of measuring time*/
		else
		{
			/*Measure distance in cm*/
			if (CoPendSem(distanceSem,10) == E_OK){
					distance = LPC_TIM0->TC/58;
					CoPostSem(distanceSem);
			}
			/*Reset the whole proces*/
			TMR0_Match.MatchValue = 10;
			TIM_ConfigMatch(LPC_TIM0, &TMR0_Match);
			TIM_ResetCounter(LPC_TIM0);

			startPulse = TRUE;
		}
	}
}

void startMeasureDistanceToGround(void)
{
	/*start timer for 10 us*/
	GPIO_SetValue(1,ULTRASONICOUTPUTPIN);
	TIM_Cmd(LPC_TIM0, ENABLE);
}

/*after 10 us set output pin low and start waiting for response on input pin*/
void TIMER0_IRQHandler (void)
{
	/*  Clear Interrupt and disable timer */
	TIM_ClearIntPending(LPC_TIM0,TIM_MR0_INT);
	TIM_Cmd(LPC_TIM0,DISABLE);
	// set output pin low
	GPIO_ClearValue(1,ULTRASONICOUTPUTPIN);

	return;
}

int getDistanceToGround(void)
{
	startMeasureDistanceToGround();
	int distanceToSend = -1;
	//if (CoPendSem(distanceSem,1) == E_OK){
		distanceToSend = distance;
	//	CoPostSem(distanceSem);
	//}
	return distanceToSend;
}
