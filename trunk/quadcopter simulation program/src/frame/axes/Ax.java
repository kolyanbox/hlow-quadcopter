package frame.axes;

import actuators.motors.Motor;

public class Ax {
	public Motor motorLeft = new Motor();
	public Motor motorRight = new Motor();
		
	private float currentAngle = -10;
	
	float currentThrustLeft;
	float currentThrustRight;
	public Ax ()
	{
		this.currentThrustLeft = motorLeft.proppeller.getThrust();
		this.currentThrustRight = motorRight.proppeller.getThrust();
	}
	
	public void recalculateAngle()
	{
		this.currentThrustLeft = motorLeft.proppeller.getThrust();
		this.currentThrustRight = motorRight.proppeller.getThrust();
		
		if (this.currentThrustLeft != this.currentThrustRight)
		{
			if (this.currentThrustLeft > this.currentThrustRight)
			{
				this.currentAngle = getCurrentAngle() + 0.1f;
			}
			else
			{
				this.currentAngle = getCurrentAngle() - 0.1f;
			}
		}
		
	}

	/**
	 * @return the currentAngle
	 */
	public float getCurrentAngle() {
		return currentAngle;
	}	
}
