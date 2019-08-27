package program;

import java.util.*;

public class BMICalculator {
    public static void main(String[] args) {
        double weight, inches;
        Scanner input = new Scanner(System.in);

        System.out.println("Please enter your weight : ");
        weight = input.nextDouble();
        System.out.println("Please enter how tall you are in inches : ");
        inches = input.nextDouble();

        final	double	KILOGRAMS_PER_POUND	= 0.45359237;
        final	double	METERS_PER_INCH	= 0.0254;

        double	weightInKilograms =	weight * KILOGRAMS_PER_POUND;
        double	heightInMeters = inches * METERS_PER_INCH;
        double	bmi	= weightInKilograms / (heightInMeters	* heightInMeters);

        System.out.println("Your BMI is " + bmi);
        if (bmi < 18.5)
            System.out.println("Underweight");
        else if (bmi < 25)
            System.out.println("Normal");
        else if (bmi < 30)
            System.out.println("Overweight");
        else
            System.out.println("Obese");
    }
}

// ComputeAndInterpretBMI.java,	to	let	the	user	enter	weight,	feet,	and inches.
// For	example,	if	a	person	is	5	feet	and	10	inches,	you	will	enter
// 5	for feet	and	10	for	inches.	Here	is	a	sample	run: