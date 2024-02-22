ch1_data=csvread('out_ch1.csv',1,0);
ch2_data=csvread('out_ch2.csv',1,0);

subplot(2,1,1);
plot(ch1_data);
subplot(2,1,2);
plot(ch2_data);
