# ESP 8266 PWM fan controller
## Homepage
![Alt text](images/page.png?raw=true "HomePage")

## Electrical scheme
The electrical scheme include the circuit to control **NON PWM Fan (3 Pin)**.

If you already have a **PWM Fan (4 Pin)** ignore this part and connect the fan pwm pin **directly to the GPIO D1**
![Alt text](images/fan-controller-circuit.png?raw=true "Electrical scheme")

## Config JSON example
Setpoint configuration file.

The algorithm calculate the fan speed between the setpoint with a linear function.
```json
{
	"fan_speed_config": [{
			"temp": 21,
			"speed": 20
		},{
			"temp": 25,
			"speed": 40
		},{
			"temp": 30,
			"speed": 65
		},{
			"temp": 35,
			"speed": 90
		},{
			"temp": 37,
			"speed": 100
		}
	]
}
```
