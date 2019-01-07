function generate_test_data(filename)

if nargin <=2
    filename = 'sollverlauf.csv';
end

t_end = 5;  % s, Simulationszeit
t_step = 0.01;  % s, Abtastzeit

t = 0:t_step:t_end; % Zeit
w = zeros(size(t));  % Sollposition

% Sprung auf 1 bei 1/3t und zurück auf 0 bei 2/3t
% x(1,floor(1*size(t,2)/3):floor(2*size(t,2)/3)) = 1;
w(1,1:floor(size(t,2)/2)) = 1;

% als csv-Datei exportieren
csv_data = [t' w'];
csvwrite(filename,csv_data);

figure(1);
plot(t,w);

keyboard;

% Das von der C-Version gefilterte Signal im Vergleich:
% (die Datei muss natürlich vorhanden sein)
filename = 'ergebnis.csv';
csv_data = csvread(filename);

t_c = csv_data(:,1)';
w_c = csv_data(:,2)';
y_c = csv_data(:,3)';
pos_diff = y_c - w_c;

figure(2);
subplot(211)
plot(t_c, w_c, t_c, y_c);
legend('Sollposition', 'Istposition');
title('Simulationsergebnis')

subplot(212)
plot(t_c, pos_diff);
title('Regelfehler')
end