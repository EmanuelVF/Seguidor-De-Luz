classdef proyectoFinal < dynamicprops
   
    properties  (SetObservable)
        s;
        statusConnected;
        dataBuffer;
        k1=20;
        k2=5;
        k3=1;
        k4=1.5;
        
        timerFlag;
        timerFlag2;
        
        LDR0;
        LDR1;
        LDR2;
        LDR3;
        POS;
        
        pTimer;
        
        timePoints = 1:1:200;
        arrayIndex = 0;
    end
    
    
    properties
        handles; 
    end
    
    methods
       
        function self = proyectoFinal(self)
            hfig = hgload('girasol.fig'); % load 'GUIDE generated' Figure.
            self.handles = guihandles(hfig); % get figure handles.
            
            self.timerFlag = 1;
            self.timerFlag2 = 0;
            
            self.k1 = 20;
            self.k2 = 5;
            self.k3 = 1;
            self.k4 = 1.5;
            self.timePoints = 1:1:200;
            self.arrayIndex = 1;
            set(self.handles.mainAxis, 'YLim', [250 730]);
            set(self.handles.ldrOne, 'YLim', [0 260]);
            set(self.handles.ldrTwo, 'YLim', [0 260]);
            set(self.handles.ldrThree, 'YLim', [0 260]);
            set(self.handles.ldrFour, 'YLim', [0 260]);
            
            set(self.handles.mainAxis, 'XLim', [0 200]);
            set(self.handles.ldrOne, 'XLim', [0 200]);
            set(self.handles.ldrTwo, 'XLim', [0 200]);
            set(self.handles.ldrThree, 'XLim', [0 200]);
            set(self.handles.ldrFour, 'XLim', [0 200]);
            hold(self.handles.mainAxis, 'on');
            hold(self.handles.ldrOne, 'on');
            hold(self.handles.ldrTwo, 'on');
            hold(self.handles.ldrThree, 'on');
            hold(self.handles.ldrFour, 'on');
            
            set(self.handles.btn_k1, 'callback', @self.btn_k1_callback);
            set(self.handles.btn_k2, 'callback', @self.btn_k2_callback);
            set(self.handles.btn_k3, 'callback', @self.btn_k3_callback);
            set(self.handles.btn_k4, 'callback', @self.btn_k4_callback);
            set(self.handles.btn_connectDisconnect, 'callback', @self.btn_connectDisconnect_callback);
            
           % lh = addlistener(self,'POS','PostSet',@self.change_POS_callback); 
            
            set(self.handles.edit_k1, 'String', num2str(self.k1));
            set(self.handles.edit_k2, 'String', num2str(self.k2));
            set(self.handles.edit_k3, 'String', num2str(self.k3));
            set(self.handles.edit_k4, 'String', num2str(self.k4));
            
            set(self.handles.deviceMenu, 'String', {'PORT COM1','PORT COM2', 'PORT COM3','PORT COM4','PORT COM5','PORT COM6', 'PORT COM7','PORT COM8','PORT COM9','PORT COM10', 'PORT COM11','PORT COM12','PORT COM13','PORT COM14', 'PORT COM15','PORT COM16', 'PORT COM17', 'PORT COM18', 'PORT COM19', 'PORT COM20', 'PORT COM21', 'PORT COM22', 'PORT COM23', 'PORT COM24', 'PORT COM25', 'PORT COM26'});
            set(self.handles.deviceMenu, 'callback', @self.deviceMenu_callback);
            
            self.s.BytesAvailableFcnCount = 4;
            self.s.BytesAvailableFcnMode = 'byte';
            self.s.BytesAvailableFcn = @self.serialInterrupt;
            
            self.statusConnected = 0;
            
            self.pTimer = timer('TimerFcn', @(~,~)(self.timercallback), 'Period', 1/20, 'ExecutionMode', 'fixedSpacing');
            
            
        end
        
      %  function btn_ask_callback(self, varargin)
       %    
        %    fwrite(self.s, 11, 'uint8');
         %   fwrite(self.s, 12, 'uint8');
          %  fwrite(self.s, 10, 'uint8');
            
       % end
        
        %function change_POS_callback(self, varargin)
          % cla(self.handles.mainAxis);
         %  plot(self.handles.mainAxis,self.timePoints(1:max(size(self.POS))),self.POS); 
        %   set(self.handles.mainAxis, 'XLim', [-10 210]);
          % set(self.handles.axes_phase,'XScale','log'); % Every time a new point is drawn, rescaling is needed.
            
       % end
        
        function timercallback(self, varargin)
            
            if(self.arrayIndex > 1)
                cla(self.handles.mainAxis);
                cla(self.handles.ldrOne);
                cla(self.handles.ldrTwo);
                cla(self.handles.ldrThree);
                cla(self.handles.ldrFour);
                plot(self.handles.ldrOne,self.timePoints(1:max(size(self.LDR0))),self.LDR0, 'LineWidth', 2);
                plot(self.handles.ldrTwo,self.timePoints(1:max(size(self.LDR1))),self.LDR1, 'LineWidth', 2); 
                plot(self.handles.ldrThree,self.timePoints(1:max(size(self.LDR2))),self.LDR2, 'LineWidth', 2); 
                plot(self.handles.ldrFour,self.timePoints(1:max(size(self.LDR3))),self.LDR3, 'LineWidth', 2); 
                plot(self.handles.mainAxis,self.timePoints(1:max(size(self.POS))),self.POS, 'LineWidth', 2); 
                
            end
            
            %if(self.timerFlag == 1)
                %self.timerFlag2 = 1;
                fwrite(self.s, 11, 'uint8');
                fwrite(self.s, 12, 'uint8');
                fwrite(self.s, 10, 'uint8');
            %end
        end
        
        function btn_k1_callback(self, varargin)
            
          self.k1 = str2num(get(self.handles.edit_k1, 'String')); 
          self.k2 = str2num(get(self.handles.edit_k2, 'String'));
          self.k3 = str2num(get(self.handles.edit_k3, 'String'));
          self.k4 = str2num(get(self.handles.edit_k4, 'String'));
          sendData(self);
            
        end
        
        function btn_k2_callback(self, varargin)
            
            self.k1 = str2num(get(self.handles.edit_k1, 'String')); 
            self.k2 = str2num(get(self.handles.edit_k2, 'String'));
            self.k3 = str2num(get(self.handles.edit_k3, 'String'));
            self.k4 = str2num(get(self.handles.edit_k4, 'String'));
           
           sendData(self); 
           
        end
        
        function btn_k3_callback(self, varargin)
            
            self.k1 = str2num(get(self.handles.edit_k1, 'String')); 
            self.k2 = str2num(get(self.handles.edit_k2, 'String'));
            self.k3 = str2num(get(self.handles.edit_k3, 'String'));
            self.k4 = str2num(get(self.handles.edit_k4, 'String'));
            sendData(self);
            
        end
        
        function btn_k4_callback(self, varargin)
            
            self.k1 = str2num(get(self.handles.edit_k1, 'String')); 
            self.k2 = str2num(get(self.handles.edit_k2, 'String'));
            self.k3 = str2num(get(self.handles.edit_k3, 'String'));
            self.k4 = str2num(get(self.handles.edit_k4, 'String'));
            sendData(self);
            
        end
        
        function btn_connectDisconnect_callback(self, varargin)
        
            self.statusConnected = ~self.statusConnected;
            portsInfo = instrfind;
            if(self.statusConnected == 1)
                if(find(strcmp(portsInfo.Port,self.s.Port),1))
                    fopen(self.s);
                    if(strcmp(self.s.Status,'open'))
                        set(self.handles.btn_connectDisconnect, 'String', 'Disconnect');
                        start(self.pTimer);
                    else
                        self.statusConnected = 0;
                        disp('Error in Connection');
                        stop(self.pTimer);
                    end
                else
                    self.statusConnected = 0;
                    disp('Error in Connection');
                    stop(self.pTimer);
                end
            else
                set(self.handles.btn_connectDisconnect, 'String', 'Connect');
                fclose(self.s);
                stop(self.pTimer);
            end
        end
        
        function deviceMenu_callback(self, varargin)
            
            
            self.s = serial(['COM',num2str(get(self.handles.deviceMenu,'Value'))], 'BaudRate', 128000, 'DataBits',8, 'InputBufferSize', 4, 'TimeOut', 0.2);
            self.s.BytesAvailableFcnCount = 4;
            self.s.BytesAvailableFcnMode = 'byte';
            self.s.BytesAvailableFcn = @self.serialInterrupt;
            switch(get(self.handles.deviceMenu,'Value'))
                case 1
                    disp('PORT COM1');
                case 2
                    disp('PORT COM2');
                case 3
                    disp('PORT COM3');
                case 4
                    disp('PORT COM4');
                case 5
                    disp('PORT COM5');
                case 6
                    disp('PORT COM6');
                case 7
                    disp('PORT COM7');
                case 8
                    disp('PORT COM8');
                case 9
                    disp('PORT COM9');
                case 10
                    disp('PORT COM10');
                case 11
                    disp('PORT COM11');
                case 12
                    disp('PORT COM12');
                case 13
                    disp('PORT COM13');
                case 14
                    disp('PORT COM14');
                case 15
                    disp('PORT COM15');
                case 16
                    disp('PORT COM16');
            end
        end

        function serialInterrupt(self, varargin)
           % disp('a');
            receivedData = fread(self.s,4,'uint8');
            self.dataBuffer = [];
            self.dataBuffer = [self.dataBuffer receivedData];
            
            if(self.arrayIndex > 200)
               self.arrayIndex = 1;
               self.LDR0 = [];
               self.LDR1 = [];
               self.LDR2 = [];
               self.LDR3 = [];
               self.POS = [];
            end
            self.LDR0(self.arrayIndex) = self.dataBuffer(1);
            self.LDR1(self.arrayIndex) = self.dataBuffer(2);
            self.LDR2(self.arrayIndex) = self.dataBuffer(3);
            self.LDR3(self.arrayIndex) = self.dataBuffer(4);
           % self.POS = self.dataBuffer(5) + 255;
           % self.POS = self.dataBuffer(5) + self.dataBuffer(6);
           %self.s.BytesAvailable
           receivedData = fread(self.s,3,'uint8');
            self.POS(self.arrayIndex) = receivedData(1)+receivedData(2)*10+receivedData(3)*100;
            self.arrayIndex = self.arrayIndex + 1;

          % disp('LDR0')
          % self.LDR0
          % disp('LDR1')
          % self.LDR1;
          % disp('LDR2');
          % self.LDR2;
          % disp('LDR3');
          % self.LDR3;
          % disp('Posicion');
          % self.POS;
           
           flushinput(self.s);

        end
        
    end
    
    
end
    
    
    
    
    