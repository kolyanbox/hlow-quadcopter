

import frame.axes.Ax;

public class main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		float position = 0;
		Ax ax = new Ax();
		int i = 0;
		while (i < 1000){
			float out = pid(0, ax.getCurrentAngle(), 0.6f, 0.5f, 0.125f);
			if (out < 0)
			{
				ax.motorRight.setRpm(out*-1);
			}
			else
			{
				ax.motorLeft.setRpm(out);
			}
			ax.recalculateAngle();
			System.out.println("out: " + out + " ax: " + ax.getCurrentAngle());
			i++;
		}
	}
	
	static float previousError = 0;
	static float integral = 0;
	static float dt = 0.1f;
	
	static float pid(float setpoint, float currentValue, float Kp, float Ki, float Kd)
	{
		  float error = setpoint - currentValue;
		  integral = integral + (error*dt);
		  float derivative = (error - previousError)/dt;
		  float output = (Kp*error) + (Ki*integral) + (Kd*derivative);
		  previousError = error;
		  return output;
	}

}