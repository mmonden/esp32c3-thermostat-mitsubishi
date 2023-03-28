// #include "Span.h"
// #include "HomeSpan.h"

enum TargetHeaterStates {
  OFF = 0,
  HEAT = 1,
  COOL = 2,
  AUTO = 3
};

enum CurrentHeaterStates {
  IDLE = 0,
  HEATING = 1,
  COOLING = 2
};

struct DEV_Thermostat : public Service::Thermostat
{
private:
	// TargetHeaterStates *heatPumpState = (TargetHeaterStates *) TargetHeaterStates::OFF;
	TargetHeaterStates heatPumpState = TargetHeaterStates::OFF;

	bool wasUpdated;

	int readPin;
	float temperature;
	unsigned long lastUpdateTime, updatePeriod;

	SpanCharacteristic *currentState;
	SpanCharacteristic *targetState;
	SpanCharacteristic *currentTemperature;
	SpanCharacteristic *targetTemperature;
	SpanCharacteristic *unit;

	void getTemperature(float *t, int pin)
	{
		int loop = 400;
		float adc = 0.0;

		for(int i = 0; i < loop; i++)
			adc += analogReadMilliVolts(pin);

		*t = (adc/loop) * 0.1 - 2;
	}

	void setTemperature(float *t)
	{
		currentTemperature->setVal(*t);
	}

public:
	DEV_Thermostat(int readPin, unsigned long updatePeriod) : Service::Thermostat()
	{
		getTemperature(&temperature, readPin);
		this->updatePeriod = updatePeriod;
		this->readPin = readPin;

		currentState = new Characteristic::CurrentHeatingCoolingState(CurrentHeaterStates::IDLE);
		targetState = new Characteristic::TargetHeatingCoolingState(heatPumpState, true);
		currentTemperature = new Characteristic::CurrentTemperature(temperature);
		targetTemperature = new Characteristic::TargetTemperature(22, true);
		unit = new Characteristic::TemperatureDisplayUnits(0);

		targetState->setValidValues(4, TargetHeaterStates::OFF, TargetHeaterStates::HEAT, TargetHeaterStates::COOL, TargetHeaterStates::AUTO);
		currentState->setValidValues(3, CurrentHeaterStates::IDLE, CurrentHeaterStates::HEATING, CurrentHeaterStates::COOLING);
		currentTemperature->setRange(-10, 40, 0.1);
		targetTemperature->setRange(10, 30, 0.5);

		lastUpdateTime = millis();
		wasUpdated = false;
	}

	virtual bool update() override
	{lmkmmllkmlkmlkmjkmkjllkmmlkmj
		wasUpdated = targetState->updated() || targetTemperature->updated();

		return true;
	}

	virtual void loop() override
	{
		if((millis() - lastUpdateTime) > updatePeriod)
		{
			getTemperature(&temperature, readPin);
			setTemperature(&temperature);
		}
	}
};