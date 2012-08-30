package actuators.motors;

import actuators.propellers.Propeller;

public class Motor {
	public Propeller proppeller = new Propeller(this);
	private final float maxRpm = 100;
	private final float minRpm = 0;
	private float rpm;
	private Boolean isTurningRight;
	/**
	 * @return the rpm
	 */
	public float getRpm() {
		return rpm;
	}
	/**
	 * @param rpm the rpm to set
	 */
	public void setRpm(float rpm) {
		if (rpm < minRpm)
		{
			rpm = minRpm;
		}
		if (rpm > maxRpm)
		{
			rpm = maxRpm;
		}
		else {
			this.rpm = rpm;
		}
	}
	/**
	 * @return the isTurningRight
	 */
	public Boolean getIsTurningRight() {
		return isTurningRight;
	}
	/**
	 * @param isTurningRight the isTurningRight to set
	 */
	public void setIsTurningRight(Boolean isTurningRight) {
		this.isTurningRight = isTurningRight;
	}	
}
