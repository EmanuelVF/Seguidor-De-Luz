function sendData(self)

self.timerFlag = 0;
if(self.statusConnected == 1)
    
    fwrite(self.s, self.k1 + '0','uint8');
    fwrite(self.s, self.k2 + '0','uint8');
    fwrite(self.s, self.k3 + '0','uint8');
    fwrite(self.s, self.k4*10 + '0','uint8');
    % Terminador
    fwrite(self.s, 10,'uint8');
else
    disp('Error: Cant send data');
end
self.timerFlag = 1;

end