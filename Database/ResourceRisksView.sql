create view resource_risks_view as 
select
	ca.Account_ID,
	cr.Resource_ID,
	r.Report_ID,
	r.Risk_Summary
from CloudAccount ca
join CloudResource cr
	on ca.Account_ID = cr.Account_ID
join SecurityScan ss
	on cr.Resource_ID = ss.Resource_ID
join Report r
	on ss.Scan_ID = r.Scan_ID;