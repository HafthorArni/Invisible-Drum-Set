clear all, close all, clc
% Open the text file for reading
fileID = fopen('data.txt', 'r');

% Skip the initial lines that do not contain numeric data
for i = 1:2
    fgetl(fileID);
end

% Read the numeric data for four columns using textscan
data = textscan(fileID, '%d %d %d');

% Close the file
fclose(fileID);

% Convert cell arrays to matrices
data1 = data{1};
data2 = data{2};
data3 = data{3}; % Additional column 3


% Define the total time of data collection in seconds
totalTime = 5;

% Generate a time vector assuming data points are evenly spaced over time
timeVector = linspace(0, totalTime, length(data1));

% Plotting
figure;

% Plot the first column of data
subplot(3, 1, 1); % Change to 4 rows for 4 subplots
plot(timeVector, data1);
hold on
plot(timeVector, data1,'.');
title('Gyro Y axis');
%xlabel('Time (seconds)');
ylabel('Degrees per second');

% Plot the second column of data
subplot(3, 1, 2); % Change to 4 rows for 4 subplots
plot(timeVector, data2);
hold on
plot(timeVector, data2,'.');
title('Pitch');
%xlabel('Time (seconds)');
ylabel('degrees');

% Plot the third column of data
subplot(3, 1, 3); % Added subplot for third column
plot(timeVector, data3);
hold on
plot(timeVector, data3,'.');
title('Yaw (heading)');
xlabel('Time (seconds)');
ylabel('degrees');

% Adjust layout to make sure there's no overlap of plots
%sgtitle('Data Plots Over Time');

length(data2)

