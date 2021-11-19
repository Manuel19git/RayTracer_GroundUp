#include "Pattern.h"

//-----------Stripe-----------
Color Stripe::pattern_at(myPoint patternPoint)
{
	//We use abs because rounding negative number to the lowest number doesnt work
	if ((int)floor(abs(patternPoint.x)) % 2 == 0)
		return a;
	else
		return b;
}

//-----------Gradient-----------
//Return color of a gradient pattern in a localPoint
Color Gradient::pattern_at(myPoint patternPoint)
{
	float fraction;
	Color distance = b - a;
	if(patternPoint.x < 0)
		fraction = patternPoint.x - ceil(patternPoint.x);
	else
		fraction = patternPoint.x - floor(patternPoint.x);

	return a + distance * fraction;
}

//-----------Ring-----------
//Return color of a ring pattern in a localPoint
Color Ring::pattern_at(myPoint patternPoint)
{
	//We use abs because rounding negative number to the lowest number doesnt work
	if ((int)floor(sqrt(pow(patternPoint.x, 2) + pow(patternPoint.z, 2))) % 2 == 0)
		return a;
	else
		return b;
}

//-----------Checker-----------
//Return color of a 3d checker pattern in a localPoint
Color Checker::pattern_at(myPoint patternPoint)
{
	//We use abs because rounding negative number to the lowest number doesnt work
	if (((int)floor(abs(patternPoint.x)) + (int)floor(abs(patternPoint.y)) + (int)floor(abs(patternPoint.z))) % 2 == 0)
		return a;
	else
		return b;
}