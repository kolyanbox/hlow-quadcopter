package actuators.propellers;

import actuators.motors.Motor;

public class Propeller {
	
	private float thrust = 0.05f;
	private Motor motor;
	
	public Propeller(Motor moter)
	{
		this.motor = moter;
	}
	
	public float getThrust()
	{
		return this.motor.getRpm()*thrust;
	}
	
}
