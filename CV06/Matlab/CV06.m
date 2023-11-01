clear all; close all; clc;

data = csvread('ntc.csv'); % Reading the Data
temperature = data(:, 1); % Assuming temperature values are in the first column
resistance = data(:, 2); % Assuming resistance values are in the second column

% Calculating ADC Values
R5 = 10; % 10k resistor in ohms
Vcc = 3.3; % Supply voltage in volts
adc = (resistance ./ (resistance + R5)) * 1023;

% Generating the Lookup Table
p = polyfit(adc, temperature, 10);

% Creating the ADC Vector and Calculating Temperatures
ad2 = 0:1023;
t2 = round(polyval(p, ad2), 1);

% Plotting the Results
plot(adc, temperature, 'b.'); % Original data points
hold on;
plot(ad2, t2, 'r'); % Fitted curve
xlabel('ADC Value'); ylabel('Temperature (°C)'); legend('Original Data', 'Fitted Curve');
grid on; hold off;

% Storing the Results
dlmwrite('data.dlm', t2 * 10, ',');
