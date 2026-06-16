--User,cloud account, check account management
SELECT U.User_Role, COUNT(CA.Account_ID) AS Total_Accounts
FROM Users U
JOIN CloudAccount CA ON U.User_ID = CA.User_ID
GROUP BY U.User_Role;

--vulnerability, risk check
SELECT P.Provider_Name, V.Severity_Level, COUNT(V.Vulnerability_ID) AS Total_Vulnerabilities
FROM Provider P
JOIN Vulnerability V ON P.Provider_ID = V.Provider_ID
GROUP BY P.Provider_Name, V.Severity_Level;

--resource type
SELECT CR.Resource_Type, COUNT(SS.Scan_ID) AS Total_Scans
FROM CloudResource CR
JOIN SecurityScan SS ON CR.Resource_ID = SS.Resource_ID
GROUP BY CR.Resource_Type;

--plans for risks
SELECT R.Risk_Summary, COUNT(RP.Plan_ID) AS Total_Plans
FROM Report R
JOIN RemedialPlan RP ON R.Report_ID = RP.Report_ID
GROUP BY R.Risk_Summary;

--overview for management
SELECT P.Industry, COUNT(CR.Resource_ID) AS Total_Resources
FROM Portfolio P
JOIN Users U ON P.Portfolio_ID = U.Portfolio_ID
JOIN CloudAccount CA ON U.User_ID = CA.User_ID
JOIN CloudResource CR ON CA.Account_ID = CR.Account_ID
GROUP BY P.Industry;