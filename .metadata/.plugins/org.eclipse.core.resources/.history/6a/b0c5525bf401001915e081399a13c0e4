%% FIR-Filterentwurf mit Ordnung n und Grenzfrequenz f_grenz
% Hinweis: f_grenz geht von 0...1, wobei 1 f_nyq entspricht
[x,t] = generate_test_data();

n = 20;
f_grenz = 0.1;
coeffs = fir1(n,f_grenz);

% Signal in MATLAB filtern (Referenz)
% der Bebber u-Uebertragungsfunktion ist der FIR immer 1
y = filter(coeffs,1,x);
plot(t,x,t,y);

%%Koeffizienten als c-Array formatieren und in Datei speichern
file = fopen('coeff.txt','wt');
fprintf(file,'double aFilterkoeffizienten[] = {');
fprintf(file, '%f', coeffs(1));
for ii = 2:n
    fprintf(file, ',%f',coeffs(ii));
end
fprintf(file,'};');
fclose(file);
