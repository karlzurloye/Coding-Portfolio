%% Loading default parameters

clear variables
close all

Default_Input;

%% Alter any parameters here
% eg: global f; f = 26;

global f; f = 26;

%% Loop and/or plotting Core results

global b;
hold on
for b = 25E-6:25E-6:200E-6
    
data = Bouncing_Droplet_Simulator_Core;

global Az;
w = f*2*pi; % plate frequency (rad/s)
p = plot(data.time,data.upper_mass_vertical_position,'r',...
    data.time,data.lower_mass_vertical_position,'b',data.time,...
    Az*sin(w*data.time),'k');
p(1).LineWidth = 2;
p(2).LineWidth = 2;
p(3).LineWidth = 2;

end

set(gca,'fontsize',20)
xlabel('time (s)')
ylabel('vertical position (m)')

% print('file_name','-dpdf')
