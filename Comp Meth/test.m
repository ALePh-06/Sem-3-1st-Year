clc;
while true
    patients_str = input("Total amount of patients in the day: ", 's');
    patients = str2double(patients_str);

    if isnan(patients)
        disp('Invalid input. Please enter a number.');
        continue;
    end

    if patients > 0 && mod(patients, 1) == 0
        break;
    else
        disp('Please enter a positive integer larger than 0')
    end
end

while true
    disp('Please select what time would you like to see the simulation:');
    disp('1. Peak Hours');
    disp('2. Non Peak Hours');

    choice = input('Enter your choice: ', 's');

    if strcmp(choice, '1')
        disp('You selected Peak Hours');
        show = 1;
        break;
    elseif strcmp(choice, '2')
        disp('You selected Non Peak Hours');
        show= 0;
        break;
    else
        disp('Invalid input. Please enter 1 or 2.');
    end
end

PatientSim(patients, show);




