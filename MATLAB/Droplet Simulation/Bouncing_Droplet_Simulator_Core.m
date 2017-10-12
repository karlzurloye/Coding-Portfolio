
function [data] = Bouncing_Droplet_Simulator_Core() 

%% Referencing global constants defined in wrapper

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

m1 = (m1_ratio)*m; % upper droplet mass (kg)
m2 = (m2_ratio)*m; % lower droplet mass (kg)
w = f*2*pi; % plate frequency (rad/s)
phase = phase_angle*pi/180; % phase angle (radians)

%% Initializing matrices for memory preallocation

t = 0:dt:run_time; % time vector
z1 = [[z0(1) vz0(1)]' zeros(2,length(t)-1)]; % upper mass [position velocity]
z2 = [[z0(2) vz0(2)]' zeros(2,length(t)-1)]; % lower mass [position velocity]
x = [[x0 vx0]' zeros(2,length(t)-1)]; % both masses [position velocity]
k1z1 = zeros(2,1); % upper mass [velocity acceleration]
k1z2 = zeros(2,1); % lower mass [velocity acceleration]
k2z1 = zeros(2,1); % upper mass [velocity acceleration]
k2z2 = zeros(2,1); % lower mass [velocity acceleration]
k3z1 = zeros(2,1); % upper mass [velocity acceleration]
k3z2 = zeros(2,1); % lower mass [velocity acceleration]
k4z1 = zeros(2,1); % upper mass [velocity acceleration]
k4z2 = zeros(2,1); % lower mass [velocity acceleration]
dz1 = zeros(2,1); % upper mass [velocity acceleration]
dz2 = zeros(2,1); % lower mass [velocity acceleration]
k1x = zeros(2,1); % both masses [velocity acceleration]
k2x = zeros(2,1); % both masses [velocity acceleration]
k3x = zeros(2,1); % both masses [velocity acceleration]
k4x = zeros(2,1); % both masses [velocity acceleration]
dx = zeros(2,1); % both masses [velocity acceleration]
contact = 0;

%% Time step loop

for ii = 1:length(t)-1
    
    % Storing trigonometric calculations for efficiency
    plate_z = Az*sin(w*t(ii));
    plate_vz = Az*w*cos(w*t(ii));
    plate_az = -Az*w^2*sin(w*t(ii));
    plate_az_half = -Az*w^2*sin(w*(t(ii)+dt/2));
    plate_az_full = -Az*w^2*sin(w*t(ii+1));
    
    plate_vx = Ax*w*cos(w*t(ii)+phase);
    plate_vx_half = Ax*w*cos(w*(t(ii)+dt/2)+phase);
    plate_vx_full = Ax*w*cos(w*t(ii+1)+phase);
    
    % Determining if droplet is in contact with plate
    if  z2(1,ii) <= plate_z;
        z2(1,ii) = plate_z;
        z2(2,ii) = plate_vz;
        contact = 1;      
    end
    
    % Determining if droplet will break contact with plate during time step
    if (b*(z1(2,ii)-z2(2,ii)) + k*(z1(1,ii)-z2(1,ii)-r) - m2*g)/m2 >= plate_az
        contact = 0;
    end
    
    % RK4 for vertical position and velocity
    k1z1(2) = -(b*(z1(2,ii)-z2(2,ii)) + k*(z1(1,ii)-z2(1,ii)-r) + m1*g)/m1; 
    k1z1(1) = z1(2,ii);
    k1z2(2) = (b*(z1(2,ii)-z2(2,ii)) + k*(z1(1,ii)-z2(1,ii)-r) - m2*g)/m2; 
    k1z2(1) = z2(2,ii);
    
    k2z1(2) = -(b*((z1(2,ii)+k1z1(2)*dt/2)-(z2(2,ii)+k1z2(2)*dt/2))...
        + k*((z1(1,ii)+k1z1(1)*dt/2)-(z2(1,ii)+k1z2(1)*dt/2)-r) + m1*g)/m1; 
    k2z1(1) = z1(2,ii)+k1z1(2)*dt/2;
    k2z2(2) = (b*((z1(2,ii)+k1z1(2)*dt/2)-(z2(2,ii)+k1z2(2)*dt/2))...
        + k*((z1(1,ii)+k1z1(1)*dt/2)-(z2(1,ii)+k1z2(1)*dt/2)-r) - m2*g)/m2; 
    k2z2(1) = z2(2,ii)+k1z2(2)*dt/2;
    
    k3z1(2) = -(b*((z1(2,ii)+k2z1(2)*dt/2)-(z2(2,ii)+k2z2(2)*dt/2))...
        + k*((z1(1,ii)+k2z1(1)*dt/2)-(z2(1,ii)+k2z2(1)*dt/2)-r) + m1*g)/m1; 
    k3z1(1) = z1(2,ii)+k2z1(2)*dt/2;
    k3z2(2) = (b*((z1(2,ii)+k2z1(2)*dt/2)-(z2(2,ii)+k2z2(2)*dt/2))...
        + k*((z1(1,ii)+k2z1(1)*dt/2)-(z2(1,ii)+k2z2(1)*dt/2)-r) - m2*g)/m2; 
    k3z2(1) = z2(2,ii)+k2z2(2)*dt/2;
    
    k4z1(2) = -(b*((z1(2,ii)+k3z1(2)*dt)-(z2(2,ii)+k3z2(2)*dt))...
        + k*((z1(1,ii)+k3z1(1)*dt)-(z2(1,ii)+k3z2(1)*dt)-r) + m1*g)/m1; 
    k4z1(1) = z1(2,ii)+k3z1(2)*dt;
    k4z2(2) = (b*((z1(2,ii)+k3z1(2)*dt)-(z2(2,ii)+k3z2(2)*dt))...
        + k*((z1(1,ii)+k3z1(1)*dt)-(z2(1,ii)+k3z2(1)*dt)-r) - m2*g)/m2; 
    k4z2(1) = z2(2,ii)+k3z2(2)*dt;
    
    dz1(2) = (k1z1(2)+2*k2z1(2)+2*k3z1(2)+k4z1(2))/6;
    dz1(1) = (k1z1(1)+2*k2z1(1)+2*k3z1(1)+k4z1(1))/6;
    dz2(2) = (k1z2(2)+2*k2z2(2)+2*k3z2(2)+k4z2(2))/6;
    dz2(1) = (k1z2(1)+2*k2z2(1)+2*k3z2(1)+k4z2(1))/6;
        
    z1(1,ii+1) = z1(1,ii) + dz1(1)*dt;
    z1(2,ii+1) = z1(2,ii) + dz1(2)*dt;
    
    % Setting lower mass vertical position and velocity equal to plate if
    % in contact throughout time step
    if contact == 0
        z2(1,ii+1) = z2(1,ii) + dz2(1)*dt; 
        z2(2,ii+1) = z2(2,ii) + dz2(2)*dt;
    else 
        z2(1,ii+1) = plate_z;
        z2(2,ii+1) = plate_vz;
    end
   
    % RK4 for horizontal position and velocity
    k1x(2) = n*pi*r^2*((m2*(plate_az - k1z2(2)))/7.7569E-5)^2*...
        (plate_vx - x(2,ii))*visc/air_gap/(m2+m1);
    k1x(1) = x(2,ii);
        
    k2x(2) = n*pi*r^2*((m2*(plate_az_half - k2z2(2)))/7.7569E-5)^2*...
        (plate_vx_half - x(2,ii)+k1x(2)*dt/2)*visc/air_gap/(m2+m1);
    k2x(1) = x(2,ii)+k1x(2)*dt/2;
        
    k3x(2) = n*pi*r^2*((m2*(plate_az_half - k3z2(2)))/7.7569E-5)^2*...
        (plate_vx_half - x(2,ii)+k2x(2)*dt/2)*visc/air_gap/(m2+m1);
    k3x(1) = x(2,ii)+k2x(2)*dt/2;
       
    k4x(2) = n*pi*r^2*((m2*(plate_az_full - k4z2(2)))/7.7569E-5)^2*...
        (plate_vx_full - x(2,ii)+k3x(2)*dt)*visc/air_gap/(m2+m1);
    k4x(1) = x(2,ii)+k3x(2)*dt;
        
    dx(1) = (k1x(1)+2*k2x(1)+2*k3x(1)+k4x(1))/6;     
    dx(2) = (k1x(2)+2*k2x(2)+2*k3x(2)+k4x(2))/6;    

    % Setting change in horizontal velocity to zero if not in contact with
    % plate throughout time step
    if contact == 0
        dx(2) = 0;
    end
    
    x(1,ii+1) = x(1,ii) + dx(1)*dt;
    x(2,ii+1) = x(2,ii) + dx(2)*dt;
        
end

%% Exporting relevent data as a structure

data = struct('time',t,'upper_mass_vertical_position',z1(1,:),...
    'upper_mass_vertical_velocity',z1(2,:),'lower_mass_vertical_position'...
    ,z2(1,:), 'lower_mass_vertical_velocity',z2(2,:),'horizontal_position'...
    ,x(1,:), 'horizontal_velocity',x(2,:));

end