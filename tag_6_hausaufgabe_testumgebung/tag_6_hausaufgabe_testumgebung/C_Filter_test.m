close all
t_end = 0.2;  % s, Simulationszeit
t_sample = 0.001;  % s, Abtastzeit
t = 0:t_sample:t_end;

load('data_iir.mat')

filename = 'csv_test_data.txt';
csv_data = csvread(filename);
x = csv_data(:)';

filename = 'csv_result.txt';
csv_data = csvread(filename,1,0);

y_ref = filter(b,a,x);

t_c = csv_data(:,1)';
x_c = csv_data(:,2)';
y_c = csv_data(:,3)';

% Betrachtung der Abweichungen
d_t = t_c - t;
d_x = x_c - x(1:201);
d_y = y_c - y_ref(1:201);

figure(3);
subplot(3,1,1);
plot(t,d_t);
subplot(3,1,2);
plot(t,d_x);
subplot(3,1,3);
plot(t,d_y);

% direkter Vergleich der Signale
figure(4);
plot(t,x(1:201),t,y_ref(1:201),t,y_c);

