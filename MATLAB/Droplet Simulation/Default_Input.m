function [void] = Default_Input(void)

% This function defines the constant parameters of a droplet experiment as
% global variables to be used by the Core. Individual wrappers can alter
% some or all of these, but these are the defaults.

% NOTE: 'n' needs to be tuned with experimental data

clear variables
close all

global m
global m1_ratio
global m2_ratio
global k
global r
global b
global Az
global Ax
global f
global g
global phase_angle
global visc
global air_gap
global dt
global run_time
global z0
global vz0
global x0
global vx0
global n

m = 5.24E-7; % total droplet mass (kg)
m1_ratio = 0.5; % ratio of mass on top
m2_ratio = 0.5; % ratio of mass on bottom
k = 0.15; % droplet elasticity (N/m)
r = 5E-4; % droplet radius (m)
b = 2E-4; % droplet damping (N-s/m)
Az = 1E-3; % plate amplitude z (m)
Ax = 1E-3; % plate amplitude x (m)
f = 26; % plate frequency (Hz)
g = 9.81; % gravity (m/s^2)
phase_angle = 0; % phase angle (degrees)
visc = 1.8E-5; % dynamic viscosity of air at room termperature (kg/m-s)
air_gap = 4E-6; % air gap approximation (m)
dt = 1/10000; % time step (s)
run_time = 0.25; % run time (s)
z0 = [3E-3 3E-3-r]'; % initial vertical position (m) [upper lower]
vz0 = [0 0]'; % initial vertical velocity (m/s) [upper lower]
x0 = 0; % initial horizontal position (m)
vx0 = 0; % initial horizontal velocity (m/s)
n = 1; % tuning parameter for horizontal motion

end