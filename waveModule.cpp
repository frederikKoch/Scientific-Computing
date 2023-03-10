//waveModule.cpp
//
//Defines the functions that are used to solve the waveequation
#include <cmath>
#include <vector>
#include <cmath>
#include <vector>
#include "wave1d.h"


std::vector<double> initializeX(Parameters param){
    std::vector<double> x (param.ngrid, 0); 
    
    //Calculate an even distribution of x values between first and last point
    for (size_t i = 0; i < param.ngrid; i++) {
        x[i] = (param.x1 + (static_cast<double>(i)*(param.x2-param.x1))/static_cast<double>(param.ngrid-1));
    } 
    return x;
};

void deriveParameters(Parameters &param){
    param.ngrid  = static_cast<size_t>((param.x2-param.x1)/param.dx);// number of x points (rounded down)
    param.dt     = 0.5*param.dx/param.c;                             // time step size
    param.nsteps = static_cast<size_t>(param.runtime/param.dt);      // number of steps to reach runtime (rounded down)
    param.nper   = static_cast<size_t>(param.outtime/param.dt);      // how many steps between snapshots (rounded down)
};



std::vector<double> initializeRho(Parameters param, std::vector<double> x){
    std::vector<double> rho (param.ngrid, 0);
    double xstart = 0.25*(param.x2-param.x1) + param.x1;
    double xmid = 0.5*(param.x2+param.x1);
    double xfinish = 0.75*(param.x2-param.x1) + param.x1;

    //Calculates a triangle wave in between xstart and xstop
    for (size_t i = 0; i < param.ngrid; i++) {
        if (x[i] < xstart or x[i] > xfinish) {
            rho[i] = 0.0;
        } else {
            rho[i] =  0.25 - fabs(x[i]-xmid)/(param.x2-param.x1);
        }
    }
    return rho;
};

std::vector<double> timeStep(std::vector<double> &rho, std::vector<double> &rho_prev, Parameters param){
        std::vector<double> rho_next(param.ngrid, 0);   

        // Set zero Dirichlet boundary conditions
        rho[0] = 0.0;
        rho[param.ngrid-1] = 0.0;

        // Evolve inner region over a time dt using a leap-frog variant
        for (size_t i = 1; i <= param.ngrid-2; i++) {
            double laplacian = pow(param.c/param.dx,2)*(rho[i+1] + rho[i-1] - 2*rho[i]);
            double friction = (rho[i] - rho_prev[i])/param.tau;
            rho_next[i] = 2*rho[i] - rho_prev[i] + param.dt*(laplacian*param.dt-friction);
        }
        return rho_next;
}


