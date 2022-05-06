package org.jfree.data;

import static org.junit.Assert.*;

import org.junit.*;
import org.junit.rules.ExpectedException;

/**
 * This class contains test cases used for testing the functionality of the
 * RangeType class from the JFreechart application
 */
public class RangeTest {

	// private field used by some methods to test the constrain() method in the
	// RangeType class
	private Range constrainRangeTest;

	// private field used by some methods to test the contains() method in the
	// RangeType class
	private Range containsRangeTest;

	@Before
	public void setUp() throws Exception {
		constrainRangeTest = new Range(-2, 7);
		containsRangeTest = new Range(-11.5, 31.5);
	}

	// Constructor

	@Test
	/**
	 * A method to test the constructor giving an exception when the second input is
	 * lower than the first.
	 */
	public void switchedInputToConstructor() {
		boolean exception = false;
		try {
			new Range(5, -10);
		} catch (IllegalArgumentException e) {
			exception = true;
		}
		assertEquals(exception, true);
	}

	/**
	 * A method that tests the constructor to make sure it does not change the
	 * input.
	 */
	@Test
	public void correctInputConstructor() {
		Range newRange = new Range(5, 10);
		assertEquals("Input doesnt change in constructor", newRange.getLowerBound(), 5, .001d);
		assertEquals("Input doesnt change in constructor", newRange.getUpperBound(), 10, .001d);
	}

	// getCentralValue

	@Test
	/**
	 * A method testing the getCentralValue() method for a negative output
	 */
	public void centralValueShouldBeNegative() {
		Range values = new Range(-10, 5);
		assertEquals("The central value of -10 and 5 should be -2.5", -2.5, values.getCentralValue(), .001d);
	}

	@Test
	/**
	 * A method testing the getCentralValue() method for a zero output
	 */
	public void centralValueShouldBeZero() {
		Range values = new Range(-10, 10);
		assertEquals("The central value of -10 and 10 should be 0", 0, values.getCentralValue(), .001d);
	}

	@Test
	/**
	 * A method testing the getCentralValue() method for a positive output
	 */
	public void centralValueShouldBePositive() {
		Range values = new Range(-5, 10);
		assertEquals("The central value of -5 and 10 should be 2.5", 2.5, values.getCentralValue(), .001d);
	}

	// contains

	@Test
	/**
	 * A method testing the contains() method for a value that is smaller than the
	 * lower boundary of a Range object
	 */
	public void containsTestForLessThanLowerBound() {
		assertEquals("The range of -11.5 to 31.5 does not conatin -13.1", false, containsRangeTest.contains(-13.1));

	}

	@Test
	/**
	 * A method testing the contains() method for a value that is within the lower
	 * boundary of a Range object
	 */
	public void containsTestForOnLowerBound() {
		assertEquals("The range of -11.5 to 31.5 does conatin -11.5", true, containsRangeTest.contains(-11.5));
	}

	@Test
	/**
	 * A method testing the contains() method for a value that is within the
	 * boundary of a Range object
	 */
	public void containsTestForInBetweenBounds() {
		assertEquals("The range of -11.5 to 31.5 does conatin 0", true, containsRangeTest.contains(0));
	}

	@Test
	/**
	 * A method testing the contains() method for a value that is equal to the upper
	 * bound limit of a Range object
	 */
	public void containsTestForOnUpperBound() {
		assertEquals("The range of -11.5 to 31.5 does conatin 31.5", true, containsRangeTest.contains(31.5));
	}

	@Test
	/**
	 * A method testing the contains() method for a value that is greater than the
	 * upper bound of a Range object
	 */
	public void containsTestForMoreThanUpperBound() {
		assertEquals("The range of -11.5 to 31.5 does not conatin 41.5", false, containsRangeTest.contains(41.5));
	}

	@Test
	/**
	 * A method testing the contains() method for a value that is equal to the
	 * minimum double value set as the upper bound of a Range object
	 */
	public void containsTestMin() {
		Range minRange = new Range(0, Double.MIN_VALUE);
		assertEquals("The range of 0 to MIN_Double does conatin MIN_Double", true, minRange.contains(Double.MIN_VALUE));
	}

	@Test
	/**
	 * A method testing the contains() method for a value that is equal to the
	 * maximum double value set as the upper bound of a Range object
	 */
	public void containsTestMax() {
		Range minRange = new Range(0, Double.MAX_VALUE);
		assertEquals("The range of 0 to MAX_Double does conatin MAX_Double", true, minRange.contains(Double.MAX_VALUE));
	}

	// equals

	@Test

	/**
	 * A method testing the equals method for equality between two Range objects
	 */
	public void equalsTestForSameRange() {
		Range values1 = new Range(-10, 10);
		Range values2 = new Range(-10, 10);
		assertEquals("Both input ranges have the same bounds", true, values1.equals(values2));
	}

	@Test
	/**
	 * A method testing the equals method for inequality between two Range objects
	 * using a lower range for the expecteed result
	 */
	public void equalsTestForLowerRange() {
		Range values1 = new Range(-10, 10);
		Range values2 = new Range(-20, 10);
		assertEquals("The input ranges have different lower bounds", false, values1.equals(values2));
	}

	/**
	 * A method testing the equals method for inequality between two Range objects
	 * using a higher range for the expected result
	 */
	@Test
	public void equalsTestForHigherRange() {
		Range values1 = new Range(-10, 10);
		Range values2 = new Range(-10, 20);
		assertEquals("The input ranges have different upper bounds", false, values1.equals(values2));
	}

	/**
	 * A method testing that equals returns false if the input is not a Range
	 * object.
	 */
	@Test
	public void equalsFalseForNonRange() {
		Range values1 = new Range(-10, 10);
		double value2 = 5;
		assertEquals(false, values1.equals(value2));
	}

	// getLowerBound

	/**
	 * A method testing the getLowerBound() method for the correct lower bound of a
	 * Range object
	 */
	@Test
	public void getLowerBoundTest() {
		Range values = new Range(-10, 10);
		assertEquals("Lower bound should be -10", -10, values.getLowerBound(), .001d);
	}

	// getUpperBound

	/**
	 * A method testing the getUpperBound() method for the correct upper bound of a
	 * Range object
	 */
	@Test
	public void getUpperBoundTest() {
		Range values = new Range(-10, 10);
		assertEquals("Upper bound should be 10", 10, values.getUpperBound(), .001d);
	}

	// combine

	/**
	 * A method testing the combine() method for the the correct combination of two
	 * Range objects
	 */
	@Test
	public void combineTestIntersect() {
		Range test1, test2;
		Range result, expected;

		test1 = new Range(0, 10);
		test2 = new Range(5, 15);
		expected = new Range(0, 15);
		result = Range.combine(test1, test2);
		assertEquals("Combine intersecting ranges", expected, result);
	}

	/**
	 * A method testing the combine() method for the the correct combination of two
	 * Range objects with no overlap
	 */
	@Test
	public void combineTestNoOverlap() {
		Range test1, test2;
		Range result, expected;

		test1 = new Range(0, 10);
		test2 = new Range(15, 20);
		expected = new Range(0, 20);
		result = Range.combine(test1, test2);
		assertEquals("Combine non overlapping ranges", expected, result);
	}

	/**
	 * A method testing the combine() method for the the correct combination of a
	 * null Range object and an initialized Range object
	 */
	@Test
	public void combineTestInput1IsNull() {
		Range test1, test2;
		Range result, expected;

		test1 = null;
		test2 = new Range(0, 10);
		expected = new Range(0, 10);
		result = Range.combine(test1, test2);
		assertEquals("Combine initilized range with a null range", expected, result);
	}

	/**
	 * A method testing the combine() method for the the correct combination of an
	 * initialized Range object and a null Range object
	 */
	@Test
	public void combineTestInput2IsNull() {
		Range test1, test2;
		Range result, expected;

		test2 = null;
		test1 = new Range(0, 10);
		expected = new Range(0, 10);
		result = Range.combine(test1, test2);
		assertEquals("Combine initilized range with a null range", expected, result);
	}

	// constrain

	/**
	 * A method testing the constrain() method for value within the range that is
	 * closest to the specified value. This value is within the range of the Range
	 * object
	 */
	@Test
	public void constrainTestMiddleOfRange() {
		double result;
		result = constrainRangeTest.constrain(2.5);
		assertEquals("Range of -2 to 7 should constrain a value of 2.5 to 2.5", 2.5, result, 0.001d);
	}

	/**
	 * A method testing the constrain() method for value within the range that is
	 * closest to the specified value. The value is greater than the range of the
	 * Range object
	 */
	@Test
	public void constrainTestOutsideRangeAbove() {
		double result;
		result = constrainRangeTest.constrain(8);
		assertEquals("Range of -2 to 7 should constrain a value of 8 to 7", 7, result, 0.001d);
	}

	/**
	 * A method testing the constrain() method for value within the range that is
	 * closest to the specified value.
	 */
	@Test
	public void constrainTestOutsideRangeBelow() {
		double result;
		result = constrainRangeTest.constrain(-3);
		assertEquals("Range of -2 to 7 should constrain a value of -3 to -2", -2, result, 0.001d);
	}

	/**
	 * A method testing the constrain() method for value within the range that is
	 * closest to the specified value. The value compared is equal to the lower
	 * range of the Range object
	 */
	@Test
	public void constrainTestOnLower() {
		double result;
		result = constrainRangeTest.constrain(-2);
		assertEquals("Range of -2 to 7 should constrain a value of -2 to -2", -2, result, 0.001d);
	}

	/**
	 * A method testing the constrain() method for value within the range that is
	 * closest to the specified value. The value compared is equal to the upper
	 * range of the Range object
	 */
	@Test
	public void constrainTestOnUpper() {
		double result;
		result = constrainRangeTest.constrain(7);
		assertEquals("Range of -2 to 7 should constrain a value of 7 to 7", 7, result, 0.001d);
	}

	/**
	 * A method testing the constrain() method for value within the range that is
	 * closest to the specified value using the minimum double value for comparison
	 */
	@Test
	public void constrainTestOnMin() {
		double precision = 0.00000000000000000000000000000000000000000000001d;
		Range minRange = new Range(0, Double.MIN_VALUE);
		double result = minRange.constrain(Double.MIN_VALUE);
		assertEquals("Range of 0 to Double.MIN_VALUE should constrain a value of MIN_VALUE to MIN_VALUE",
				Double.MIN_VALUE, result, precision);
	}

	/**
	 * A method testing the constrain() method for value within the range that is
	 * closest to the specified value using the maximum double value for comparison
	 */
	@Test
	public void constrainTestOnMax() {
		double precision = 0.00000000000000000000000000000000000000000000001d;
		Range minRange = new Range(0, Double.MAX_VALUE);
		double result = minRange.constrain(Double.MAX_VALUE);
		assertEquals("Range of 0 to Double.MAX_VALUE should constrain a value of MAX_VALUE to MAX_VALUE",
				Double.MAX_VALUE, result, precision);

	}

	// shift(Range, double, boolean)

	/**
	 * Method that tests that shift method works when Zero Crossing is set to true.
	 */
	@Test
	public void shiftWithZeroCrossing() {
		Range test = new Range(-10, 10);
		Range result = new Range(10, 30);
		test = Range.shift(test, 20, true);
		assertEquals(test.equals(result), true);
	}

	/**
	 * A method that tests whether the shift method works when setting Zero Crossing
	 * to false and trying to cross from a higher value.
	 */
	@Test
	public void shiftWithNoZeroCrossingWithValuesAboveZero() {
		Range test = new Range(-10, 10);
		Range result = new Range(-30, 0);
		test = Range.shift(test, -20, false);
		assertEquals(test.equals(result), true);
	}

	/**
	 * A method that tests whether the shift method works when setting Zero Crossing
	 * to false and trying to cross from a lower value.
	 */
	@Test
	public void shiftWithNoZeroCrossingWithValuesBelowZero() {
		Range test = new Range(-10, 10);
		Range result = new Range(0, 30);
		test = Range.shift(test, 20, false);
		assertEquals(test.equals(result), true);
	}

	/**
	 * A method that tests whether the shift method works when setting Zero Crossing
	 * to false and using zero values.
	 */
	@Test
	public void shiftWithNoZeroCrossingWithZeroValuesNegativeDelta() {
		Range test = new Range(0, 10);
		Range result = new Range(-20, 0);
		test = Range.shift(test, -20, false);
		assertEquals(test.equals(result), true);
	}

	/**
	 * A method that tests whether the shift method works when setting Zero Crossing
	 * to false and using zero values.
	 */
	@Test
	public void shiftWithNoZeroCrossingWithZeroValuesPositiveDelta() {
		Range test = new Range(0, 10);
		Range result = new Range(20, 30);
		test = Range.shift(test, 20, false);
		assertEquals(test.equals(result), true);
	}

	// shiftWithNoZeroCrossing
	/**
	 * A method that tests the shift method when the test range is less than 1
	 */
	@Test
	public void siftWithNoZeroCrossingNorm() {
		Range test = new Range(0.2, 0.5);
		Range result = new Range(1.2, 1.5);
		assertEquals(Range.shift(test, 1, false), result);
	}

	// CombineIgnoringNaN

	@Test
	public void CombineIgnoringNaNBothNULL() {

	}

	// getLength()
	/**
	 * A method that tests getLength when the range has different values.
	 */
	@Test
	public void getLengthDifferentValues() {
		Range test = new Range(5, 10);
		assertEquals("getLength with different values", 5, test.getLength(), .001d);
	}

	/**
	 * A method that tests getLength when the range has the same value for upper and
	 * lower
	 */
	@Test
	public void getLengthSameValues() {
		Range test = new Range(5, 5);
		assertEquals("getLength with same values", 0, test.getLength(), .001d);
	}

	// intersects(double b0, double b1)

	@Test
	/**
	 * A method that tests intersects() on boundary condition
	 */
	public void intersectsBoundaryCondition() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(5, 5), false);
	}

	@Test
	/**
	 * A method that tests intersects() when the intersect range to goes from
	 * outside to inside the range
	 */
	public void intersectsOutIn() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(0, 7), true);
		assertEquals(new Range(5, 10), test);
	}

	@Test
	/**
	 * A method that tests intersects() to make sure that intersects does not change
	 * the input range.
	 */
	public void intersectsLowerRangeDoesNotChange() {
		Range test = new Range(5, 10);
		test.intersects(0, 7);
		assertEquals(new Range(5, 10), test);
	}

	@Test
	/**
	 * A method that tests intersects() to make sure that intersects does not change
	 * the input range.
	 */
	public void intersectsUpperRangeDoesNotChange() {
		Range test = new Range(5, 10);
		test.intersects(7, 15);
		assertEquals(new Range(5, 10), test);
	}

	@Test
	/**
	 * A method that tests intersects() to test when the test value is on the upper
	 * bound on the range.
	 */
	public void intersectsOutOnUpper() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(10, 15), false);
	}

	@Test
	/**
	 * A method that tests intersects() to test when the test value is on the lower
	 * bound of the range.
	 */
	public void intersectsOutOnLower() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(0, 5), false);
	}

	@Test
	/**
	 * A method that tests intersects() when b0 is on the lower bound and b1 is on
	 * the upper bound.
	 */
	public void intersectsOnLowerIn() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(5, 15), true);
	}

	@Test
	/**
	 * A method that tests intersects() when b1 is right above the lower bound.
	 */
	public void intersectsRightAboveLower() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(0, 6), true);
	}

	@Test
	/**
	 * A method that tests intersects() when the input values are equal.
	 */
	public void intersectsSameValues() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(7, 7), true);
	}

	@Test
	/**
	 * A method that tests intersects when the intersect range to goes from inside
	 * to outside the range
	 */
	public void intersectsInOut() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(7, 15), true);
	}

	@Test
	/**
	 * A method that tests intersects when the intersect range to goes from inside
	 * to inside the range
	 */
	public void intersectsInIn() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(7, 8), true);
	}

	@Test
	/**
	 * A method that tests intersects when the intersect range to goes from outside
	 * to outside below the range
	 */
	public void intersectsOutOutLow() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(1, 4), false);
	}

	@Test
	/**
	 * A method that tests intersects when the intersect range to goes from outside
	 * to outside below the range
	 */
	public void intersectsOutOutHigh() {
		Range test = new Range(5, 10);
		assertEquals(test.intersects(11, 15), false);
	}

	@Test
	/**
	 * A method that tests intersects when the intersect range to goes from outside
	 * below to outside above the range
	 */
	public void intersectsOutOutLowHigh() {
		Range test = new Range(5, 6);
		assertEquals(test.intersects(4, 7), true);
	}

	// interests(Range range)
	@Test
	/**
	 * A method that tests intersects(Range range) when the intersect range
	 * intersects the test range
	 */
	public void intersectsRange() {
		Range test = new Range(5, 10);
		Range intersect = new Range(7, 9);
		assertEquals(test.intersects(intersect), true);
	}

	@Test
	/**
	 * A method that tests intersects(Range range) when the intersect range does not
	 * intersect the test range
	 */
	public void intersectsRangeFalse() {
		Range test = new Range(5, 10);
		Range intersect = new Range(1, 3);
		assertEquals(test.intersects(intersect), false);
	}

	// combine IgnoringNaN(Range range1, Range range2)
	/**
	 * A method testing the combineIgnoringNaN with ranges that intersect
	 */
	@Test
	public void ignoringnanIntersecting() {

		Range rang1 = new Range(5, 10);
		Range rang2 = new Range(7, 15);
		Range expected = new Range(5, 15);
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine intersecting ranges", expected, result);
	}

	/**
	 * A method testing the combineIgnoringNaN with the first null range
	 */
	@Test
	public void ignoringnanFirstNull() {

		Range rang1 = null;
		Range rang2 = new Range(7, 15);
		Range expected = new Range(7, 15);
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine initilized range with a null", expected, result);
	}

	/**
	 * A method testing the combineIgnoringNaN with the second null range
	 */
	@Test
	public void ignoringnanSecondNull() {

		Range rang1 = new Range(5, 10);
		Range rang2 = null;
		Range expected = new Range(5, 10);
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine initilized range with a null range", expected, result);
	}

	/**
	 * A method testing the combineIgnoringNaN with the first range all NaN and the
	 * second range null
	 */
	@Test
	public void ignoringnanFirstNaNSecondNull() {

		Range rang1 = new Range(Double.NaN, Double.NaN);
		Range rang2 = null;
		Range expected = null;
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine initilized range with a null range", expected, result);
	}

	@Test
	public void combineWithOneNanUpper() {

		Range rang1 = new Range(10.0, Double.NaN);
		Range rang2 = new Range(5.0, 15);
		Range expected = new Range(5.0, 15);
		;
		Range result = Range.combineIgnoringNaN(rang2, rang1);
		assertEquals("Combine should result in a range between 5 and 15", expected, result);
	}

	@Test
	public void combineWithOneNanLower() {

		Range rang1 = new Range(Double.NaN, 10.0);
		Range rang2 = new Range(5.0, 15);
		Range expected = new Range(5.0, 15);
		;
		Range result = Range.combineIgnoringNaN(rang2, rang1);
		assertEquals("Combine should result in a range between 5 and 15", expected, result);
	}

	@Test
	public void combineWithOneNanUpperSecondRange() {

		Range rang1 = new Range(10.0, Double.NaN);
		Range rang2 = new Range(5.0, 15);
		Range expected = new Range(5.0, 15);
		;
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine should result in a range between 5 and 15", expected, result);
	}

	@Test
	public void combineWithOneNanLowerSecondRange() {

		Range rang1 = new Range(Double.NaN, 10.0);
		Range rang2 = new Range(5.0, 15);
		Range expected = new Range(5.0, 15);
		;
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine should result in a range between 5 and 15", expected, result);
	}

	/**
	 * A method testing the combineIgnoringNaN with the second range all NaN and the
	 * first range null
	 */
	@Test
	public void ignoringnanFirstNullSecondNaN() {

		Range rang2 = new Range(Double.NaN, Double.NaN);
		Range rang1 = null;
		Range expected = null;
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine initilized range with a null range", expected, result);
	}

	/**
	 * A method testing the combineIgnoringNaN with both ranges all NaN
	 */
	@Test
	public void ignoringnanBothNaN() {

		Range rang2 = new Range(Double.NaN, Double.NaN);
		Range rang1 = new Range(Double.NaN, Double.NaN);
		Range expected = null;
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine initilized range with a null range", expected, result);
	}

	/**
	 * A method testing the combineIgnoringNaN with all the ranges null
	 */
	@Test
	public void ignoringnanBothNull() {

		Range rang2 = null;
		Range rang1 = null;
		Range expected = null;
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine null range with a null range", expected, result);
	}

	/**
	 * A method testing the combineIgnoringNaN with the ranges lower value is NaN
	 */
	@Test
	public void ignoringnanLowerNan() {

		Range rang2 = new Range(Double.NaN, 5.0);
		Range rang1 = new Range(Double.NaN, 10.0);
		Range expected = new Range(Double.NaN, 10.0);
		Range result = Range.combineIgnoringNaN(rang1, rang2);
		assertEquals("Combine initialized NaN range with a initialized NaN range", expected.getLowerBound(),
				result.getLowerBound(), 0.0001);
		assertEquals("Combine initialized NaN range with a initialized NaN range", expected.getUpperBound(),
				result.getUpperBound(), 0.0001);
	}

	// expandToInclude(Range range, double value)

	/**
	 * A method testing expandToInclude with the value above the range
	 */
	@Test
	public void expandToIncludeAbove() {
		Range rang = new Range(5, 10);
		Range result = Range.expandToInclude(rang, 15);
		Range expected = new Range(5, 15);
		assertEquals("expandToInclude with value above range", expected, result);
	}

	@Test
	/**
	 * A method testing expandToInclude with the value below the range
	 */
	public void expandToIncludeBelow() {
		Range rang = new Range(5, 10);
		Range result = Range.expandToInclude(rang, 1);
		Range expected = new Range(1, 10);
		assertEquals("expandToInclude with value below range", expected, result);
	}

	/**
	 * A method testing expandToInclude with a null range and value
	 */
	@Test
	public void expandToIncludeNull() {
		Range rang = null;
		Range result = Range.expandToInclude(rang, 15);
		Range expected = new Range(15, 15);
		assertEquals("expandToInclude with value and null range", expected, result);
	}

	/**
	 * A method testing expandToInclude with a value already inside the range
	 */
	@Test
	public void expandToIncludeInside() {
		Range rang = new Range(5, 10);
		Range result = Range.expandToInclude(rang, 7);
		Range expected = new Range(5, 10);
		assertEquals("expandToInclude with value already inside range", expected, result);
	}

	// expand(Range range, double lowerMargin, double upperMargin)

	/**
	 * A method testing expand with the lowerbound moving down and the upperbound
	 * moving up
	 */
	@Test
	public void expandNull() {
		boolean testResult = false;
		try {
			Range.expand(null, 0.05, 0.05);
		} catch (IllegalArgumentException e) {
			testResult = true;
		} finally {
			assertEquals("Expand from null results in an error", true, testResult);
		}
	}

	/**
	 * A method testing expand with the lower bound moving down and the upper bound
	 * moving up
	 */
	@Test
	public void expandEqual() {
		Range rang = new Range(5, 10);
		Range result = Range.expand(rang, 0.05, 0.05);
		Range expected = new Range(4.75, 10.25);
		assertEquals("expand with equal margins", expected, result);
	}

	/**
	 * A method testing expand with the lower bound moving down and the upper bound
	 * moving up at different rate
	 */
	@Test
	public void expandChangeAtDifferentRate() {
		Range rang = new Range(5, 10);
		Range result = Range.expand(rang, 0.05, 0.15);
		Range expected = new Range(4.75, 10.75);
		assertEquals("expand with unequal margins", expected, result);
	}

	/**
	 * A method testing expand with the lower bound moving higher than the upper
	 * bound
	 */
	@Test
	public void expandLowerBecomesBigger() {
		Range rang = new Range(5, 10);
		Range result = Range.expand(rang, -2, 0);
		Range expected = new Range(12.5, 12.5);
		assertEquals("expand with positive values", expected, result);
	}

	// shift(Range base, double delta)

	@Test
	public void shiftNull() {
		boolean testResult = false;
		try {
			Range.shift(null, 5);
		} catch (IllegalArgumentException e) {
			testResult = true;
		} finally {
			assertEquals("Shift from null results in an error", true, testResult);
		}
	}

	/**
	 * A method testing shift which used shift(Range base, double delta, boolean
	 * allowZeroCrossing) to complete.
	 */
	@Test
	public void shiftBasicValue() {
		Range rang = new Range(5, 10);
		Range result = Range.shift(rang, 5);
		Range expected = new Range(10, 15);
		assertEquals("shiftBasicValue with positive delta", expected, result);
	}

	// scale(Range base, double factor)

	@Test
	public void scaleNull() {
		boolean testResult = false;
		try {
			Range.scale(null, 2);
		} catch (IllegalArgumentException e) {
			testResult = true;
		} finally {
			assertEquals("Scale from null results in an error", true, testResult);
		}
	}

	/**
	 * A method testing scale with a positive scaling factor
	 */
	@Test
	public void scalePositiveFactor() {
		Range rang = new Range(5, 10);
		Range result = Range.scale(rang, 2);
		Range expected = new Range(10, 20);
		assertEquals("scale with positive factor", expected, result);
	}

	@Test
	public void scaleZeroFactor() {
		Range rang = new Range(5, 10);
		Range result = Range.scale(rang, 0);
		Range expected = new Range(0, 0);
		assertEquals("scale with positive factor", expected, result);
	}

	@Rule
	public final ExpectedException exception = ExpectedException.none();

	/**
	 * A method testing scale with a negative scaling factor. Exception is thrown by
	 * scale when it is negative.
	 */
	@Test
	public void scaleNegativeFactor() {
		Range rang = new Range(5, 10);
		exception.expect(IllegalArgumentException.class);
		Range.scale(rang, -2);
	}

	// toString

	/**
	 * A method testing toString functionality of range class
	 */
	@Test
	public void toStringTest() {
		Range rang = new Range(5, 10);
		assertEquals(rang.toString(), "Range[" + 5.0 + "," + 10.0 + "]");
	}

	// isNaNRange

	@Test
	/**
	 * Test isNaNRange with a non NaN range.
	 */
	public void isNaNRangeFalse() {
		Range rang = new Range(5, 10);
		assertEquals(rang.isNaNRange(), false);
	}

	@Test
	/**
	 * Test isNaNRange does not change the input range.
	 */
	public void isNaNRangeFalseNoChange() {
		Range rang = new Range(5, 10);
		rang.isNaNRange();
		assertEquals(rang, new Range(5, 10));
	}

	@Test
	/**
	 * Test isNaNRange with a NaN range.
	 */
	public void isNaNRangeTrue() {
		Range rang = new Range(Double.NaN, Double.NaN);
		assertEquals(rang.isNaNRange(), true);
	}

	// hashCode

	@Test
	public void hashCodeTest() {
		Range rang = new Range(5, 10);
		assertEquals(rang.hashCode(), -2107113472);
	}
}
