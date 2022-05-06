package org.jfree.data;

import static org.junit.Assert.*;

import org.jfree.data.DataUtilities;
import org.jfree.data.KeyedValues;
import org.jfree.data.Values2D;
import org.jmock.*;
import org.junit.*;

public class DataUtilitiesTest_v2 {
	private Mockery mockingContext;

	@Before
	public void setUp() {
		mockingContext = new Mockery();
	}

	// createNumberArray

	@Test
	/**
	 * A method for testing the createNumberArray(double[] data) method when the
	 * input is null.
	 */
	public void createNumberArrayForNULL() {
		boolean testResult = false;
		try {
			DataUtilities.createNumberArray(null);
		} catch (Exception e) {
			testResult = true;
		} finally {
			assertEquals("Creating Number array from null results in an error", true, testResult);
		}
	}

	@Test
	/**
	 * A method for testing the createNumberArray(double[][] data) method when the
	 * input is 1 value.
	 */
	public void createNumberArrayOfSize1() {
		double[] input = { 10.5 };
		Number[] expected = { 10.5 };
		Number[] result = DataUtilities.createNumberArray(input);
		assertArrayEquals("One value double array creates a one value Number array", expected, result);
	}

	@Test
	/**
	 * A method for testing the createNumberArray(double[] data) method when the
	 * input is 10 values.
	 */
	public void createNumberArrayOfSize10() {
		double[] input = new double[10];
		Number[] expected = new Number[10];
		for (int n = 0; n < 10; n++) {
			input[n] = (n - 5) * 1.5;
			expected[n] = (n - 5) * 1.5;
		}
		Number[] result = DataUtilities.createNumberArray(input);
		assertArrayEquals("Ten value double array creates a ten value Number array", expected, result);
	}

	// createNumberArray2D

	@Test
	/**
	 * A method for testing the createNumberArray2D(double[][] data) method when the
	 * input is null.
	 */
	public void createNumberArray2DForNULL() {
		boolean testResult = false;
		try {
			DataUtilities.createNumberArray2D(null);
		} catch (Exception e) {
			testResult = true;
		} finally {
			assertEquals("Creating 2D Number array from null results in an error", true, testResult);
		}
	}

	@Test
	/**
	 * A method for testing the createNumberArray2D(double[][] data) method when the
	 * input has minimum minimum values.
	 */
	public void createNumberArray2DFor1By1Array() {
		double[][] input = { { 10.5 } };
		Number[][] expected = { { 10.5 } };
		Number[][] result = DataUtilities.createNumberArray2D(input);
		assertArrayEquals(expected, result);
	}

	@Test
	/**
	 * A method for testing the createNumberArray2D(double[][] data) method when the
	 * input has minimum high values.
	 */
	public void createNumberArray2DFor1By10Array() {
		double[][] input = new double[1][10];
		Number[][] expected = new Number[1][10];
		for (int n = 0; n < 10; n++) {
			input[0][n] = n * 1.5;
			expected[0][n] = n * 1.5;
		}
		Number[][] result = DataUtilities.createNumberArray2D(input);
		assertArrayEquals(expected, result);
	}

	@Test
	/**
	 * A method for testing the createNumberArray2D(double[][] data) method when the
	 * input has high minimum values.
	 */
	public void createNumberArray2DFor10By1Array() {
		double[][] input = new double[10][1];
		Number[][] expected = new Number[10][1];
		for (int n = 0; n < 10; n++) {
			input[n][0] = n * 1.5;
			expected[n][0] = n * 1.5;
		}
		Number[][] result = DataUtilities.createNumberArray2D(input);
		assertArrayEquals(expected, result);
	}

	@Test
	/**
	 * A method for testing the createNumberArray2D(double[][] data) method when the
	 * input has high high values.
	 */
	public void createNumberArray2DFor10By10Array() {
		double[][] input = new double[10][10];
		Number[][] expected = new Number[10][10];
		for (int m = 0; m < 10; m++) {
			for (int n = 0; n < 10; n++) {
				input[m][n] = n * 1.5 + m;
				expected[m][n] = n * 1.5 + m;
			}
		}
		Number[][] result = DataUtilities.createNumberArray2D(input);
		assertArrayEquals(expected, result);
	}

	// equal

	/**
	 * A method for testing the equal(double[][]a, double[][]b) method when the
	 * input is low low values.
	 */
	@Test
	public void equalFor1By1ArrayTest() {
		double[][] array1 = { { 10.5 } };
		double[][] array2 = { { 10.5 } };
		assertEquals(DataUtilities.equal(array1, array2), true);
		array1[0][0] = -1.55;
		assertEquals(DataUtilities.equal(array1, array2), false);
	}

	@Test
	/**
	 * A method for testing the equal(double[][]a, double[][]b) method when the
	 * input is low high values.
	 */
	public void equalFor1By10ArrayTest() {
		double[][] array1 = new double[1][10];
		double[][] array2 = new double[1][10];
		for (int i = 0; i < 10; i++) {
			array1[0][i] = i * 1.5;
			array2[0][i] = i * 1.5;
		}
		assertEquals(DataUtilities.equal(array1, array2), true);
	}

	@Test
	/**
	 * A method for testing the equal(double[][]a, double[][]b) method when the
	 * input is high low values.
	 */
	public void equalFor10By1ArrayTest() {
		double[][] array1 = new double[10][1];
		double[][] array2 = new double[10][1];
		for (int i = 0; i < 10; i++) {
			array1[i][0] = i * 1.5;
			array2[i][0] = i * 1.5;
		}
		assertEquals(DataUtilities.equal(array1, array2), true);
	}

	@Test
	/**
	 * A method for testing the equal(double[][]a, double[][]b) method when the
	 * input is high high values.
	 */
	public void equalFor10By10ArrayTest() {
		double[][] array1 = new double[10][10];
		double[][] array2 = new double[10][10];
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				array1[i][j] = i * 1.5 + j;
				array2[i][j] = i * 1.5 + j;
			}
		}
		assertEquals(DataUtilities.equal(array1, array2), true);
	}

	@Test
	/**
	 * A method for testing the equal(double[][]a, double[][]b) method when the
	 * input values are not equal.
	 */
	public void notEqualFor10By10ArrayTest() {
		double[][] array1 = new double[10][10];
		double[][] array2 = new double[10][10];
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				array1[i][j] = i * 1.5 + j;
				array2[i][j] = i * 2.5 + j;
			}
		}
		assertEquals(DataUtilities.equal(array1, array2), false);
	}

	@Test
	/**
	 * A method for testing the equal(double[][]a, double[][]b) method when both
	 * inputs are null.
	 */
	public void equalForBothValuesNull() {
		double[][] array1 = null;
		double[][] array2 = null;
		assertEquals(DataUtilities.equal(array1, array2), true);
	}

	@Test
	/**
	 * A method for testing the equal(double[][]a, double[][]b) method when the
	 * first value is null, but the second isn't.
	 */
	public void equalForFirstValueNull() {
		double[][] array1 = new double[10][10];
		double[][] array2 = null;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				array1[i][j] = i * 1.5 + j;
			}
		}
		assertEquals(DataUtilities.equal(array2, array1), false);
	}

	@Test
	/**
	 * A method for testing the equal(double[][]a, double[][]b) method when the
	 * second value is null, but the first isn't.
	 */
	public void equalForSecondValueNull() {
		double[][] array1 = new double[10][10];
		double[][] array2 = null;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				array1[i][j] = i * 1.5 + j;
			}
		}
		assertEquals(DataUtilities.equal(array1, array2), false);
	}

	@Test
	/**
	 * A method for testing the equal(double[][]a, double[][]b) method when the
	 * inputs have unequal length.
	 */
	public void equalForInequalLengths() {
		double[][] array1 = new double[5][5];
		double[][] array2 = new double[10][10];
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (j < 5 && i < 5)
					array1[i][j] = i * 1.5 + j;
				array2[i][j] = i * 1.5 + j;
			}
		}
		assertEquals(DataUtilities.equal(array1, array2), false);
	}

	// getCumulativePercentages

	/**
	 * A method for testing the getCumulativePercentages(KeyedValues data) method
	 * when the inputs are positive.
	 */
	@Test
	public void getCumulativePercentagesForThreeValues() {
		KeyedValues values = mockingContext.mock(KeyedValues.class);
		mockingContext.checking(new Expectations() {
			{
				allowing(values).getItemCount();
				will(returnValue(3));

				allowing(values).getValue(0);
				will(returnValue(2));
				allowing(values).getKey(0);
				will(returnValue(0));

				allowing(values).getValue(1);
				will(returnValue(1));
				allowing(values).getKey(1);
				will(returnValue(1));

				allowing(values).getValue(2);
				will(returnValue(2));
				allowing(values).getKey(2);
				will(returnValue(2));
			}
		});

		KeyedValues result = DataUtilities.getCumulativePercentages(values);
		assertEquals("First index has 2 thus 2/5 result should be 0.4", 0.4, result.getValue(0));
		assertEquals("Second index has 1 thus 3/5 result should be 0.6", 0.6, result.getValue(1));
		assertEquals("Third index has 2 thus 5/5 result should be 1.0", 1.0, result.getValue(2));
	}

	@Test
	/**
	 * A method for testing the getCumulativePercentages(KeyedValues data) method
	 * when the inputs are zero.
	 */
	public void getCumulativePercentagesForZeroValues() {
		KeyedValues values = mockingContext.mock(KeyedValues.class);
		mockingContext.checking(new Expectations() {
			{
				allowing(values).getItemCount();
				will(returnValue(0));
			}
		});

		KeyedValues result = DataUtilities.getCumulativePercentages(values);
		assertEquals("Empty input results in an empty output", 0, result.getItemCount());
	}

	// calculateColumnTotal
	
	@Test
	/**
	 * A method for testing the calculateColumnTotal(Values2D data, int column)
	 * method when the rows contain a null value.
	 */
	public void calculateColumnTotalForNullValues() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getRowCount();
				will(returnValue(2));
				oneOf(values).getValue(0, 0);
				will(returnValue(null));
				oneOf(values).getValue(1, 0);
				will(returnValue(2.5));
			}
		});
		double result = DataUtilities.calculateColumnTotal(values, 0);
		assertEquals("First comlum has value 2.5 which results in 2.5", 2.5, result, .01d);
	}

	@Test
	/**
	 * A method for testing the calculateColumnTotal(Values2D data, int column)
	 * method when the number of rows is above the boundary of 1.
	 */
	public void calculateColumnTotalForTwoValues() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getRowCount();
				will(returnValue(2));
				oneOf(values).getValue(0, 0);
				will(returnValue(1.25));
				oneOf(values).getValue(1, 0);
				will(returnValue(2.5));
			}
		});
		double result = DataUtilities.calculateColumnTotal(values, 0);
		assertEquals("First comlum has values 1.25 and 2.5 which resultsin 3.75", 3.75, result, .01d);
	}

	@Test
	/**
	 * A method for testing the calculateColumnTotal(Values2D data, int column)
	 * method when the number of rows is above the boundary of 1 has negative
	 * values.
	 */
	public void calculateColumnTotalForNegativeValues() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getRowCount();
				will(returnValue(3));
				oneOf(values).getValue(0, 0);
				will(returnValue(1.25));
				oneOf(values).getValue(1, 0);
				will(returnValue(2.5));
				oneOf(values).getValue(2, 0);
				will(returnValue(-5.5));
			}
		});
		double result = DataUtilities.calculateColumnTotal(values, 0);
		assertEquals("First comlum has values -5.5, 1.25 and 2.5 which resultsin -1.75", -1.75, result, .01d);
	}

	@Test
	/**
	 * A method for testing the calculateColumnTotal(Values2D data, int column)
	 * method when the number of rows is on the boundary of 1.
	 */
	public void calculateColumnTotalForOneValues() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getRowCount();
				will(returnValue(1));
				oneOf(values).getColumnCount();
				will(returnValue(2));
				oneOf(values).getValue(0, 1);
				will(returnValue(1.25));
			}
		});
		double result = DataUtilities.calculateColumnTotal(values, 1);
		assertEquals("Second column has only 1.25", 1.25, result, .01d);
	}

	@Test
	/**
	 * A method for testing the calculateColumnTotal(Values2D data, int column)
	 * method when the number of rows is below the boundary of 1.
	 */
	public void calculateColumnTotalForZeroValues() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getRowCount();
				will(returnValue(0));
			}
		});
		double result = DataUtilities.calculateColumnTotal(values, 0);
		assertEquals("Empty input should return 0", 0, result, .01d);
	}

	// clone
	/**
	 * A method for testing the clone(double [][] source) method when the row values
	 * are null.
	 */
	@Test
	public void cloneForNullRowArrayTest() {
		double[][] array = new double[10][];
		double[][] result = DataUtilities.clone(array);
		assertArrayEquals(result, array);
	}

	/**
	 * A method for testing the clone(double [][] source) method when the values are
	 * low low.
	 */
	@Test
	public void cloneFor1By1ArrayTest() {
		double[][] array = { { 10.5 } };
		double[][] result = DataUtilities.clone(array);
		assertArrayEquals(result, array);
		array[0][0] = -1.5;
		assertNotEquals(array, result);
	}

	@Test
	/**
	 * A method for testing the clone(double [][] source) method when the values are
	 * low high.
	 */
	public void cloneFor1By10ArrayTest() {
		double[][] array = new double[1][10];
		for (int i = 0; i < 10; i++) {
			array[0][i] = i * 1.5;
		}
		double[][] result = DataUtilities.clone(array);
		assertArrayEquals(result, array);
	}

	@Test
	/**
	 * A method for testing the clone(double [][] source) method when the values are
	 * high low.
	 */
	public void cloneFor10By1ArrayTest() {
		double[][] array = new double[10][1];
		for (int i = 0; i < 10; i++) {
			array[i][0] = i * 1.5;
		}
		double[][] result = DataUtilities.clone(array);
		assertArrayEquals(result, array);
	}

	@Test
	/**
	 * A method for testing the clone(double [][] source) method when the values are
	 * high high.
	 */
	public void cloneFor10By10ArrayTest() {
		double[][] array = new double[10][10];
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				array[i][j] = i * 1.5 + j;
			}
		}
		double[][] result = DataUtilities.clone(array);
		assertArrayEquals(result, array);
	}

	@Test
	/**
	 * A method for testing the clone(double [][] source) method when the values are
	 * not cloned and others are cloned.
	 */
	public void notCloneFor10By10ArrayTest() {
		double[][] array1 = new double[10][10];
		double[][] array2 = new double[10][10];
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				array1[i][j] = i * 1.5 + j;
				array2[i][j] = i * 2.5 + j;
			}
		}
		double[][] result = DataUtilities.clone(array1);
		assertArrayEquals(result, array1);
		assertNotEquals(array2, result);
	}

	// calculateRowTotal
	
	@Test
	/**
	 * A method for testing the calculateRowTotal(Values2D data, int row) method
	 * when there are two columns with a null value and the result is positive.
	 */
	public void calculateRowTotalForNullValue() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getColumnCount();
				will(returnValue(2));
				oneOf(values).getValue(0, 0);
				will(returnValue(null));
				oneOf(values).getValue(0, 1);
				will(returnValue(2.5));
			}
		});
		double result = DataUtilities.calculateRowTotal(values, 0);
		assertEquals("First comlum has value 2.5 which results in 2.5", 2.5, result, .01d);
	}

	@Test
	/**
	 * A method for testing the calculateRowTotal(Values2D data, int row) method
	 * when there are two columns and the result is positive.
	 */
	public void calculateRowTotalForTwoValues() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getColumnCount();
				will(returnValue(2));
				oneOf(values).getValue(0, 0);
				will(returnValue(1.25));
				oneOf(values).getValue(0, 1);
				will(returnValue(2.5));
			}
		});
		double result = DataUtilities.calculateRowTotal(values, 0);
		assertEquals("First comlum has values 1.25 and 2.5 which results in 3.75", 3.75, result, .01d);
	}

	@Test
	/**
	 * A method for testing the calculateRowTotal(Values2D data, int row) method
	 * when the number of rows is above the boundary of 1 has negative values.
	 */
	public void calculateRowTotalForNegativeValues() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getColumnCount();
				will(returnValue(3));
				oneOf(values).getValue(0, 0);
				will(returnValue(1.25));
				oneOf(values).getValue(0, 1);
				will(returnValue(2.5));
				oneOf(values).getValue(0, 2);
				will(returnValue(-5.5));
			}
		});
		double result = DataUtilities.calculateRowTotal(values, 0);
		assertEquals("First comlum has values -5.5, 1.25 and 2.5 which resultsin -1.75", -1.75, result, .01d);
	}

	@Test
	/**
	 * A method for testing the calculateRowTotal(Values2D data, int row) method
	 * when the number of rows is on the boundary of 1.
	 */
	public void calculateRowTotalForOneValues() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getColumnCount();
				will(returnValue(1));
				oneOf(values).getRowCount();
				will(returnValue(2));
				oneOf(values).getValue(1, 0);
				will(returnValue(1.25));
			}
		});
		double result = DataUtilities.calculateRowTotal(values, 1);
		assertEquals("Second column has only 1.25", 1.25, result, .01d);
	}

	@Test
	/**
	 * A method for testing the calculateRowTotal(Values2D data, int row) method
	 * when the number of rows is below the boundary of 1.
	 */
	public void calculateRowTotalForZeroValues() {
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getColumnCount();
				will(returnValue(0));
			}
		});
		double result = DataUtilities.calculateRowTotal(values, 0);
		assertEquals("Empty input should return 0", 0, result, .01d);
	}

	// calculateColumnTotal validRows
	
	/**
	 * A method for testing the calculateColumnTotal with valid rows argument
	 * the input has a null value among them
	 */
	@Test
	public void calculateColumnTotalValidRowsForNullValue() {
		int[] validRows = {0,2};
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getRowCount();
				will(returnValue(3));
				oneOf(values).getValue(0, 0);
				will(returnValue(null));
				oneOf(values).getValue(1, 0);
				will(returnValue(2.5));
				oneOf(values).getValue(2, 0);
				will(returnValue(1.5));

			}
		});
		double result = DataUtilities.calculateColumnTotal(values, 0, validRows);
		assertEquals(1.5, result, .01d);
	}
	
	// calculateRowTotal validColumns
	
	/**
	 * A method for testing the calculateRowTotal with valid columns argument
	 * the input has a null value among them
	 */
	@Test
	public void calculateRowTotalValidColumnsForNullValue() {
		int[] validColumns = {0,2};
		Values2D values = mockingContext.mock(Values2D.class);
		mockingContext.checking(new Expectations() {
			{
				oneOf(values).getColumnCount();
				will(returnValue(3));
				oneOf(values).getValue(0, 0);
				will(returnValue(null));
				oneOf(values).getValue(0, 1);
				will(returnValue(2.5));
				oneOf(values).getValue(0, 2);
				will(returnValue(1.5));

			}
		});
		double result = DataUtilities.calculateRowTotal(values, 0, validColumns);
		assertEquals(1.5, result, .01d);
	}
	
}
