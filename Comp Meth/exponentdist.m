function [arrival, service] = exponentdist(patients, meanService, minT, maxT, Hour)
  if patients == 0
    arrival = [];
    service = [];
  return;
endif

totalT = Hour*60;

  % --- Arrival times ---
  pth = patients / Hour;
  meanArr = 60 / pth;
while true
    Uarr = rand(patients, 1);
    interArr = -meanArr .* log(Uarr);
    arrival = cumsum(interArr);

    if arrival(end) <= totalT
        break;
    endif
endwhile

  % --- Service times ---
  Userv = rand(patients, 1);
  service = -meanService .* log(Userv);

  % Regenerate only out-of-range values
  invalid = (service < minT) | (service > maxT);

  while any(invalid)
    Unew = rand(sum(invalid), 1);
    service(invalid) = -meanService .* log(Unew);

    invalid = (service < minT) | (service > maxT);
  endwhile

endfunction
