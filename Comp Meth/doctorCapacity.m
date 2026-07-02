function capacity = doctorCapacity(t)
  if t < 480          % 9am - 5pm (Peak Hours)
    capacity = 5;
  elseif t < 1440      % 5pm - 9am next day (Non-Peak Hours)
    capacity = 3;
  else                  % after 9am next day - closed
    capacity = 0;
  endif
endfunction
