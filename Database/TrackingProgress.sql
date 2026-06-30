create view tracking_progress as 
select 
	r.Report_ID,
	r.Risk_Summary,
	rp.Plan_Name,
	pt.Tracker_Status
from 
	Report r
join RemedialPlan rp
	on r.Report_ID = rp.Report_ID
join ProgressTracker pt
	on rp.Plan_ID = pt.Plan_ID;