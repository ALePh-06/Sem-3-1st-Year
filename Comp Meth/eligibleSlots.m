function range = eligibleSlots(t, improve)
  if improve == 1
    dayDoctors   = 7;
    nightDoctors = 5;
  else
    dayDoctors   = 5;
    nightDoctors = 3;
  endif

  if t < 480
    range = 1:dayDoctors;
  elseif t < 1440
    range = (dayDoctors+1):(dayDoctors+nightDoctors);
  else
    range = [];
  endif
endfunction
