function [x,t] = generate_test_data()

    filename = 'csv_data.txt';


t_end = 3;
t_step = 0.01;

% Nyquist-Frequenz: 1/t_step/2;
f_nyq = 1/t_step/2;

t = 0:t_step:t_end;
x = zeros(size(t));

% Sprung auf 2 bei 2/3t und zurueck auf 0 2/3 t
x(1,floor(1*size(t,2)/3):floor(2*size(t,2)/3)) = 1;

% Ueberlagerung mit hochfrequentem Sinus (f = f_nyq/4)
x = x + 0.1*sin(f_nyq/4*2*pi*t);
plot(t,x);

%% als csv-Datei exportieren
csv_data = [t.' x.'];
csvwrite(filename, csv_data);
