/*
Author: Priyanshu Singh
College: IIT DELHI
Branch: Chemical Engineering
Email: ch1221465@iitd.ac.in
*/

/*
This program is written for the term paper of the course CLL121: Thermodynamics for Chemical Engineers
Journal used : Prediction of CO2-CH4-H2S-N2 gas mixtures solubility in brine using a non-iterative
fugacity-activity model relevant to CO2-MEOR Babak Shabani, Javier Vilcáez
DOI: http://dx.doi.org/10.1016/j.petrol.2016.12.012
*/
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <math.h>
#include <random>
using namespace std;

// Constants
const double M_water = 18.01528; // Molar Mass of Water in g/mol;
const double Tc_Water = 647.1;   // Critical Temperature of Water in K
const double Pc_Water = 220.55;  // Critical Pressure of Water in bar
const double Zc_Water = 0.229;   // Critical Compressibility of Water
const double w_Water = 0.345;    // Acentric Factor of Water
const double R = 83.144598;       // Universal Gas Constant in cm^3-bar / mol-K
const double Tc_CO2 = 304.2;     // Critical Temperature of CO2 in K
const double Pc_CO2 = 73.83;     // Critical Pressure of CO2 in bar
const double Zc_CO2 = 0.274;     // Critical Compressibility of CO2
const double w_CO2 = 0.224;      // Acentric Factor of CO2

// Lambda CO2-Na
const double c1 = -0.0652869; // Second order interaction parameters (Ziabakhsh-Ganji and Kooi, 2012).
const double c2 = 1.6790636e-4;
const double c3 = 40.838951;
const double c4 = 0;
const double c5 = 0;
const double c6 = -3.9266518e-2;
const double c7 = 0;
const double c8 = 2.1157167e-2;
const double c9 = 6.5486487e-6;
const double c10 = 0;

// Lambda CO2-K
const double c1_ = -1.144624e-2;
const double c2_ = 2.8274958e-5;
const double c3_ = 0;
const double c4_ = 0;
const double c5_ = 0;
const double c6_ = 1.3980876e-2;
const double c7_ = 0;
const double c8_ = -1.4349005e-2;
const double c9_ = 0;
const double c10_ = 0;

// Parameters for Henry's constant for CO2
const double H = -0.114535;
const double TAU = -5.279063; // cm^3/g
const double BETA = 6.187967; // cm^3 K^0.5/g

// Other Constants
const double sigma = 1 + sqrt(2);
const double epsilon = 1 - sqrt(2);

// Functions for Calculation of Gamma (Activity Coefficient)
double Lambda_Na(double T, double P) // T and Pressure in Kelvin and bar
{

    return c1 + c2 * T + c3 / T + c4 * P + c5 / P + c6 * P / T + c7 * T / (P * P) + c8 * P / (630 - T) + c9 * T * log(P) + c10 * P / (T * T);
}

double Epsilon_Na_Cl(double T, double P) // T and Pressure in Kelvin and bar
{
    return c1_ + c2_ * T + c3_ / T + c4_ * P + c5_ / P + c6_ * P / T + c7_ * T / (P * P) + c8_ * P / (630 - T) + c9_ * T * log(P) + c10_ * P / (T * T);
}

double Gamma(double T, double P, double Ma, double Mc) // T and Pressure in Kelvin and bar and Ma and Mc in mol/dm^3
{
    return exp(2 * Mc * Lambda_Na(T, P) + 2 * Ma * Mc * Epsilon_Na_Cl(T, P));
}

// Functions for Calculation of Z (Compressibility Factor for Vapor Phase)
double Z_for_vapor(double T, double Tc, double P, double Pc, double w) // T in Kelvin and P in bar
{
    double m = 0.37464 + 1.54226 * w - 0.26992 * w * w;
    double a = 0.457326 * (R * R * Tc * Tc / Pc) * pow(1 + m * (1 - sqrt(T / Tc)), 2);
    double b = 0.077796 * R * Tc / Pc;
    double Beta = b * P / (R * T);
    double Q = a / (b * R * T);
    double Z = 1;
    double dZ = 1;
    while (dZ > 1e-6)
    {
        double Zold = Z;
        Z = 1 + Beta - Q * Beta * (Z - Beta) / ((Z + epsilon * Beta) * (Z + sigma * Beta));
        dZ = abs(Z - Zold);
    }
    return Z;
}

// Functions for Calculation of Z (Compressibility Factor for Liquid Phase)
double Z_for_liquid(double T, double Tc, double P, double Pc, double w) // T in Kelvin and P in bar
{
    double m = 0.37464 + 1.54226 * w - 0.26992 * w * w;
    double a = 0.457326 * (R * R * Tc * Tc / Pc) * pow(1 + m * (1 - sqrt(T / Tc)), 2);
    double b = 0.077796 * R * Tc / Pc;
    double Beta = b * P / (R * T);
    double Q = a / (b * R * T);
    double Z = Beta;
    double dZ = 1;
    while (dZ > 1e-6)
    {
        double Zold = Z;
        Z = Beta + (Z + epsilon * Beta) * (Z + sigma * Beta) * (1 + Beta - Z) / (Q * Beta);
        dZ = abs(Z - Zold);
    }
    return Z;
}

// Functions for Calculation of Fugacity Coefficient for Pure Vapor
double fugacityCoefficientVapor(double T, double Tc, double P, double Pc, double w) // T in Kelvin and P in bar
{
    double Z = Z_for_vapor(T, Tc, P, Pc, w);
    double m = 0.37464 + 1.54226 * w - 0.26992 * w * w;
    double a = 0.457326 * (R * R * Tc * Tc / Pc) * pow(1 + m * (1 - sqrt(T / Tc)), 2);
    double b = 0.077796 * R * Tc / Pc;
    double Beta = b * P / (R * T);
    double Q = a / (b * R * T);
    double I =log((Z + sigma * Beta) / (Z + epsilon * Beta))/(sigma - epsilon);
    double lnPhi = Z - 1 - log(Z - Beta) - Q * I;
    return exp(lnPhi);
}

// Functions for calculation of Psat (Saturation Pressure of Water)
double Psat(double T, double Tc, double Pc) // T in Kelvin and P in bar
{
    double T_ = 1 - T / Tc;
    double a1 = -7.8395178, a2 = 1.8440825, a3 = -11.786649, a4 = 22.680741, a5 = -15.9619719, a6 = 1.8012250;
    double lnPsat_Pc = (Tc / T) * (a1 * T_ + a2 * pow(T_, 1.5) + a3 * pow(T_, 3) + a4 * pow(T_, 3.5) + a5 * pow(T_, 4) + a6 * pow(T_, 7.5));
    return Pc * exp(lnPsat_Pc);
}

// Function for calculation of Density of Water in g/cm^3
double DensityOfWater(double T, double P) // T in Kelvin and P in bar
{
    T = T - 273.15;
    double v0 = (1 + 18.1597 * 1e-3 * T) / (0.9998 + 18.2249 * 1e-3 * T - 7.9222 * 1e-6 * T * T - 55.4485 * 1e-9 * T * T * T + 149.7562 * 1e-12 * T * T * T * T - 393.2952 * 1e-15 * T * T * T * T * T);
    double B = 19654.32 + 147.037 * T - 2.2155 * T * T + 1.0478 * 1e-2 * T * T * T - 2.2789 * 1e-5 * T * T * T * T;
    double A1 = 3.2891 - 2.391 * 1e-3 * T + 2.8446 * 1e-4 * T * T - 2.82 * 1e-6 * T * T * T + 8.477 * 1e-9 * T * T * T * T;
    double A2 = 6.245 * 1e-5 - 3.913 * 1e-8 * T - 3.499 * 1e-8 * T * T + 7.942 * 1e-10 * T * T * T - 3.299 * 1e-12 * T * T * T * T;
    return 1 / (v0 - v0 * P / (B + A1 * P + A2 * P * P));
}

// Function for calculation of Fugacity of Pure Water
double fugacityOfPureWater(double T, double P) // T in Kelvin and P in bar
{
    double Psat_ = Psat(T, Tc_Water, Pc_Water);
    return Psat_ * exp(M_water * (P - Psat_) / (R * T * DensityOfWater(T, P)));
}

// Function for calculation of Henry's constant
double Hi(double T, double P) // T in Kelvin and P in bar
{
    double DeltaB = TAU + BETA * pow((1000 / T), 0.5);
    return exp((1 - H) * log(fugacityOfPureWater(T, P)) + H * log(R * T * DensityOfWater(T, P) / M_water) + 2 * DensityOfWater(T, P) * DeltaB);
}

// Function for calculation of mole fraction of CO2 in liquid phase
double X_CO2(double T, double P, double M, double *K_H2O, double *Y_H2O) // T in Kelvin , P in bar and M in mol/dm^3
{
    double h = Hi(T, P);
    double y = Gamma(T, P, M, M);
    double fi = fugacityCoefficientVapor(T, Tc_CO2, P, Pc_CO2, w_CO2);
    double x = P * fi / (h * y);
    double t = T - 273.15;
    double fH2O = fugacityOfPureWater(T, P);
    // Normalizing the mole fraction of CO2
    double K0_H2O = pow(10, (-2.209 + 3.097 * 1e-2 * t - 1.098 * 1e-4 * t * t + 2.048 * 1e-7 * t * t * t));
    double k_H2O = (K0_H2O / (fH2O * P)) * exp((P - 1) * 18.18 / (R * T));
    double y_H2O = (1 - x) / (1 / k_H2O - x);
    double yn_CO2 = 1 / (1 + y_H2O);
    double X_CO2 = yn_CO2 * x;
    *K_H2O = k_H2O;
    *Y_H2O = y_H2O;
    return X_CO2;
}

int main()
{
    ifstream input("input.txt");// for reading input from input.txt file
    ofstream output("output.txt");// for writing output to output.txt file
    double T, P, M;// Temperature, Pressure and Molality
    double K_H2O; // Equilibrium constant for H2O
    double Y_H2O; // Vapor phase mole fraction of H2O
    int testCases; // Number of test cases
    input >> testCases;
    while (testCases--)
    {
        input >> T >> P >> M; // T ,P and M should be in Kelvin, Bar and mol/kg respectively
        double ans = X_CO2(T, P, M, &K_H2O, &Y_H2O);
        output << "Fugacity coefficient for CO2: " << fugacityCoefficientVapor(T, Tc_CO2, P, Pc_CO2, w_CO2) << "\n";
        output << "Henry's constant for CO2: " << Hi(T, P) << " Bar\n";
        output << "Equilibrium constant for H2O: " << K_H2O << "\n";
        output << "Vapor phase mole fraction of H2O Y_H2O: " << Y_H2O << "\n";
        output << "Liquid phase mole fraction of CO2 X_CO2: " << ans << "\n";
        output << "\n";
    }
    return 0;
}
