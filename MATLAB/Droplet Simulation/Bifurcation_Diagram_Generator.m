
%% Initializing constants
 
clear variables
close all
hold on

save_mat = false; % toggle whether to save a .mat file of the simulation data
 
r = 2; % normalized droplet radius (unitless)
m1 = 0.5; % upper droplet mass ratio
m2 = 0.5; % lower droplet mass ratio
ksi = 0.26; % dissipation coefficient
omega = 0.5; % elastic coefficient
 
dt = 1/1000; % time step
run_time = 1500; % normalized run time (unitless)
 
ksi1 = ksi/sqrt(m1/0.5); % dissipation coefficient upper mass
ksi2 = ksi/sqrt(m2/0.5); % dissipation coefficient lower mass
omega1 = omega*sqrt(m1/0.5); % elastic coefficient upper mass
omega2 = omega*sqrt(m2/0.5); % elastic coefficient lower mass
 
%% Initializing matrices for memory preallocation
 
t = 0:dt:run_time; % time vector
z0 = [5 5-r]'; % initial position [upper lower]
v0 = [0 0]'; % initial velocity [upper lower]
z1 = [[z0(1) v0(1)]' zeros(2,length(t)-1)]; % upper mass [position velocity]
z2 = [[z0(2) v0(2)]' zeros(2,length(t)-1)]; % lower mass [position velocity]
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
 
gamma_jj = 0:0.05:4.5; % range and resolution of plot
flight_mat = zeros(61,length(gamma_jj));
 
%% Wrapper loop - varies gamma
 
for jj = 1:length(gamma_jj)
    
    gamma = gamma_jj(jj); % setting gamma for each run through the loop
    fprintf('%3.1f%s\n',jj/length(gamma_jj)*100,'%'); % progress report
     
% Initializing / resetting contact variables
flight = zeros(100,1);
contact = 0;
contact_last = 0;
count = 1;
start = 0;
 
%% Time step loop
 
for ii = 1:length(t)-1
    
    % Storing trigonometric calculations for efficiency
    s = sin(t(ii));
    c = cos(t(ii));
    
    % Determining if droplet is in contact with plate
    if z2(1,ii) <= s;
        z2(1,ii) = s;
        z2(2,ii) = c;
        contact_last = contact;
        contact = 1;       
    end
    
    % Determining if droplet will break contact with plate during time step
    if (2*ksi2/omega2*(z1(2,ii)-z2(2,ii)) + (1/omega2^2)*(z1(1,ii)-...
            z2(1,ii)-r) - (1/gamma)) >= -s
        contact_last = contact;
        contact = 0;
    end
     
    % starts count if droplet breaks contact with plate during time step
    % ie. droplet begins flight
    if (contact==0) && (contact_last==1)
        start = t(ii);
    end
    
    % ends count if droplet makes contact with plate during time step
    % ie. droplet flight ends
    if (contact==1) && (contact_last==0)
         flight(count) = t(ii)-start; 
         count = count + 1;
         if count == 101 % each run is capped at 100 flights for efficiency
             break
         end
    end
    
    % RK4 for vertical position and velocity
    k1z1(2) = -(2*ksi1/omega1*(z1(2,ii)-z2(2,ii)) + (1/omega1^2)...
        *(z1(1,ii)-z2(1,ii)-r) + (1/gamma)); 
    k1z1(1) = z1(2,ii);
    k1z2(2) = (2*ksi2/omega2*(z1(2,ii)-z2(2,ii)) + (1/omega2^2)...
        *(z1(1,ii)-z2(1,ii)-r) - (1/gamma)); 
    k1z2(1) = z2(2,ii);
    
    k2z1(2) = -(2*ksi1/omega1*((z1(2,ii)+k1z1(2)*dt/2)-(z2(2,ii)+k1z2(2)*dt/2))...
        + (1/omega1^2)*((z1(1,ii)+k1z1(1)*dt/2)-(z2(1,ii)+k1z2(1)*dt/2)-r)...
        + (1/gamma)); 
    k2z1(1) = z1(2,ii)+k1z1(2)*dt/2;
    k2z2(2) = (2*ksi2/omega2*((z1(2,ii)+k1z1(2)*dt/2)-(z2(2,ii)+k1z2(2)*dt/2))...
        + (1/omega2^2)*((z1(1,ii)+k1z1(1)*dt/2)-(z2(1,ii)+k1z2(1)*dt/2)-r)...
        - (1/gamma)); 
    k2z2(1) = z2(2,ii)+k1z2(2)*dt/2;
    
    k3z1(2) = -(2*ksi1/omega1*((z1(2,ii)+k2z1(2)*dt/2)-(z2(2,ii)+k2z2(2)*dt/2))...
        + (1/omega1^2)*((z1(1,ii)+k2z1(1)*dt/2)-(z2(1,ii)+k2z2(1)*dt/2)-r)...
        + (1/gamma)); 
    k3z1(1) = z1(2,ii)+k2z1(2)*dt/2;
    k3z2(2) = (2*ksi2/omega2*((z1(2,ii)+k2z1(2)*dt/2)-(z2(2,ii)+k2z2(2)*dt/2))...
        + (1/omega2^2)*((z1(1,ii)+k2z1(1)*dt/2)-(z2(1,ii)+k2z2(1)*dt/2)-r)...
        - (1/gamma)); 
    k3z2(1) = z2(2,ii)+k2z2(2)*dt/2;
    
    k4z1(2) = -(2*ksi1/omega1*((z1(2,ii)+k3z1(2)*dt)-(z2(2,ii)+k3z2(2)*dt))...
        + (1/omega1^2)*((z1(1,ii)+k3z1(1)*dt)-(z2(1,ii)+k3z2(1)*dt)-r) + (1/gamma)); 
    k4z1(1) = z1(2,ii)+k3z1(2)*dt;
    k4z2(2) = (2*ksi2/omega2*((z1(2,ii)+k3z1(2)*dt)-(z2(2,ii)+k3z2(2)*dt))...
        + (1/omega2^2)*((z1(1,ii)+k3z1(1)*dt)-(z2(1,ii)+k3z2(1)*dt)-r) - (1/gamma)); 
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
        z2(1,ii+1) = s;
        z2(2,ii+1) = c;
    end   
    
    contact_last = contact;
end
 
flight = flight(40:end); % removing first 40 datum points to avoid
                         % plotting the transient response
flight_mat(:,jj) = flight;                         
flight = flight(flight~=0); % removing trailing zeros if 100 flights
                            % are not made within set time
 
%% Plotting                            
                            
x = gamma_jj(jj)*ones(length(flight),1);
scatter(x,flight) % plotting one column on bifurcation diagram
 
end
 
set(gca,'fontsize',20)
xlabel('Gamma')
ylabel('Flight Time')

if save_mat == true
    save('Bifurcation Diagram Data') % saves workspace
end
