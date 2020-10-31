classdef aOperate < handle
    properties
        currentAngles
        speeds
        directions
        states
        requestedAngles
        trjAngles
        index
        data
        idata
        index1
        t
        error1
        tmr
        fn
    end
    methods
        function o = aOperate(varargin)
            o.currentAngles = [0 0 0 0 0 0];
            o.speeds = [50 50 50 50 50 50];
            o.directions = [1 1 1 1 1 1];
            o.states = [0 0 0 0 0 0];
            o.index = [1 1 1 1 1 1];
            o.idata = 1;
            %o.fn = 0;
            o.tmr = timer('StartDelay', 1, 'Period', 1, 'TasksToExecute', 1,'ExecutionMode', 'fixedRate');
        end
        function go(o,robot,bt,guiHandles,fnn,pose,time,varargin)
            stop(o.tmr);
            o.fn = fnn;
            if o.fn == 1
            elseif o.fn == 2                                               % Set point
                o.index = [1 1 1 1 1 1];
                o.t = (0:time/20:time);
                for i=1:1:6
                    o.speeds(i) = time/20;
                end
                o.trjAngles = robot.ikine6s(ctraj(robot.fkine(o.currentAngles),pose,length(o.t)),'ru');
                if isnan(o.trjAngles(1,1)) || isnan(o.trjAngles(21,1))
                    set(guiHandles.pushbutton1,'Enable','On'); %GUI
                else
                    o.index1 = 21;
                    writeAngles(o,bt,guiHandles,time);
                end
            elseif o.fn == 3
            elseif o.fn == 4                                               % Set angles
                o.index = [1 1 1 1 1 1];
                o.t = (0:time/20:time);
                for i=1:1:6
                    o.speeds(i) = time/20;
                end
                o.trjAngles = pose;
                if isnan(o.trjAngles(1,1))
                    set(guiHandles.pushbutton1,'Enable','On'); %GUI
                else
                    o.index1 = 1;
                    writeAngles(o,bt,guiHandles,time);
                end                
            elseif o.fn == 5
            elseif o.fn == 6                                               % Set angles
                o.index = [1 1 1 1 1 1];
                o.t = (0:time/20:time);
                for i=1:1:6
                    o.speeds(i) = time/20;
                end
                if pose == 1
                    o.trjAngles = guiHandles.qze;
                elseif pose == 2
                    o.trjAngles = guiHandles.qre;
                elseif pose == 3
                    o.trjAngles = guiHandles.qst;
                elseif pose == 4
                    o.trjAngles = guiHandles.qno;
                end
                if isnan(o.trjAngles(1,1))
                    set(guiHandles.pushbutton1,'Enable','On'); %GUI
                else
                    o.index1 = 1;
                    writeAngles(o,bt,guiHandles,time);
                end                
            elseif o.fn == 10                                              % Realtime joystick move
                o.states = [0 0 0 0 0 0];
                o.trjAngles = (o.currentAngles/180)*pi;
                o.requestedAngles = o.trjAngles;
                for i=1:1:6
                    o.speeds(i) = time/20;
                end
                o.tmr = timer('StartDelay', 0.5, 'Period', 0.5, 'TasksToExecute', 1000000,'ExecutionMode', 'fixedRate');
                o.tmr.TimerFcn = {@o.joystickPeriodicUpdate,guiHandles};
                start(o.tmr);
            end
        end
        function joystickPeriodicUpdate(o,obj,event,guiHandles)
            buttons = [0 0 0 0 0 0 0 0];
            axes = [0 0];
            for i = 1:1:8
                buttons(i) = button(guiHandles.joystick, i);
            end
            for i = 1:1:2
                axes(i) = axis(guiHandles.joystick, i);
            end
            %disp(buttons)
            %disp(axes)
            T1 = transl(0,0,0);
            T1(:,:) = 0;
            if axes(1) == 1         %change in x-axis
                T1(1,4) = 0.05;
            elseif axes(1) == -1
                T1(1,4) = -0.05;
            end
            if axes(2) == 1         %change in y-axis
                T1(2,4) = 0.05;
            elseif axes(2) == -1
                T1(2,4) = -0.05;
            end
            if buttons(5) == 1 && buttons(7) == 0   %change in z-axis
                T1(3,4) = 0.05;
            elseif buttons(5) == 0 && buttons(7) == 1
                T1(3,4) = -0.05;
            end
            %disp(T1)
            requestedAnglesOld = o.requestedAngles;
            o.requestedAngles = guiHandles.cra.ikine6s((guiHandles.cra.fkine(o.requestedAngles) + T1),'ru');
            %disp(o.requestedAngles)
            if isnan(o.requestedAngles(1,1))
                disp('nan')
                o.requestedAngles = requestedAnglesOld;
            end
            absAnglesDifference = abs(o.requestedAngles - (o.currentAngles*(pi/180)));
            if o.states(1) == 0 && o.states(2) == 0 && o.states(3) == 0 && o.states(4) == 0 && o.states(5) == 0 && o.states(6) == 0
                o.trjAngles = o.requestedAngles;
                for i=1:1:6
                    if absAnglesDifference(i) >= 0.0314
                        [w1,w2,w3,w4,w5,w6,w7] = cMessage(o,i);
                        fwrite(guiHandles.bt,uint8([10,w1,w2,w3,w4,w5,w6,w7]));
                        o.states(i) = 1;                           % Waitting                    
                    end
                end
            else
                for i=1:1:6
                    if o.states(i) == 3        %message missed
                        %o.trjAngles(i) = o.requestedAngles(i);
                        [w1,w2,w3,w4,w5,w6,w7] = cMessage(o,i);
                        disp([w1,w2,w3,w4,w5,w6,w7]);
                        fwrite(guiHandles.bt,uint8([10,w1,w2,w3,w4,w5,w6,w7]));
                        o.states(i) = 1;                           % Waitting                            
                        %long time wait so i will send check message 
                    elseif o.states(i) == 1
                        fwrite(guiHandles.bt,uint8([10,83,i-1,10])); %check state of motor
                        %o.requestedAngles(i) = requestedAnglesOld(i);
                        %Motor is busy or miss idle message so we will send new
                        %message as if being idle or check message .
                    elseif o.states(i) == 2                %motor is busy
                        %check if last sent message is smaller than
                        %requested 1.8 degree
                        %if abs(o.requestedAngles(i) - o.trjAngles(i)) >= 0.0314
                                %Send requested angle
                                %o.states(i) = 1;
                                %[w1,w2,w3,w4,w5,w6,w7] = cMessage(o,i);
                                %fwrite(guiHandles.bt,uint8([10,w1,w2,w3,w4,w5,w6,w7]));
                            %else
                                %send check message to motot to ensure that is
                                %in busy 
                        fwrite(guiHandles.bt,uint8([10,83,i-1,10])); %check state of motor
                                %o.requestedAngles = requestedAnglesOld;
                            %end
                    end
                end
                %pause(0.3);
            end
            disp(o.states)
            cartesianXYZ = guiHandles.cra.fkine((o.currentAngles/180)*pi);
            disp(cartesianXYZ(:,4));
%             set(guiHandles.edit1,'String',num2str(cartesianXYZ(1,4)));
%             set(guiHandles.edit2,'String',num2str(cartesianXYZ(2,4)));
%             set(guiHandles.edit3,'String',num2str(cartesianXYZ(3,4)));            
%             
%             set(guiHandles.edit4,'String',num2str(o.currentAngles(1)));
%             set(guiHandles.edit5,'String',num2str(o.currentAngles(2)));
%             set(guiHandles.edit6,'String',num2str(o.currentAngles(3)));
%             set(guiHandles.edit7,'String',num2str(o.currentAngles(4)));
%             set(guiHandles.edit8,'String',num2str(o.currentAngles(5)));
%             set(guiHandles.edit9,'String',num2str(o.currentAngles(6)));
            stop(o.tmr);            
            o.tmr = timer('StartDelay', 0.5, 'Period', 0.5, 'TasksToExecute', 10,'ExecutionMode', 'fixedRate');
            o.tmr.TimerFcn = {@o.joystickPeriodicUpdate,guiHandles};            
            start(o.tmr);
        end
        function writeAngles(o,bt,guiHandles,time,varargin)
            if o.index1 > 0
                for i = 1:1:6
                    o.states(i) = 1;                               % Waitting
                    [w1,w2,w3,w4,w5,w6,w7] = cMessage(o,guiHandles,i);
                    fwrite(bt,uint8([10,w1,w2,w3,w4,w5,w6,w7]));
                end
                    o.t = time/20000; % Delay in second
                    o.error1 = 1;
                    o.tmr = timer('StartDelay', 0.02, 'Period', 0.02, 'TasksToExecute', 1000000,'ExecutionMode', 'fixedRate');
                    o.tmr.TimerFcn = {@o.rewriteAngles,bt,guiHandles,time};
                    start(o.tmr);
            else
                set(guiHandles.pushbutton1,'Enable','On'); %GUI
            end
        end
        function rewriteAngles(o,obj,event,bt,guiHandles,time)
            if o.error1
                o.error1 = 0;
                o.t = o.t - 0.02;
                if o.t < 0 
                	o.t = 0;
                end
                for i = 1:1:6
                  if o.states(i) == 1 || o.states(i) == 2
                    	fwrite(bt,uint8([10,83,i-1,10]));                  %check state of motor
                        o.error1 = 1;
                  elseif o.states(i) == 3
                    	o.states(i) = 1;
                    	[w1,w2,w3,w4,w5,w6,w7] = cMessage(o,guiHandles,i);
                        fwrite(bt,uint8([10,w1,w2,w3,w4,w5,w6,w7]));
                        o.error1 = 1;
                  end
                end
                %disp(o.index)
                disp(o.states)
            end
            if o.error1 == 0
                stop(o.tmr);
                o.index1 = o.index1 - 1;
                disp(o.index1)
                if o.t > 0
                    o.tmr = timer('StartDelay', o.t, 'Period', 1, 'TasksToExecute', 1,'ExecutionMode', 'fixedRate'); %delay
                    o.tmr.TimerFcn = {@o.writeAngles,bt,guiHandles,time};
                    start(o.tmr);
                else
                    writeAngles(o,bt,guiHandles,time);
                end
            end
        end
        
        function res = goToPose(o,robot,bt,T2,time,varargin)
            %o.requestedAngles = robot.ikine6s(xyz);
            o.index = [1 1 1 1 1 1];
            o.t = (0:time/20:time);
            for i=1:1:6
                o.speeds(i) = time/20;
            end
            o.trjAngles = robot.ikine6s(ctraj(robot.fkine(o.currentAngles),T2,length(o.t)),'ru');
            if isnan(o.trjAngles(1,1))|| isnan(o.trjAngles(21,1))
                res = 1;
            else
                res = 0;
                o.index1 = 21;
                while o.index1
                    for i = 1:1:6
                        o.states(i) = 1;                               % Waitting 
                        [w1,w2,w3,w4,w5,w6,w7] = cMessage(o,i);
                        fwrite(bt,uint8([10,w1,w2,w3,w4,w5,w6,w7]));
                    end
                    o.t = time/20000; % Delay in second
                    o.error1 = 1;
                    while o.error1
                        o.error1 = 0;
                        o.t = o.t - 0.02;
                        if o.t < 0 
                            o.t = 0;
                        end
                        pause(0.02);
                        for i = 1:1:6
                            if o.states(i) == 1 || o.states(i) == 2
                                fwrite(bt,uint8([10,83,i-1,10])); %check state of motor
                                o.error1 = 1;
                            elseif o.states(i) == 3
                                o.states(i) = 1;
                                [w1,w2,w3,w4,w5,w6,w7] = cMessage(o,i);
                                fwrite(bt,uint8([10,w1,w2,w3,w4,w5,w6,w7]));
                                o.error1 = 1;
                            end
                        end
                        o.index
                    end
                    
                    pause(o.t);
                    o.index1 = o.index1 - 1;
                    disp(o.index1)
                end
            end
        end
        function [w1,w2,w3,w4,w5,w6,w7] = cMessage(o, guiHandles, motorNumber, varargin)
            w1 = 77;                                                       % 'M'
            w2 = bitsll(motorNumber-1,4);% Motor number
            if o.fn == 10       %Real time (one pose)
                tempIndex = 1;
            elseif o.fn == 2    %trajectory (multiple poses)
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % Chech if sign forgetten - , + 
                tempIndex = o.index(motorNumber);								% Next pose
                tempIndexN1 = o.index(motorNumber)+1;
                tempIndexN2 = o.index(motorNumber)+2;
                %[tempIndex,tempIndexN1,tempIndexN2]
                if tempIndexN2 == 23
                    tempIndexN2 = 19;
                    tempIndexN1 = 20;
                end
                if tempIndexN2 == 22
                    tempIndexN2 = 19;
                end
                tA  = o.trjAngles(tempIndex,motorNumber);
                tA1 = o.trjAngles(tempIndexN1,motorNumber);
                tA2 = o.trjAngles(tempIndexN2,motorNumber);
                if (tA>0 && tA1<0 && tA2<0)||(tA<0 && tA1>0 && tA2>0)
                    tempIndex = tempIndexN1;
                end
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            elseif o.fn == 6 || o.fn == 4	                               % Set angles fn
                tempIndex = 1;
            end
            
            if o.trjAngles(tempIndex,motorNumber) > 0                      % if you need to reverse diirectios
                                                                            % of all motors
                if o.trjAngles(tempIndex,motorNumber) > guiHandles.cra.qlim(motorNumber, 2)   %chech if angle in scope
                    o.trjAngles(tempIndex,motorNumber) = guiHandles.cra.qlim(motorNumber, 2);
                end
                o.directions(motorNumber) = 1;
            else
                if o.trjAngles(tempIndex,motorNumber) < guiHandles.cra.qlim(motorNumber, 1)
                    o.trjAngles(tempIndex,motorNumber) = guiHandles.cra.qlim(motorNumber, 1);
                end
                o.directions(motorNumber) = 0;
            end
            disp('o.directions');
            disp(o.directions);
            w2  = bitor(w2,bitsll(o.directions(motorNumber),3));           % Motor direction
            w22 = (abs(o.trjAngles(tempIndex,motorNumber))*180.0)/pi;      % In degree
            %w22 = fi(w22,0,11,8);                                         % fixed point numeric object
            %w23 = bitsrl(w22,8);                                          % Intger value of angle
            w23 = uint16(w22);
            w3  = bitand(w23,hex2dec('FF'));                               % Least significant 8 bits of angle
            if w3 == 10
                w3 = 11;
            end
            w4  = uint8((w22 - w23)*100);
            if w4 == 10
                w4 = 11;
            end
            w23 = bitand(bitsrl(w23,8),3);
            w2  = bitor(w2,w23);                                 % Most signficant 3 bits of angle
            w2  = bitor(w2,128);                                   % Nothing
                 % Angle percession 
            w5  = bitand(bitsrl(uint16(o.speeds(motorNumber)),8),hex2dec('FF'));
            w6  = bitand(uint16(o.speeds(motorNumber)),hex2dec('FF'));
            if w6 == 10 
                w6 = 11;
            end
            w7  = 10;                                                      % Line feed
        end
        function my_callback_fcn(o, obj, event)
            txt1 = ' event occurred at ';
            msg = [txt1 txt1];
            disp(msg)
        end
        function serialCallback(o , obj , event )
            received = fread(obj,1,'uint8');
            if received == 10 && o.idata ~= 1
                o.data(o.idata) = received;
                if (o.data(1) == 77 && o.idata == 4 && o.data(2)<6)
                    switch o.data(3)
                        case 89     % Ack
                            if o.states(o.data(2)+1) == 1
                                if o.fn == 2     %Trajectory 
                                    o.index(o.data(2)+1) = o.index(o.data(2)+1) + 1;    % Increment 
                                end
                                o.states(o.data(2)+1) = 2;                        % Busy state
                            end
%                        case 78
%                            o.states(o.data(2)+1) = 0;
                        case 73     %idle
                            if o.states(o.data(2)+1) == 2
                                o.states(o.data(2)+1) = 0;
                                if o.fn == 2
                                    o.currentAngles(o.data(2)+1) = o.trjAngles(o.index(o.data(2)+1)-1,o.data(2)+1)/pi*180.0;    % Set current angle in degree
                                elseif o.fn == 10
                                    o.currentAngles(o.data(2)+1) = o.trjAngles(o.data(2)+1)/pi*180.0;    % Set current angle in degree
                                    disp('incremented')
                                elseif o.fn == 6 || o.fn == 4
                                    o.currentAngles(o.data(2)+1) = o.trjAngles(o.data(2)+1)*180.0/pi;    % Set current angle in degree
                                    disp('reached')                                    
                                end
                            end
%                        otherwise
%                            error('Msg is not understandable');
                    end
                elseif (o.data(1) == 83 && o.idata == 4 && o.data(2)<6)
                    if o.states(o.data(2)+1) == 1
                        disp('writed');                        
                        o.states(o.data(2)+1) = 3;  % Rewrite the pose
                    elseif o.states(o.data(2)+1) == 2 && o.data(3) == 73
                        o.states(o.data(2)+1) = 0;  % Finishing pose
                        if o.fn == 2
                            o.currentAngles(o.data(2)+1) = o.trjAngles(o.index(o.data(2)+1)-1,o.data(2)+1)*180.0/pi;    % Set current angle in degree
                        elseif o.fn == 10 
                            o.currentAngles(o.data(2)+1) = o.trjAngles(o.data(2)+1)*180.0/pi;    % Set current angle in degree
                            disp('incremented')
                        elseif o.fn == 6 || o.fn == 4
                          	o.currentAngles(o.data(2)+1) = o.trjAngles(o.data(2)+1)*180.0/pi;    % Set current angle in degree
                            disp('reached')
                        end
                    end
                end
                o.idata = 1;
            elseif received ~= 10
                o.data(o.idata) = received;
                o.idata = o.idata + 1;
                if o.idata == 11
                    o.idata = 1;
                end
            end
        end
     end
 end
