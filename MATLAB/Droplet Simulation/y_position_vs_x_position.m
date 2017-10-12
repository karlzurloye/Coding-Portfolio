%% Loading default parameters

clear variables
close all

Default_Input;

%% Alter any parameters here
% eg: global f; f = 22;

global f; f = 22;
global run_time; run_time = 1;
global Az; Az = 1.2E-3;
global phase_angle; phase_angle = 90;

%% Loop and/or plotting Core results
data = Bouncing_Droplet_Simulator_Core;

global f;
w = f*2*pi; % plate frequency (rad/s)
p = plot(data.horizontal_position,data.upper_mass_vertical_position,'r',...
    data.horizontal_position,data.lower_mass_vertical_position,'b',...
    data.horizontal_position,Az*sin(w*data.time),'k');
p(1).LineWidth = 2;
p(2).LineWidth = 2;
p(3).LineWidth = 2;
set(gca,'fontsize',20)
xlabel('horizontal position (m)')
ylabel('vertical position (m)')

% axis([-2E-4, 12E-4, -1.5E-3, 3.5E-3])
% print('file_name','-dpdf')
