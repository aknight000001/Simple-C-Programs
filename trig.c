/**
    @file trig.c
    @author Alex Knight (ajknigh2)
    This program reads in an inputted double value representing
    an angle and then prints out increasingly accurate approximations of
    the angle applied to sin, cos, and tan in a formatted table by using the 
    Taylor series.
*/
#include <stdio.h>
#include <stdlib.h>

/** Approximation of PI.  For now, we're using our own constant for
    this. There's also one provided by the gnu math library, once we
    learn about that. */
#define PI 3.14159265358979323846

/**
    Allows the program to exit from any function
    @param status The integer value to give as exit status
*/
void exit( int status );


/**
    Reads the angle input from the user and checks to make sure the value
    is a valid, real number within the correct range of numbers. If the value
    is not valid it will tell the user and exit the program. If the number is
    valid it returns the value.
    @return angle The angle value that was input.
*/
double getAngle()
{
    /** Used for storing the angle value that was input */
    double angle;
    
    /** The number of matches to the desired type */
    int matches = scanf( "%lf", &angle );
    
    if( matches == 0 ) {
        printf( "Invalid input\n" );
        exit( 1 );
    }
    else if ( angle <= PI && angle >= -PI ) {
        return angle;
    }
    else {
        printf( "Invalid input\n" );
        exit( 1 );
    }
}

/**
   Used to tell when the current estimate of sine and cosine is accurate
   enough by reporting the positive difference between each current and previous value.
   @param prev The previous value calculated.
   @param curr The current value calculated.
   @return d The difference between the two input values.
*/
double difference( double prev, double curr )
{
    /** The difference between both input values */
    double d; 
    
    d = prev - curr;
    
    if ( d < 0 ) {
        d *= -1;
    }
    
    return d;
}

/**
    Prints the top two lines of the table that are consistent across all
    inputs. Includes the column labels and separating line between the headers
    and all of the numbers.
*/
void tableHeader()
{
    printf( "terms |         sin |         cos |         tan \n" );
    printf( "------+-------------+-------------+-------------\n" );
}

/**
    Prints a row of table values corresponding to the number of terms, the sin, cos,
    and tan values. The row maintains spacing formatting of the top row.
    @param terms The number of terms used in the series to achieve the value.
    @param s The sin value estimated with the current number of terms.
    @param c The cos value estimated with the current number of terms.
    @param t The tan value estimated with the current number of terms.
*/
void tableRow( int terms, double s, double c, double t )
{
    printf( "%5d | %11.7f | %11.7f | %11.7f\n", terms, s, c, t );
}

/**
    Raises a given number to a given power and returns the value
    that is calculated.
    @param n The number being raised to the power.
    @param p The power to raise the number to.
    @return power The calculated value.
*/
double myPow( double n, int p )
{
    /** The value that will be returned */
    double power = 1;
    
    if( p == 0 ) {
        power = 1;
    }
    for( int i = 1; i <= p; i++ ) {
        power *= n;
    }
    
    return power;
}

/**
    Calculates the factorial value of the given number.
    @param n The number to find the factorial of.
    @return fact The value of the factorial that was calculated.
*/
double myFact( int n )
{
    /** The value that will be returned */
    double fact = 1;
    
    if( n == 0 ) {
        fact = 1;
    }
    
    for( int i = n; i >= 1; i-- ) {
        fact *= i;
    }
    
    return fact;
}

/**
    Calculates the sin approximation using the Taylor series
    given the term number.
    @param t The number of terms in the series.
    @param a The angle input to the program
    @return sinTaylor The value that was calculated.
*/
double sinTaylor( int t, double a )
{
    /** The value that will be returned */
    double sTayl = 0;
    
    /** Holds the value of the term */
    int fin = t;
    
    /** A counter variable to keep track of the current term in the series */
    int c = 0;
    
    for( int i = 0; i < fin; i++ ) {
        if( i == 0 ) {
            c++;
        }
        else if( i == 1 ) {
            sTayl += (myPow( a, c ) / myFact( c ));
            c++;
        }
        else if( i == 2 ) {
            c++;
        }
        else if( i == 3 ) {
            sTayl -= myPow( a, c ) / myFact( c );
            i -= 4;
            fin -= 4;
            c++;
        }
    }
    
    return sTayl;
}

/**
    Calculates the cos approximation using the Taylor series
    given the term number.
    @param t The number of terms in the series.
    @param a The angle input to the program.
    @return cTayl The value that was calculated.
*/
double cosTaylor( int t, double a )
{
    /** The value that will be returned */
    double cTayl = 0;
    
    /** Holds the value of the term */
    int fin = t;
    
    /** A counter variable to keep track of the current term in the series */
    int c = 0;
    
    for( int i = 0; i < fin; i++ ) {
        if( i == 0 ) {
            cTayl += (myPow( a, c ) / myFact( c ));
            c++;
        }
        else if( i == 1 ) {
            c++;
        }
        else if( i == 2 ) {
            cTayl -= myPow( a, c ) / myFact( c );
            c++;
        }
        else if( i == 3 ) {
            i -= 4;
            fin -= 4;
            c++;
        }
    }
    
    
    return cTayl;
}

/**
    Uses the Taylor series functions and their helper functions as well as the getAngle,
    difference, tableHeader, and tableRow functions to build a table of increasingly accurate
    approximations of trig values for the given input until a desired accuracy is met.
    @return Program exit status.
  */
int main()
{
    /** The angle value (in radians ) entered by the user */
    double ang = getAngle();
    
    /** The number of terms/iterations through the Taylor series used */
    int term = 1;
    
    /** The threshold for determining if the approximations are accurate enough */
    double threshold = 0.000001;
    
    /** The previous sin value calculated */
    double sPrev;
    
    /** The current sin value calculated */
    double sCurr;
    
    /** The difference between the previous and current sin values */
    double sDiff = 1;
    
    /** The previous cos value calculated */
    double cPrev;
    
    /** The current cos value calculated */
    double cCurr;
    
    /** The difference between the previous and current cos values */
    double cDiff = 1;
    
    /** The current tan value calculated */
    double tVal;
    
    tableHeader();
    
    while ( sDiff > threshold || cDiff > threshold ) {
        if( term == 1 ) {
            sCurr = sinTaylor( term, ang );
            cCurr = cosTaylor( term, ang );
            tVal = sCurr / cCurr;
            tableRow( term, sCurr, cCurr, tVal );
            sPrev = sCurr;
            cPrev = cCurr;
        }
        else {
            sCurr = sinTaylor( term, ang );
            cCurr = cosTaylor( term, ang );
            tVal = sCurr / cCurr;
            tableRow( term, sCurr, cCurr, tVal );
            sDiff = difference( sPrev, sCurr );
            cDiff = difference( cPrev, cCurr );
            sPrev = sCurr;
            cPrev = cCurr;
        }
        term++;
    
    }
    
    
    return EXIT_SUCCESS;
}











