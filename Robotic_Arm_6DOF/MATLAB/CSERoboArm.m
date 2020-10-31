function varargout = CSERoboArm(varargin)
% CSEROBOARM MATLAB code for CSERoboArm.fig
%      CSEROBOARM, by itself, creates a new CSEROBOARM or raises the existing
%      singleton*.
%
%      H = CSEROBOARM returns the handle to a new CSEROBOARM or the handle to
%      the existing singleton*.
%
%      CSEROBOARM('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in CSEROBOARM.M with the given input arguments.
%
%      CSEROBOARM('Property','Value',...) creates a new CSEROBOARM or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before CSERoboArm_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to CSERoboArm_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help CSERoboArm

% Last Modified by GUIDE v2.5 14-Feb-2018 14:04:50

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @CSERoboArm_OpeningFcn, ...
                   'gui_OutputFcn',  @CSERoboArm_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

% --- Executes just before CSERoboArm is made visible.
function CSERoboArm_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to CSERoboArm (see VARARGIN)
    startup_rvc;
    mdl_CSERoboArmEdit;
    handles.o_cra = aOperate();
    handles.bt = Bluetooth('BLUEDEMO',1);
    handles.bt.BytesAvailableFcnCount = 1;
    handles.bt.BytesAvailableFcnMode = 'byte';
    handles.bt.BytesAvailableFcn = @(src,evnt)handles.o_cra.serialCallback(src,evnt);
    %fopen(handles.bt);
    handles.joystick = vrjoystick(1);   %joystick object
    set(handles.popupmenu1,'String',{'Config','Joystick','Move'});
    set(handles.popupmenu3,'String',{'Cartesian','Joint-Space'});
% Choose default command line output for CSERoboArm
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes CSERoboArm wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = CSERoboArm_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%     fclose(handles.bt);
%     delete(handles.bt);
% Get default command line output from handles structure
varargout{1} = handles.output;



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as text
%        str2double(get(hObject,'String')) returns contents of edit2 as a double


% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit3_Callback(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit3 as text
%        str2double(get(hObject,'String')) returns contents of edit3 as a double


% --- Executes during object creation, after setting all properties.
function edit3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    if get(handles.popupmenu1,'Value') == 3 %Move mode
        moveFunction = get(handles.popupmenu2,'Value');
        if moveFunction == 2
            set(hObject,'Enable','off');
            xf = str2double(get(handles.edit1,'String'));
            yf = str2double(get(handles.edit2,'String'));
            zf = str2double(get(handles.edit3,'String'));
            if isnan(xf) || isnan(yf) || isnan(zf)
                errordlg('You must enter a numeric value','Invalid Input','modal') 
                set(hObject,'Enable','on');
                return
            else
                T2 = transl(xf,yf,zf);
                timeToReach = str2double(get(handles.edit10,'String'));
                go(handles.o_cra, handles.cra, handles.bt, handles, moveFunction, T2, timeToReach);
        %         if (goToPose(handles.o_cra,handles.cra,handles.bt,T2,2000) == 0)
        %             set(handles.text5,'String','Succeed');
        %             set(hObject,'Enable','on');
        %         else
        %             set(handles.text5,'String','Failed');
        %             set(hObject,'Enable','on');
        %         end
            end
        elseif moveFunction == 6
            set(hObject,'Enable','off');
            Pose = get(handles.popupmenu5,'Value'); 
            timeToReach = str2double(get(handles.edit10,'String'));
            go(handles.o_cra, handles.cra, handles.bt, handles, moveFunction, Pose, timeToReach);
        elseif moveFunction == 4
            set(hObject,'Enable','off');
            an1 = str2double(get(handles.edit4,'String'))*pi/180;
            an2 = str2double(get(handles.edit5,'String'))*pi/180;
            an3 = str2double(get(handles.edit6,'String'))*pi/180;
            an4 = str2double(get(handles.edit7,'String'))*pi/180;
            an5 = str2double(get(handles.edit8,'String'))*pi/180;
            an6 = str2double(get(handles.edit9,'String'))*pi/180;
            Pose = [an1 an2 an3 an4 an5 an6];
            %disp(Pose);
            %set(hObject,'Enable','on');
            timeToReach = str2double(get(handles.edit10,'String'));
            go(handles.o_cra, handles.cra, handles.bt, handles, moveFunction, Pose, timeToReach);
        elseif moveFunction == 5
            set(handles.edit4,'String',num2str(handles.o_cra.currentAngles(1)));
            set(handles.edit5,'String',num2str(handles.o_cra.currentAngles(2)));
            set(handles.edit6,'String',num2str(handles.o_cra.currentAngles(3)));
            set(handles.edit7,'String',num2str(handles.o_cra.currentAngles(4)));
            set(handles.edit8,'String',num2str(handles.o_cra.currentAngles(5)));
            set(handles.edit9,'String',num2str(handles.o_cra.currentAngles(6)));
        end
    elseif get(handles.popupmenu1,'Value') == 2 %Joystick mode
        JoystickFunction = get(handles.popupmenu2,'Value');
        if JoystickFunction == 1                %Joystick normal
            timeToReach = str2double(get(handles.edit10,'String'));            
            go(handles.o_cra, handles.cra, handles.bt, handles, 10, 0, timeToReach); %2000
        end
    end
    set(hObject,'Enable','on');


% --- Executes on button press in radiobutton1.
function radiobutton1_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% Hint: get(hObject,'Value') returns toggle state of radiobutton1

% --- Executes on button press in radiobutton2.
function radiobutton2_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton2


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    fclose(handles.bt);
    delete(handles.bt);


% --- Executes on selection change in popupmenu1.
function popupmenu1_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    
% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu1
    if get(handles.popupmenu1,'Value') == 1         %configuraiton mode
        set(handles.uipanel2,'Visible','On')        %Bluetooth panel (2)
        set(handles.uipanel3,'Visible','Off')       %Cartesian panel (3)
        set(handles.uipanel4,'Visible','Off')       %Angles panel (4)
        set(handles.popupmenu2,'Visible','Off')     %hide function
        set(handles.popupmenu3,'Visible','Off')     %hide motion
    elseif get(handles.popupmenu1,'Value') == 2     %Joystick mode
        %Changing content of function 
        set(handles.popupmenu2,'String',{'Move','..'});        
        set(handles.uipanel2,'Visible','Off')
        set(handles.uipanel3,'Visible','On')
        set(handles.uipanel4,'Visible','On')
        set(handles.edit1,'Enable','Off')
        set(handles.edit2,'Enable','Off')
        set(handles.edit3,'Enable','Off')
        set(handles.edit4,'Enable','Off')
        set(handles.edit5,'Enable','Off')
        set(handles.edit6,'Enable','Off')
        set(handles.edit7,'Enable','Off')
        set(handles.edit8,'Enable','Off')
        set(handles.edit9,'Enable','Off')
        set(handles.popupmenu2,'Visible','On')     %show function
        set(handles.popupmenu3,'Visible','On')     %show motion
    elseif get(handles.popupmenu1,'Value') == 3     %Move mode
        %Changing content of function 
        set(handles.popupmenu2,'String',{'Hold','SetPoint','GetPoint','SetAngles','GetAngles','SetAnglesFromSavedPoses'});
        set(handles.popupmenu5,'String',{'zero angles','ready','stretch','nominal'});    
        set(handles.uipanel2,'Visible','Off')
        set(handles.uipanel3,'Visible','On')
        set(handles.uipanel4,'Visible','Off')
        set(handles.edit1,'Enable','On')
        set(handles.edit2,'Enable','On')
        set(handles.edit3,'Enable','On')
        set(handles.edit4,'Enable','On')
        set(handles.edit5,'Enable','On')
        set(handles.edit6,'Enable','On')
        set(handles.edit7,'Enable','On')
        set(handles.edit8,'Enable','On')
        set(handles.edit9,'Enable','On')
        set(handles.popupmenu2,'Visible','On')     %show function
        set(handles.popupmenu3,'Visible','On')     %show motion
    end

function popupmenu2_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    
% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu1
    val = get(handles.popupmenu2,'Value');
    if val == 2 || val == 3                     %move mode - cartesian panel 
        set(handles.uipanel3,'Visible','On')
        set(handles.uipanel4,'Visible','Off')
    elseif val == 4 || val == 5                 %move mode - angles panel
        set(handles.uipanel3,'Visible','Off')
        set(handles.uipanel4,'Visible','On')
    end

    
% --- Executes during object creation, after setting all properties.
function popupmenu1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
    
% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function popupmenu2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
    
% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    %set(hObject,'Enable','off');
    fopen(handles.bt);



function edit10_Callback(hObject, eventdata, handles)
% hObject    handle to edit10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double

function edit9_Callback(hObject, eventdata, handles)
% hObject    handle to edit9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double
function edit8_Callback(hObject, eventdata, handles)
% hObject    handle to edit8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double
function edit7_Callback(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double
function edit6_Callback(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double
function edit5_Callback(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double
function edit4_Callback(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double

% --- Executes during object creation, after setting all properties.
function edit10_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function edit9_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function edit8_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function edit7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function edit6_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function edit5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function edit4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes on selection change in popupmenu3.
function popupmenu3_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu3 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu3


% --- Executes during object creation, after setting all properties.
function popupmenu3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in popupmenu5.
function popupmenu5_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu5 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu5


% --- Executes during object creation, after setting all properties.
function popupmenu5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
