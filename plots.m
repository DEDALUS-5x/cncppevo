% The purpose of this code is to plot easily the feedrate function
% evolution over time. Created for debugging and developing the lookahead
% algorithm

log = readtable("log.csv");

% Extract fields from the csv. In particular feedrate, time_tot and the
% number of the line are the intersted variables
n = log.n;
t = log.t_tot;
feedrate = log.feedrate;

figure;
plot(t, feedrate, 'b-', 'LineWidth', 1.5);
hold on;

% plot a circle whenever the cnc cross on a new opint
dn = [true; diff(n) ~= 0];  
plot(t(dn), feedrate(dn), 'ro', 'MarkerFaceColor', 'r');

% Labels
xlabel('Time [s]');
ylabel('Feedrate [mm/min]');
title('feedrate in function of time');
grid on;
legend('Feedrate', 'New Point');

