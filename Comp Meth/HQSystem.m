clc;
while true
    printf("========================================\n");
    printf("Welcome to\n");
    printf("Hospital Emergency Department Queuing System v4.10\n\n\n");
    choice = input("Enter 1 to continue. Else to exit\n",'s');
     if strcmp(choice, '1')
       disp("System starting\n");
     else
       disp("Goodbye and thank you for using our system");
       break
     endif

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
            show = 0;
            break;
        else
            disp('Invalid input. Please enter 1 or 2.');
        end
    end
  U = rand(patients, 1);
  groups = repmat('P', patients, 1);
  groups(U >= 0.6) = 'N';

  patientsPH= sum(groups == 'P');
  patientsNH = sum(groups == 'N');
    clc;
    improve = 0;
    printf("Normal Version\n\n");
    PatientSim(show, improve, patients, patientsPH, patientsNH);

    improve = 1;
    printf("\nImproved Version\n\n");
    PatientSim(show, improve, patients, patientsPH, patientsNH);

    % --- Wait for any input, then clear and loop back to main menu ---
    input("\nPress any key to return to the main menu...", 's');
    clc;
endwhile
