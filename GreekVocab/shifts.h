#ifndef SHIFTS
#define SHIFTS
class Shifts	{
public:
	enum enumShifts{NONE=0,SHIFT=1,IOTA=2,SMOOTH=4,ROUGH=8,ALT=16};
	Shifts(enumShifts publicVal) : value(publicVal) {}
	short operator()(){return value;}
	Shifts operator +(Shifts right){value += right.value;return *this;}
	Shifts operator +=(Shifts right){value += right.value;return *this;}
	Shifts operator +(enumShifts right){value += right;return *this;}
	Shifts operator +=(enumShifts right){value += right;return *this;}
	bool operator ==(Shifts right){return (value == right.value);}
	bool operator ==(enumShifts right){return (value == right);}

private:
	short value;
	Shifts(){}
};
#endif